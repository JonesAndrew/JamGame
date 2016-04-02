#include <SFML/Graphics.hpp>
#include "game.hpp"
#include "director.hpp"

int main()
{
    Game *scene = new Game();
    Director::getInstance()->initialize();
    Director::getInstance()->startWithScene(scene);
    return EXIT_SUCCESS;
}
