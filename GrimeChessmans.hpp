#pragma once
#include "GrimeObjects.hpp"
#include "GrimeMap.cpp"

namespace GrimeObjects{

    std::vector<Cell*> getCellsCanMoveToFigure(Map* map, Point chessmanPosition, Point moveFigure){
    }
    /**
     * @brief 
     * Добавляет в указанный вектор новые клетки, находящиеся на пути фигуры и удовлетворяющие условиям
     * @param map Игровая карта
     * @param cellsVector Ссылка на изменяемый вектор
     * @param iteratorPosition Итератор. Перебирает все клетки, прибавляя к своему значению moveStep
     * @param endPosition Значение, после которого итератор перестаёт работать
     * @param moveStep Двумерный шаг, на который движется итератор
     */
    void appendCellsOnDirection(Map* map, std::vector<Cell*>& cellsVector, Point iteratorPosition, const Point endPosition, Point moveStep){
        do{
            Cell* cell = map->tryGetCell(iteratorPosition);
            if(cell != nullptr){
                if(cell->chessmanOnCell == nullptr && cell->isPassable){
                    cellsVector.emplace_back(cell);
                }
            }
            iteratorPosition += moveStep;
        } while (iteratorPosition <= endPosition);
    }

    std::vector<Cell*> getCellsCanMoveTo(Map* map, Point chessmanPosition, Point movePoints, eDirection moveDirection){
        std::vector<Cell*> cellsOnPath;
        switch (moveDirection)
        {
        case eDirection::up:
            Point actualPoint = chessmanPosition + Point::upPoint();
            appendCellsOnDirection(map, cellsOnPath, actualPoint, chessmanPosition + movePoints, Point::upPoint());
            break;
        default:
            break;
        }
    }

    class Pawn: Chessman{
        public:
            Pawn(Point position): Chessman(position, pawn)
            {}
            std::vector<Cell*> doOnSelect(Map* map) override{
                int i = 1;
            }
        private:
    };
}