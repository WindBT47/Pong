#include "Game.h"

const int thickness = 15;

Game::Game()
{
	mWindows = nullptr;
	mIsRunning = true;
}

bool Game::Initialize()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	//Create window
	mWindows = SDL_CreateWindow(
		"Pong Game! C++",
		100,
		100,
		1024,
		768,
		0
	);

	if (!mWindows)
	{
		SDL_Log("Unable to create window: %s", SDL_GetError());
		return false;
	}

	//Create renderer
	mRenderer = SDL_CreateRenderer(
		mWindows,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("Unable to create renderer: %s", SDL_GetError());
		return false;
	}

	//Set origin position
	mPaddlePos.x = 0;
	mPaddlePos.y = 384;
	mBallPos.x = 512;
	mBallPos.y = 384;

	return true;
}

void Game::Shutdown()
{
	SDL_DestroyWindow(mWindows);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	//Get status of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	//If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

}

void Game::UpdateGame()
{

}

void Game::GenerateOutput()
{
	//Render window
	SDL_SetRenderDrawColor(
		mRenderer,
		255,
		122,
		155,
		255
	);
	SDL_RenderClear(mRenderer);

	//Render wall
	SDL_SetRenderDrawColor(
		mRenderer,
		255,
		255,
		255,
		255
	);
	SDL_Rect wall{
		0,
		0,
	    1024,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &wall);

	wall.y = 753;
	SDL_RenderFillRect(mRenderer, &wall);

	wall.x = 1009;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 1024;
	SDL_RenderFillRect(mRenderer, &wall);

	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y-50),
		thickness,
		100
	};

	SDL_RenderFillRect(mRenderer, &paddle);

	SDL_RenderPresent(mRenderer);

}