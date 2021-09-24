#pragma once
#include "BaseApp.h"
#include "Figure.h"


namespace
{

const float STEP_VELOCITY { 1.f };
const float VELOCITY_MULTIPLIER { 0.85f };

const int START_X { 7 };
const int START_Y { 1 };

const int BG_SIZE_X { 16 };
const int BG_SIZE_Y { 20 };

};


class GameSpace : public BaseApp
{

public:


	GameSpace();
	virtual ~GameSpace() {}


protected:


	// Print walls of gamespace
	void PrintWalls();


	// Proccessing keyboard input value
	virtual void KeyPressed(int btnCode);


	// Updates screen every tick to see common picture
	virtual void UpdateF(float deltaTime);


	// Outputs game info to the screen
	void ShowGameInfo();


	// Randomly creating new figure (false if space to spawn is busy)
	bool CreateFigure();


	// Print current object and put it into matrix
	void PrintFigure(const Figure*); // Figure is object of our created figure


	// Print next figure to spawn in info block
	void PrintNextFigureToSpawn();


	// Called every tick to update position of the figure(redraw it)
	void RedrawFigureInPosition();


	// Locking current object in matrix if it hits bottom
	void LockFigureInMatrix();


	// Check if it is possible to turn
	bool isRightEmpty();
	bool isLeftEmpty();


	// Check if there is bottom below
	bool isDownEmpty();

	
	// Check if we are able to rotate
	bool isCapableToRotate();


	// Check if we have any row to burn (true - burn the row)
	bool isBurnable();


	// Called when you lost
	void GameOver();


private:


	int Score;
	float CurrentTime;
	float Step;

	// Figure which is moving right now
	unique_ptr<Figure> CurrentFigure;

	// Saved position of CurrentFigure every frame before
	unique_ptr<Figure> PreviousFigure;

	// Randomly defined index of object which will be spawned 
	int NextFigureIndex;

	// Randomly defined index of flip state which will be spawned 
	bool NextFlipIndex;


};