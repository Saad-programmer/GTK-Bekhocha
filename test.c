#include "structures.h"

void on_color_toggled(GtkWidget *widget, gpointer data) {
    radio_button *rdb = (radio_button*)data;
    if (is_radio_button_active(rdb)) {
        g_print("Selected color: %s\n", rdb->label);
    }
}

void on_size_toggled(GtkWidget *widget, gpointer data) {
    radio_button *rdb = (radio_button*)data;
    if (is_radio_button_active(rdb)) {
        g_print("Selected size: %s\n", rdb->label);
    }
}

int main(int argc, char *argv[]) {
    ProgramStart();
    
    // Create window
    window *win = create_window();
    win->title = "Radio Button Groups";
    win->width = 350;
    win->height = 300;
    win->resizable = TRUE;
    
    // Create header bar
    header_bar *hdb = create_header_bar();
    hdb->title = "Radio Button Demo";
    hdb->close_button_show = TRUE;
    update_header_bar(hdb);
    
    // Create main box
    box *main_box = create_vbox(10, FALSE);
    main_box->margin_top = 20;
    main_box->margin_bottom = 20;
    main_box->margin_left = 20;
    main_box->margin_right = 20;
    
    // ===== GROUP 1: Colors =====
    label *color_title = create_label();
    color_title->text = "Choose a color:";
    color_title->style = "bold";
    update_label(color_title);
    add_to_box(main_box, color_title->label);
    
    // Create radio buttons for colors (group "colors")
    radio_button *red = create_radio_button("colors", "Red");
    red->toggled_callback = on_color_toggled;
    red->callback_data = red;
    update_radio_button(red);
    add_to_box(main_box, red->radio_button);
    
    radio_button *green = create_radio_button("colors", "Green");
    green->toggled_callback = on_color_toggled;
    green->callback_data = green;
    update_radio_button(green);
    add_to_box(main_box, green->radio_button);
    
    radio_button *blue = create_radio_button("colors", "Blue");
    blue->toggled_callback = on_color_toggled;
    blue->callback_data = blue;
    update_radio_button(blue);
    add_to_box(main_box, blue->radio_button);
    
    // Set default selection
    set_radio_button_active(red, TRUE);
    
    // Add separator
    GtkWidget *sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    add_to_box(main_box, sep);
    
    // ===== GROUP 2: Sizes =====
    label *size_title = create_label();
    size_title->text = "Choose a size:";
    size_title->style = "bold";
    update_label(size_title);
    add_to_box(main_box, size_title->label);
    
    // Create radio buttons for sizes (group "sizes")
    radio_button *small = create_radio_button("sizes", "Small");
    small->toggled_callback = on_size_toggled;
    small->callback_data = small;
    update_radio_button(small);
    add_to_box(main_box, small->radio_button);
    
    radio_button *medium = create_radio_button("sizes", "Medium");
    medium->toggled_callback = on_size_toggled;
    medium->callback_data = medium;
    update_radio_button(medium);
    add_to_box(main_box, medium->radio_button);
    
    radio_button *large = create_radio_button("sizes", "Large");
    large->toggled_callback = on_size_toggled;
    large->callback_data = large;
    update_radio_button(large);
    add_to_box(main_box, large->radio_button);
    
    // Set default selection
    set_radio_button_active(medium, TRUE);
    
    // Display everything
    update_box(main_box);
    update_window(win);
    
    ADD_TO_WINDOW(win, get_box_widget(main_box));
    ADD_TO_WINDOW(win, hdb->header_bar);
    
    display_window(win);
    
    MainStart();
    ProgramEnd();
    
    return 0;
}