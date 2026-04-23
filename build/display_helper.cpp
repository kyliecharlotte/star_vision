#include "display.h"

void Grid::set_up_threshold_values() {
    submit_canny.set_label("submit");
    submit_canny.signal_clicked().connect(sigc::mem_fun(*this, 
        &Grid::on_canny_submit));

    reset_canny.set_label("reset");
    reset_canny.signal_clicked().connect(sigc::mem_fun(*this, 
        &Grid::on_canny_reset));

    threshold_one_entry.set_text("100");
    threshold_one_entry.set_max_length(5);
    threshold_one_entry.set_visible(false);

    threshold_two_entry.set_text("200");
    threshold_two_entry.set_max_length(5);
    threshold_two_entry.set_visible(false);

    one_left_side_right_side.pack_start(threshold_one_entry, Gtk::PACK_SHRINK);
    one_left_side_right_side.hide();
    one_left_side_left_side.set_vexpand(false);
    one_left_side_left_side.set_hexpand(true);
    one_left_side_left_side.pack_start(*(Gtk::make_managed<Gtk::Label>("Set threshold 1 value:")));

    threshold_one_options.pack_start(one_left_side_left_side, true, false);
    threshold_one_options.pack_start(one_left_side_right_side, true, false);

    two_left_side_right_side.pack_start(threshold_two_entry, Gtk::PACK_SHRINK);
    two_left_side_right_side.hide();
    two_left_side_left_side.set_vexpand(false);
    two_left_side_left_side.set_hexpand(true);
    two_left_side_left_side.pack_start(*(Gtk::make_managed<Gtk::Label>("Set threshold 2 value:")));

    threshold_two_options.pack_start(two_left_side_left_side, true, false);
    threshold_two_options.pack_start(two_left_side_right_side, true, false);

    three_left_side_left_side.pack_start(*(Gtk::make_managed<Gtk::Label>("")));
    three_left_side_right_side.pack_start(submit_canny, Gtk::PACK_SHRINK);
    three_left_side_right_side.pack_start(reset_canny, Gtk::PACK_SHRINK);

    canny_submit_options.pack_start(three_left_side_left_side, true, false);
    canny_submit_options.pack_start(three_left_side_right_side, true, false);
};

void Grid::set_up_sift_values() {
    submit_sift.set_label("submit");
    submit_sift.signal_clicked().connect(sigc::mem_fun(*this, 
        &Grid::on_sift_submit));

    reset_sift.set_label("reset");
    reset_sift.signal_clicked().connect(sigc::mem_fun(*this, 
        &Grid::on_sift_reset));

    nfeatures_entry.set_text("0");
    nfeatures_entry.set_max_length(5);
    nfeatures_entry.set_width_chars(10);
    nfeatures_entry.set_hexpand(false);
    nfeatures_entry.set_visible(false);

    nlayers_entry.set_text("3");
    nlayers_entry.set_max_length(5);
    nlayers_entry.set_width_chars(10);
    nlayers_entry.set_hexpand(false);
    nlayers_entry.set_visible(false);

    contrast_threshold.set_text("0.04");
    contrast_threshold.set_max_length(5);
    contrast_threshold.set_width_chars(10);
    contrast_threshold.set_hexpand(false);
    contrast_threshold.set_visible(false);

    edge_threshold.set_text("10");
    edge_threshold.set_max_length(5);
    edge_threshold.set_width_chars(10);
    edge_threshold.set_hexpand(false);
    edge_threshold.set_visible(false);

    sift_one_left_side_right_side.pack_start(*(Gtk::make_managed<Gtk::Label>("Set n features:")));
    sift_one_left_side_right_side.pack_start(nfeatures_entry, Gtk::PACK_SHRINK);
    sift_one_left_side_left_side.pack_start(*(Gtk::make_managed<Gtk::Label>("Set n octave layers:")));
    sift_one_left_side_left_side.pack_start(nlayers_entry, Gtk::PACK_SHRINK);
    sift_one_left_side_left_side.set_hexpand(false);
    sift_one_left_side_right_side.set_hexpand(false);
    sift_one_left_side_right_side.hide();
    sift_one_left_side_left_side.hide();

    n_options.pack_start(sift_one_left_side_left_side, true, false);
    n_options.pack_start(sift_one_left_side_right_side, true, false);

    sift_two_left_side_right_side.pack_start(*(Gtk::make_managed<Gtk::Label>("Set contrast threshold:")));
    sift_two_left_side_right_side.pack_start(contrast_threshold, Gtk::PACK_SHRINK);
    sift_two_left_side_left_side.pack_start(*(Gtk::make_managed<Gtk::Label>("Set edge threshold:")));
    sift_two_left_side_left_side.pack_start(edge_threshold, Gtk::PACK_SHRINK);
    sift_two_left_side_left_side.set_hexpand(false);
    sift_two_left_side_right_side.set_hexpand(false);
    sift_two_left_side_right_side.hide();
    sift_two_left_side_left_side.hide();

    threshold_options.pack_start(sift_two_left_side_left_side, true, false);
    threshold_options.pack_start(sift_two_left_side_right_side, true, false);

    sift_three_left_side_left_side.pack_start(*(Gtk::make_managed<Gtk::Label>("")));
    sift_three_left_side_right_side.pack_start(submit_sift, Gtk::PACK_SHRINK);
    sift_three_left_side_right_side.pack_start(reset_sift, Gtk::PACK_SHRINK);

    sift_submit_options.pack_start(sift_three_left_side_left_side, true, false);
    sift_submit_options.pack_start(sift_three_left_side_right_side, true, false);
}

void Grid::canny_hide_text() {
    one_left_side_right_side.hide();
    one_left_side_left_side.hide();
    two_left_side_left_side.hide();
    two_left_side_right_side.hide();
    three_left_side_right_side.hide();
    three_left_side_left_side.hide();
};

void Grid::canny_show_text() {
    one_left_side_right_side.show();
    one_left_side_left_side.show();
    two_left_side_left_side.show();
    two_left_side_right_side.show();
    three_left_side_right_side.show();
    three_left_side_left_side.show();
};

void Grid::sift_hide_text() {
    sift_one_left_side_right_side.hide();
    sift_one_left_side_left_side.hide();
    sift_two_left_side_left_side.hide();
    sift_two_left_side_right_side.hide();
    sift_three_left_side_right_side.hide();
    sift_three_left_side_left_side.hide();
};

void Grid::sift_show_text() {
    sift_one_left_side_right_side.show();
    sift_one_left_side_left_side.show();
    sift_two_left_side_left_side.show();
    sift_two_left_side_right_side.show();
    sift_three_left_side_right_side.show();
    sift_three_left_side_left_side.show();
};
