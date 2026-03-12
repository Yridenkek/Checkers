#include "Game/Game.h"
#include <SDL.h>
#include <SDL_image.h>

int main(int argc, char** argv)
{
    Game g;
    return g.play();
}