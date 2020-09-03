#pragma once

#include <gtkmm/drawingarea.h>
#include <cairomm/context.h>

#include "../game/Game.hpp"

class GameField : public Gtk::DrawingArea{
    Game *game;

    public:
        GameField(){
            set_size_request(400, 400);
            this->game = new Game(20, 20);
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
  
        /*GameField(int count_width = 20, int count_height = 20, int cell_size = 20){
            set_size_request( (cell_size+1)*count_width + 1, (cell_size+1)*count_height + 1);
        }*/
    
    protected:
        bool on_draw(const Cairo::RefPtr<Cairo::Context> &context) override{
            drawGrid(context);
            drawCells(context);

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


};