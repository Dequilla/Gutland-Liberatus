#include "Game.h"
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int main(int argc, void** argv[])
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    Game game;

    while (!game.GetWindow()->IsDone())
    {
        game.Update();
        game.Render();
        game.LateUpdate();
    }
    return 0;
    _CrtDumpMemoryLeaks();
}
