#pragma once
#include "GrimeObjects.h"

namespace GrimeObjects{
    class Pawn: Chessman{
        public:
            Pawn(Point position): Chessman(position, pawn)
            {}
            void move(Event event) override{
                setPosition(getPosition() += Point(0, 1));
            }
            void attack(Event event) override{
                
            }
        private:
    };
}