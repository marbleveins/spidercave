#include <SFML/Graphics.hpp>
#include <fstream>
#include "ScenarioViewer.h"
#include "Scenario.h"

#define TILESIZE 16

ScenarioViewer::ScenarioViewer(Scenario *p_scenario)
{
    //ctor
    scenario = p_scenario;
    currentLevelTiles = getTilesName((*scenario).currentLevelName);
    currentLevelTEXTiles = getTiles(currentLevelTiles);
}


std::string ScenarioViewer::getTilesName(std::string levelName)
{
    std::string result;
    std::string filePath = std::string ("maps/") + levelName;
    std::ifstream file(filePath.c_str());
    if (file.is_open())
    {
        while (!file.eof())
        {
            std::string line;
            std::getline(file, line);
            if (line.find("[TILES]") != std::string::npos)
                std::getline(file, result);
        }
    }
    file.close();
    return result;
}

std::vector <sf::Texture> ScenarioViewer::getTiles(std::string tilesName)
{
    std::vector <sf::Texture> result;
    sf::Image tilesFile;
    std::string filePath = "maps/" + tilesName;
    if (tilesFile.loadFromFile(filePath.c_str()))
    {
        sf::Image tileImg;
        tileImg.create(TILESIZE, TILESIZE, sf::Color::Transparent);
        int lineAmount = tilesFile.getSize().y/TILESIZE;
        int columnAmount = tilesFile.getSize().x/TILESIZE;
        for (int line = 1 ; line<=lineAmount ; line++)
        {
            for (int column = 1 ; column<=columnAmount ; column++)
            {
                for (int y=1 ; y<=TILESIZE ; y++)
                {
                    for (int x=1 ; x<=TILESIZE ; x++)
                    {
                        tileImg.setPixel(x-1, y-1, tilesFile.getPixel(x-1+(TILESIZE*(column-1)), y-1+(TILESIZE*(line-1))));
                        if (x>=TILESIZE && y>=TILESIZE)
                        {
                            sf::Texture tileTex;
                            tileTex.loadFromImage(tileImg);
                            result.push_back(tileTex);
                        }
                    }
                }
            }
        }
    }
    return result;
}

void ScenarioViewer::draw(sf::RenderWindow *gameWindow)
{
    int mapLinesAmount = (*scenario).currentMap.size()-1;
    for (int j=0 ; j<=mapLinesAmount ; j++)
    {
        int mapColumnsAmount = (*scenario).currentMap[j].size()-1;
        for (int i=0 ; i<=mapColumnsAmount ; i++)
        {
            sf::Sprite sprite(currentLevelTEXTiles[(*scenario).currentMap[j][i]]);
            sprite.setPosition(i*TILESIZE, j*TILESIZE);
            (*gameWindow).draw(sprite);
        }
    }
}
