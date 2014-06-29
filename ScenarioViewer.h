#ifndef SCENARIOVIEWER_H
#define SCENARIOVIEWER_H
#include "Scenario.h"

class ScenarioViewer //ESTA DEBERIA DIBUJAR UN objeto "Scenario"
{
    public:
        ScenarioViewer(Scenario *p_scenario);

        void draw(sf::RenderWindow *gameWindow);


    private:

        Scenario *scenario;

        std::string getTilesName(std::string levelName);
        std::vector <sf::Texture> getTiles(std::string tilesName);

        std::vector <sf::Texture> currentLevelTEXTiles;
        std::string currentLevelTiles;

};

#endif // SCENARIOVIEWER_H
