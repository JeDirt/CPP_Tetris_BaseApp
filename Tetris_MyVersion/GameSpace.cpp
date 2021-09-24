#include "GameSpace.h"
#include <ctime>
#include <iostream>


// Constructor
GameSpace::GameSpace() :

	// Default values
	BaseApp(BG_SIZE_X+7, BG_SIZE_Y),
	CurrentFigure(nullptr),
	PreviousFigure(nullptr),
	CurrentTime(0),
	Score(0),
	Step(STEP_VELOCITY)

{

	srand(time(0));

	// Generate random indexes
	NextFigureIndex = rand() % 5;
	NextFlipIndex = rand() % 2;

	CreateFigure();

}


// Assign default game space with walls
void GameSpace::PrintWalls()
{

	for (int x = 0; x <= X_SIZE; x++)
	{

		// Setting X axis upper wall with '='
		SetChar(x, 0, HORIZONTAL_WALL_CHAR);

	}

	for (int x = 0; x <= BG_SIZE_X; x++)
	{

		// Setting X axis lower wall with '='
		SetChar(x, Y_SIZE, HORIZONTAL_WALL_CHAR);

	}

	for (int y = 0; y <= Y_SIZE; y++)
	{

		// Setting Y axis right and left wall with '|'
		SetChar(0, y, VERTICAL_WALL_CHAR);
		SetChar(BG_SIZE_X, y, VERTICAL_WALL_CHAR);

	}

	for (int y = 0; y <= Y_SIZE; y++)
	{

		// Setting the Y axis wall to separate game info right side
		SetChar(X_SIZE, y, VERTICAL_WALL_CHAR);

	}

	for (int x = BG_SIZE_X + 1; x < X_SIZE; x++)
	{

		// Drawing separators of score, figure
		SetChar(x, 5, HORIZONTAL_WALL_CHAR);
		SetChar(x, 14, HORIZONTAL_WALL_CHAR);
		SetChar(x, Y_SIZE, HORIZONTAL_WALL_CHAR);

	}

}


// Catch keyboard input
void GameSpace::KeyPressed(int btnCode)
{

	/*
		80 Arrow DOWN 
		75 Arrow LEFT 
		77 Arrow RIGHT
		32 Space
	*/


	if (btnCode == 75)
	{

		// Move left if true
		if (isLeftEmpty())
		{
			CurrentFigure->position.SetX(CurrentFigure->position.GetX() - 1);
		}

	}
	else if (btnCode == 77) 
	{

		// Move right if true
		if (isRightEmpty())
		{
			CurrentFigure->position.SetX(CurrentFigure->position.GetX() + 1);
		}

	}
	else if (btnCode == 32) 
	{

		// Additional redraw to be sure we wont hit anything
		RedrawFigureInPosition();


		// if true - we rotate
		if (isCapableToRotate())
		{
			CurrentFigure->RotateFigure();
		}
			

		// Save previous state in the space
		PreviousFigure.reset(new Figure{ *CurrentFigure.get() });

	}
	else if (btnCode == 80) 
	{

		// Move until figure hits the bottom
		while (isDownEmpty())
		{
			CurrentFigure->position.SetY(CurrentFigure->position.GetY()+1);
		}
			
		CurrentTime = Step;

	}

	
	// Check position of the figure if it doesnt have wrong coordinates
	if (CurrentFigure->position.GetX() < 0)
	{
		CurrentFigure->position.SetX(0);
	}
	else if (CurrentFigure->position.GetX() + CurrentFigure->GetX_Size() > BG_SIZE_X - 1)
	{
		CurrentFigure->position.SetX(BG_SIZE_X - CurrentFigure->GetX_Size());
	}

}


// Updating frame
void GameSpace::UpdateF(float deltaTime)
{

	// Starting update position of figure every frame
	RedrawFigureInPosition(); 


	// Check if it's possible to burn any row
	isBurnable();


	CurrentTime += deltaTime; // updated every frame

	if (CurrentTime > Step)
	{

		CurrentTime = 0;
		
		// If step below is not locked field - continue moving current object
		if (isDownEmpty())
		{
			CurrentFigure->position.SetY(CurrentFigure->position.GetY() + 1);
		}		
		else // below is locked bottom
		{

			// Lock our object in matrix
			LockFigureInMatrix();

			
			// If cant create new object - stop running
			if (!CreateFigure())
			{
				GameOver();
			}

		}

	}


	// Save previous state in the space
	PreviousFigure.reset(new Figure{ *CurrentFigure.get() });


	PrintFigure(CurrentFigure.get());
	PrintWalls();
	PrintNextFigureToSpawn();

	ShowGameInfo();

}


