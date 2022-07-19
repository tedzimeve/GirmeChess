#pragma once
#include "GrimeObjects.h"

namespace GrimeObjects{
    class Pawn: Chessman{
        public:
            Pawn(Point position): Chessman(position, pawn)
            {}
            std::vector<Point> doOnSelect(PointMap* map) override{

            }
        private:
    };
}