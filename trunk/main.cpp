#include <SFML/Graphics.hpp>
#include <fstream>
#include "Scenario.h"
#include "ScenarioViewer.h"
#include "Player.h"

#define SCREENWIDTH 320
#define SCREENHEIGHT 480

std::vector <std::string> getLevels()
{
    std::vector <std::string> result;
    std::ifstream file("levels.txt");
    if (file.is_open())
    {

        while (!file.eof())
        {
            std::string levelName;
            std::getline(file, levelName);
            if (levelName.length() > 0)
            {
                result.push_back(levelName);
            }

        }
    }
    file.close();
    return result;
}

int main()
{
    sf::RenderWindow gameWindow(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT, 32), "320x480");
    std::vector <std::string> levels;
    levels = getLevels();
    Scenario *scenario = new Scenario(0, levels[0]);
    Player *player = new Player(scenario);
    ScenarioViewer *scenarioViewer = new ScenarioViewer(scenario);

    sf::Clock clock;
    float frames = 1000/60;


    while(gameWindow.isOpen())
    {
        sf::Event Event;
        if (gameWindow.pollEvent(Event))
            if (Event.type == sf::Event::Closed)
                gameWindow.close();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            gameWindow.close();


        if (clock.getElapsedTime() >= sf::milliseconds(frames) )
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
            {
                int id = 0;
                int levelsAmount = levels.size()-1;
                if (scenario->currentLevelId < levelsAmount)
                    id = scenario->currentLevelId+1;

                //delete scenario->Enemys? todo va en el destructor
                delete player;
                player = new Player(scenario);
                delete scenario;
                scenario = new Scenario(id, levels[id]);
                delete scenarioViewer;
                scenarioViewer = new ScenarioViewer(scenario);
            }

            scenarioViewer->draw(&gameWindow);
            player->update(&gameWindow);
            gameWindow.display();
            clock.restart();
        }
    }

    return 0;
}
