#pragma once
#include "GrimeObjects.h"

namespace GrimeObjects{
    class WhiteCell: public Cell{
        public:
            WhiteCell(Point position): Cell(position, white)
            {}
        private:
    };

    class BlackCell: public Cell{
        public:
            BlackCell(Point position): Cell(position, black)
            {}
        private:
    };
}
