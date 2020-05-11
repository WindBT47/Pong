#include "SDL.h"
#include <vector>

struct Vector2
{
	float x, y;
};

struct Ball 
{
	Vector2 BallPos;
	Vector2 BallVel;
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

	std::vector<Ball> Balls;

	Ball mBall;

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

	Vector2 mPaddlePos_01;

	Vector2 mPaddlePos_02;

	Vector2 mBallPos;

	Vector2 mBallVel;

	Uint32 mTicksCount;

	int mPaddleDir_01;

	int mPaddleDir_02;

	//The distance between the ball and paddle
	float Diff_01;

	float Diff_02;

};