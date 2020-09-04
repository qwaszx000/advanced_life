#pragma once

typedef unsigned char uint8;

class Alive{
    static uint8 id_counter;
    public:
        uint8 hp = 100, energy = 40, current_command_index = 0, id, x = 0, y = 0, heard_x = 0, heard_y = 0, heard_id = 0, heard_data = 0, last_command = 0, cmp_result = 0;
        uint8 memory[4];
        uint8 dna_code[128];

        Alive(uint8 x, uint8 y){
            this->x = x;
            this->y = y;
            this->id = Alive::id_counter;
            Alive::id_counter++;
        }
};

uint8 Alive::id_counter = 0;