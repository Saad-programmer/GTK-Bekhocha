#include "structures.h"

// Menu callbacks
void on_new_file(GtkWidget *widget, gpointer data) {
    g_print("File -> New\n");
}

void on_open_file(GtkWidget *widget, gpointer data) {
    g_print("File -> Open: %s\n", (char*)data);
}

void on_save_file(GtkWidget *widget, gpointer data) {
    g_print("File -> Save\n");
}

void on_save_as(GtkWidget *widget, gpointer data) {
    g_print("File -> Save As\n");
}

void on_quit(GtkWidget *widget, gpointer data) {
    g_print("Quitting...\n");
    ProgramEnd();
}

void on_undo(GtkWidget *widget, gpointer data) {
    g_print("Edit -> Undo\n");
}

void on_redo(GtkWidget *widget, gpointer data) {
    g_print("Edit -> Redo\n");
}

void on_cut(GtkWidget *widget, gpointer data) {
    g_print("Edit -> Cut\n");
}

void on_copy(GtkWidget *widget, gpointer data) {
    g_print("Edit -> Copy\n");
}

void on_paste(GtkWidget *widget, gpointer data) {
    g_print("Edit -> Paste\n");
}

void on_zoom_in(GtkWidget *widget, gpointer data) {
    g_print("View -> Zoom In\n");
}

void on_zoom_out(GtkWidget *widget, gpointer data) {
    g_print("View -> Zoom Out\n");
}

void on_fullscreen_toggled(GtkWidget *widget, gpointer data) {
    gboolean active = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));
    g_print("Fullscreen: %s\n", active ? "ON" : "OFF");
}

void on_show_statusbar_toggled(GtkWidget *widget, gpointer data) {
    gboolean active = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));
    g_print("Show Statusbar: %s\n", active ? "ON" : "OFF");
}

void on_view_mode_changed(GtkWidget *widget, gpointer data) {
    if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) {
        g_print("View Mode: %s\n", (char*)data);
    }
}

void on_about(GtkWidget *widget, gpointer data) {
    g_print("Help -> About\n");
}

void on_documentation(GtkWidget *widget, gpointer data) {
    g_print("Help -> Documentation\n");
}

// Callbacks for project1.gtk submenu items
void on_open_readonly(GtkWidget *widget, gpointer data) {
    g_print("Opening project1.gtk in read-only mode\n");
}

void on_open_with_backup(GtkWidget *widget, gpointer data) {
    g_print("Opening project1.gtk with backup\n");
}

void on_project_properties(GtkWidget *widget, gpointer data) {
    g_print("Showing properties for project1.gtk\n");
}

void on_export_project(GtkWidget *widget, gpointer data) {
    g_print("Exporting project1.gtk\n");
}

void on_duplicate_project(GtkWidget *widget, gpointer data) {
    g_print("Duplicating project1.gtk\n");
}

void on_delete_project(GtkWidget *widget, gpointer data) {
    g_print("Deleting project1.gtk\n");
}

// Button callbacks
void on_button_click(GtkWidget *widget, gpointer data) {
    g_print("Button clicked!\n");
}

void on_check_toggled(GtkWidget *widget, gpointer data) {
    check_button *chb = (check_button*)data;
    g_print("Check button is now: %s\n", is_check_button_active(chb) ? "Checked" : "Unchecked");
}

void on_radio_toggled(GtkWidget *widget, gpointer data) {
    radio_button *rdb = (radio_button*)data;
    if (is_radio_button_active(rdb)) {
        g_print("Selected: %s\n", rdb->label);
    }
}

void on_entry_changed(GtkWidget *widget, gpointer data) {
    entry *en = (entry*)data;
    g_print("Entry text: %s\n", get_entry_text(en));
}

