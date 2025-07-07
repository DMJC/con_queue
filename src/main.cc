#include <gtkmm.h>
#include <gst/gst.h>

class PlaybackWindow : public Gtk::Window {
public:
    PlaybackWindow() {
        set_title("Playback");
        set_default_size(640, 480);
        add(m_area);
        m_area.show();
    }
protected:
    bool on_key_press_event(GdkEventKey* event) override {
        if (event->keyval == GDK_KEY_F || event->keyval == GDK_KEY_f) {
            if (is_fullscreen)
                unfullscreen();
            else
                fullscreen();
            is_fullscreen = !is_fullscreen;
            return true;
        }
        return Gtk::Window::on_key_press_event(event);
    }
private:
    Gtk::DrawingArea m_area;
    bool is_fullscreen{false};
};

class ClockWindow : public Gtk::Window {
public:
    ClockWindow() {
        set_title("Clock");
        set_default_size(200, 100);
        add(m_label);
        m_label.set_text("00:00");
        m_label.show();
    }
private:
    Gtk::Label m_label;
};

class CueListColumns : public Gtk::TreeModel::ColumnRecord {
public:
    CueListColumns() {
        add(col_number);
        add(col_name);
        add(col_pre_wait);
        add(col_action);
        add(col_post_wait);
    }
    Gtk::TreeModelColumn<int> col_number;
    Gtk::TreeModelColumn<Glib::ustring> col_name;
    Gtk::TreeModelColumn<Glib::ustring> col_pre_wait;
    Gtk::TreeModelColumn<Glib::ustring> col_action;
    Gtk::TreeModelColumn<Glib::ustring> col_post_wait;
};

class PlaylistWindow : public Gtk::Window {
public:
    PlaylistWindow() {
        set_title("Playlist");
        set_default_size(800, 600);

        add(m_vbox);
        m_vbox.pack_start(m_menubar, Gtk::PACK_SHRINK);
        auto file_menu = Gtk::manage(new Gtk::MenuItem("File"));
        m_menubar.append(*file_menu);

        m_go_button.set_label("GO");
        m_go_button.set_size_request(-1, 80);
        m_go_button.set_hexpand(true);
        auto attr = Pango::AttrList();
        attr.insert(Pango::Attribute::create_attr_scale(Pango::SCALE_XX_LARGE));
        m_go_button.set_attributes(attr);
        m_vbox.pack_start(m_go_button, Gtk::PACK_SHRINK);

        m_vbox.pack_start(m_paned);

        m_ref_model = Gtk::ListStore::create(m_columns);
        m_treeview.set_model(m_ref_model);
        m_treeview.append_column("#", m_columns.col_number);
        m_treeview.append_column("Cue Name", m_columns.col_name);
        m_treeview.append_column("Pre Wait", m_columns.col_pre_wait);
        m_treeview.append_column("Action", m_columns.col_action);
        m_treeview.append_column("Post Wait", m_columns.col_post_wait);
        m_scrolled.add(m_treeview);
        m_paned.add1(m_scrolled);

        m_controls_box.set_border_width(5);
        m_controls_box.set_spacing(5);
        m_controls_box.pack_start(m_play_button);
        m_controls_box.pack_start(m_pause_button);
        m_controls_box.pack_start(m_stop_button);
        m_controls_box.pack_start(m_remove_button);
        m_controls_box.pack_start(m_vol_down_button);
        m_controls_box.pack_start(m_vol_up_button);

        m_paned.add2(m_controls_box);

        m_play_button.set_label("Play");
        m_pause_button.set_label("Pause");
        m_stop_button.set_label("Stop");
        m_remove_button.set_label("Remove");
        m_vol_down_button.set_label("Vol -");
        m_vol_up_button.set_label("Vol +");

        show_all_children();
    }
private:
    Gtk::VBox m_vbox;
    Gtk::MenuBar m_menubar;
    Gtk::Button m_go_button;
    Gtk::Paned m_paned{Gtk::ORIENTATION_HORIZONTAL};

    Gtk::ScrolledWindow m_scrolled;
    Gtk::TreeView m_treeview;
    Glib::RefPtr<Gtk::ListStore> m_ref_model;
    CueListColumns m_columns;

    Gtk::VBox m_controls_box;
    Gtk::Button m_play_button, m_pause_button, m_stop_button,
                m_remove_button, m_vol_down_button, m_vol_up_button;
};

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "com.example.conqueue");
    Gst::init(argc, argv);

    PlaybackWindow playback;
    PlaylistWindow playlist;
    ClockWindow clock;

    playback.show();
    playlist.show();
    clock.show();

    return app->run();
}
