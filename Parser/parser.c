#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_LINE 4096
#define MAX_WIDGETS 100
#define MAX_ATTRS 50
#define MAX_STACK 50

typedef struct Attribute {
    char name[64];
    char value[512];
} Attribute;

typedef struct Widget {
    char type[64];
    char name[64];
    char parent[64];
    char text_content[1024];
    Attribute attrs[MAX_ATTRS];
    int attr_count;
    int depth;
    int is_container;
} Widget;

typedef struct {
    Widget widgets[MAX_WIDGETS];
    int widget_count;
    char stack[MAX_STACK][64];
    int stack_top;
} Parser;

// Helper functions
char* trim(char *str) {
    if (!str) return "";
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

void parse_attributes(const char *tag, Attribute *attrs, int *count) {
    *count = 0;
    char tag_copy[MAX_LINE];
    strcpy(tag_copy, tag);
    
    char *ptr = tag_copy;
    while (*ptr && *count < MAX_ATTRS) {
        // Skip to attribute
        while (*ptr && *ptr != ' ' && *ptr != '\t') ptr++;
        if (!*ptr) break;
        ptr++;
        while (*ptr == ' ' || *ptr == '\t') ptr++;
        
        // Get attribute name
        char name[64] = {0};
        int i = 0;
        while (*ptr && *ptr != '=' && *ptr != ' ' && *ptr != '\t' && i < 63) {
            name[i++] = *ptr++;
        }
        name[i] = '\0';
        
        if (*ptr == '=' && strlen(name) > 0) {
            ptr++;
            while (*ptr == ' ' || *ptr == '\t') ptr++;
            
            char quote = *ptr;
            if (quote == '"' || quote == '\'') {
                ptr++;
                char value[512] = {0};
                i = 0;
                while (*ptr && *ptr != quote && i < 511) {
                    value[i++] = *ptr++;
                }
                value[i] = '\0';
                if (*ptr == quote) ptr++;
                
                strcpy(attrs[*count].name, name);
                strcpy(attrs[*count].value, value);
                (*count)++;
            }
        }
    }
}

int parse_html(const char *filename, Parser *parser) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return -1;
    }
    
    char line[MAX_LINE];
    parser->widget_count = 0;
    parser->stack_top = -1;
    int line_num = 0;
    
    printf("Parsing file...\n");
    
    while (fgets(line, sizeof(line), file)) {
        line_num++;
        char *trimmed = trim(line);
        if (trimmed[0] == '\0') continue;
        
        // Check for opening tag
        if (trimmed[0] == '<' && trimmed[1] != '/' && trimmed[strlen(trimmed)-1] == '>') {
            char tag_name[64] = {0};
            char *space = strchr(trimmed + 1, ' ');
            char *end = strchr(trimmed, '>');
            
            if (!end) {
                continue;
            }
            
            if (space && space < end) {
                int len = space - (trimmed + 1);
                if (len > 63) len = 63;
                strncpy(tag_name, trimmed + 1, len);
                tag_name[len] = '\0';
            } else {
                int len = end - (trimmed + 1);
                if (len > 63) len = 63;
                strncpy(tag_name, trimmed + 1, len);
                tag_name[len] = '\0';
            }
            
            // Skip non-widget tags
            if (strcmp(tag_name, "?xml") == 0 || strcmp(tag_name, "!DOCTYPE") == 0) {
                continue;
            }
            
            printf("  Found tag: %s at line %d\n", tag_name, line_num);
            
            // Create widget
            if (parser->widget_count >= MAX_WIDGETS) {
                fprintf(stderr, "Warning: Too many widgets, stopping at %d\n", MAX_WIDGETS);
                break;
            }
            
            Widget *w = &parser->widgets[parser->widget_count];
            strcpy(w->type, tag_name);
            
            // Generate unique name
            snprintf(w->name, sizeof(w->name), "%s_%d", tag_name, parser->widget_count);
            
            w->text_content[0] = '\0';
            w->depth = parser->stack_top + 1;
            
            // Set parent - THIS IS CRITICAL
            if (parser->stack_top >= 0) {
                strcpy(w->parent, parser->stack[parser->stack_top]);
            } else {
                strcpy(w->parent, "none");
            }
            
            // Check if this widget can contain children
            w->is_container = (strcmp(tag_name, "window") == 0 || 
                              strcmp(tag_name, "vbox") == 0 || 
                              strcmp(tag_name, "hbox") == 0 ||
                              strcmp(tag_name, "headerbar") == 0 ||
                              strcmp(tag_name, "menu") == 0 ||
                              strcmp(tag_name, "menu_button") == 0 ||
                              strcmp(tag_name, "menu_item") == 0);  // menu_item can contain submenu
            
            // Parse attributes
            parse_attributes(trimmed, w->attrs, &w->attr_count);
            
            parser->widget_count++;
            
            // Push to stack if it's a container (can have children)
            if (w->is_container && trimmed[strlen(trimmed)-2] != '/') {
                parser->stack_top++;
                if (parser->stack_top < MAX_STACK) {
                    strcpy(parser->stack[parser->stack_top], w->name);
                    printf("    Pushed to stack: %s, stack_top=%d\n", w->name, parser->stack_top);
                }
            }
        }
        // Check for closing tag
        else if (trimmed[0] == '<' && trimmed[1] == '/') {
            // Get closing tag name
            char tag_name[64] = {0};
            char *end = strchr(trimmed, '>');
            if (end) {
                int len = end - (trimmed + 2);
                if (len > 63) len = 63;
                strncpy(tag_name, trimmed + 2, len);
                tag_name[len] = '\0';
                
                // Pop from stack if this is a container
                if (parser->stack_top >= 0) {
                    // Check if the tag on stack matches
                    if (strstr(parser->stack[parser->stack_top], tag_name) != NULL) {
                        printf("    Popping from stack, stack_top was %d\n", parser->stack_top);
                        parser->stack_top--;
                    }
                }
            }
        }
        // Text content - only for non-container widgets that have text
        else if (parser->stack_top >= 0 && trimmed[0] != '<') {
            if (parser->widget_count > 0) {
                // Get the last widget that might need text content
                Widget *w = &parser->widgets[parser->widget_count - 1];
                // Only add text to widgets that typically have text content
                if (strcmp(w->type, "label") == 0 || strcmp(w->type, "button") == 0 ||
                    strcmp(w->type, "checkbox") == 0 || strcmp(w->type, "radiobutton") == 0 ||
                    strcmp(w->type, "menu_button") == 0 || strcmp(w->type, "menu_item") == 0) {
                    if (strlen(w->text_content) + strlen(trimmed) < sizeof(w->text_content) - 2) {
                        if (w->text_content[0]) {
                            strcat(w->text_content, " ");
                            strcat(w->text_content, trimmed);
                        } else {
                            strcpy(w->text_content, trimmed);
                        }
                    }
                }
            }
        }
    }
    
    fclose(file);
    
    if (parser->widget_count == 0) {
        fprintf(stderr, "Error: No valid widgets found in file\n");
        return -1;
    }
    
    printf("\nWidget hierarchy:\n");
    for (int i = 0; i < parser->widget_count; i++) {
        printf("  %s (type: %s, parent: %s)\n", parser->widgets[i].name, 
               parser->widgets[i].type, parser->widgets[i].parent);
    }
    
    return 0;
}

