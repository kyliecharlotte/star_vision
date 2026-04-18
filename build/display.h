#ifndef DISPLAY_H
#define DISPLAY_H

#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gdkmm/pixbuf.h>
#include <gtkmm/image.h>
#include <gtkmm/filechooserdialog.h>

class DisplayWindow : public Gtk::Window {
    public:
        DisplayWindow();
};

class Grid : public Gtk::Grid {
    public:
        Grid();
        void set_image(const std::string& filename);
    
    private:
        Gtk::Paned paned{Gtk::ORIENTATION_HORIZONTAL};
        Gtk::Paned left_paned{Gtk::ORIENTATION_HORIZONTAL};
        Gtk::Box left_panel{Gtk::ORIENTATION_VERTICAL};
        Gtk::Box left_bottom{Gtk::ORIENTATION_VERTICAL};
        Gtk::Box right_panel{Gtk::ORIENTATION_VERTICAL};

    protected:

        void on_button_press();
        void choose_file_button_press();
        Gtk::Widget* create_image_panel();

        Gtk::Button choose_file_button;
        Gtk::ToggleButton button;

        Gtk::Label method_label;
        Gtk::Label file_label;
        Gtk::Label edge_desc;
        Gtk::Label sift_desc;
        Gtk::Label text_file_name;

        Gtk::Image image;
        Gtk::Box image_box;
};

#endif