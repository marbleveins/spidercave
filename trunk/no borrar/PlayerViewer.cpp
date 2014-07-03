#include <SFML/Graphics.hpp>
#include <fstream>
#include "PlayerViewer.h"

#define PLAYERSIZE 32

PlayerViewer::PlayerViewer()
{
    //ctor
    playerFileName = "playerset.png";
    TEXvectorStand = getTiles(1);
    TEXvectorRun = getTiles(2);
    TEXvectorJump = getTiles(3);

}

std::vector <sf::Texture> PlayerViewer::getTiles(int line)
{
    std::vector <sf::Texture> result;
    sf::Image playerFile;
    std::string filePath = "" + playerFileName;
    if (playerFile.loadFromFile(filePath.c_str()))
    {
        sf::Color backColor = playerFile.getPixel(0,0);
        int columnAmount = playerFile.getSize().x/PLAYERSIZE;
        for (int column = 1 ; column<=columnAmount ; column++)
        {
            sf::Image playerIMG;
            sf::Texture playerTEX;
            playerIMG.create(PLAYERSIZE, PLAYERSIZE, sf::Color::Transparent);
            for (int y=1 ; y<=PLAYERSIZE ; y++)
            {
                for (int x=1 ; x<=PLAYERSIZE ; x++)
                {
                    sf::Color setColor = playerFile.getPixel(x-1+(PLAYERSIZE*(column-1)), y-1+(PLAYERSIZE*(line-1)));
                    if (setColor != backColor)
                        playerIMG.setPixel(x-1, y-1, setColor);
                    if (x>=PLAYERSIZE && y>=PLAYERSIZE )
                    {
                        if (validateIMG(playerIMG, backColor))
                        {
                            playerTEX.loadFromImage(playerIMG);
                            result.push_back(playerTEX);

                        }
                        else return result;
                    }
                }
            }
        }
    }
    return result;
}

bool PlayerViewer::validateIMG(sf::Image img, sf::Color backColor)
{
    int space = PLAYERSIZE/8;
    for (int y=1 ; y<=PLAYERSIZE ; y+=space)
    {
        for (int x=1 ; x<=PLAYERSIZE ; x+=space)
        {
            if ((img.getPixel(x-1,y-1) != backColor) && (img.getPixel(x-1,y-1) != sf::Color::Transparent))
                return true;
        }
    }
    return false;
}

void PlayerViewer::update(Player *player)
{
    if (player->state == player->Standing)
    {
        player->sprite.setTexture(TEXvectorStand[0]);
    }
    else if (player->state == player->Running)
    {
        player->sprite.setTexture(TEXvectorRun[0]);
    }
    else if (player->state == player->Jumping)
    {
        player->sprite.setTexture(TEXvectorJump[0]);
    }

    player->sprite.setPosition(player->xPos, player->yPos);
}

void PlayerViewer::draw(sf::RenderWindow *gameWindow, Player *player)
{
    (*gameWindow).draw(player->sprite);
}
