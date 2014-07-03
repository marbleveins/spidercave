#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Graphics.hpp>
#include "Scenario.h"

class Player
{
    public:
        Player(Scenario *p_scenario);
        void moveAsKeyBoard(float _gravity);
        void collision();
        bool feetOnFloor();
        void update(sf::RenderWindow *gameWindow);
        int currentAnimSize();
        void suavizarContacto();
        Scenario *currentScenario;

        float xPos;
        float yPos;
        float speedX;
        float speedY;
        float movScale;
        float accelX, decelX;
        float maxSpeedX, maxSpeedY;

        float jumpStartSpeedY;
        bool jumping, jumpKeyDown;
        int iterations;
        int state;
        sf::String direction;
        sf::Sprite sprite;
        int animFrame;//0 a 7 - o la cantidad de frames que haya
        double animTimeCounter;
        sf::Clock clock;

        int collisionPoint[8][2];

        enum states {
            Standing=0,
            Running=1,
            Jumping=2
        };

        void draw(sf::RenderWindow *gameWindow);
        void updateTex();

    private:
        std::vector <sf::Texture> getTiles(int line);
        bool validateIMG(sf::Image img, sf::Color backColor);

        std::vector <sf::Texture> TEXvectorStand;
        std::vector <sf::Texture> TEXvectorRun;
        std::vector <sf::Texture> TEXvectorJump;
        std::string playerFileName;
        int playerCurrentTile;
        void initCollisionPoint();

};

#endif // PLAYER_H

