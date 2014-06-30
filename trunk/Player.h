#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Graphics.hpp>
#include "Scenario.h"

class Player
{
    public:
        Player(Scenario *p_scenario);
        void moveAsKeyBoard(float _gravity);
        void colission();
        void update(sf::RenderWindow *gameWindow);

        Scenario *currentScenario;

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

        void draw(sf::RenderWindow *gameWindow);
        void updateView();

    private:
        std::vector <sf::Texture> getTiles(int line);
        bool validateIMG(sf::Image img, sf::Color backColor);

        std::vector <sf::Texture> TEXvectorStand;
        std::vector <sf::Texture> TEXvectorRun;
        std::vector <sf::Texture> TEXvectorJump;
        std::string playerFileName;
        int playerCurrentTile;

};

#endif // PLAYER_H

