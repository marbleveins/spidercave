#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Scenario.h"

Player::Player()
{
    //ctor
    state = Standing;
    xPos = 100;
    yPos = 100;
    velocityX = 0;
    velocityY = 0;
    acceleration = 1;
    resistance = 0;

}

void Player::move(float _gravity)
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
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        velocityX = acceleration;
    }else
    {
        velocityX = 0;
    }

    yPos += velocityY;
    xPos += velocityX;
}

void Player::colission(Scenario *p_scenario)
{
     while ( (*p_scenario).getTileCode(xPos, yPos+35) == 1)
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
