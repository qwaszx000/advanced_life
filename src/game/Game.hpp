#pragma once

#include <vector>
#include "alive/alive.hpp"
#include "food/food.hpp"

#include <iostream>

enum GameObjects{
    EMPTY = 0,
    WALL = 1,
    FOOD = 2,
    //other - cell
};

enum Commands{
    nop = 0,
    move = 1,
    look = 2,
    jmp = 3,
    attack = 4,
    get_sun_energy = 5,
    select_memory = 19,
    write_to_mem = 20,
    add = 21,
    sub = 22,
    prev_mem = 23,
    next_mem = 24,
    split = 25
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
        std::vector<Food*> food;

        //ingame
        uint8 told_info = 0, told_x, told_y, told_id;

        Game(uint8 width, uint8 height){
            this->width = width;
            this->height = height;

            Alive *first_alive = new Alive(1, 1);
            uint8 first_dna[] = {Commands::get_sun_energy, Commands::get_sun_energy, Commands::get_sun_energy, Commands::get_sun_energy,
                                    Commands::get_sun_energy, Commands::split, Commands::jmp, 0}; //get 100(50 will be consumpted) energy, split and jmp to first gene
                                    
            first_alive->setDna( first_dna );
            this->alives.push_back( first_alive );
        }

        void step(){
            std::vector<Alive*>::size_type size = this->alives.size();
            for(std::vector<Alive*>::size_type i = 0; i < size; ++i){
                Alive *a = this->alives[i];

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
                    this->alives.erase(this->alives.begin() + i);
                    --size;
                    this->food.push_back( new Food(a->x, a->y) );
                    --i; //to compensate it++
                    delete a;
                    continue;
                }

                //hear told info
                if(told_info > 0){
                    a->heard_data = told_info;
                    a->heard_id = told_id;
                    a->heard_x = told_x;
                    a->heard_y = told_y;
                }

                process_turn(a);
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

            for(Food *f : this->food){
                if(f->x == x && f->y == y){
                    return FOOD;
                }
            }

            return EMPTY;
        }


        void process_turn(Alive *a){
            uint8 loop_limit = 70;
            while(loop_limit != 0){
                --loop_limit;

                //run through dna code and interpretate it
                switch(a->dna_code[a->current_command_index])
                {
                    //do nothing, skip turn
                    case Commands::nop:{
                        //end turn
                        loop_limit = 0;
                        break;
                    }

                    case Commands::move:{
                        move_alive(a, a->dna_code[a->current_command_index+1]);
                        //2 = command + argument 
                        a->current_command_index += 1;
                        //end turn
                        loop_limit = 0;
                        break;
                    }

                    case Commands::look:{
                        break;
                    }

                    case Commands::jmp:{
                        uint8 arg = a->dna_code[a->current_command_index+1];
                        if(arg == 255){
                            arg = a->memory[a->selected_memory];
                        }
                        if(arg > 127){
                            arg -= 128;
                        }
                        //-1 to compensate +=1 at the end of loop
                        a->current_command_index = arg - 1;
                        break;
                    }
                    
                    case Commands::attack:{
                        loop_limit = 0;
                        break;
                    }

                    case Commands::get_sun_energy:{
                        this->current_available_energy -= 20;
                        a->energy += 20;
                        loop_limit = 0;
                        break;
                    }

                    case Commands::select_memory:{
                        uint8 arg = a->dna_code[a->current_command_index+1];
                        if(arg == 255){
                            arg = a->memory[a->selected_memory];
                        }
                        while(arg > 3){
                            arg -= 4;
                        }
                        a->selected_memory = arg;
                        a->current_command_index += 1;
                        break;
                    }

                    case Commands::write_to_mem:{
                        uint8 arg = a->dna_code[a->current_command_index+1];
                        a->memory[a->selected_memory] = arg;
                        a->current_command_index += 1;
                        break;
                    }

                    case Commands::split:{
                        if(a->energy >= split_cost){
                            a->energy -= split_cost;
                            loop_limit = 0;
                        }
                        Alive *child = new Alive(0, 0);
                        child->setDna(a->dna_code);
                        this->alives.push_back( child );
                        break;
                    }

                    //skip unknown command
                    default:
                        break;
                }
                //next command
                a->current_command_index += 1;
            }
        }


        void move_alive(Alive *a, uint8 dir){
            switch(dir)
            {
                case 0:
                    a->x -= 1;
                    a->y -= 1;
                    break;
                case 1:
                    a->y -= 1;
                    break;
                case 2:
                    a->x += 1;
                    a->y -= 1;
                    break;
                case 3:
                    a->x += 1;
                    break;
                case 4:
                    a->x += 1;
                    a->y += 1;
                    break;
                case 5:
                    a->y += 1;
                    break;
                case 6:
                    a->x -= 1;
                    a->y += 1;
                    break;
                case 7:
                    a->x -= 1;
                    break;
                case 255:
                    move_alive(a, a->memory[a->selected_memory]);
                    break;
                default:
                    break;
            }
        }


};