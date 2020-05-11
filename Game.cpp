#include "Game.h"

const int thickness = 15;

Game::Game()
{
	mWindows = nullptr;
	mIsRunning = true;
	mRenderer = nullptr;
	mTicksCount = 0;
	mBallPos.x = 0;
	mBallPos.y = 0;
	mBallVel.x = 0.0f;
	mBallVel.y = 0.0f;
	mPaddlePos_01.x = 0;
	mPaddlePos_01.y = 0;
	mPaddleDir_01 = 0;
	mPaddleDir_02 = 0;
	Diff_01 = 0;
	Diff_02 = 0;
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
	mPaddlePos_01.x = 10;
	mPaddlePos_01.y = 384;

	mPaddlePos_02.x = 999;
	mPaddlePos_02.y = 384;

	mPaddleDir_01 = 0;

	mPaddleDir_02 = 0;
	//Ball 1
	mBallPos.x = 512;
	mBallPos.y = 384;
	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;

	mBall.BallPos = mBallPos;
	mBall.BallVel = mBallVel;
	Balls.push_back(mBall);

	//Ball 2
	mBallPos.x = 412;
	mBallPos.y = 284;
	mBallVel.x = -300.0f;
	mBallVel.y = 335.0f;

	mBall.BallPos = mBallPos;
	mBall.BallVel = mBallVel;
	Balls.push_back(mBall);

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
	SDL_Event Event;
	while (SDL_PollEvent(&Event))
	{
		switch (Event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	//Get status of keyboard
	const Uint8* State = SDL_GetKeyboardState(NULL);
	//If escape is pressed, also end loop
	if (State[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	if (State[SDL_SCANCODE_W])
	{
		mPaddleDir_01 = -1;
	}

	if (State[SDL_SCANCODE_S])
	{
		mPaddleDir_01 = 1;
	}

	if (State[SDL_SCANCODE_I])
	{
		mPaddleDir_02 = -1;
	}

	if (State[SDL_SCANCODE_K])
	{
		mPaddleDir_02 = 1;
	}
}

void Game::UpdateGame()
{
	//Wait at least 16ms apart
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;
	//Convert to seconds
	float DeltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	//Update tick counts
	mTicksCount = SDL_GetTicks();
	//Clamp maximum delta time value
	if (DeltaTime > 0.05f)
	{
		DeltaTime = 0.05f;
	}
	//Paddle 1 setting
	if (mPaddleDir_01 != 0)
	{
		mPaddlePos_01.y += mPaddleDir_01 * 200.0f * DeltaTime;
		if (mPaddlePos_01.y < (50.0f + thickness))
		{
			mPaddlePos_01.y = 50.0f + thickness;
		}
		if (mPaddlePos_01.y > 768.0f - 50.0f - thickness)
		{
			mPaddlePos_01.y = 768.0f - 50.0f - thickness;
		}
	}
	//Paddle 2 setting
	if (mPaddleDir_02 != 0)
	{
		mPaddlePos_02.y += mPaddleDir_02 * 200.0f * DeltaTime;
		if (mPaddlePos_02.y < (50.0f + thickness))
		{
			mPaddlePos_02.y = 50.0f + thickness;
		}
		if (mPaddlePos_02.y > 768.0f - 50.0f - thickness)
		{
			mPaddlePos_02.y = 768.0f - 50.0f - thickness;
		}
	}
	//Balls
	for (int i = 0;i<Balls.size();i++)
	{
		//Ball's movement
		Balls[i].BallPos.x += Balls[i].BallVel.x * DeltaTime;
		Balls[i].BallPos.y += Balls[i].BallVel.y * DeltaTime;

		//Collision between balls and walls
		if (Balls[i].BallPos.y <= thickness && Balls[i].BallVel.y < 0.0f)
		{
			Balls[i].BallVel.y *= -1;
		}
		else if (Balls[i].BallPos.y >= (768.0f - thickness) && Balls[i].BallVel.y > 0.0f)
		{
			Balls[i].BallVel.y *= -1;
		}

		//Collision between balls and paddles
		Diff_01 = Balls[i].BallPos.y - mPaddlePos_01.y;
		Diff_01 = (Diff_01 > 0.0f) ? Diff_01 : -Diff_01;
		if (Diff_01 <= 50.0f && Balls[i].BallPos.x <= (thickness + 10) && Balls[i].BallVel.x < 0.0f)
		{
			Balls[i].BallVel.x *= -1;
		}
		Diff_02 = Balls[i].BallPos.y - mPaddlePos_02.y;
		Diff_02 = (Diff_02 > 0.0f) ? Diff_02 : -Diff_02;
		if (Diff_02 <= 50.0f && Balls[i].BallPos.x >= (1014 - thickness) && Balls[i].BallVel.x > 0.0f)
		{
			Balls[i].BallVel.x *= -1;
		}

		if (Balls[i].BallPos.x < 0 || Balls[i].BallPos.x>1024)
		{
			Balls[i].BallPos.x = 512;
			Balls[i].BallPos.y = 384;
		}

		if (i==Balls.size())
		{
			i = 0;
		}
	}



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
	SDL_Rect Wall{
		0,
		0,
	    1024,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &Wall);

	Wall.y = 753;
	SDL_RenderFillRect(mRenderer, &Wall);
	
	for (int i = 0; i < Balls.size(); i++)
	{
		SDL_Rect Ball{
			static_cast<int>(Balls[i].BallPos.x - thickness / 2),
			static_cast<int>(Balls[i].BallPos.y - thickness / 2),
			thickness,
			thickness
		};
		SDL_RenderFillRect(mRenderer, &Ball);
	}

	SDL_Rect Paddle_01{
		static_cast<int>(mPaddlePos_01.x),
		static_cast<int>(mPaddlePos_01.y-50),
		thickness,
		100
	};

	SDL_RenderFillRect(mRenderer, &Paddle_01);

	SDL_Rect Paddle_02{
	static_cast<int>(mPaddlePos_02.x),
	static_cast<int>(mPaddlePos_02.y - 50),
	thickness,
	100
	};

	SDL_RenderFillRect(mRenderer, &Paddle_02);

	SDL_RenderPresent(mRenderer);

}