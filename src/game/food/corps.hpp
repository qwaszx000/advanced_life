#pragma once

typedef unsigned char uint8;

class Corps{
    public:
        uint8 x, y, energy=40;

        Corps(uint8 x, uint8 y, uint8 energy = 40){
            this->x = x;
            this->y = y;
            this->energy = energy;
        }
};