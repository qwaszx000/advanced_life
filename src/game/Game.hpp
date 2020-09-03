#pragma once

#include <vector>
#include "alive/alive.hpp"

class Game{
    public:
        bool paused = true;
        uint8 width, height;
        std::vector<Alive*> alives;

        Game(uint8 width, uint8 height){
            this->width = width;
            this->height = height;
            this->alives.push_back( new Alive(1, 1) );
        }
};