int main(int argc, char *argv[]) {
    ProgramStart();
    
    // Create main window
    window *win = create_window();
    win->title = "Complete GTK Application";
    win->width = 900;
    win->height = 700;
    win->resizable = TRUE;
    win->bg_color = "#f5f5f5";
    update_window(win);
    
    // Create header bar
    header_bar *hdb = create_header_bar();
    hdb->title = "My Application";
    hdb->title_style = "bold";
    hdb->close_button_show = TRUE;
    update_header_bar(hdb);
    
    // Create main vertical box
    box *main_vbox = create_vbox(0, FALSE);
    
    // ===== HORIZONTAL MENU BAR =====
    menu *menubar = create_horizontal_menu();
    
    // ----- File Menu -----
    menu *file_menu = create_submenu();
    add_menu_item_with_icon(file_menu, "New", "document-new", on_new_file, NULL);
    add_menu_item_with_icon(file_menu, "Open", "document-open", on_open_file, "example.txt");
    add_menu_item_with_icon(file_menu, "Save", "document-save", on_save_file, NULL);
    add_menu_item_with_icon(file_menu, "Save As", "document-save-as", on_save_as, NULL);
    add_menu_separator(file_menu);
    
    // Recent Files submenu with project1.gtk having its own submenu
    menu *recent_menu = create_submenu();
    
    // Create submenu for project1.gtk
    menu *project1_submenu = create_submenu();
    add_menu_item_with_icon(project1_submenu, "Open Read-Only", "object-locked", on_open_readonly, NULL);
    add_menu_item_with_icon(project1_submenu, "Open with Backup", "document-save", on_open_with_backup, NULL);
    add_menu_separator(project1_submenu);
    add_menu_item_with_icon(project1_submenu, "Properties", "document-properties", on_project_properties, NULL);
    add_menu_item_with_icon(project1_submenu, "Export", "document-export", on_export_project, NULL);
    add_menu_item_with_icon(project1_submenu, "Duplicate", "edit-copy", on_duplicate_project, NULL);
    add_menu_separator(project1_submenu);
    add_menu_item_with_icon(project1_submenu, "Delete", "edit-delete", on_delete_project, NULL);
    
    // Add project1.gtk with its submenu
    add_submenu_with_icon(recent_menu, "project1.gtk", "application-x-gtk", project1_submenu);
    
    // Add other recent files as simple items
    add_menu_item_with_icon(recent_menu, "project2.gtk", "application-x-gtk", on_open_file, "project2.gtk");
    add_menu_item_with_icon(recent_menu, "project3.gtk", "application-x-gtk", on_open_file, "project3.gtk");
    add_menu_separator(recent_menu);
    add_menu_item(recent_menu, "Clear Recent Files", on_open_file, "Clear");
    
    add_submenu_with_icon(file_menu, "Recent Files", "document-open-recent", recent_menu);
    
    add_menu_separator(file_menu);
    add_menu_item_with_icon(file_menu, "Quit", "application-exit", on_quit, NULL);
    add_submenu(menubar, "File", file_menu);
    
    // ----- Edit Menu -----
    menu *edit_menu = create_submenu();
    add_menu_item_with_icon(edit_menu, "Undo", "edit-undo", on_undo, NULL);
    add_menu_item_with_icon(edit_menu, "Redo", "edit-redo", on_redo, NULL);
    add_menu_separator(edit_menu);
    add_menu_item_with_icon(edit_menu, "Cut", "edit-cut", on_cut, NULL);
    add_menu_item_with_icon(edit_menu, "Copy", "edit-copy", on_copy, NULL);
    add_menu_item_with_icon(edit_menu, "Paste", "edit-paste", on_paste, NULL);
    add_submenu(menubar, "Edit", edit_menu);
    
    // ----- View Menu with Check Items -----
    menu *view_menu = create_submenu();
    add_menu_item_with_icon(view_menu, "Zoom In", "zoom-in", on_zoom_in, NULL);
    add_menu_item_with_icon(view_menu, "Zoom Out", "zoom-out", on_zoom_out, NULL);
    add_menu_separator(view_menu);
    add_check_menu_item(view_menu, "Fullscreen", FALSE, on_fullscreen_toggled, NULL);
    add_check_menu_item(view_menu, "Show Statusbar", TRUE, on_show_statusbar_toggled, NULL);
    add_menu_separator(view_menu);
    
    // Radio items for view mode
    add_radio_menu_item(view_menu, "Normal View", TRUE, on_view_mode_changed, "Normal");
    add_radio_menu_item(view_menu, "Compact View", FALSE, on_view_mode_changed, "Compact");
    add_radio_menu_item(view_menu, "List View", FALSE, on_view_mode_changed, "List");
    add_submenu(menubar, "View", view_menu);
    
    // ----- Help Menu -----
    menu *help_menu = create_submenu();
    add_menu_item_with_icon(help_menu, "Documentation", "help-contents", on_documentation, NULL);
    add_menu_separator(help_menu);
    add_menu_item_with_icon(help_menu, "About", "help-about", on_about, NULL);
    add_submenu(menubar, "Help", help_menu);
    
    // Add menubar to box
    box *menu_box = create_hbox(0, FALSE);
    add_to_box(menu_box, menubar->menu_widget);
    
    // ===== TOOLBAR WITH MENU BUTTONS =====
    box *toolbar = create_hbox(5, FALSE);
    toolbar->margin_top = 5;
    toolbar->margin_bottom = 5;
    toolbar->margin_left = 10;
    toolbar->margin_right = 10;
    toolbar->bg_color = "#e0e0e0";
    update_box(toolbar);
    
    // File menu button
    menu_button *file_btn = create_menu_button_with_icon("document-open");
    file_btn->label = "File";
    set_menu_button_menu(file_btn, file_menu);
    file_btn->bg_color = "#d0d0d0";
    update_menu_button(file_btn);
    add_menu_button_to_box(toolbar, file_btn);
    
    // Edit menu button
    menu_button *edit_btn = create_menu_button_with_icon("edit");
    edit_btn->label = "Edit";
    set_menu_button_menu(edit_btn, edit_menu);
    edit_btn->bg_color = "#d0d0d0";
    update_menu_button(edit_btn);
    add_menu_button_to_box(toolbar, edit_btn);
    
    // View menu button
    menu_button *view_btn = create_menu_button("View");
    set_menu_button_menu(view_btn, view_menu);
    view_btn->bg_color = "#d0d0d0";
    update_menu_button(view_btn);
    add_menu_button_to_box(toolbar, view_btn);
    
    // Separator
    GtkWidget *sep = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    add_to_box(toolbar, sep);
    
    // Regular button
    button *action_btn = create_button();
    action_btn->label = "Action";
    action_btn->bg_color = "#4CAF50";
    action_btn->label_style = "bold";
    action_btn->width = 100;
    action_btn->height = 30;
    action_btn->click_callback = on_button_click;
    update_button(action_btn);
    add_to_box(toolbar, action_btn->button);
    
    // ===== CONTENT AREA =====
    box *content_box = create_vbox(15, FALSE);
    content_box->margin_top = 20;
    content_box->margin_bottom = 20;
    content_box->margin_left = 20;
    content_box->margin_right = 20;
    update_box(content_box);
    
    // Title label
    label *title_label = create_label();
    title_label->text = "Welcome to the Complete GTK Application";
    title_label->style = "bold";
    title_label->color = "#2c3e50";
    title_label->font_family = "Arial";
    title_label->width = 500;
    update_label(title_label);
    add_to_box(content_box, title_label->label);
    
    // Description
    label *desc_label = create_label();
    desc_label->text = "This demo showcases all the available widgets and features:\n\n"
                       "• Horizontal Menu Bar with nested submenus\n"
                       "• project1.gtk has its own submenu with multiple options\n"
                       "• Menu Buttons in toolbar\n"
                       "• Radio buttons, Check buttons, Entry fields\n"
                       "• Progress bar and Spinner\n"
                       "• Customizable styling with CSS\n\n"
                       "Try clicking File -> Recent Files -> project1.gtk to see the nested submenu!";
    desc_label->color = "#34495e";
    desc_label->position = "center";
    update_label(desc_label);
    add_to_box(content_box, desc_label->label);
    
    // Entry field
    label *entry_label = create_label();
    entry_label->text = "Enter your name:";
    entry_label->style = "bold";
    update_label(entry_label);
    add_to_box(content_box, entry_label->label);
    
    entry *name_entry = create_entry();
    name_entry->placeholder_text = "Type here...";
    name_entry->width = 300;
    name_entry->height = 35;
    name_entry->bg_color = "#ffffff";
    name_entry->color = "#000000";
    name_entry->changed_callback = on_entry_changed;
    name_entry->callback_data = name_entry;
    update_entry(name_entry);
    add_to_box(content_box, name_entry->entry);
    
    // Radio buttons group
    label *radio_label = create_label();
    radio_label->text = "Choose an option:";
    radio_label->style = "bold";
    update_label(radio_label);
    add_to_box(content_box, radio_label->label);
    
    box *radio_box = create_hbox(10, FALSE);
    radio_box->margin_left = 20;
    update_box(radio_box);
    
    radio_button *option1 = create_radio_button("options", "Option 1");
    option1->toggled_callback = on_radio_toggled;
    option1->callback_data = option1;
    update_radio_button(option1);
    add_to_box(radio_box, option1->radio_button);
    
    radio_button *option2 = create_radio_button("options", "Option 2");
    option2->toggled_callback = on_radio_toggled;
    option2->callback_data = option2;
    update_radio_button(option2);
    add_to_box(radio_box, option2->radio_button);
    
    radio_button *option3 = create_radio_button("options", "Option 3");
    option3->toggled_callback = on_radio_toggled;
    option3->callback_data = option3;
    update_radio_button(option3);
    add_to_box(radio_box, option3->radio_button);
    
    set_radio_button_active(option1, TRUE);
    add_to_box(content_box, radio_box->box);
    
    // Check button
    check_button *feature_chb = create_check_button();
    feature_chb->label = "Enable Advanced Features";
    feature_chb->toggled_callback = on_check_toggled;
    feature_chb->callback_data = feature_chb;
    update_check_button(feature_chb);
    add_to_box(content_box, feature_chb->checkbutton);
    
    // Progress bar
    label *progress_label = create_label();
    progress_label->text = "Loading Progress:";
    progress_label->style = "bold";
    update_label(progress_label);
    add_to_box(content_box, progress_label->label);
    
    progress_bar *pb = create_progress_bar();
    pb->width = 400;
    pb->height = 25;
    pb->show_text = TRUE;
    update_progress_bar(pb);
    add_to_box(content_box, pb->progress);
    
    // Spinner
    spinner *spin = create_spinner();
    spin->text = "Processing...";
    update_spinner(spin);
    add_to_box(content_box, get_spinner_widget(spin));
    
    // Animate progress bar
    start_progress_pulse(pb);
    start_spinner(spin);
    
    // Simulate progress update after 3 seconds
    g_timeout_add(3000, (GSourceFunc)set_progress_value, pb);
    g_timeout_add(5000, (GSourceFunc)stop_progress_pulse, pb);
    g_timeout_add(5000, (GSourceFunc)stop_spinner, spin);
    
    // Add everything to main box
    add_to_box(main_vbox, menu_box->box);
    add_to_box(main_vbox, toolbar->box);
    add_to_box(main_vbox, content_box->box);
    
    // Add to window
    update_box(main_vbox);
    ADD_TO_WINDOW(win, get_box_widget(main_vbox));
    ADD_TO_WINDOW(win, hdb->header_bar);
    
    // Connect close handler
    close_window(win);
    
    // Display
    display_window(win);
    display_header_bar(hdb);
    
    MainStart();
    ProgramEnd();
    
    return 0;
}