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
    gravity = 0.2;
    currentLevelId = levelId;
    currentLevelName = levelName;
    currentMap = getMap(currentLevelName);
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
