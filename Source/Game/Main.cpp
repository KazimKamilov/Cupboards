#include "Game/Game.hpp"

/**
	Main entry!
*/
int main(int argc, char* argv[])
{
	app::Game game;

	game.run();

	game.update();

	game.destroy();

	return 0;
}