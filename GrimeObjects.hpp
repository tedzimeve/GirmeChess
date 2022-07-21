#pragma once
#include <iostream>
#include <string>
#include <math.h>
#include "Teams.hpp"
#include <vector>

namespace GrimeObjects{
    // Структура, обозначающая точку нахождения объекта в пространстве
    struct Point{
        Point(float x, float y){
            this->x = x;
            this->y = y;
        }
        int x;
        int y;

        void round(){
            x = roundf(x);
            y = roundf(y);
        }

        static Point upPoint(){
            return Point(0, 1);
        }

        static Point downPoint(){
            return Point(0, -1);
        }

        static Point rightPoint(){
            return Point(1, 0);
        }

        static Point leftPoint(){
            return Point(-1, 0);
        }

        Point& operator += (Point point2){
            x += point2.x;
            y += point2.y;
            return *this;
        }
        Point& operator -= (Point point2){
            x -= point2.x;
            y -= point2.y;
            return *this;
        }
        Point& operator *= (Point point2){
            x *= point2.x;
            y *= point2.y;
            return *this;
        }
        Point& operator /= (Point point2){
            x /= point2.x;
            y /= point2.y;
            return *this;
        }
    };
    // Тип игрового объекта
    enum eObjectType{
        cell, // игровая клетка
        chessman, //игровая фигура
    };
    // Простейший класс, описывающий базовый функционал любого игрового объекта
    class GrimeObject{
        public:
            eObjectType objectType;
            GrimeObject(Point position, eObjectType objectType) : m_position(position), objectType(objectType)
            {
            }
            Point getPosition(){
                return m_position;
            }
            void setPosition(Point position){
                // TODO: надо сделать, чтобы в этой функции менялась и позиция на экране
                m_position = position;
            }
        private:
            Point m_position;
    };

    // Общий класс события. Могут происходить при перемещении или атаке (хз)
    class Event{
        public:
            std::string identifier;
            void (*function) (Point eventPosition);
            GrimeObject* actionObject;
            Event(std::string identifier, void(*function) (Point)): identifier(identifier), function(function)
            {}
            // вообще это потом лучше вынести в отдельный файл, т.к. это не совсем объект + в целом надо доделать
        private:
    };

    // Общий примитивный класс карты. Может и (скорее всего) должен наследоваться более усложнёнными вариантами. 
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

    class Stuff{
    public:
    enum eDirection{
        up,
        down,
        right,
        left,

        upRight,
        upLeft,
        downRight,
        downLeft,

        all,
        allStraight,
        allDiagonal,
    };
    };
using eDirection = Stuff::eDirection;
#pragma region PointOperators
    Point operator + (Point point1, Point point2){
        return Point(point1.x + point2.x, point1.y + point2.y);
    }
    Point operator - (Point point1, Point point2){
        return Point(point1.x - point2.x, point1.y - point2.y);
    }
    Point operator * (Point point1, Point point2){
        return Point(point1.x * point2.x, point1.y * point2.y);
    }
    Point operator / (Point point1, Point point2){
        return Point(point1.x / point2.x, point1.y / point2.y);
    }

    bool operator == (Point point1, Point point2){
        return point1.x == point2.x && point1.y == point2.y;
    }
    bool operator != (Point point1, Point point2){
        return point1.x != point2.x && point1.y != point2.y;
    }
    bool operator > (Point point1, Point point2){
        float c1 = sqrt(pow(point1.x, 2) + pow(point1.y, 2));
        float c2 = sqrt(pow(point2.x, 2) + pow(point2.y, 2));
        return c1 > c2;
    }
    bool operator >= (Point point1, Point point2){
        float c1 = sqrt(pow(point1.x, 2) + pow(point1.y, 2));
        float c2 = sqrt(pow(point2.x, 2) + pow(point2.y, 2));
        return c1 >= c2;
    }
    bool operator < (Point point1, Point point2){
        float c1 = sqrt(pow(point1.x, 2) + pow(point1.y, 2));
        float c2 = sqrt(pow(point2.x, 2) + pow(point2.y, 2));
        return c1 < c2;
    }
    bool operator <= (Point point1, Point point2){
        float c1 = sqrt(pow(point1.x, 2) + pow(point1.y, 2));
        float c2 = sqrt(pow(point2.x, 2) + pow(point2.y, 2));
        return c1 <= c2;
    }

    std::ostream& operator << (std::ostream& oStream, Point point){
        return oStream << "x: "<< point.x<< "\ty: "<< point.y;
    }
#pragma endregion
   
    // Тип игровой фигуры
    enum eChessmanType{
        //пешка
        pawn,
        queen,
        king,
    };

    enum eCellType{
        black = 1,
        white,
    };

    class Cell;

    // Класс, обозначающий игровую шахматную фигуру
    class Chessman: public GrimeObject{
        public:
            eChessmanType chessmanType;
            bool isCanSwim;
            bool isCanFly;
            Team team;
            virtual std::vector<Cell*> doOnSelect(PointMap* map){}; //здесь будем выводить доступные для ходьбы клетки
            virtual void move(Point moveTo, Event* event){setPosition(moveTo);}
            virtual void attack(Point moveTo, Event* event){setPosition(moveTo); /*TODO:функция убийства*/} //атака нужна, когда, например, пешка ходит вперёд, но атакует по диагонали 
            virtual void die(Event* event){}
            Chessman(Point position, eChessmanType chessmanType): GrimeObject(position, chessman), chessmanType(chessmanType)
            {isCanFly = false; isCanSwim = false;}
            Chessman(Point position, eChessmanType chessmanType, bool isCanSwim, bool isCanFly): GrimeObject(position, chessman), chessmanType(chessmanType), isCanSwim(isCanSwim), isCanFly(isCanFly)
            {}
        private:
    };

    // Класс, обозначающий игровую клетку
    class Cell: public GrimeObject{
        public:
            // TODO: спрайт тут должен быть или чёто такое хз
            eCellType cellType;
            bool isPassable;
            Chessman* chessmanOnCell = nullptr;
            virtual void doOnEnter(Chessman* chessman, Event* event){chessmanOnCell = chessman;}
            virtual void doOnExit(Chessman* chessman, Event* event){chessmanOnCell = nullptr;}
            Cell(Point position, eCellType cellType, bool isPassable): GrimeObject(position, cell), cellType(cellType), isPassable(isPassable)
            {}
        private:
    };
}
