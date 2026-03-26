#include "display.h"
#include <iostream>

DisplayWindow::DisplayWindow() : Gtk::Window() {
    set_title("Working Window");
    button.set_label("Click");
    button.signal_clicked().connect(sigc::mem_fun(*this, 
        &DisplayWindow::on_button_press));
    add(button);
    show_all_children();
}

void DisplayWindow::on_button_press() {
    std::cout << "hello world" << std::endl;
}


