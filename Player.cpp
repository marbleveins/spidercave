#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Scenario.h"

#define PLAYERSIZE 32

Player::Player(Scenario *p_scenario)
{
    //ctor
    state = Standing;
    xPos = 23;
    yPos = 210;
    velocityX = 0;
    velocityY = 0;
    acceleration = 1.5;
    resistance = 0;
    animFrame = 1;
    animTimeCounter = 0;

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
    updateTex();
    draw(gameWindow);
}

void Player::moveAsKeyBoard(float _gravity)
{
    bool feetsOnFloor = feetOnFloor();
    if (feetsOnFloor == false)
        velocityY += _gravity;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        velocityX = -acceleration;
        if (feetsOnFloor) state = Running;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        velocityX = acceleration;
        if (feetsOnFloor) state = Running;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) == false
        && sf::Keyboard::isKeyPressed(sf::Keyboard::D) == false)
    {
        velocityX = 0;
        if (feetsOnFloor) state = Standing;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        if (state != Jumping)
        {
            velocityY = -4;
            state = Jumping;
        }
    }

    yPos += velocityY;
    xPos += velocityX;
}

void Player::colission()
{
    int tileCode = currentScenario->getTileCode(xPos+PLAYERSIZE/2, yPos+PLAYERSIZE-12);// -12 el tile pisable tiene una parte negra arriba entonces se reduce para que pise el piso del dibujo
    while ( tileCode == 2 || tileCode == 5 || tileCode == 6 || tileCode == 7)
    {
        yPos--;
        velocityY = 0;
        tileCode = currentScenario->getTileCode(xPos+PLAYERSIZE/2, yPos+PLAYERSIZE-12);
    }
    if (yPos+16 >= 450)
    {
        xPos = 23;
        yPos = 210;
        velocityY = 0;
    }
    if (xPos <= 16 || xPos+16 >= 315)
    {
        xPos = 23;
        yPos = 210;
        velocityY = 0;
    }
}
bool Player::feetOnFloor()
{
    int tileCode = currentScenario->getTileCode(xPos+PLAYERSIZE/2, yPos+PLAYERSIZE-12 +1);//no sacar el +1 porque empieza a rebotar
    if (tileCode == 2 || tileCode == 5 || tileCode == 6 || tileCode == 7)
        return true;

    return false;
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

void Player::updateTex()
{
    if (state == Standing)
    {
        sprite.setTexture(TEXvectorStand[animFrame-1]);
    }
    else if (state == Running)
    {
        sprite.setTexture(TEXvectorRun[animFrame-1]);
    }
    else if (state == Jumping)
    {
        sprite.setTexture(TEXvectorJump[animFrame-1]);
    }

    if (clock.getElapsedTime() >= sf::milliseconds(250/acceleration) )
    {
        animFrame ++;
        clock.restart();
    }
    if ( animFrame > currentAnimSize() )
        animFrame = 1;

    sprite.setPosition(xPos, yPos);
}
int Player::currentAnimSize()
{
    switch (state)
    {
    case Standing:
        return int( TEXvectorStand.size() );
        break;
    case Running:
        return int( TEXvectorRun.size() );
        break;
    case Jumping:
        return int( TEXvectorJump.size() );
        break;
    }
    return 1;
}
void Player::draw(sf::RenderWindow *gameWindow)
{
    gameWindow->draw(sprite);
}




























