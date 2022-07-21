#pragma once
#include "GrimeObjects.hpp"

namespace GrimeObjects{
    const int cellsTypesCount = 2;
    class WhiteCell: public Cell{
        public:
            WhiteCell(Point position): Cell(position, white, true)
            {}
        private:
    };

    class BlackCell: public Cell{
        public:
            BlackCell(Point position): Cell(position, black, true)
            {}
        private:
    };
}
