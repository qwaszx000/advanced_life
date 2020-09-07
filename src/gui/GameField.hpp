#pragma once

#include <gtkmm/drawingarea.h>
#include <glibmm.h>
#include <cairomm/context.h>

#include "../game/Game.hpp"

class GameField : public Gtk::DrawingArea{
    public:
        Game *game;
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
                context->set_source_rgb(1.0, 0.0, 0.0);
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


};