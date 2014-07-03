#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include "Scenario.h"
#include <math.h>

#define SCREENWIDTH 320
#define SCREENHEIGHT 480
#define TILESIZE 16

Scenario::Scenario(int levelId, std::string levelName)
{
    //ctor
    gravity = 0.3;
    currentLevelId = levelId;
    currentLevelName = levelName;
    currentMap = getMap(currentLevelName);
    collisionPoint = getCollisionPoints();
}



std::vector <std::vector <int> > Scenario::getMap(std::string levelName)
{
    std::vector <std::vector <int> > result;
    std::vector <int> lineOfMap;
    std::string filePath = std::string ("maps/") + levelName;
    std::string tile;
    std::ifstream file(filePath.c_str());
    if (file.is_open())
    {
        while (!file.eof())
        {
            std::string line;
            std::getline(file, line);
            if (line.find("[MAP]") != std::string::npos)
            {
                while (line.find("[/MAP]") == std::string::npos)
                {
                    std::getline(file, line);
                    std::stringstream lineStream(line);
                    while (!lineStream.eof())
                    {
                        std::getline(lineStream, tile, ' ');
                        lineOfMap.push_back(atoi(tile.c_str()));
                    }
                    result.push_back(lineOfMap);
                    lineOfMap.clear();
                }
            }
        }
    }
    file.close();
    return result;
}

int Scenario::getTileCode(int x, int y)
{
    int xTile = ceil( x/(SCREENWIDTH/(SCREENWIDTH/TILESIZE)) );
    int yTile = ceil( y/(SCREENHEIGHT/(SCREENHEIGHT/TILESIZE)) );

    return (currentMap[yTile])[xTile];
}

std::map<int, std::map<int,int> > Scenario::getCollisionPoints()
{
    /**
    *   uno para cada tipo de tile? no jodas, RE-IMPLEMENTAR
    **/
    std::map<int, std::map<int,int> > result;
    for (std::vector<std::vector<int> >::iterator itl=currentMap.begin(); itl < currentMap.end(); ++itl)
    {
        int l = itl - currentMap.begin();// l es el index del currentMap actual
        for (std::vector<int>::iterator itc=itl->begin(); itc < itl->end(); ++itc)
        {
            int c = itc - itl->begin();// c es el index del del mapa en el iterator itL

            //tiles completamente Solidos
            //Y PAREDES DERECHA XQ EL PERSONAJE TENDRIA QUE QUEDAR DETRAS DE LA PARED Y SE DIBUJA DSPS...
            if ( currentMap[l][c] == 1)
            {
                for (int x=0; x < TILESIZE; x++)
                {
                    for (int y=0; y < TILESIZE; y++)
                    {
                        result[x+(c*TILESIZE)][y+(l*TILESIZE)] = 1;//!=0
                        //result[x+(l*TILESIZE)].insert(std::make_pair( y+(c*TILESIZE), 1 ));
                    }
                }

            }
            //tiles esquinero derecha abajo
            if ( currentMap[l][c] == 3)
            {
                for (int x=8; x < TILESIZE; x++)
                {
                    for (int y=11; y < TILESIZE; y++)
                    {
                        result[x+(c*TILESIZE)][y+(l*TILESIZE)] = 1;//!=0
                        //result[x+(l*TILESIZE)].insert(std::make_pair( y+(c*TILESIZE), 1 ));
                    }
                }

            }
            //tiles esquina caida derecha abajo
            if ( currentMap[l][c] == 7)
            {
                for (int x=0; x < 8; x++)
                {
                    for (int y=11; y < TILESIZE; y++)
                    {
                        result[x+(c*TILESIZE)][y+(l*TILESIZE)] = 1;//!=0
                        //result[x+(l*TILESIZE)].insert(std::make_pair( y+(c*TILESIZE), 1 ));
                    }
                }

            }
            //tiles d
            //tiles de esquina caida izquierda abajo
            if ( currentMap[l][c] == 5 || currentMap[l][c] == 6)
            {
                for (int x=8; x < TILESIZE; x++)
                {
                    for (int y=11; y < TILESIZE; y++)
                    {
                        result[x+(c*TILESIZE)][y+(l*TILESIZE)] = 1;//!=0
                        //result[x+(l*TILESIZE)].insert(std::make_pair( y+(c*TILESIZE), 1 ));
                    }
                }

            }
            //tiles de pared derecha - cuack, leer Solidos
            //tiles de pared izquierda
            if ( currentMap[l][c] == 4)
            {
                for (int x=8; x < TILESIZE; x++)
                {
                    for (int y=0; y < TILESIZE; y++)
                    {
                        result[x+(c*TILESIZE)][y+(l*TILESIZE)] = 1;//!=0
                        //result[x+(l*TILESIZE)].insert(std::make_pair( y+(c*TILESIZE), 1 ));
                    }
                }

            }
            //tiles de piso 12 pixels debajo del top
            if ( currentMap[l][c] == 2)
            {
                for (int x=0; x < TILESIZE; x++)
                {
                    for (int y=11; y < TILESIZE; y++)
                    {
                        result[x+(c*TILESIZE)][y+(l*TILESIZE)] = 1;//!=0
                    }
                }
            }
        }
    }
    return result;
}

bool Scenario::collides(int x, int y)
{
    if (collisionPoint[x][y] != 0)
        return true;
    return false;
}




