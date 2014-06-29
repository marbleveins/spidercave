#ifndef PLAYERVIEWER_H
#define PLAYERVIEWER_H
#include <SFML/Graphics.hpp>
#include "Player.h"

class PlayerViewer
{
    public:
        PlayerViewer(Player *p_player);

        void draw(sf::RenderWindow *gameWindow, Player *player);
        void update(Player *player);
    private:

        std::vector <sf::Texture> getTiles(int line);
        bool validateIMG(sf::Image img, sf::Color backColor);

        std::vector <sf::Texture> TEXvectorStand;
        std::vector <sf::Texture> TEXvectorRun;
        std::vector <sf::Texture> TEXvectorJump;
        std::string playerFileName;
        int playerCurrentTile;

};

#endif // PLAYERVIEWER_H
