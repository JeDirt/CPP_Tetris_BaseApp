#pragma once
#include <vector>

using namespace std;


// All symbols can be used to fill the game space
enum EnvironmentSymbols : char
{

	EMPTY_CHAR = ' ',
	OBJECT_CHAR = '•',
	DESTROYED_CHAR = '*',
	HORIZONTAL_WALL_CHAR = '=',
	VERTICAL_WALL_CHAR = '|'

};


// Stores coordinates of every point in matrix
class Coordinates
{

public:


	Coordinates(int x, int y) : X(x), Y(y) {}
	Coordinates() : X(0), Y(0) {}

	Coordinates(const Coordinates& ref)
	{
		this->X = ref.X;
		this->Y = ref.Y;
	}
	Coordinates& operator =(const Coordinates& ref)
	{

		if (this == &ref)
		{
			return *this;
		}

		this->X = ref.X;
		this->Y = ref.Y;

		return *this;
	}

	Coordinates( Coordinates&& ref)noexcept
	{

		this->X = ref.X;
		this->Y = ref.Y;

		ref.X = 0;
		ref.Y = 0;

	}
	Coordinates& operator =( Coordinates&& ref)noexcept
	{

		if (this == &ref)
		{
			return *this;
		}

		this->X = ref.X;
		this->Y = ref.Y;

		ref.X = 0;
		ref.Y = 0;

		return *this;
	}

	virtual ~Coordinates() {}


public:


	const int GetX()const
	{
		return this->X;
	}
	const int GetY() const
	{
		return this->Y;
	}


	void SetX(int _x)
	{
		this->X = _x;
	}
	void SetY(int _y) 
	{
		this->Y = _y;
	}

	
private:


	 int X;
	 int Y;


};


// Base class for all types of figures
class Figure
{

public:


	Figure(Coordinates _position, int axis_X, int axis_Y);

	Figure(const Figure& ref);
	Figure& operator = (const Figure& ref);

	Figure(Figure&& ref)noexcept;
	Figure& operator = (Figure&& ref)noexcept;

	virtual ~Figure();


public:


	const int GetX_Size() const; 
	const int GetY_Size() const; 
	virtual void RotateFigure();
	

protected:


	Figure(Coordinates);
	void InitializeMatrix();
	void FlipFigure();


public:


	Coordinates position;
	vector <vector<char>> Matrix;


protected:

	
	int X_Size;
	int Y_Size;


};


// All accessible figure types
class O_Figure : public Figure
{

public:


	O_Figure(int x, int y);
	virtual ~O_Figure() {}

	void RotateFigure()override;


};
class I_Figure : public Figure
{

public:


	I_Figure(int x, int y, bool _side = false);
	virtual ~I_Figure() {}

	void RotateFigure()override;


private:

	bool Side;

};
class T_Figure : public Figure
{

public:


	T_Figure(int x, int y, int _side = 0);
	virtual ~T_Figure() {}

	void RotateFigure()override;


private:

	int Side;

};
class L_Figure : public Figure
{

public:


	L_Figure(int x, int y, int _side = 0, bool _flip = false);
	virtual ~L_Figure() {}

	void RotateFigure()override;


private:


	int Side;
	const bool Flip;


};
class Z_Figure : public Figure
{

public:


	Z_Figure(int x, int y, int _side = 0, bool _flip = false);
	virtual ~Z_Figure() {}

	void RotateFigure()override;


private:


	int Side;
	const bool Flip;


};