// Player's game stats
void GameSpace::ShowGameInfo()
{

	string info_field;

	// Assign info_field to Score value
	info_field = to_string(Score);

	// Printing score info in the viewport
	for (int i = 0; i < 6; i++)
	{
		SetChar(X_SIZE - 6 + i, 16, *(L"Score:" + i));
	}

	for (int i = 0; i < info_field.length(); i++)
	{
		SetChar(X_SIZE - 4 + i, 17, info_field[i]);
	}

}


// Creating some generated(randomly) figure object
bool GameSpace::CreateFigure()
{

	// Get randomly generated index to local ShapeSample variable
	int ShapeSample = NextFigureIndex;
	NextFigureIndex = rand() % 5; // reroll again


	// Generate 1 random side to flip
	int Side = rand() % 4;
	bool Flip = NextFlipIndex;

	NextFlipIndex = rand() % 2; // reroll again


	// Depending on generated index create object
	switch (ShapeSample)
	{

	case 0:

		CurrentFigure.reset(new O_Figure(START_X, START_Y));
		break;

	case 1:

		CurrentFigure.reset(new I_Figure(START_X, START_Y, (bool)(Side > 1) ? Side - 2 : Side)) ;
		break;

	case 2:

		CurrentFigure.reset(new T_Figure(START_X, START_Y, Side));
		break;

	case 3:

		CurrentFigure.reset(new L_Figure(START_X, START_Y, Side, Flip));
		break;

	case 4:

		CurrentFigure.reset(new Z_Figure(START_X, START_Y, Side, Flip));
		break;

	default:

		break;

	}


	for (int x = 0; x < CurrentFigure->GetX_Size(); x++)
	{

		for (int y = 0; y < CurrentFigure->GetY_Size(); y++)
		{

			// Check if any coordinate filled with any symbol already
			if (GetChar(CurrentFigure->position.GetX() + x, CurrentFigure->position.GetY() + y) != EMPTY_CHAR)
			{
				return false;
			}
				
		}
			
	}
		

	// Create reserve copy of current figure to store previous state in the space
	PreviousFigure.reset(new Figure{ *CurrentFigure.get() });


	return true;

}


// Prints object to the player's game space in top
void GameSpace::PrintFigure(const Figure* object)
{
	
	for (int x = 0; x < object->GetX_Size(); x++)
	{

		for (int y = 0; y < object->GetY_Size(); y++)
		{

			// Check every field and draw symbols
			if (object->Matrix[x][y] != EMPTY_CHAR)
			{
				SetChar(x + object->position.GetX(), y + object->position.GetY(), object->Matrix[x][y]);
			}
				
		}
			
	}
		
}


// Print next figure which will be spawned
void GameSpace::PrintNextFigureToSpawn()
{

	unique_ptr <Figure> NextFigure;


	// Clear previous figure from the viewport
	for (int y = 1; y < 5; y++)
	{

		for (int x = X_SIZE - 6; x < X_SIZE; x++)
		{
			SetChar(x, y, EMPTY_CHAR);
		}

	}
		
	
	// Generating new figure depending on random index
	switch (NextFigureIndex)
	{

	case 0:

		NextFigure.reset(new O_Figure(X_SIZE - 4, 2));
		break;

	case 1:

		NextFigure.reset(new I_Figure(X_SIZE - 5, 3, true));
		break;

	case 2:

		NextFigure.reset(new T_Figure(X_SIZE - 5, 2, 2));
		break;

	case 3:

		NextFigure.reset((NextFlipIndex) ? new L_Figure(X_SIZE - 5, 2, 1, NextFlipIndex) :
			new L_Figure(X_SIZE - 5, 2, 3, NextFlipIndex));
		break;

	case 4:

		NextFigure.reset(new Z_Figure(X_SIZE - 5, 2, 0, NextFlipIndex));
		break;

	default:

		return;
		break;

	}


	// Printing next figure to the viewport
	PrintFigure(NextFigure.get());

}


// Updates position of current dropping object and it's state
void GameSpace::RedrawFigureInPosition()
{

	for (int x = 0; x < PreviousFigure->GetX_Size(); x++)
	{

		for (int y = 0; y < PreviousFigure->GetY_Size(); y++)
		{

			// Checking previous position (saved state in PreviosFigure) and draw in new position
			if (PreviousFigure->Matrix[x][y] != EMPTY_CHAR)
			{
				SetChar(x + PreviousFigure->position.GetX(), y + PreviousFigure->position.GetY(), EMPTY_CHAR);
			}
				
		}	

	}
		
}


