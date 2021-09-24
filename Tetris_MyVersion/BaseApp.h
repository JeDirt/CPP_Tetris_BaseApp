#pragma once

#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>


using namespace std;


class BaseApp
{

public:


	// @params - size of horizontal and vertical output ( in symbols )
	BaseApp(int xSize = 100, int ySize = 80);
	virtual ~BaseApp();


public:


	// Run game cycle
	void Run();


protected:


	// Get/Set certain coordinate on the screen with symbol
	void SetChar(int x, int y, wchar_t c);
	wchar_t GetChar(int x, int y);


	// Called every iteration (frame) , @param deltaTime - time difference between ticks ( sec )
	// Can be redefined in child class, possible to change deltaTime
	virtual void UpdateF (float deltaTime){}


	// Called to read keyboard input,  @param btnCode - code of pressed button
	// Can be redefined in child class, possible to change buttons for the input
	virtual void KeyPressed (int btnCode){}


private:


	void Render();


private:


	HANDLE mConsole;
	HANDLE mConsoleIn;

	CHAR_INFO* mChiBuffer;
	COORD mDwBufferSize;
	COORD mDwBufferCoord;
	SMALL_RECT mLpWriteRegion;


protected:


	// Size of output area horizontal and vertical ( in symbols count )
	const int X_SIZE;
	const int Y_SIZE;


};