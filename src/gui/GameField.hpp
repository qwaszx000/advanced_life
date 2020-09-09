#pragma once

#include <gtkmm/drawingarea.h>
#include <glibmm.h>
#include <cairomm/context.h>
#include <gtkmm/textview.h>
#include <string>

//#include "../game/alive/alive.hpp"
#include "../game/Game.hpp"

class GameField : public Gtk::DrawingArea{
    public:
        Game *game;
        Glib::RefPtr<Gtk::TextBuffer> info_buffer;
        Alive *selected_alive = NULL;
        //timer
        int delay_ms = 1000;
        sigc::slot<bool> timer_slot;
        sigc::connection timer_conn;

        GameField(){
            set_size_request(400, 400);
            this->game = new Game(20, 20);
            //timer
            timer_slot = sigc::mem_fun(*this, &GameField::redraw);
            timer_conn = Glib::signal_timeout().connect(timer_slot, delay_ms);
            //allow onclick handling
            add_events(Gdk::BUTTON_PRESS_MASK);
        }

        ~GameField(){
            delete this->game;
        }

        bool getPauseState(){
            return this->game->paused;
        }

        void togglePauseState(){
            this->game->paused = !this->game->paused;
        }
  
        void one_step(){
            this->game->step();
            this->queue_draw();
            update_selected_cell_info();
        }

        void on_delay_change(){
            this->timer_conn.disconnect();
            this->timer_conn = Glib::signal_timeout().connect(this->timer_slot, this->delay_ms);
        }

        void set_info_buffer(Glib::RefPtr<Gtk::TextBuffer> buffer){
            this->info_buffer = buffer;
        }

    protected:
        bool on_draw(const Cairo::RefPtr<Cairo::Context> &context) override{
            drawGrid(context);
            drawFood(context);
            drawCells(context);
            return true;
        }

        bool redraw(){
            if(!getPauseState()){
                this->game->step();
                this->queue_draw();
                update_selected_cell_info();
            }
            return true;
        }

        bool on_button_press_event(GdkEventButton *event){
            uint8 clicked_cell_x = 0;
            uint8 clicked_cell_y = 0;

            //lmb
            if( (event->type == GDK_BUTTON_PRESS) && (event->button == 1) ){
                clicked_cell_x = event->x / 20;
                clicked_cell_y = event->y / 20;

                Alive *a = this->game->getAliveByCoords(clicked_cell_x, clicked_cell_y);
                this->selected_alive = a;
                update_selected_cell_info();
            }
            return true;
        }

    private:
        void drawGrid(const Cairo::RefPtr<Cairo::Context> &context){
            //black line
            context->set_line_width(1.0);
            context->set_source_rgb(0.0, 0.0, 0.0);

            for(int x = 0; x <= this->get_width(); x += 20){
                context->move_to(x, 0);
                context->line_to(x, 400);
            }

            for(int y = 0; y <= this->get_height(); y += 20){
                context->move_to(0, y);
                context->line_to(400, y);
            }
            context->stroke();
        }

        void drawCells(const Cairo::RefPtr<Cairo::Context> &context){
            for(Alive *a : this->game->alives){
                context->set_source_rgb(a->color_red, a->color_green, a->color_blue);
                context->rectangle(a->x*20+1, a->y*20+1, 18, 18);
                context->fill();
                context->stroke();
            }
        }

        void drawFood(const Cairo::RefPtr<Cairo::Context> &context){
            for(Food *f : this->game->food){
                context->set_source_rgb(0.5, 0.5, 0.5);
                context->rectangle(f->x*20+1, f->y*20+1, 18, 18);
                context->fill();
                context->stroke();
            }
        }

        void update_selected_cell_info(){
            if(this->selected_alive != NULL){
                std::string text;
                text.append("DNA: ");
                for(uint8 i = 0; i < ARRAY_LEN(this->selected_alive->dna_code); ++i){
                    text.append(std::to_string(this->selected_alive->dna_code[i]) );
                    text.append(i % 30 == 0 && i != 0 ? " \n" : " ");
                }
                text.append("\nHP: " + std::to_string(this->selected_alive->hp) + "\nEnergy: " + std::to_string(this->selected_alive->energy));
                this->info_buffer->set_text(text.c_str());
            }
        }

};