void generate_c_code(Parser *parser, const char *output_c) {
    FILE *out = fopen(output_c, "w");
    if (!out) {
        fprintf(stderr, "Error: Cannot create %s\n", output_c);
        return;
    }
    
    printf("Generating C code...\n");
    
    // Write header
    fprintf(out, "#include \"structures.h\"\n");
    fprintf(out, "#include <glib.h>\n\n");
    
    // Generate callback declarations
    fprintf(out, "// Callback functions\n");
    fprintf(out, "void on_button_click(GtkWidget *widget, gpointer data) {\n");
    fprintf(out, "    g_print(\"Button clicked: %%s\\n\", (char*)data);\n");
    fprintf(out, "}\n\n");
    
    fprintf(out, "void on_entry_changed(GtkWidget *widget, gpointer data) {\n");
    fprintf(out, "    const char *text = gtk_entry_get_text(GTK_ENTRY(widget));\n");
    fprintf(out, "    g_print(\"Entry changed: %%s\\n\", text);\n");
    fprintf(out, "}\n\n");
    
    fprintf(out, "void on_check_toggled(GtkWidget *widget, gpointer data) {\n");
    fprintf(out, "    gboolean active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));\n");
    fprintf(out, "    g_print(\"Check button toggled: %%s\\n\", active ? \"ON\" : \"OFF\");\n");
    fprintf(out, "}\n\n");
    
    fprintf(out, "void on_radio_toggled(GtkWidget *widget, gpointer data) {\n");
    fprintf(out, "    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {\n");
    fprintf(out, "        g_print(\"Radio selected: %%s\\n\", (char*)data);\n");
    fprintf(out, "    }\n");
    fprintf(out, "}\n\n");
    
    fprintf(out, "void on_menu_item_clicked(GtkWidget *widget, gpointer data) {\n");
    fprintf(out, "    g_print(\"Menu item clicked: %%s\\n\", (char*)data);\n");
    fprintf(out, "}\n\n");
    
    fprintf(out, "int main(int argc, char *argv[]) {\n");
    fprintf(out, "    ProgramStart();\n\n");
    
    // Store menu buttons and their menus for linking
    typedef struct {
        char button_name[64];
        char menu_name[64];
    } ButtonMenuLink;
    
    typedef struct {
        char parent_item[64];
        char submenu_name[64];
    } ItemSubmenuLink;
    
    ButtonMenuLink links[MAX_WIDGETS];
    int link_count = 0;
    
    ItemSubmenuLink submenu_links[MAX_WIDGETS];
    int submenu_count = 0;
    
    // First pass: Create all widgets
    for (int i = 0; i < parser->widget_count; i++) {
        Widget *w = &parser->widgets[i];
        
        if (strcmp(w->type, "window") == 0) {
            fprintf(out, "    // Create window\n");
            fprintf(out, "    window *%s = create_window();\n", w->name);
            
            for (int a = 0; a < w->attr_count; a++) {
                if (strcmp(w->attrs[a].name, "title") == 0)
                    fprintf(out, "    %s->title = \"%s\";\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "width") == 0)
                    fprintf(out, "    %s->width = %s;\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "height") == 0)
                    fprintf(out, "    %s->height = %s;\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "bgcolor") == 0)
                    fprintf(out, "    %s->bg_color = \"%s\";\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "resizable") == 0)
                    fprintf(out, "    %s->resizable = %s;\n", w->name, 
                           strcmp(w->attrs[a].value, "true") == 0 ? "TRUE" : "FALSE");
            }
            fprintf(out, "    update_window(%s);\n", w->name);
            fprintf(out, "\n");
        }
        else if (strcmp(w->type, "headerbar") == 0 || strcmp(w->type, "headbar") == 0) {
            fprintf(out, "    // Create header bar\n");
            fprintf(out, "    header_bar *%s = create_header_bar();\n", w->name);
            
            for (int a = 0; a < w->attr_count; a++) {
                if (strcmp(w->attrs[a].name, "title") == 0)
                    fprintf(out, "    %s->title = \"%s\";\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "subtitle") == 0)
                    fprintf(out, "    %s->sub_title = \"%s\";\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "showclose") == 0)
                    fprintf(out, "    %s->close_button_show = %s;\n", w->name,
                           strcmp(w->attrs[a].value, "true") == 0 ? "TRUE" : "FALSE");
            }
            fprintf(out, "    update_header_bar(%s);\n", w->name);
            fprintf(out, "\n");
        }
        else if (strcmp(w->type, "vbox") == 0) {
            int spacing = 0;
            int homogeneous = 0;
            
            for (int a = 0; a < w->attr_count; a++) {
                if (strcmp(w->attrs[a].name, "spacing") == 0)
                    spacing = atoi(w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "homogeneous") == 0)
                    homogeneous = strcmp(w->attrs[a].value, "true") == 0;
            }
            
            fprintf(out, "    // Create vertical box\n");
            fprintf(out, "    box *%s = create_vbox(%d, %s);\n", w->name, spacing, homogeneous ? "TRUE" : "FALSE");
            
            for (int a = 0; a < w->attr_count; a++) {
                if (strcmp(w->attrs[a].name, "bgcolor") == 0)
                    fprintf(out, "    %s->bg_color = \"%s\";\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "margin") == 0) {
                    fprintf(out, "    %s->margin_top = %s;\n", w->name, w->attrs[a].value);
                    fprintf(out, "    %s->margin_bottom = %s;\n", w->name, w->attrs[a].value);
                    fprintf(out, "    %s->margin_left = %s;\n", w->name, w->attrs[a].value);
                    fprintf(out, "    %s->margin_right = %s;\n", w->name, w->attrs[a].value);
                }
            }
            fprintf(out, "    update_box(%s);\n", w->name);
            fprintf(out, "\n");
        }
        else if (strcmp(w->type, "hbox") == 0) {
            int spacing = 0;
            int homogeneous = 0;
            
            for (int a = 0; a < w->attr_count; a++) {
                if (strcmp(w->attrs[a].name, "spacing") == 0)
                    spacing = atoi(w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "homogeneous") == 0)
                    homogeneous = strcmp(w->attrs[a].value, "true") == 0;
            }
            
            fprintf(out, "    // Create horizontal box\n");
            fprintf(out, "    box *%s = create_hbox(%d, %s);\n", w->name, spacing, homogeneous ? "TRUE" : "FALSE");
            
            for (int a = 0; a < w->attr_count; a++) {
                if (strcmp(w->attrs[a].name, "bgcolor") == 0)
                    fprintf(out, "    %s->bg_color = \"%s\";\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "margin") == 0) {
                    fprintf(out, "    %s->margin_top = %s;\n", w->name, w->attrs[a].value);
                    fprintf(out, "    %s->margin_bottom = %s;\n", w->name, w->attrs[a].value);
                    fprintf(out, "    %s->margin_left = %s;\n", w->name, w->attrs[a].value);
                    fprintf(out, "    %s->margin_right = %s;\n", w->name, w->attrs[a].value);
                }
            }
            fprintf(out, "    update_box(%s);\n", w->name);
            fprintf(out, "\n");
        }
        else if (strcmp(w->type, "label") == 0) {
            fprintf(out, "    // Create label\n");
            fprintf(out, "    label *%s = create_label();\n", w->name);
            
            // Check if text is in content or attribute
            char label_text[512] = "";
            if (strlen(w->text_content) > 0) {
                char *text = trim(w->text_content);
                // Clean up the text
                char clean_text[512];
                char *p = clean_text;
                while (*text) {
                    if (*text != '\n' && *text != '\r') {
                        *p++ = *text;
                    }
                    text++;
                }
                *p = '\0';
                strcpy(label_text, clean_text);
            }
            
            for (int a = 0; a < w->attr_count; a++) {
                if (strcmp(w->attrs[a].name, "text") == 0)
                    fprintf(out, "    %s->text = \"%s\";\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "color") == 0)
                    fprintf(out, "    %s->color = \"%s\";\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "bgcolor") == 0)
                    fprintf(out, "    %s->bg_color = \"%s\";\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "font") == 0)
                    fprintf(out, "    %s->font_family = \"%s\";\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "style") == 0)
                    fprintf(out, "    %s->style = \"%s\";\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "position") == 0)
                    fprintf(out, "    %s->position = \"%s\";\n", w->name, w->attrs[a].value);
            }
            
            if (strlen(label_text) > 0) {
                fprintf(out, "    %s->text = \"%s\";\n", w->name, label_text);
            }
            
            fprintf(out, "    update_label(%s);\n", w->name);
            fprintf(out, "\n");
        }
        else if (strcmp(w->type, "button") == 0) {
            fprintf(out, "    // Create button\n");
            fprintf(out, "    button *%s = create_button();\n", w->name);
            
            // Check if label is in content or attribute
            char button_label[512] = "";
            if (strlen(w->text_content) > 0) {
                char *text = trim(w->text_content);
                char clean_text[512];
                char *p = clean_text;
                while (*text) {
                    if (*text != '\n' && *text != '\r') {
                        *p++ = *text;
                    }
                    text++;
                }
                *p = '\0';
                strcpy(button_label, clean_text);
            }
            
            for (int a = 0; a < w->attr_count; a++) {
                if (strcmp(w->attrs[a].name, "label") == 0)
                    fprintf(out, "    %s->label = \"%s\";\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "bgcolor") == 0)
                    fprintf(out, "    %s->bg_color = \"%s\";\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "width") == 0)
                    fprintf(out, "    %s->width = %s;\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "height") == 0)
                    fprintf(out, "    %s->height = %s;\n", w->name, w->attrs[a].value);
            }
            
            if (strlen(button_label) > 0) {
                fprintf(out, "    %s->label = \"%s\";\n", w->name, button_label);
            }
            
            fprintf(out, "    %s->click_callback = on_button_click;\n", w->name);
            fprintf(out, "    %s->callback_data = %s->label;\n", w->name, w->name);
            fprintf(out, "    update_button(%s);\n", w->name);
            fprintf(out, "\n");
        }
        else if (strcmp(w->type, "entry") == 0) {
            fprintf(out, "    // Create entry\n");
            fprintf(out, "    entry *%s = create_entry();\n", w->name);
            
            for (int a = 0; a < w->attr_count; a++) {
                if (strcmp(w->attrs[a].name, "placeholder") == 0)
                    fprintf(out, "    %s->placeholder_text = \"%s\";\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "width") == 0)
                    fprintf(out, "    %s->width = %s;\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "bgcolor") == 0)
                    fprintf(out, "    %s->bg_color = \"%s\";\n", w->name, w->attrs[a].value);
            }
            
            fprintf(out, "    %s->changed_callback = on_entry_changed;\n", w->name);
            fprintf(out, "    update_entry(%s);\n", w->name);
            fprintf(out, "\n");
        }
        else if (strcmp(w->type, "checkbox") == 0) {
            fprintf(out, "    // Create checkbox\n");
            fprintf(out, "    check_button *%s = create_check_button();\n", w->name);
            
            // Check if label is in content or attribute
            char checkbox_label[512] = "";
            if (strlen(w->text_content) > 0) {
                char *text = trim(w->text_content);
                char clean_text[512];
                char *p = clean_text;
                while (*text) {
                    if (*text != '\n' && *text != '\r') {
                        *p++ = *text;
                    }
                    text++;
                }
                *p = '\0';
                strcpy(checkbox_label, clean_text);
            }
            
            for (int a = 0; a < w->attr_count; a++) {
                if (strcmp(w->attrs[a].name, "label") == 0)
                    fprintf(out, "    %s->label = \"%s\";\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "active") == 0)
                    fprintf(out, "    %s->active = %s;\n", w->name,
                           strcmp(w->attrs[a].value, "true") == 0 ? "TRUE" : "FALSE");
            }
            
            if (strlen(checkbox_label) > 0) {
                fprintf(out, "    %s->label = \"%s\";\n", w->name, checkbox_label);
            }
            
            fprintf(out, "    %s->toggled_callback = on_check_toggled;\n", w->name);
            fprintf(out, "    update_check_button(%s);\n", w->name);
            fprintf(out, "\n");
        }
        else if (strcmp(w->type, "radiobutton") == 0) {
            char group[64] = "default";
            for (int a = 0; a < w->attr_count; a++) {
                if (strcmp(w->attrs[a].name, "group") == 0)
                    strcpy(group, w->attrs[a].value);
            }
            
            fprintf(out, "    // Create radio button\n");
            
            // Get the label text from content or attribute
            char radio_label[256] = "";
            if (strlen(w->text_content) > 0) {
                char *text = trim(w->text_content);
                // Clean up the text
                char clean_text[256];
                char *p = clean_text;
                while (*text) {
                    if (*text != '\n' && *text != '\r') {
                        *p++ = *text;
                    }
                    text++;
                }
                *p = '\0';
                strcpy(radio_label, clean_text);
            } else {
                // Check if label is specified as attribute
                for (int a = 0; a < w->attr_count; a++) {
                    if (strcmp(w->attrs[a].name, "label") == 0) {
                        strcpy(radio_label, w->attrs[a].value);
                        break;
                    }
                }
            }
            
            // If still empty, use a default
            if (strlen(radio_label) == 0) {
                strcpy(radio_label, "Option");
            }
            
            fprintf(out, "    radio_button *%s = create_radio_button(\"%s\", \"%s\");\n", 
                    w->name, group, radio_label);
            
            for (int a = 0; a < w->attr_count; a++) {
                if (strcmp(w->attrs[a].name, "active") == 0)
                    fprintf(out, "    %s->active = %s;\n", w->name,
                           strcmp(w->attrs[a].value, "true") == 0 ? "TRUE" : "FALSE");
            }
            
            fprintf(out, "    %s->toggled_callback = on_radio_toggled;\n", w->name);
            fprintf(out, "    %s->callback_data = %s->label;\n", w->name, w->name);
            fprintf(out, "    update_radio_button(%s);\n", w->name);
            fprintf(out, "\n");
        }
        else if (strcmp(w->type, "progressbar") == 0) {
            fprintf(out, "    // Create progress bar\n");
            fprintf(out, "    progress_bar *%s = create_progress_bar();\n", w->name);
            
            for (int a = 0; a < w->attr_count; a++) {
                if (strcmp(w->attrs[a].name, "width") == 0)
                    fprintf(out, "    %s->width = %s;\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "height") == 0)
                    fprintf(out, "    %s->height = %s;\n", w->name, w->attrs[a].value);
            }
            
            fprintf(out, "    %s->show_text = TRUE;\n", w->name);
            fprintf(out, "    update_progress_bar(%s);\n", w->name);
            fprintf(out, "\n");
        }
        else if (strcmp(w->type, "spinner") == 0) {
            fprintf(out, "    // Create spinner\n");
            fprintf(out, "    spinner *%s = create_spinner();\n", w->name);
            
            for (int a = 0; a < w->attr_count; a++) {
                if (strcmp(w->attrs[a].name, "text") == 0)
                    fprintf(out, "    %s->text = \"%s\";\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "active") == 0)
                    fprintf(out, "    %s->active = %s;\n", w->name,
                           strcmp(w->attrs[a].value, "true") == 0 ? "TRUE" : "FALSE");
            }
            
            fprintf(out, "    update_spinner(%s);\n", w->name);
            fprintf(out, "\n");
        }
        else if (strcmp(w->type, "image") == 0) {
            fprintf(out, "    // Create image\n");
            fprintf(out, "    image *%s = create_image();\n", w->name);
            
            for (int a = 0; a < w->attr_count; a++) {
                if (strcmp(w->attrs[a].name, "file") == 0)
                    fprintf(out, "    %s->file_name = \"%s\";\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "width") == 0)
                    fprintf(out, "    %s->width = %s;\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "height") == 0)
                    fprintf(out, "    %s->height = %s;\n", w->name, w->attrs[a].value);
            }
            
            fprintf(out, "    update_image(%s);\n", w->name);
            fprintf(out, "\n");
        }
        else if (strcmp(w->type, "menu") == 0) {
            // Check if it's horizontal or vertical
            int is_horizontal = 0;
            for (int a = 0; a < w->attr_count; a++) {
                if (strcmp(w->attrs[a].name, "orientation") == 0 && 
                    strcmp(w->attrs[a].value, "horizontal") == 0) {
                    is_horizontal = 1;
                    break;
                }
            }
            
            if (is_horizontal) {
                fprintf(out, "    menu *%s = create_horizontal_menu();\n", w->name);
            } else {
                fprintf(out, "    menu *%s = create_vertical_menu();\n", w->name);
            }
            
            for (int a = 0; a < w->attr_count; a++) {
                if (strcmp(w->attrs[a].name, "bgcolor") == 0)
                    fprintf(out, "    %s->bg_color = \"%s\";\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "color") == 0)
                    fprintf(out, "    %s->text_color = \"%s\";\n", w->name, w->attrs[a].value);
            }
            fprintf(out, "\n");
        }
        else if (strcmp(w->type, "menu_button") == 0) {
            fprintf(out, "    // Create menu button\n");
            
            // Get the label from content or attribute
            char button_label[512] = "";
            if (strlen(w->text_content) > 0) {
                char *text = trim(w->text_content);
                char clean_text[512];
                char *p = clean_text;
                while (*text) {
                    if (*text != '\n' && *text != '\r') {
                        *p++ = *text;
                    }
                    text++;
                }
                *p = '\0';
                strcpy(button_label, clean_text);
            }
            
            // Check if it has an icon
            int has_icon = 0;
            char icon_name[128] = "";
            for (int a = 0; a < w->attr_count; a++) {
                if (strcmp(w->attrs[a].name, "icon") == 0) {
                    strcpy(icon_name, w->attrs[a].value);
                    has_icon = 1;
                    break;
                }
            }
            
            if (has_icon) {
                fprintf(out, "    menu_button *%s = create_menu_button_with_icon(\"%s\");\n", 
                        w->name, icon_name);
            } else if (strlen(button_label) > 0) {
                fprintf(out, "    menu_button *%s = create_menu_button(\"%s\");\n", 
                        w->name, button_label);
            } else {
                // Check for label attribute
                char label_attr[512] = "";
                for (int a = 0; a < w->attr_count; a++) {
                    if (strcmp(w->attrs[a].name, "label") == 0) {
                        strcpy(label_attr, w->attrs[a].value);
                        break;
                    }
                }
                if (strlen(label_attr) > 0) {
                    fprintf(out, "    menu_button *%s = create_menu_button(\"%s\");\n", 
                            w->name, label_attr);
                } else {
                    fprintf(out, "    menu_button *%s = create_menu_button(\"Menu\");\n", w->name);
                }
            }
            
            // Apply attributes
            for (int a = 0; a < w->attr_count; a++) {
                if (strcmp(w->attrs[a].name, "width") == 0)
                    fprintf(out, "    %s->width = %s;\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "height") == 0)
                    fprintf(out, "    %s->height = %s;\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "bgcolor") == 0)
                    fprintf(out, "    %s->bg_color = \"%s\";\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "color") == 0)
                    fprintf(out, "    %s->text_color = \"%s\";\n", w->name, w->attrs[a].value);
                else if (strcmp(w->attrs[a].name, "position") == 0)
                    fprintf(out, "    %s->position = \"%s\";\n", w->name, w->attrs[a].value);
            }
            
            fprintf(out, "    update_menu_button(%s);\n", w->name);
            fprintf(out, "\n");
            
            // Look for menu that has this menu_button as parent
            for (int j = 0; j < parser->widget_count; j++) {
                if (strcmp(parser->widgets[j].type, "menu") == 0 &&
                    strcmp(parser->widgets[j].parent, w->name) == 0) {
                    strcpy(links[link_count].button_name, w->name);
                    strcpy(links[link_count].menu_name, parser->widgets[j].name);
                    link_count++;
                    break;
                }
            }
        }
        else if (strcmp(w->type, "menu_item") == 0) {
            // Get the label for this menu item
            char item_label[512] = "";
            if (strlen(w->text_content) > 0) {
                char *text = trim(w->text_content);
                char clean_text[512];
                char *p = clean_text;
                while (*text) {
                    if (*text != '\n' && *text != '\r') {
                        *p++ = *text;
                    }
                    text++;
                }
                *p = '\0';
                strcpy(item_label, clean_text);
            } else {
                for (int a = 0; a < w->attr_count; a++) {
                    if (strcmp(w->attrs[a].name, "label") == 0) {
                        strcpy(item_label, w->attrs[a].value);
                        break;
                    }
                }
            }
            
            if (strlen(item_label) == 0) strcpy(item_label, "Item");
            
            // Check if this menu item has a submenu (a menu as a child)
            for (int j = 0; j < parser->widget_count; j++) {
                if (strcmp(parser->widgets[j].type, "menu") == 0 &&
                    strcmp(parser->widgets[j].parent, w->name) == 0) {
                    strcpy(submenu_links[submenu_count].parent_item, w->name);
                    strcpy(submenu_links[submenu_count].submenu_name, parser->widgets[j].name);
                    submenu_count++;
                    break;
                }
            }
            
            fprintf(out, "    // Menu item %s will be added in second pass\n", w->name);
        }
        else if (strcmp(w->type, "menu_separator") == 0) {
            fprintf(out, "    // Menu separator will be added in second pass\n");
        }
    }
    
    // Second pass: Add widgets to their parents and link menus
    fprintf(out, "\n    // Add widgets to containers\n");
    
    // Find main window
    char main_window[64] = "main_window";
    for (int i = 0; i < parser->widget_count; i++) {
        if (strcmp(parser->widgets[i].type, "window") == 0) {
            strcpy(main_window, parser->widgets[i].name);
            break;
        }
    }
    
    // Link menu buttons to their menus
    if (link_count > 0) {
        fprintf(out, "\n    // Link menu buttons to their menus\n");
        for (int i = 0; i < link_count; i++) {
            fprintf(out, "    set_menu_button_menu(%s, %s);\n", 
                    links[i].button_name, links[i].menu_name);
        }
    }
    
    // First, add all menu items and separators to their parent menus
    fprintf(out, "\n    // Add menu items to menus\n");
    for (int i = 0; i < parser->widget_count; i++) {
        Widget *w = &parser->widgets[i];
        
        if (strcmp(w->type, "menu_item") == 0) {
            // Get the label
            char item_label[512] = "";
            if (strlen(w->text_content) > 0) {
                char *text = trim(w->text_content);
                char clean_text[512];
                char *p = clean_text;
                while (*text) {
                    if (*text != '\n' && *text != '\r') {
                        *p++ = *text;
                    }
                    text++;
                }
                *p = '\0';
                strcpy(item_label, clean_text);
            } else {
                for (int a = 0; a < w->attr_count; a++) {
                    if (strcmp(w->attrs[a].name, "label") == 0) {
                        strcpy(item_label, w->attrs[a].value);
                        break;
                    }
                }
            }
            
            if (strlen(item_label) == 0) strcpy(item_label, "Item");
            
            // Check if it's a check menu item
            int is_check = 0;
            for (int a = 0; a < w->attr_count; a++) {
                if (strcmp(w->attrs[a].name, "check") == 0 && 
                    strcmp(w->attrs[a].value, "true") == 0) {
                    is_check = 1;
                    break;
                }
            }
            
            // Check if this menu item has a submenu
            int has_submenu = 0;
            char submenu_name[64] = "";
            for (int s = 0; s < submenu_count; s++) {
                if (strcmp(submenu_links[s].parent_item, w->name) == 0) {
                    has_submenu = 1;
                    strcpy(submenu_name, submenu_links[s].submenu_name);
                    break;
                }
            }
            
            if (has_submenu) {
                // This menu item has a submenu - create it with submenu
                fprintf(out, "    {\n");
                fprintf(out, "        GtkWidget *menu_item = gtk_menu_item_new_with_label(\"%s\");\n", item_label);
                fprintf(out, "        gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item), %s->menu_widget);\n", submenu_name);
                fprintf(out, "        gtk_menu_shell_append(GTK_MENU_SHELL(%s->menu_widget), menu_item);\n", w->parent);
                fprintf(out, "        gtk_widget_show(menu_item);\n");
                fprintf(out, "    }\n");
            } else if (is_check) {
                fprintf(out, "    add_check_menu_item(%s, \"%s\", FALSE, on_menu_item_clicked, \"%s\");\n", 
                        w->parent, item_label, item_label);
            } else {
                fprintf(out, "    add_menu_item(%s, \"%s\", on_menu_item_clicked, \"%s\");\n", 
                        w->parent, item_label, item_label);
            }
        }
        else if (strcmp(w->type, "menu_separator") == 0) {
            if (strcmp(w->parent, "none") != 0) {
                fprintf(out, "    add_menu_separator(%s);\n", w->parent);
            }
        }
    }
    
    // Add other widgets to containers
    fprintf(out, "\n    // Add other widgets to containers\n");
    for (int i = 0; i < parser->widget_count; i++) {
        Widget *w = &parser->widgets[i];
        
        // Skip window and headerbar as they're added separately
        if (strcmp(w->type, "window") == 0) continue;
        
        // Add headerbar to window
        if (strcmp(w->type, "headerbar") == 0 || strcmp(w->type, "headbar") == 0) {
            if (strcmp(w->parent, "none") != 0) {
                fprintf(out, "    ADD_TO_WINDOW(%s, %s->header_bar);\n", w->parent, w->name);
            } else {
                fprintf(out, "    ADD_TO_WINDOW(%s, %s->header_bar);\n", main_window, w->name);
            }
        }
        // Add boxes to window or parent boxes
        else if (strcmp(w->type, "vbox") == 0 || strcmp(w->type, "hbox") == 0) {
            if (strcmp(w->parent, "none") != 0) {
                // Check if parent is a window or another box
                int parent_is_window = 0;
                for (int j = 0; j < parser->widget_count; j++) {
                    if (strcmp(parser->widgets[j].name, w->parent) == 0 &&
                        strcmp(parser->widgets[j].type, "window") == 0) {
                        parent_is_window = 1;
                        break;
                    }
                }
                
                if (parent_is_window) {
                    fprintf(out, "    ADD_TO_WINDOW(%s, get_box_widget(%s));\n", w->parent, w->name);
                } else {
                    fprintf(out, "    add_to_box(%s, get_box_widget(%s));\n", w->parent, w->name);
                }
            } else {
                fprintf(out, "    ADD_TO_WINDOW(%s, get_box_widget(%s));\n", main_window, w->name);
            }
        }
        // Add menu buttons to their parent boxes
        else if (strcmp(w->type, "menu_button") == 0) {
            if (strcmp(w->parent, "none") != 0) {
                fprintf(out, "    add_to_box(%s, %s->button);\n", w->parent, w->name);
            }
        }
        // Add other widgets to their parent boxes
        else if (strcmp(w->type, "label") == 0) {
            if (strcmp(w->parent, "none") != 0) {
                fprintf(out, "    add_to_box(%s, %s->label);\n", w->parent, w->name);
            }
        }
        else if (strcmp(w->type, "button") == 0) {
            if (strcmp(w->parent, "none") != 0) {
                fprintf(out, "    add_to_box(%s, %s->button);\n", w->parent, w->name);
            }
        }
        else if (strcmp(w->type, "entry") == 0) {
            if (strcmp(w->parent, "none") != 0) {
                fprintf(out, "    add_to_box(%s, %s->entry);\n", w->parent, w->name);
            }
        }
        else if (strcmp(w->type, "checkbox") == 0) {
            if (strcmp(w->parent, "none") != 0) {
                fprintf(out, "    add_to_box(%s, %s->checkbutton);\n", w->parent, w->name);
            }
        }
        else if (strcmp(w->type, "radiobutton") == 0) {
            if (strcmp(w->parent, "none") != 0) {
                fprintf(out, "    add_to_box(%s, %s->radio_button);\n", w->parent, w->name);
            }
        }
        else if (strcmp(w->type, "progressbar") == 0) {
            if (strcmp(w->parent, "none") != 0) {
                fprintf(out, "    add_to_box(%s, %s->progress);\n", w->parent, w->name);
            }
        }
        else if (strcmp(w->type, "spinner") == 0) {
            if (strcmp(w->parent, "none") != 0) {
                fprintf(out, "    add_to_box(%s, get_spinner_widget(%s));\n", w->parent, w->name);
            }
        }
        else if (strcmp(w->type, "image") == 0) {
            if (strcmp(w->parent, "none") != 0) {
                fprintf(out, "    add_to_box(%s, %s->image);\n", w->parent, w->name);
            }
        }
    }
    
    // Add main loop
    fprintf(out, "\n    // Display window and start main loop\n");
    fprintf(out, "    gtk_widget_show_all(%s->win);\n", main_window);
    fprintf(out, "    g_signal_connect(%s->win, \"destroy\", G_CALLBACK(gtk_main_quit), NULL);\n", main_window);
    fprintf(out, "    MainStart();\n");
    fprintf(out, "    ProgramEnd();\n\n");
    fprintf(out, "    return 0;\n");
    fprintf(out, "}\n");
    
    fclose(out);
    printf("C code generated: %s\n", output_c);
}

void compile_and_run(const char *c_file, const char *exe_name) {
    char command[1024];
    
    // Check if structures.c exists
    if (access("structures.c", F_OK) != 0) {
        fprintf(stderr, "\nError: structures.c not found!\n");
        fprintf(stderr, "Make sure structures.c is in the current directory.\n");
        return;
    }
    
    // Compile
    printf("\nCompiling...\n");
    snprintf(command, sizeof(command), 
             "gcc -o %s %s structures.c `pkg-config --cflags --libs gtk+-3.0` -lm 2>&1",
             exe_name, c_file);
    
    printf("Command: %s\n", command);
    int result = system(command);
    
    if (result == 0) {
        printf("\n✓ Success! Executable created: %s\n", exe_name);
        printf("\nTo run: ./%s\n", exe_name);
        
        // Ask if user wants to run
        printf("\nRun now? (y/n): ");
        char answer[10];
        if (fgets(answer, sizeof(answer), stdin)) {
            if (answer[0] == 'y' || answer[0] == 'Y') {
                snprintf(command, sizeof(command), "./%s", exe_name);
                system(command);
            }
        }
    } else {
        printf("\n✗ Compilation failed!\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("GTK HTML/XML Parser - Direct Executable Generator\n");
        printf("==================================================\n\n");
        printf("Usage: %s <input.html> [output_name]\n\n", argv[0]);
        printf("Example HTML file with submenu (test.html):\n");
        printf("------------------------------------\n");
        printf("<window title=\"My App\" width=\"500\" height=\"400\">\n");
        printf("  <vbox spacing=\"10\">\n");
        printf("    <menu_button label=\"File\">\n");
        printf("      <menu orientation=\"vertical\">\n");
        printf("        <menu_item label=\"New\"/>\n");
        printf("        <menu_item label=\"Open\">\n");
        printf("          <menu orientation=\"vertical\">\n");
        printf("            <menu_item label=\"Recent File 1\"/>\n");
        printf("            <menu_item label=\"Recent File 2\"/>\n");
        printf("          </menu>\n");
        printf("        </menu_item>\n");
        printf("        <menu_separator/>\n");
        printf("        <menu_item label=\"Exit\"/>\n");
        printf("      </menu>\n");
        printf("    </menu_button>\n");
        printf("  </vbox>\n");
        printf("</window>\n");
        printf("------------------------------------\n\n");
        printf("Supported tags: window, headerbar, vbox, hbox, label, button,\n");
        printf("               entry, checkbox, radiobutton, progressbar, spinner, image,\n");
        printf("               menu, menu_button, menu_item, menu_separator\n");
        printf("Note: menu_item can contain a nested menu for submenus!\n");
        return 1;
    }
    
    char *input_file = argv[1];
    char *exe_name = argc > 2 ? argv[2] : "gtk_app";
    char c_file[512];
    snprintf(c_file, sizeof(c_file), "%s.c", exe_name);
    
    // Check if input file exists
    if (access(input_file, F_OK) != 0) {
        fprintf(stderr, "Error: File '%s' does not exist!\n", input_file);
        return 1;
    }
    
    printf("\nParsing %s...\n", input_file);
    
    Parser parser = {0};
    if (parse_html(input_file, &parser) != 0) {
        return 1;
    }
    
    printf("Found %d widgets\n", parser.widget_count);
    
    generate_c_code(&parser, c_file);
    
    compile_and_run(c_file, exe_name);
    
    // Cleanup C file if desired
    printf("\nCleanup generated .c file? (y/n): ");
    char answer[10];
    if (fgets(answer, sizeof(answer), stdin)) {
        if (answer[0] == 'y' || answer[0] == 'Y') {
            unlink(c_file);
            printf("Removed %s\n", c_file);
        }
    }
    
    return 0;
}