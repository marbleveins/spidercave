#ifndef SCENARIO_H
#define SCENARIO_H
#include <SFML/Graphics.hpp>

class Scenario
{
    public:
        Scenario(int levelId, std::string levelName);

        std::string currentLevelName;
        std::vector <std::vector <int> > currentMap;
        int currentLevelId;
        float gravity;

        int getTileCode(int x, int y);
        std::map<int, std::map<int,int> > collisionPoint;//hash_map - es un diccionario con un diccionario adentro
        std::map<int, std::map<int,int> > getCollisionPoints();
        bool collides(int x, int y);


    private:

        std::vector <std::vector <int> > getMap(std::string levelName);

};

#endif // SCENARIO_H
