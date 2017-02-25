#ifndef _INPUT_H
#define _INPUT_H

// Input class

#include <Windows.h>

class Input
{
	typedef struct Mouse
	{
		int x, y, xOld, yOld;
		bool left, right, leftClicked, rightClicked;
	};

	public:
	void SetKeyDown(WPARAM);
	void SetKeyUp(WPARAM);
	bool isKeyDown(int);
	void setMouseX(int);
	void setMouseY(int);
	int getMouseX();
	int getMouseY();
	void setLeftClick(bool);
	void setRightClick(bool);
	bool getLeftClick();
	bool getRightClick();
	void setMouseXOld();
	void setMouseYOld();
	int getMouseXOld();
	int getMouseYOld();
	void setLeftClicked(bool);
	void setRightClicked(bool);
	bool getLeftClicked();
	bool getRightClicked();
	
private:
	bool keys[256];
	Mouse mouse;
};

#endif