// Called if we reached bottom of matrix or some locked figure
void GameSpace::LockFigureInMatrix()
{

	// Additional redraw to avoid misses
	RedrawFigureInPosition();
	
	for (int x = 0; x < CurrentFigure->GetX_Size(); x++)
	{

		for (int y = 0; y < CurrentFigure->GetY_Size(); y++)
		{

			// Lock figure in the botton
			if (CurrentFigure->Matrix[x][y] != EMPTY_CHAR)
			{
				SetChar(x + CurrentFigure->position.GetX(), y + CurrentFigure->position.GetY(), OBJECT_CHAR);
			}

		}
			
	}
	
	// Objects are not needed anymore, they're locked
	CurrentFigure.reset();
	PreviousFigure.reset();

}


// Check space around figure
bool GameSpace::isRightEmpty()
{

	for (int y = 0; y < CurrentFigure->GetY_Size(); y++)
	{

		for (int x = CurrentFigure->GetX_Size() - 1; x >= 0; x--)
		{

			if (CurrentFigure->Matrix[x][y] != EMPTY_CHAR)
			{

				// Checking if right space is empty relatively to every coordinate of figure
				if (GetChar(CurrentFigure->position.GetX() + x + 1, CurrentFigure->position.GetY() + y) == DESTROYED_CHAR)
				{
					return false;
				}
				
				break;

			}

		}

	}

	return true;

}
bool GameSpace::isLeftEmpty()
{

	for (int y = 0; y < CurrentFigure->GetY_Size(); y++)
	{

		for (int x = 0; x < CurrentFigure->GetX_Size(); x++)
		{

			if (CurrentFigure->Matrix[x][y] != EMPTY_CHAR)
			{

				// Checking if left space is empty relatively to every coordinate of figure
				if (GetChar(CurrentFigure->position.GetX() + x - 1, CurrentFigure->position.GetY() + y) != EMPTY_CHAR)
				{
					return false;
				}
				
				break;

			}

		}

	}

	return true;

}


// Check space under the figure
bool GameSpace::isDownEmpty()
{
	
	// Checking Y axis lenght (same as bottom value)
	if (CurrentFigure->position.GetY() + CurrentFigure->GetY_Size() > Y_SIZE)
	{
		return false;
	}


	// Checking X axis
	for (int x = 0; x < CurrentFigure->GetX_Size(); x++)
	{

		for (int y = CurrentFigure->GetY_Size() - 1; y >= 0; y--)
		{

			if (CurrentFigure->Matrix[x][y] != EMPTY_CHAR)
			{

				// Checking bottom relatively to every coordinate of figure
				if (GetChar(CurrentFigure->position.GetX() + x, CurrentFigure->position.GetY() + y + 1) != EMPTY_CHAR)
				{
					return false;
				}
				
				break;

			}

		}	

	}
		

	return true;

}


// Returns true if we can rotate our object and it doesnt touch anything around
bool GameSpace::isCapableToRotate()
{

	// Create 'ghost' figure and rotate it invisible (to not try rotate original figure)
	shared_ptr<Figure> TempFigure(new Figure{ *CurrentFigure.get() });


	TempFigure->RotateFigure();


	for (int y = 0; y < CurrentFigure->GetY_Size(); y++)
	{

		for (int x = 0; x < TempFigure->GetX_Size(); x++)
		{

			// Check if 'ghost' figure is valid after rotation 
			if (TempFigure->Matrix[x][y] != EMPTY_CHAR && 
				GetChar(TempFigure->position.GetX() + x, TempFigure->position.GetY() + y) == DESTROYED_CHAR)
			{
				return false;
			}

		}
			
	}
		

	return true;

}


// True when we have some rows common to be destroyed(checked every frame)
bool GameSpace::isBurnable()
{

	// Variable to check if string is filled with locked symbols enough
	bool FilledString;


	for (int y = Y_SIZE - 1; y > 0; y--)
	{

		FilledString = true;


		for (int x = 1; x < BG_SIZE_X; x++)
		{

			// Check if our coordinates are valid to be burned
			if (GetChar(x, y) == EMPTY_CHAR)
			{
			
				FilledString = false;
				break;

			}

		}
			
		
		// We haven't any filled string
		if (!FilledString) { continue; }
		

		for (int x = 1; x < BG_SIZE_X; x++)
		{
			// Set string with empty symbols if its possible
			SetChar(x, y, EMPTY_CHAR);
		}


		for (int i = y; i > 1; i--)
		{

			// Drop the string on the space of burnt string
			for (int x = 0; x < BG_SIZE_X; x++)
			{
				SetChar(x, i, GetChar(x, i - 1));
			}
				
		}
			

		// Update user's results
		Score++;
		Step *= VELOCITY_MULTIPLIER;


		return true;

	}

	return false;

}


// User's results in the end
void GameSpace::GameOver()
{

	system("cls");
	cout << "Game over!\nYour result:\n " << Score << " points" << endl;
	exit(0);

}