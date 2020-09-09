#pragma once

#include <vector>
#include "alive/alive.hpp"
#include "food/food.hpp"
#include "utils.hpp"

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
                      
            /*uint8 first_dna[] = {Commands::move, 3, Commands::get_sun_energy, Commands::get_sun_energy,
                                    Commands::get_sun_energy, Commands::split, Commands::jmp, 0};*/
            first_alive->setDna( first_dna );
            this->alives.push_back( first_alive );
        }

        Alive* getAliveByCoords(uint8 x, uint8 y){
            for(Alive *a : this->alives){
                if(a->x == x && a->y == y){
                    return a;
                }
            }

            return NULL;
        }

        Food* getFoodByCoords(uint8 x, uint8 y){
            for(Food *f : this->food){
                if(f->x == x && f->y == y){
                    return f;
                }
            }

            return NULL;
        }

        void step(){
            std::vector<Alive*>::size_type size = this->alives.size();
            for(std::vector<Alive*>::size_type i = 0; i < size; ++i){
                Alive *a = this->alives[i];

                //consumpt energy or hp
                a->starve(energy_consumption);
                //remove from alives if dead
                //and create corps that can be eaten
                if(a->hp <= 0){
                    this->alives.erase(this->alives.begin() + i);
                    --size;
                    this->food.push_back( new Food(a->x, a->y) );
                    --i; //to compensate ++i
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

        std::vector<uint8>* checkFreeCoords(std::vector<uint8>* coords){
            std::vector<uint8>* free_coords = new std::vector<uint8>();
            for(uint8 i = 0; i < coords->size(); i+=2){
                uint8 coordsInfo = lookAtCoords(coords->at(i), coords->at(i+1));
                if(coordsInfo == GameObjects::EMPTY || coordsInfo == GameObjects::FOOD){
                    free_coords->push_back(coords->at(i));
                    free_coords->push_back(coords->at(i+1));
                }
            }
            delete coords;
            return free_coords;
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

                        Food *food = getFoodByCoords(a->x, a->y);
                        if(food != NULL){
                            remove_food(food);
                            a->energy += 40;
                            a->hp += 10;
                        }
                        
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

                        uint8 *coords = getCoordsByDirection(a, a->dna_code[a->current_command_index+1]);
                        Alive *target = getAliveByCoords(coords[0], coords[1]);
                        target->hp -= 20;

                        a->current_command_index += 1;
                        loop_limit = 0;
                        break;
                    }

                    case Commands::get_sun_energy:{
                        if(this->current_available_energy >= 20){
                            this->current_available_energy -= 20;
                            a->energy += 20;
                            loop_limit = 0;
                        }
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
                            std::vector<uint8> *free_coords = checkFreeCoords(getCoordinatesAround(a));
                            if(free_coords->size() >= 2){
                                Alive *child = new Alive(free_coords->at(0), free_coords->at(1));
                                delete free_coords;
                                child->setDna(a->dna_code);
                                mutate(child);
                                this->alives.push_back( child );

                                //end turn and pay energy for child
                                a->energy -= split_cost;
                                loop_limit = 0;
                            }
                        }
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


        //TODO fix moving in already alive cell
        void move_alive(Alive *a, uint8 dir){
            uint8 *move = new uint8[2];
            if(dir == 255){
                move = getCoordsByDirection(a, a->memory[a->selected_memory]);
            } else {
                move = getCoordsByDirection(a, dir);
            }
            
            a->x = move[0] > this->width - 1 ? 0 : move[0];
            a->y = move[1] > this->height - 1 ? 0 : move[1];
        }

        void mutate(Alive *a){
            //percent to chance
            uint8 tmp = 100 / mutation_chance;
            if(rand() % tmp == 0){
                //choise random gene | sizeof returns size in bytes, translate it to count of elements by / on 1 element
                //tmp = rand() % sizeof(a->dna_code) / sizeof(a->dna_code[0]);
                tmp = rand() % ARRAY_LEN(a->dna_code);
                a->dna_code[tmp] = rand() % 256; //0-255

                //mutate color
                a->color_red = randDouble();
                a->color_green = randDouble();
                a->color_blue = randDouble();
            }
        }

        std::vector<uint8>* getCoordinatesAround(Alive *a){
            std::vector<uint8> *around = new std::vector<uint8>();
            uint8 *coord;
            
            for(uint8 i = 0; i<8; ++i){
                coord = getCoordsByDirection(a, i);
                around->push_back(coord[0]);
                around->push_back(coord[1]);
            }

            return around;
        }

        uint8* getCoordsByDirection(Alive *a, uint8 dir){
            uint8 *coords = new uint8[2];
            switch(dir)
            {
                case 0:
                    coords[0] = a->x - 1;
                    coords[1] = a->y - 1;
                    break;
                case 1:
                    coords[0] = a->x;
                    coords[1] = a->y - 1;
                    break;
                case 2:
                    coords[0] = a->x + 1;
                    coords[1] = a->y - 1;
                    break;
                case 3:
                    coords[0] = a->x + 1;
                    coords[1] = a->y;
                    break;
                case 4:
                    coords[0] = a->x + 1;
                    coords[1] = a->y + 1;
                    break;
                case 5:
                    coords[0] = a->x;
                    coords[1] = a->y + 1;
                    break;
                case 6:
                    coords[0] = a->x - 1;
                    coords[1] = a->y + 1;
                    break;
                case 7:
                    coords[0] = a->x - 1;
                    coords[1] = a->y;
                    break;
                default:
                    coords[0] = a->x;
                    coords[1] = a->y;
                    break;
            }
            return coords;
        }

        void remove_food(Food *food){
            std::vector<Food*>::size_type size = this->food.size();
            for(std::vector<Food*>::size_type i = 0; i < size; ++i){
                if(this->food[i] == food){
                    this->food.erase(this->food.begin() + i);
                    delete food;
                    return;
                }
            }
        }

        void remove_alive(Alive *alive){
            std::vector<Alive*>::size_type size = this->alives.size();
            for(std::vector<Alive*>::size_type i = 0; i < size; ++i){
                if(this->alives[i] == alive){
                    this->alives.erase(this->alives.begin() + i);
                    delete alive;
                    return;
                }
            }
        }


};