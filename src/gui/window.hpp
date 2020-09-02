#include <gtkmm/window.h>
#include <gtkmm/fixed.h>

#include <gtkmm/button.h>
#include <gtkmm/textview.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <iostream>

#include "GameField.hpp"
#include "../game/Game.hpp"
#include <gtkmm/drawingarea.h>

class GameWindow : public Gtk::Window{
    Game *game;
    GameField field;
    //Gtk::DrawingArea field;

    Gtk::Fixed fixed_layout;
    Gtk::Button pause_btn;
    Gtk::TextView selected_cell_genes_view;

    Gtk::Label delay_option_label, mutation_option_label, split_energy_option_label;
    Gtk::Entry delay_entry, mutation_entry, split_energy_entry;

    public:
        GameWindow(){
            game = new Game();

            //prepare window
            set_border_width(10);
            set_title("Life simulation");
            set_default_size(800, 500);

            //prepare fixed layout
            add(fixed_layout);

            //create pause btn
            pause_btn.set_label("Run");
            pause_btn.signal_clicked().connect(sigc::mem_fun(*this, &GameWindow::pause_toggle));
            fixed_layout.put(pause_btn, 205, 450);

            //create options
            delay_option_label.set_text("Delay(ms):");
            fixed_layout.put(delay_option_label, 420, 125);
            delay_entry.set_max_length(5);
            fixed_layout.put(delay_entry, 570, 120);

            mutation_option_label.set_text("Mutation chanse(%):");
            fixed_layout.put(mutation_option_label, 420, 155);
            mutation_entry.set_max_length(3);
            fixed_layout.put(mutation_entry, 570, 150);

            split_energy_option_label.set_text("Energy to split:");
            fixed_layout.put(split_energy_option_label, 420, 185);
            split_energy_entry.set_max_length(5);
            fixed_layout.put(split_energy_entry, 570, 180);

            //create selected cell genes textbox
            auto selected_cell_genes_buffer = Gtk::TextBuffer::create();
            selected_cell_genes_view.set_border_width(5);
            selected_cell_genes_view.set_size_request(315, 100);
            selected_cell_genes_view.set_buffer(selected_cell_genes_buffer);

            fixed_layout.put(selected_cell_genes_view, 420, 10);
            //selected_cell_genes_buffer->set_text("Test");

            //create main field
            field.setGame(game);
            fixed_layout.put(field, 10, 10);

            show_all();
        }

    private:
        //pause_btn click handler
        void pause_toggle(){
            this->game->paused = !this->game->paused;
            if(this->game->paused){
                this->pause_btn.set_label("Run");
            } else {
                this->pause_btn.set_label("Pause");
            }
        }
};