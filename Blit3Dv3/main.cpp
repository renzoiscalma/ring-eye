/*
	Simple example of loading/rotating/displaying sprites in Blit3D
*/
//memory leak detection
#define CRTDBG_MAP_ALLOC
#ifdef _DEBUG
	#ifndef DBG_NEW
		#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
		#define new DBG_NEW
	#endif
#endif  // _DEBUG

#include <stdlib.h>
#include <crtdbg.h>

#include "Blit3D.h"
#include "FlyingEye.h"
#include "Background.h"
#include "Ring.h"
#include "CollisionCheck.h"
#include <lua.hpp>
#include "Lua.h"

Blit3D *blit3D = NULL;

//GLOBAL DATA
enum GameState {
	MAIN_MENU,
	PLAYING,
	GAME_OVER,
};

FlyingEye* flyingEye = NULL;
Background* background = NULL;
AngelcodeFont* font = NULL;
GameState gameState;
RingFactory rf;
CollisionCheck collision;

std::vector<Ring> rings;
double elapsedTime = 0;
float timeSlice = 1.f / 120.f;
float totalTime = 0;
float gameTime = 0;
float gameVelocity;
int score = 0;
float gameOverTimer = 0;
int currHighScore = -1;
bool saved = false;

Lua L;

void Init()
{
	background = new Background;
	flyingEye = new FlyingEye;
	gameVelocity = -300;
	gameState = MAIN_MENU;
	L = Lua();
	currHighScore = L.getMaxScore();
	std::cout << "High Score: " << currHighScore;
	for (int i = 1; i < 4; i++) {
		flyingEye->death.push_back(blit3D->MakeSprite(150 * i, 0, 150, 150, "Media\\Death.png"));
	};

	for (int i = 0; i < 8; i++) {
		flyingEye->flight.push_back(blit3D->MakeSprite(150 * i, 0, 150, 150, "Media\\Flight.png"));
	}

	for (int i = 0; i < 4; i++) {
		flyingEye->damage.push_back(blit3D->MakeSprite(150 * i, 0, 150, 150, "Media\\TakeHit.png"));
	}
	
	background->sprite = blit3D->MakeSprite(0, 0, 1920, 1080, "Media\\Background.png");

	rf.ringSprites.push_back(blit3D->MakeSprite(0, 0, 18, 50, "Media\\Ring-L.png"));
	rf.ringSprites.push_back(blit3D->MakeSprite(0, 0, 19, 50, "Media\\Ring-R.png"));

	font = blit3D->MakeAngelcodeFontFromBinary32("Media\\TheBoldFont.bin");
}

void DeInit(void)
{
	//any sprites/fonts still allocated are freed automatically by the Blit3D object when we destroy it
	if (flyingEye != NULL) delete flyingEye;
	if (background != NULL)	delete background;
}

void Update(double seconds)
{
	if (seconds < 0.15) {
		elapsedTime += seconds;
		gameTime += seconds;
	}
	else {
		elapsedTime += 0.15;
		gameTime += 0.15;
	}

	switch (gameState) {
	case GameState::MAIN_MENU:
		while (elapsedTime >= timeSlice) {
			elapsedTime -= timeSlice;
			flyingEye->Update(timeSlice);
			background->Update(timeSlice);
		}
		break;
	case GameState::PLAYING:
		while (elapsedTime >= timeSlice) {
			elapsedTime -= timeSlice;
			rf.spawnTimer -= timeSlice;
			flyingEye->Update(timeSlice);
			background->Update(timeSlice);

			if (rf.spawnTimer <= 0 && !flyingEye->damaged) {
				rings.push_back(rf.generateRing(seconds, 1));
				rf.spawnTimer = 3;
			}

			for (int i = rings.size() - 1; i >= 0; i--) {
				if (!rings[i].Update(timeSlice))
					rings.erase(rings.begin() + i);

				// check for collision
				if (collision.EyeRingCollide(*flyingEye, rings[i]) && !flyingEye->damaged) {
					flyingEye->damaged = true;
					rings[i].bumped = true;
					gameVelocity = 0;
				}
				// check if scored
				if (collision.EyeRingScore(*flyingEye, rings[i]) && !flyingEye->damaged) {
					std::cout << "Scored! \n + Game Velocity: " << gameVelocity;
					rings[i].scored = true;
					score++;
					gameVelocity -= 15;
				}
			}

			if (flyingEye->dead && flyingEye->deathFrame == flyingEye->death.size() - 1) {
				gameState = GameState::GAME_OVER;
				gameOverTimer = 0.5;
			}
		}
		break;
	case GameState::GAME_OVER:
		while (elapsedTime >= timeSlice) {
			elapsedTime -= timeSlice;
			gameOverTimer -= timeSlice;
			flyingEye->Update(timeSlice);
			background->Update(timeSlice);
			if (!saved) {
				if (score > currHighScore) {
					currHighScore = score;
				}
				L.saveScore(score);
				saved = true;
			}
		}
		break;

	}
}

