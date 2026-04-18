#include "display.h"
#include "algorithms.h"
#include <iostream>

DisplayWindow::DisplayWindow() : Gtk::Window() {
    set_title("Star Vision");
    set_default_size(1000,700);
    //show_all_children();
};

void Grid::choose_file_button_press() {
    Gtk::FileChooserDialog dialog("Select images", Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.add_button("_CANCEL", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_OPEN", Gtk::RESPONSE_OK);
    dialog.set_default_size(800,500);
    
    int result = dialog.run();

    if (result == Gtk::RESPONSE_OK) {
        std::string filename = dialog.get_filename();
        curr_img = cv::imread(filename);
        orig_img = curr_img.clone();
        set_image(filename);
        text_file_name.set_visible(true);
        text_file_name.set_text("File selected:" + filename);
    }
}


void Grid::update_from_res(const cv::Mat& result) {
    cv::Mat rgb;

    if (result.channels() == 1){
        cv::cvtColor(result,display_mem,cv::COLOR_GRAY2RGB);
    } else if (result.channels() == 3){
        cv::cvtColor(result,display_mem,cv::COLOR_BGR2RGB);
    };

    auto pix = Gdk::Pixbuf::create_from_data(
        display_mem.data,
        Gdk::COLORSPACE_RGB,
        false,
        8,
        display_mem.cols,
        display_mem.rows,
        display_mem.step
    );

    image.set(pix);
};

void Grid::apply_canny() {
    if (orig_img.empty()) {return;}
    else {
        cv::Mat res = method_edge_detection(curr_img,display_width,display_height);
        curr_img = res;
        update_from_res(curr_img);
    };
}

void Grid::apply_sift() {
    if (orig_img.empty()) {return;}
    else {
        cv::Mat res = method_sift_detection(curr_img,display_width,display_height);
        curr_img = res;
        update_from_res(curr_img);
    };
};

void Grid::restore_original() {
    curr_img = orig_img.clone();
    std::cout << "Restoring" << std::endl;
    render_image(curr_img);
};

void Grid::on_canny_button_press() {
    if (canny_button.get_active()) {
        apply_canny();
    } else {
        if (orig_img.empty()) {return;}
        restore_original();
    }
};

void Grid::on_sift_button_press() {
    if (sift_button.get_active()) {
        apply_sift();
    } else {
        if (orig_img.empty()) {return;}
        restore_original();
    }
};

void Grid::render_image(const cv::Mat& mat){
    cv::Mat resized;
    cv::resize(mat, resized,cv::Size(display_width, display_height));

    if (resized.channels() == 1){
        cv::cvtColor(resized,display_mem,cv::COLOR_GRAY2RGB);
    } else if (resized.channels() == 3){
        cv::cvtColor(resized,display_mem,cv::COLOR_BGR2RGB);
    };

    auto pix = Gdk::Pixbuf::create_from_data(
        display_mem.data,
        Gdk::COLORSPACE_RGB,
        false,
        8,
        display_mem.cols,
        display_mem.rows,
        display_mem.step
    );

    image.set(pix);

};

void Grid::set_image(const std::string& filename) {
    auto pix = Gdk::Pixbuf::create_from_file(filename);
    int tar_width = 650;
    int original_width = pix->get_width();
    int original_height = pix->get_height();
    int tar_height = (tar_width * original_height) / original_width;
    display_width = tar_width;
    display_height = tar_height;
    //curr_img = cv::imread(filename);
    //orig_img = curr_img.clone();
    render_image(curr_img);
};

Grid::Grid() : Gtk::Grid() {

    choose_file_button.set_label("Load File");
    choose_file_button.signal_clicked().connect(sigc::mem_fun(*this, 
        &Grid::choose_file_button_press));
    file_label.set_markup("<b>Import File:</b>");
    left_panel.pack_start(file_label, Gtk::PACK_SHRINK);
    left_panel.pack_start(choose_file_button, Gtk::PACK_SHRINK);

    text_file_name.set_text("");
    text_file_name.set_visible(false);
    left_panel.pack_start(text_file_name, Gtk::PACK_SHRINK);

    method_label.set_markup("<b>Select Method(s):</b>");
    edge_desc.set_markup("1. Canny Edge Detection - Identify edges");
    sift_desc.set_markup("2. SIFT (Scale-Invariant Feature Transform) - Identifies objects");

    canny_button.set_label("Edge Detection");
    canny_button.signal_toggled().connect(sigc::mem_fun(*this, 
        &Grid::on_canny_button_press));

    sift_button.set_label("SIFT");
    sift_button.signal_toggled().connect(sigc::mem_fun(*this, 
        &Grid::on_sift_button_press));

    method_label.set_halign(Gtk::ALIGN_CENTER);
    method_label.set_hexpand(true);

    left_panel.pack_start(method_label, Gtk::PACK_SHRINK);
    left_panel.pack_start(edge_desc,Gtk::PACK_SHRINK);
    left_panel.pack_start(canny_button,Gtk::PACK_SHRINK);
    left_panel.pack_start(sift_desc,Gtk::PACK_SHRINK);
    left_panel.pack_start(sift_button,Gtk::PACK_SHRINK);

    left_bottom.set_vexpand(true);
    left_bottom.pack_start(*(Gtk::make_managed<Gtk::Label>("")));

    image.set_hexpand(true);
    image.set_vexpand(true);
    right_panel.pack_start(image);
    /*right_panel.signal_size_allocate().connect(
        sigc::mem_fun(*this,&Grid::on_resize) 
    );*/

    left_paned.pack1(left_panel, false, false);
    left_paned.pack2(left_bottom, true, false);
    left_paned.set_position(200);
    paned.pack1(left_paned, true, false);
    paned.pack2(right_panel, true, false);
    paned.set_position(300);

    attach(paned,0,0,1,1);
    
};


