#include "Input.h"

void Input::SetKeyDown(WPARAM key)
{
	keys[key] = true;
}

void Input::SetKeyUp(WPARAM key)
{
	keys[key] = false;
}

bool Input::isKeyDown(int key)
{
	return keys[key];
}

void Input::setMouseX(int pos)
{
	mouse.x = pos;
}

void Input::setMouseY(int pos)
{
	mouse.y = pos;
}

int Input::getMouseX()
{
	return mouse.x;
}

int Input::getMouseY()
{
	return mouse.y;
}

void Input::setLeftClick(bool left)
{
	mouse.left = left;
	setRightClicked(true);
}

void Input::setRightClick(bool right)
{
	mouse.right = right;
	setRightClicked(true);
}

bool Input::getLeftClick()
{
	return mouse.left;
}

bool Input::getRightClick()
{
	return mouse.right;
}

void Input::setMouseXOld()
{
	mouse.xOld = mouse.x;
}

void Input::setMouseYOld()
{
	mouse.yOld = mouse.y;
}

int Input::getMouseXOld()
{
	return mouse.xOld;
}

int Input::getMouseYOld()
{
	return mouse.yOld;
}

void Input::setLeftClicked(bool left)
{
	mouse.leftClicked = left;
}

void Input::setRightClicked(bool right)
{
	mouse.rightClicked = right;
}

bool Input::getLeftClicked()
{
	return mouse.leftClicked;
}

bool Input::getRightClicked()
{
	return mouse.rightClicked;
}