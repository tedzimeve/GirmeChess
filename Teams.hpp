#pragma once
#include <iostream>

static const int maxTeamsCountInGame = 2;

class Team{
    public:
        int getTeamIndex(){return m_teamIndex;}
        void setTeamIndex(int index){
            if(index > maxTeamsCountInGame || index == 0){
                std::cout<<"Индекс команды не может быть больше "<<maxTeamsCountInGame<<"/ или равен нулю"<<std::endl;
            }
            else{
                m_teamIndex = index;
            }
        }
        Team(int index){
            setTeamIndex(index);
        }
        Team(){}
    private:
        unsigned int m_teamIndex;
};