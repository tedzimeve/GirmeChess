#pragma once
#include "GrimeCells.hpp"
#include <exception>
#include <string_view>

namespace GrimeObjects{

Cell* selectRandomCellType(Point position){
    int randomIndex = std::rand() % cellsTypesCount + 1;
    switch (randomIndex)
    {
    case black:
        return new BlackCell(position);
    case white:
        return new WhiteCell(position);
    default:
        throw std::runtime_error("selectRandomCellType error");
    }
}

class Map: public PointMap{
    public:
        const std::vector<Cell*>* getCells(){return &m_cellsVector;}
        void generateCellMap(){
            for(Point pos: *getMapPoints()){
                m_cellsVector.emplace_back(selectRandomCellType(pos));
            }
        }
        Map(int xLength, int yLength): PointMap(xLength, yLength){
            generateCellMap();
        }
        bool isContainPoint(Point point){
            for(Point pos: *getMapPoints()){
                if(pos == point) return true;
            }
            return false;
        }
        Cell* tryGetCell(Point cellPosition){
            for(Cell* cell: m_cellsVector){
                if(cellPosition == cell->getPosition()) return cell;
            }
            return nullptr;
        }
    private:
        std::vector<Cell*> m_cellsVector;
};
}
