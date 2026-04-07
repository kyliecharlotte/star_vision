#ifndef DISPLAY_H
#define DISPLAY_H

#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/filechooserdialog.h>

class DisplayWindow : public Gtk::Window {
    public:
        DisplayWindow();
};

class Grid : public Gtk::Grid {
    public:
        Grid();
    
    protected:
        void on_button_press();
        Gtk::Button button;
    };

#endif