#include "display.h"

void Grid::set_up_threshold_values() {

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
};

void Grid::canny_hide_text() {
    one_left_side_right_side.hide();
    one_left_side_left_side.hide();
    two_left_side_left_side.hide();
    two_left_side_right_side.hide();
};

void Grid::canny_show_text() {
    one_left_side_right_side.show();
    one_left_side_left_side.show();
    two_left_side_left_side.show();
    two_left_side_right_side.show();
};

