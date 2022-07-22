#pragma once
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include "Teams.hpp"
#include "GrimeMap.hpp"

namespace GrimeObjects{
    #pragma region CoreTypes
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
    #pragma endregion

    #pragma region Chessman
    // Тип игровой фигуры
    enum eChessmanType{
        //пешка
        pawn,
        queen,
        king,
    };

    class Cell;

    // Класс, обозначающий игровую шахматную фигуру
    class Chessman: public GrimeObject{
        public:
            eChessmanType chessmanType;
            bool isCanSwim;
            bool isCanFly;
            Team team;
            virtual std::vector<Cell*> doOnSelect(Map* map){}; //здесь будем выводить доступные для ходьбы клетки
            virtual void move(Point moveTo, Event* event){setPosition(moveTo);}
            virtual void die(Event* event){}
            virtual void attack(Point moveTo, std::vector<Chessman*> attackedChessmans ,Event* event){//атака нужна, когда, например, пешка ходит вперёд, но атакует по диагонали 
                setPosition(moveTo);
                for(Chessman* chessman: attackedChessmans){
                    chessman->die(nullptr);
                }
            } 
            Chessman(Point position, eChessmanType chessmanType): GrimeObject(position, chessman), chessmanType(chessmanType)
            {isCanFly = false; isCanSwim = false;}
            Chessman(Point position, eChessmanType chessmanType, bool isCanSwim, bool isCanFly): GrimeObject(position, chessman), chessmanType(chessmanType), isCanSwim(isCanSwim), isCanFly(isCanFly)
            {}
        private:
    };
    #pragma endregion

    #pragma region Cell
    enum eCellType{
        black = 1,
        white,
    };
    /**
     * @brief 
     * Структура, отражающая рельеф клетки
     * @param isPassable абсолютная проходимость. Если true, то клетку нельзя никак преодолеть
     * @param isHightPlace клетку с таким рельефом могут преодолевать летающие существа. Может быть использовано, например, для гор
     * @param isLiquidPlace клетку с таким рельефом могут преодолевать плавающие существа. Может быть использовано, например, для воды
     */
    struct CellRelief{
        bool isPassable = true;
        bool isHightPlace = false;
        bool isLiquidPlace = false;
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
    #pragma endregion
}
