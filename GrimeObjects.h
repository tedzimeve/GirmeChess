#pragma once
#include <iostream>
#include <string>
#include <math.h>
#include "Teams.h"
#include <vector>

namespace GrimeObjects{
    // Структура, обозначающая точку нахождения объекта в пространстве
    struct Point{
        Point(float x, float y){
            this->x = x;
            this->y = y;
        }
        float x;
        float y;

        void round(){
            x = roundf(x);
            y = roundf(y);
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
    enum ObjectType{
        cell, // игровая клетка
        chessman, //игровая фигура
    };
    // Простейший класс, описывающий базовый функционал любого игрового объекта
    class GrimeObject{
        public:
            ObjectType objectType;
            GrimeObject(Point position, ObjectType objectType) : m_position(position), objectType(objectType)
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
            std::vector<Point>* getMapPoints(){return &m_allCellsPositions;}
            void generateMap(int xLength, int yLength){
                for(int x = 0; x < xLength; x++){
                    for(int y = 0; y < yLength; y++){
                        m_allCellsPositions.push_back(Point(x, y));
                    }
                }
            }
        private:
            std::vector<Point> m_allCellsPositions;
    };

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

    // Тип игровой фигуры
    enum ChessmanType{
        //пешка
        pawn,
        queen,
        king,
    };

    enum CellType{
        black,
        white,
    };

    // Класс, обозначающий игровую шахматную фигуру
    class Chessman: public GrimeObject{
        public:
            ChessmanType chessmanType;
            bool isCanSwim;
            bool isCanFly;
            Team team;
            virtual std::vector<Point> doOnSelect(PointMap* map){}; //здесь будем выводить доступные для ходьбы клетки
            virtual void move(Point moveTo, Event* event){setPosition(moveTo);}
            virtual void attack(Point moveTo, Event* event){setPosition(moveTo); /*TODO:функция убийства*/} //атака нужна, когда, например, пешка ходит вперёд, но атакует по диагонали 
            virtual void die(Event* event){}
            Chessman(Point position, ChessmanType chessmanType): GrimeObject(position, chessman), chessmanType(chessmanType)
            {isCanFly = false; isCanSwim = false;}
            Chessman(Point position, ChessmanType chessmanType, bool isCanSwim, bool isCanFly): GrimeObject(position, chessman), chessmanType(chessmanType), isCanSwim(isCanSwim), isCanFly(isCanFly)
            {}
        private:
    };

    // Класс, обозначающий игровую клетку
    class Cell: public GrimeObject{
        public:
            // TODO: спрайт тут должен быть или чёто такое хз
            CellType cellType;
            bool isPassable;
            virtual void doOnEnter(Chessman* chessman, Event* event){}
            Cell(Point position, CellType cellType, bool isPassable): GrimeObject(position, cell), cellType(cellType), isPassable(isPassable)
            {}
        private:
    };
}
