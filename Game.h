#include "SDL.h"

struct Vector2
{
	float x, y;
};
class Game
{
public:

	Game();

	//Initialize the game
	bool Initialize();

	//Run the game loop until the game is over
	void RunLoop();

	//Shutdown the game
	void Shutdown();

private:

	//Helper Function for the game
	void ProcessInput();

	void UpdateGame();

	void GenerateOutput();

	//Create a window by SDL
	SDL_Window* mWindows;

	//Create a renderer by SDL
	SDL_Renderer* mRenderer;

	//Is game should continue to run?
	bool mIsRunning;

	Vector2 mPaddlePos;

	Vector2 mBallPos;
};