void Draw(void)
{
	glClearColor(1.0f, 0.0f, 1.0f, 0.0f);	//clear colour: r,g,b,a 	
	// wipe the drawing surface clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//draw stuff here
	background->Draw();

	// draw left part of the ring first
	for (auto& ring : rings)
		ring.DrawSpriteL();

	// then draw the flying eye
	flyingEye->Draw();

	// then draw the right part of the ring in order to go in front of the flying eye
	for (auto& ring : rings) {
		ring.DrawSpriteR();
	}
	float gameStartWidth, gameOverWidth, gameScoreWidth, hiScoreWidth;
	std::string scoreStr, hiScoreStr;
	switch (gameState) {
	case GameState::MAIN_MENU:
		gameStartWidth = font->WidthText("PRESS SPACE TO START");
		font->BlitText(1920.f / 2 - gameStartWidth / 2, 1080.f / 2, "PRESS SPACE TO FLY");
		break;
	case GameState::GAME_OVER:
		if (gameOverTimer <= 0) {
			gameOverWidth = font->WidthText("GAME OVER! PRESS SPACE TO RESTART!");
			font->BlitText(1920.f / 2 - gameOverWidth / 2, 1080.f / 2, "GAME OVER! PRESS SPACE TO RESTART");
		}
		scoreStr = "SCORE: " + std::to_string(score);
		gameScoreWidth = font->WidthText(scoreStr);
		font->BlitText(30, 1050, scoreStr);
		break;
	case GameState::PLAYING:
		scoreStr = "SCORE: " + std::to_string(score);
		gameScoreWidth = font->WidthText(scoreStr);
		font->BlitText(30, 1050, scoreStr);
		break;
	}
	hiScoreStr = "HI-SCORE: " + std::to_string(currHighScore);
	hiScoreWidth = font->WidthText(hiScoreStr);
	font->BlitText(blit3D->screenWidth - hiScoreWidth - 20, 1050, hiScoreStr);
}

//the key codes/actions/mods for DoInput are from GLFW: check its documentation for their values
void DoInput(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		if (gameState == GameState::MAIN_MENU || gameState == GameState::PLAYING && !flyingEye->damaged) {
			gameState = GameState::PLAYING;
			flyingEye->pulse = true;
		}
		else if (gameState == GameState::GAME_OVER && gameOverTimer <= 0) {	// game over timer is needed just to avoid accidentally spamming space
			// restart the game
			gameTime = 0;
			gameVelocity = -300;
			flyingEye->reset();
			rings.clear();
			score = 0;
			gameState = GameState::PLAYING;
			saved = false;
		}
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		blit3D->Quit(); //start the shutdown sequence
}

//called whenever the user resizes the window
void DoResize(int width, int height)
{
	blit3D->trueScreenWidth = blit3D->screenWidth = static_cast<float>(width);
	blit3D->trueScreenHeight = blit3D->screenHeight = static_cast<float>(height);
	blit3D->Reshape(blit3D->shader2d);
}

int main(int argc, char *argv[])
{
	//memory leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//set X to the memory allocation number in order to force a break on the allocation:
	//useful for debugging memory leaks, as long as your memory allocations are deterministic.
	//_crtBreakAlloc = X;

	blit3D = new Blit3D(Blit3DWindowModel::DECORATEDWINDOW, 1920, 1080);

	//set our callback funcs
	blit3D->SetInit(Init);
	blit3D->SetDeInit(DeInit);
	blit3D->SetUpdate(Update);
	blit3D->SetDraw(Draw);
	blit3D->SetDoInput(DoInput);
	blit3D->SetDoResize(DoResize);

	//Run() blocks until the window is closed
	blit3D->Run(Blit3DThreadModel::SINGLETHREADED);
	if (blit3D) delete blit3D;
}