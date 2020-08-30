#include <gtkmm/window.h>
#include <gtkmm/fixed.h>
#include <gtkmm/button.h>
//#include <iostream>

#include "../game/Game.hpp"

class GameWindow : public Gtk::Window{
    Game *game;

    Gtk::Button pause_btn;
    Gtk::Fixed fixed_layout;

    public:
        GameWindow(){
            game = new Game();
            //prepare window
            set_border_width(10);
            set_default_size(800, 500);

            //prepae fixed layout
            fixed_layout = Gtk::Fixed();
            add(fixed_layout);
            fixed_layout.show();

            //create btn
            pause_btn = Gtk::Button("Run");
            pause_btn.signal_clicked().connect(sigc::mem_fun(*this, &GameWindow::pause_toggle));
            fixed_layout.put(pause_btn, 205, 450);
            pause_btn.show();
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