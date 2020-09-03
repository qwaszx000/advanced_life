#pragma once

typedef unsigned char uint8;

class Alive{
    public:
        uint8 hp, energy, current_command_index, id, x, y, heard_x, heard_y, heard_id, heard_data, last_command, cmp_result, energy_to_split;
        uint8 memory[4];
        uint8 dna_code[128];

        Alive(uint8 x, uint8 y){
            this->x = x;
            this->y = y;
        }
};