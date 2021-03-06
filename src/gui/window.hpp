#include <gtkmm/window.h>
#include <gtkmm/fixed.h>

#include <gtkmm/button.h>
#include <gtkmm/textview.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>

#include "GameField.hpp"
#include "../game/Game.hpp"
#include <gtkmm/drawingarea.h>

class GameWindow : public Gtk::Window{
    GameField field;

    Gtk::Fixed fixed_layout;
    Gtk::Button pause_btn, one_step_btn;
    Gtk::TextView selected_cell_genes_view;

    Gtk::Label delay_option_label, mutation_option_label, split_energy_option_label, round_energy_option_label, energy_consumption_option_label;
    Gtk::Entry delay_entry, mutation_entry, split_energy_entry, round_energy_entry, energy_consumption_entry;

    public:
        GameWindow(){
            //prepare window
            set_border_width(10);
            set_title("Life simulation");
            set_default_size(800, 500);

            //prepare fixed layout
            add(fixed_layout);

            //create pause btn
            pause_btn.set_label("Run");
            pause_btn.signal_clicked().connect(sigc::mem_fun(*this, &GameWindow::pause_toggle));
            fixed_layout.put(pause_btn, 200, 450);
            //one step btn
            one_step_btn.set_label("One step");
            one_step_btn.signal_clicked().connect(sigc::mem_fun(*this, &GameWindow::one_step));
            fixed_layout.put(one_step_btn, 250, 450);

            //create options
            delay_option_label.set_text("Delay(ms):");
            fixed_layout.put(delay_option_label, 420, 185);
            delay_entry.set_max_length(5);
            delay_entry.set_text("1000");
            delay_entry.signal_changed().connect(sigc::mem_fun(*this, &GameWindow::delay_change));
            fixed_layout.put(delay_entry, 570, 180);

            mutation_option_label.set_text("Mutation chanse(%):");
            fixed_layout.put(mutation_option_label, 420, 215);
            mutation_entry.set_max_length(3);
            mutation_entry.set_text("10");
            mutation_entry.signal_changed().connect(sigc::mem_fun(*this, &GameWindow::mutation_chance_change));
            fixed_layout.put(mutation_entry, 570, 210);

            split_energy_option_label.set_text("Energy to split:");
            fixed_layout.put(split_energy_option_label, 420, 245);
            split_energy_entry.set_max_length(5);
            split_energy_entry.set_text("50");
            split_energy_entry.signal_changed().connect(sigc::mem_fun(*this, &GameWindow::split_cost_change));
            fixed_layout.put(split_energy_entry, 570, 240);

            round_energy_option_label.set_text("Energy per round:");
            fixed_layout.put(round_energy_option_label, 420, 275);
            round_energy_entry.set_max_length(5);
            round_energy_entry.set_text("200");
            round_energy_entry.signal_changed().connect(sigc::mem_fun(*this, &GameWindow::round_energy_change));
            fixed_layout.put(round_energy_entry, 570, 270);

            energy_consumption_option_label.set_text("Energy consumption:");
            fixed_layout.put(energy_consumption_option_label, 420, 305);
            energy_consumption_entry.set_max_length(5);
            energy_consumption_entry.set_text("10");
            energy_consumption_entry.signal_changed().connect(sigc::mem_fun(*this, &GameWindow::energy_consumption_change));
            fixed_layout.put(energy_consumption_entry, 570, 300);

            //create selected cell genes textbox
            auto selected_cell_genes_buffer = Gtk::TextBuffer::create();
            selected_cell_genes_view.set_border_width(5);
            selected_cell_genes_view.set_size_request(315, 100);
            selected_cell_genes_view.set_buffer(selected_cell_genes_buffer);

            fixed_layout.put(selected_cell_genes_view, 420, 10);

            //create main field
            field.set_info_buffer(selected_cell_genes_buffer);
            fixed_layout.put(field, 10, 10);

            show_all();
        }

    private:
        //pause_btn click handler
        void pause_toggle(){
            field.togglePauseState();
            this->pause_btn.set_label(field.getPauseState() ? "Run" : "Pause");
        }
        //one_step_btn click handler
        void one_step(){
            field.one_step();
        }
        //per step delay change handler
        void delay_change(){
            field.delay_ms = atoi(delay_entry.get_text().c_str());
            field.on_delay_change();
        }
        //mutation chance change handler
        void mutation_chance_change(){
            field.game->mutation_chance = atoi(mutation_entry.get_text().c_str());
            if(field.game->mutation_chance == 0){
                field.game->mutation_chance = 1;
            } else if (field.game->mutation_chance > 100){
                field.game->mutation_chance = 100;
            }
            
        }
        //split energy cost change handler
        void split_cost_change(){
            field.game->split_cost = atoi(split_energy_entry.get_text().c_str());
        }
        //split energy cost change handler
        void round_energy_change(){
            field.game->energy_per_round = atoi(round_energy_entry.get_text().c_str());
        }
        //split energy cost change handler
        void energy_consumption_change(){
            field.game->energy_consumption = atoi(energy_consumption_entry.get_text().c_str());
        }
};