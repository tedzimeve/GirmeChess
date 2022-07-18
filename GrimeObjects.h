#pragma once
#include <iostream>
#include <string>
#include <math.h>

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
            virtual void move(Event event){}
            virtual void attack(Event event){} //атака нужна, когда, например, пешка ходит вперёд, но атакует по диагонали 
            virtual void die(Event event){}
            Chessman(Point position, ChessmanType chessmanType): GrimeObject(position, chessman), chessmanType(chessmanType)
            {}
        private:
    };

    // Класс, обозначающий игровую клетку
    class Cell: public GrimeObject{
        public:
            // TODO: спрайт тут должен быть или чёто такое хз
            CellType cellType;
            virtual void onEnter(Chessman* chessman, Event event){}
            Cell(Point position, CellType cellType): GrimeObject(position, cell), cellType(cellType)
            {}
        private:
    };
}
