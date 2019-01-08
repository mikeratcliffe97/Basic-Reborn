#include <iostream>
#include "Source/MyGame.h"
int main()
{
    MyGame game;
    if (!game.init())
    {
        return -1;
    }

    game.run();
    std::cout << "Exiting Game!" << std::endl;
    return 0;
}