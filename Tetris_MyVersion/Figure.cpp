#include "Figure.h"


// Parent class for all figures
Figure::Figure(Coordinates _position, int axis_X, int axis_Y) :

	// Default position in matrix
	X_Size(axis_X),
	Y_Size(axis_Y),
	Matrix(0)

{

	this->position = _position;
	InitializeMatrix();

}
Figure::Figure(Coordinates _position) :

	X_Size(0),
	Y_Size(0),
	Matrix(0)

{
	this->position = _position;
}


Figure::Figure(const Figure& ref)
{

	this->X_Size = ref.X_Size;
	this->Y_Size = ref.Y_Size;

	this->Matrix = ref.Matrix;
	this->position = ref.position;

}
Figure& Figure::operator=(const Figure& ref)
{

	if (this == &ref)
	{
		return *this;
	}

	this->X_Size = ref.X_Size;
	this->Y_Size = ref.Y_Size;

	this->Matrix = ref.Matrix;
	this->position = ref.position;

	return *this;

}


Figure::Figure(Figure&& ref)noexcept
{

	this->X_Size = ref.X_Size;
	this->Y_Size = ref.Y_Size;

	this->Matrix = ref.Matrix;
	this->position = ref.position;


	ref.X_Size = 0;
	ref.Y_Size = 0;

	ref.Matrix.clear();
	ref.position.SetX(0);
	ref.position.SetY(0);

	
}
Figure& Figure::operator=(Figure&& ref)noexcept
{

	if (this == &ref)
	{
		return *this;
	}

	this->X_Size = ref.X_Size;
	this->Y_Size = ref.Y_Size;

	this->Matrix = ref.Matrix;
	this->position = ref.position;


	ref.X_Size = 0;
	ref.Y_Size = 0;

	ref.Matrix.clear();
	ref.position.SetX(0);
	ref.position.SetY(0);

	return *this;

}


Figure::~Figure()
{
	
}


const int Figure::GetX_Size() const
{
	return this->X_Size;
}
const int Figure::GetY_Size() const
{
	return this->Y_Size;
}


// Creates empty matrix for the figure
void Figure::InitializeMatrix()
{

	// Give matrix some start buffer to avoid exceptions
	Matrix.resize(100, vector<char>(100));


	// Init full empty matrix
	for (int x = 0; x < X_Size; x++)
	{

		for (int y = 0; y < Y_Size; y++)
		{
			Matrix[x][y] = EMPTY_CHAR;
		}

	}
		
}


// Figure movement in the space
void Figure::RotateFigure()
{

	// If matrix of figure is empty we dont continue running this func
	if (Matrix.empty())
	{
		return;
	}


	// Create buffer figure matrix to store it's position temporarily
	vector<vector<char>>TempMatrix(Matrix);


	// Rotate and reinit matrix of figure
	swap(X_Size, Y_Size);
	InitializeMatrix();


	for (int x = 0; x < X_Size; x++)
	{

		for (int y = 0; y < Y_Size; y++)
		{
			Matrix[x][y] = TempMatrix[y][X_Size - x - 1];
		}

	}
		
}
void Figure::FlipFigure()
{
	
	for (int y = 0; y < Y_Size; y++)
	{

		for (int x = 0; x < X_Size / 2; x++)
		{
			swap(Matrix[x][y], Matrix[X_Size - x - 1][y]);
		}

	}

}


/* ------------------ Figure types --------------------------- */


// Square figure to spawn
O_Figure::O_Figure(int x, int y) :

	// Init square figure
	Figure(Coordinates(x, y), 2, 2)

{

	for (int i = 0; i < 2; i++)
	{

		for (int j = 0; j < 2; j++)
		{
			Matrix[i][j] = OBJECT_CHAR;
		}

	}
		
}
void O_Figure::RotateFigure(){}


// Stick figure to spawn
I_Figure::I_Figure(int x, int y, bool _side) :

	// Init stick figure
	Figure(Coordinates(x, y), 1, 4),
	Side(false)

{

	for (int x = 0; x < 1; x++)
	{

		for (int y = 0; y < 4; y++)
		{
			Matrix[x][y] = OBJECT_CHAR;
		}

	}
		

	if (_side != Side)
	{
		RotateFigure();
	}

}
void I_Figure::RotateFigure()
{
	Figure::RotateFigure();
	Side = !Side;
}


// T shaped figure to spawn
T_Figure::T_Figure(int x, int y, int _side) :

	// Init T figure
	Figure(Coordinates(x, y), 3, 2),
	Side(0)

{

	// Fill matrix of figure
	Matrix[1][1] = OBJECT_CHAR;
	Matrix[0][0] = OBJECT_CHAR;
	Matrix[1][0] = OBJECT_CHAR;
	Matrix[2][0] = OBJECT_CHAR;


	// Define possible rotation directions
	while (Side < _side)
	{
		RotateFigure();
	}

}
void T_Figure::RotateFigure()
{

	Figure::RotateFigure();

	// Check current rotation state
	if (++Side > 3)
	{
		Side = 0;
	}

}


// L shaped figure to spawn
L_Figure::L_Figure(int x, int y, int _side, bool _flip) :

	// Init L figure
	Figure(Coordinates(x, y), 2, 3),
	Side(0),
	Flip(_flip)

{

	// Fill matrix of figure with chars
	Matrix[0][0] = OBJECT_CHAR;
	Matrix[0][1] = OBJECT_CHAR;
	Matrix[0][2] = OBJECT_CHAR;
	Matrix[1][2] = OBJECT_CHAR;


	if (Flip)
	{
		Figure::FlipFigure();
	}


	while (Side < _side)
	{
		RotateFigure();
	}

}
void L_Figure::RotateFigure()
{

	Figure::RotateFigure();

	if (++Side > 3)
	{
		Side = 0;
	}

}


// Z shaped figure to spawn
Z_Figure::Z_Figure(int x, int y, int _side, bool _flip) :

	// Init Z figure
	Figure(Coordinates(x, y), 3, 2),
	Side(0),
	Flip(_flip)

{

	Matrix[0][0] = OBJECT_CHAR;
	Matrix[1][0] = OBJECT_CHAR;
	Matrix[1][1] = OBJECT_CHAR;
	Matrix[2][1] = OBJECT_CHAR;


	if (Flip)
	{
		Figure::FlipFigure();
	}


	while (Side < _side)
	{
		RotateFigure();
	}

}
void Z_Figure::RotateFigure()
{

	Figure::RotateFigure();

	if (++Side > 3)
	{
		Side = 0;
	}

}


