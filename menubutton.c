#include "structures.h"

// Callback functions
void on_button_click(GtkWidget *widget, gpointer data) {
    g_print("Button clicked! Data: %s\n", (char*)data);
}

void on_entry_changed(GtkWidget *widget, gpointer data) {
    entry *en = (entry*)data;
    g_print("Entry text: %s\n", get_entry_text(en));
}

int main(int argc, char *argv[]) {
    ProgramStart();
    
    // Create window
    window *win = create_window();
    win->title = "My Application";
    win->width = 600;
    win->height = 400;
    win->resizable = TRUE;
    win->bg_color = "#f0f0f0";
    
    // Create header bar
    header_bar *hdb = create_header_bar();
    hdb->title = "My App";
    hdb->title_style = "bold";
    hdb->close_button_show = TRUE;
    
    // Create a button
    button *btn = create_button();
    btn->label = "Click Me";
    btn->bg_color = "#4CAF50";
    btn->label_style = "bold";
    btn->width = 120;
    btn->height = 40;
    btn->click_callback = on_button_click;
    btn->callback_data = "Hello from button";
    
    // Create a label
    label *lb = create_label();
    lb->text = "Welcome to my app!";
    lb->color = "#333333";
    lb->font_family = "Arial";
    lb->style = "bold";
    lb->position = "center";
    
    // Create an entry
    entry *en = create_entry();
    en->placeholder_text = "Enter text here...";
    en->width = 200;
    en->height = 35;
    en->bg_color = "#ffffff";
    en->color = "#000000";
    en->changed_callback = on_entry_changed;
    en->callback_data = en;
    
    // Create a progress bar
    progress_bar *pb = create_progress_bar();
    pb->width = 300;
    pb->height = 25;
    pb->show_text = TRUE;
    
    // Create a spinner
    spinner *spin = create_spinner();
    spin->text = "Loading...";
    spin->active = TRUE;
    
    // Create a box to organize widgets
    box *vbox = create_vbox(10, FALSE);
    vbox->margin_top = 20;
    vbox->margin_bottom = 20;
    vbox->margin_left = 20;
    vbox->margin_right = 20;
    
    // Add widgets to box
    add_to_box(vbox, lb->label);
    add_to_box(vbox, en->entry);
    add_to_box(vbox, btn->button);
    add_to_box(vbox, pb->progress);
    add_to_box(vbox, get_spinner_widget(spin));
    
    // Update all widgets (apply their properties)
    update_window(win);
    update_header_bar(hdb);
    update_button(btn);
    update_label(lb);
    update_entry(en);
    update_progress_bar(pb);
    update_spinner(spin);
    update_box(vbox);
    
    // Add to window
    ADD_TO_HEADER_START(hdb, get_spinner_widget(spin));
    ADD_TO_HEADER_END(hdb, btn->button);
    ADD_TO_WINDOW(win, get_box_widget(vbox));
    ADD_TO_WINDOW(win, hdb->header_bar);
    
    // Start progress bar pulse
    start_progress_pulse(pb);
    
    // Set progress value after 3 seconds
    // This would need proper implementation - just an example
    
    // Display window
    display_window(win);
    display_header_bar(hdb);
    
    MainStart();
    ProgramEnd();
    
    return 0;
}