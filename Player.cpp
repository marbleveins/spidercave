#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Scenario.h"

#define PLAYERSIZE 32

Player::Player(Scenario *p_scenario)
{
    //ctor
    state = Standing;
    xPos = 100;
    yPos = 100;
    velocityX = 0;
    velocityY = 0;
    acceleration = 1;
    resistance = 0;

    playerFileName = "playerset.png";
    TEXvectorStand = getTiles(1);
    TEXvectorRun = getTiles(2);
    TEXvectorJump = getTiles(3);

    currentScenario = p_scenario;
}

void Player::update(sf::RenderWindow *gameWindow)
{
    moveAsKeyBoard(currentScenario->gravity);
    colission();
    updateView();
    draw(gameWindow);
}

void Player::moveAsKeyBoard(float _gravity)
{
    velocityY += _gravity;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        velocityY = -2;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        velocityX = -acceleration;
        state = Running;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        velocityX = acceleration;
        state = Running;
    }else
    {
        velocityX = 0;
    }

    yPos += velocityY;
    xPos += velocityX;
}

void Player::colission()
{
     while ( currentScenario->getTileCode(xPos, yPos+20) == 2
            || currentScenario->getTileCode(xPos, yPos+20) == 5
            || currentScenario->getTileCode(xPos, yPos+20) == 6
            || currentScenario->getTileCode(xPos, yPos+20) == 7)
    {
        yPos--;
        velocityY = 0;
    }
    if (yPos+35 >= 450)
    {
        yPos = 100;
        velocityY = 0;
    }
    if (xPos <= 17 || xPos+35 >= 315)
    {
        xPos = 100;
    }
}

std::vector <sf::Texture> Player::getTiles(int line)
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

bool Player::validateIMG(sf::Image img, sf::Color backColor)
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

void Player::updateView()
{
    if (state == Standing)
    {
        sprite.setTexture(TEXvectorStand[0]);
    }
    else if (state == Running)
    {
        sprite.setTexture(TEXvectorRun[0]);
    }
    else if (state == Jumping)
    {
        sprite.setTexture(TEXvectorJump[0]);
    }

    sprite.setPosition(xPos, yPos);
}

void Player::draw(sf::RenderWindow *gameWindow)
{
    gameWindow->draw(sprite);
}
