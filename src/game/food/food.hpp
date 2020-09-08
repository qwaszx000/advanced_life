#pragma once

#include "../utils.hpp"

class Food{
    public:
        uint8 x, y, energy=40;

        Food(uint8 x, uint8 y, uint8 energy = 40){
            this->x = x;
            this->y = y;
            this->energy = energy;
        }
};