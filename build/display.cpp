#include "display.h"
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
    
    int result = dialog.run();

    if (result == Gtk::RESPONSE_OK) {
        std::string filename = dialog.get_filename();
        set_image(filename);
        text_file_name.set_visible(true);
        text_file_name.set_text("File selected:" + filename);
    }
}

void Grid::on_button_press() {
    std::cout << "hello world" << std::endl;
};

void Grid::set_image(const std::string& filename) {
    auto pix = Gdk::Pixbuf::create_from_file(filename);
    int tar_width = 500;
    int original_width = pix->get_width();
    int original_height = pix->get_height();
    int tar_height = (tar_width * original_height) / original_width;
    pix = pix->scale_simple(
        tar_width,
        tar_height,
        Gdk::INTERP_BILINEAR
    );
    image.set(pix);
};

Gtk::Widget* Grid::create_image_panel() {
    image_box.set_hexpand(true);
    image_box.set_vexpand(true);
    image_box.set_halign(Gtk::ALIGN_CENTER);
    image_box.set_valign(Gtk::ALIGN_CENTER);
    image.set_hexpand(true);
    image_box.pack_start(image);
    return &image_box;
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
    sift_desc.set_markup("2. SIFT");

    button.set_label("Click");
    button.signal_toggled().connect(sigc::mem_fun(*this, 
        &Grid::on_button_press));

    method_label.set_halign(Gtk::ALIGN_CENTER);
    method_label.set_hexpand(true);

    left_panel.pack_start(method_label, Gtk::PACK_SHRINK);
    left_panel.pack_start(edge_desc,Gtk::PACK_SHRINK);
    left_panel.pack_start(button,Gtk::PACK_SHRINK);

    left_bottom.set_vexpand(true);
    left_bottom.pack_start(*(Gtk::make_managed<Gtk::Label>("")));

    image.set_hexpand(true);
    image.set_vexpand(true);
    right_panel.pack_start(image);

    left_paned.pack1(left_panel, false, false);
    left_paned.pack2(left_bottom, true, false);
    left_paned.set_position(200);
    paned.pack1(left_paned, true, false);
    paned.pack2(right_panel, true, false);
    paned.set_position(300);

    attach(paned,0,0,1,1);
    
};


