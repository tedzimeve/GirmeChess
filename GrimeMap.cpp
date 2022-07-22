#pragma once
#include "GrimeCells.hpp"
#include <exception>
#include <string_view>

namespace GrimeObjects{

class PointMap{
        public:
            const std::vector<Point>* getMapPoints(){return &m_allCellsPositions;}
            Point getMapSize(){return m_mapSize;}
            void generatePointMap(int xLength, int yLength){
                for(int x = 0; x < xLength; x++){
                    for(int y = 0; y < yLength; y++){
                        m_allCellsPositions.emplace_back(Point(x, y));
                    }
                }
                m_mapSize.x = xLength;
                m_mapSize.y = yLength;
            }
            PointMap(int xLength, int yLength){
                generatePointMap(xLength, yLength);
            }
        private:
            std::vector<Point> m_allCellsPositions;
            Point m_mapSize = Point(0,0);
};

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
