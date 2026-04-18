#ifndef DISPLAY_H
#define DISPLAY_H

#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gdkmm/pixbuf.h>
#include <gtkmm/image.h>
#include <gtkmm/filechooserdialog.h>

#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>

class DisplayWindow : public Gtk::Window {
    public:
        DisplayWindow();
};

class Grid : public Gtk::Grid {
    public:
        Grid();
        void set_image(const std::string& filename);
        void update_from_res(const cv::Mat& result);
        void render_image(const cv::Mat& mat);
        //void pipeline_update_algorithm_button();
    
    private:
        Gtk::Paned paned{Gtk::ORIENTATION_HORIZONTAL};
        Gtk::Paned left_paned{Gtk::ORIENTATION_HORIZONTAL};
        Gtk::Box left_panel{Gtk::ORIENTATION_VERTICAL};
        Gtk::Box left_bottom{Gtk::ORIENTATION_VERTICAL};
        Gtk::Box right_panel{Gtk::ORIENTATION_VERTICAL};

    protected:

        void apply_canny();
        void apply_sift();
        void restore_original();
        void on_canny_button_press();
        void on_sift_button_press();
        void choose_file_button_press();
        void on_resize(Gtk::Allocation& allocation);
        Gtk::Widget* create_image_panel();

        Gtk::Button choose_file_button;
        Gtk::ToggleButton canny_button;
        Gtk::ToggleButton sift_button;

        Gtk::Label method_label;
        Gtk::Label file_label;
        Gtk::Label edge_desc;
        Gtk::Label sift_desc;
        Gtk::Label text_file_name;

        Gtk::Image image;
        Gtk::Box image_box;

        cv::Mat curr_img;
        cv::Mat orig_img;
        cv::Mat display_mem;
        int display_height = 600;
        int display_width = 800;
};

#endif