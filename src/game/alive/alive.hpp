#pragma once

#include <array>
#include "../utils.hpp"

class Alive{
    static uint8 id_counter;
    public:
        double color_red = 1.0, color_green, color_blue;
        uint8 hp = 100, current_command_index = 0, id, x = 0, y = 0, heard_x = 0, heard_y = 0, heard_id = 0, heard_data = 0, last_command = 0, cmp_result = 0;
        uint8 memory[4], selected_memory = 0;
        uint8 dna_code[128];
        //std::array<uint8, 128> dna_code;
        u_int16_t energy = 40;

        Alive(uint8 x, uint8 y){
            this->x = x;
            this->y = y;
            this->id = Alive::id_counter;
            
            if(Alive::id_counter == 255){
                Alive::id_counter = 0;
            } else {
                Alive::id_counter++;
            }
        }

        void setDna(uint8 genes[]){
            for(uint8 i = 0; i < ARRAY_LEN(genes); ++i){
                this->dna_code[i] = genes[i];
            }
        }

        void starve(uint8 energy_consumption){
            if(this->energy <= 0){
                if(this->hp < energy_consumption){
                    this->hp = 0;
                } else {
                    this->hp -= energy_consumption;
                }
            } else {
                if(this->energy < energy_consumption){
                    this->energy = 0;
                } else {
                    this->energy -= energy_consumption;
                }
            }
        }


};

uint8 Alive::id_counter = 0;