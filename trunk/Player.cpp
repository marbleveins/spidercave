#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Scenario.h"

#define PLAYERSIZE 32

Player::Player(Scenario *p_scenario)
{
    //ctor
    state = Standing;
    xPos = 20;
    yPos = 210;
    speedX = 0;
    speedY = 0;

    movScale = 1;
    accelX = 1.5 * movScale;
    decelX = 3 * movScale;
    maxSpeedX = 3 * movScale;
    maxSpeedY = 10 * movScale;
    jumpStartSpeedY = 4 * movScale;
    jumping = false;
    jumpKeyDown = false;

    animFrame = 1;
    animTimeCounter = 0;

    playerFileName = "playerset.png";
    TEXvectorStand = getTiles(1);
    TEXvectorRun = getTiles(2);
    TEXvectorJump = getTiles(3);

    initCollisionPoint();


    currentScenario = p_scenario;
}

void Player::update(sf::RenderWindow *gameWindow)
{
    suavizarContacto();
    yPos += speedY;
    xPos += speedX;
    if (xPos >= 320-PLAYERSIZE || xPos <= 0 || yPos >= 480-PLAYERSIZE || yPos <= 0) {xPos = 20;yPos = 210;}
    moveAsKeyBoard(currentScenario->gravity);
    updateTex();
    draw(gameWindow);
}

void Player::moveAsKeyBoard(float _gravity)
{
    bool moveRequest = false;
    bool feetsOnFloor = feetOnFloor();

    if (feetsOnFloor) jumping = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        speedX -= accelX;
        if (feetsOnFloor) state = Running;
        moveRequest = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        speedX += accelX;
        if (feetsOnFloor) state = Running;
        moveRequest = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !jumping && !jumpKeyDown)
    {
        speedY = -jumpStartSpeedY;
        state = Jumping;
        jumping = true;
        jumpKeyDown = true;
    }
    if ( ! sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        jumpKeyDown = false;

    if (speedX > maxSpeedX) speedX = maxSpeedX;
    if (speedX < -maxSpeedX) speedX = -maxSpeedX;
    if (speedY < -maxSpeedY) speedY = -maxSpeedY;

    speedY += currentScenario->gravity;

    if (!moveRequest)
    {
        if (speedX < 0) speedX += decelX;
        if (speedX > 0) speedX -= decelX;
        if (speedX > 0 && speedX < decelX) speedX = 0;
        if (speedX < 0 && speedX > -decelX) speedX = 0;
        if (!jumping) state = Standing;
    }

}
void Player::suavizarContacto()
{
    bool contactX = false, contactYbottom = false, contactYtop = false;//tipo de colision
    float nextMoveX = speedX;
    float nextMoveY = speedY;

    float projectedMoveX, projectedMoveY, originalMoveX, originalMoveY;
    originalMoveX = nextMoveX;
    originalMoveY = nextMoveY;
    //////////////////////////////////////
    //aca se fija la colision, ver katy's
    // Iterate over each object whose bounding box intersects with the player's bounding box
    // until a collision is found
    for (int dir = 0; dir < 4; dir++) {
        // Skip the test if the expected direction of movement makes the test irrelevant
        // For example, we only want to test the top of the player's head when movement is
        // upwards, not downwards. This is really important! If we don't do this, we can
        // make corrections in the wrong direction, causing the player to magically jump
        // up to platforms or stick to ceilings.
        if (dir == 0 && nextMoveY > 0) continue;
        if (dir == 1 && nextMoveY < 0) continue;
        if (dir == 2 && nextMoveX > 0) continue;
        if (dir == 3 && nextMoveX < 0) continue;
        projectedMoveX = (dir >= 2? nextMoveX : 0);
        projectedMoveY = (dir <  2? nextMoveY : 0);

        while ( currentScenario->collides(collisionPoint[dir*2][0] + xPos + projectedMoveX,
                                          collisionPoint[dir*2][1] + yPos + projectedMoveY)//punto 1 o punto 2 de cada parte del personaje
               || currentScenario->collides(collisionPoint[dir*2+1][0] + xPos + projectedMoveX,
                                        collisionPoint[dir*2+1][1] + yPos + projectedMoveY))
        {
            if (dir == 0) projectedMoveY++;
            if (dir == 1) projectedMoveY--;
            if (dir == 2) projectedMoveX++;
            if (dir == 3) projectedMoveX--;
        }

        if (dir >= 2 && dir <= 3) nextMoveX = projectedMoveX;
        if (dir >= 0 && dir <= 1) nextMoveY = projectedMoveY;

        // Close the for loop (repeat for all four directions)
    }

    // Detect what type of contact has occurred based on a comparison of
    // the original expected movement vector and the new one
    if (nextMoveY > originalMoveY && originalMoveY < 0)
    {
        contactYtop = true;
    }
    if (nextMoveY < originalMoveY && originalMoveY > 0)
    {
        contactYbottom = true;
    }
    if (abs(nextMoveX - originalMoveX) > 0.01f)
    {
        contactX = true;
    }

    // The player can't continue jumping if we hit the side of something, must fall instead
    // Without this, a player hitting a wall during a jump will continue trying to travel
    // upwards
    if (contactX && contactYtop && speedY < 0)
        speedY = nextMoveY = 0;

    // If a contact has been detected, apply the re-calculated movement vector
    // and disable any further movement this frame (in either X or Y as appropriate)
    if (contactYbottom || contactYtop)
    {
        yPos += nextMoveY;
        speedY = 0;

        if (contactYbottom)
            jumping = false;
    }

    if (contactX)
    {
        xPos += nextMoveX;
        speedX = 0;
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

    if (clock.getElapsedTime() >= sf::milliseconds(250/accelX) )
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

void Player::initCollisionPoint()
{
    int points[8][2] = {
        { 13,  5  }, { 17, 5  }, // Top of head
        { 13,  30 }, { 17, 30 }, // Feet
        { 6,  10 }, { 6,  20 }, // Left arm
        { 25, 10 }, { 25, 20 }  // Right arm
    };
    for (int i = 0; i < 8; i++)
    {
        collisionPoint[i][0] = points[i][0];
        collisionPoint[i][1] = points[i][1];
    }
}























