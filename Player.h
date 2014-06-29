#ifndef PLAYER_H
#define PLAYER_H
#include "Scenario.h"

class Player
{
    public:
        Player();
        void move(float _gravity);
        void colission(Scenario *p_scenario);

        float xPos;
        float yPos;
        float velocityX;
        float velocityY;
        float acceleration;
        int state;
        float resistance;
        sf::String direction;
        sf::Sprite sprite;

        enum states {
            Standing=0,
            Running=1,
            Jumping=2
        };

    private:


};

#endif // PLAYER_H

