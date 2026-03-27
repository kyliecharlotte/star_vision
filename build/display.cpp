#include "display.h"
#include <iostream>

DisplayWindow::DisplayWindow() : Gtk::Window() {
    set_title("Working Window");
    set_default_size(200,200);
    
    /*button.set_label("Click");
    button.signal_clicked().connect(sigc::mem_fun(*this, 
        &DisplayWindow::on_button_press));
    add(button);*/

    show_all_children();
};

void Grid::on_button_press() {
    std::cout << "hello world" << std::endl;
};

Grid::Grid() : Gtk::Grid() {
    button.set_label("Click");
    button.signal_clicked().connect(sigc::mem_fun(*this, 
        &Grid::on_button_press));
    attach(button,5,5,10,10);
};


