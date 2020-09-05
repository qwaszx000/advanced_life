#pragma once

#include <vector>
#include "alive/alive.hpp"
#include "food/corps.hpp"

#include <iostream>

enum GameObjects{
    EMPTY = 0,
    WALL = 1,
    FOOD = 2,
    //other - cell
};

class Game{
    public:
        //options
        int energy_per_round = 200;
        int current_available_energy = 200;
        uint8 split_cost = 50;
        uint8 energy_consumption = 10;
        uint8 mutation_chance = 10;

        bool paused = true;
        uint8 width, height;
        std::vector<Alive*> alives;
        std::vector<Corps*> corps;

        //ingame
        uint8 told_info = 0, told_x, told_y, told_id;

        Game(uint8 width, uint8 height){
            this->width = width;
            this->height = height;
            this->alives.push_back( new Alive(1, 1) );
        }

        void step(){
            for(auto it = this->alives.begin(); it != this->alives.end();it++){
                Alive *a = this->alives[it - this->alives.begin()];

                std::cout<<(int)a->energy<<std::endl<<(int)a->hp<<std::endl;

                //consumpt energy or hp
                if(a->energy <= 0){
                    a->hp -= energy_consumption;
                } else {
                    a->energy -= energy_consumption;
                }
                //remove from alives if dead
                //and create corps that can be eaten
                if(a->hp <= 0){
                    it = this->alives.erase(it);
                    this->corps.push_back( new Corps(a->x, a->y) );
                    it--; //to compensate it++
                    continue;
                }

                //hear told info
                if(told_info > 0){
                    a->heard_data = told_info;
                    a->heard_id = told_id;
                    a->heard_x = told_x;
                    a->heard_y = told_y;
                }
            }

            told_info = 0;
            current_available_energy = energy_per_round;
        }

    private:
        uint8 lookAtCoords(uint8 x, uint8 y){
            for(Alive *a : this->alives){
                if(a->x == x && a->y == y){
                    if(a->id < 253)
                        return a->id + 3; //minimum - 3
                    else
                        return a->id;
                }
            }

            for(Corps *c : this->corps){
                if(c->x == x && c->y == y){
                    return FOOD;
                }
            }

            return EMPTY;
        }


};