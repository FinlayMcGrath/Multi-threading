#ifndef SCENE3D_H
#define SCENE3D_H

#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <math.h>
#include <thread>
#include <gl/gl.h>
#include <gl/glu.h>
#include "Input.h"
#include "Camera.h"
#include "mandelbrot.h"

#define COLOUR_DEPTH 16	//Colour depth
#define SCREEN_HEIGHT 900
#define SCREEN_WIDTH 1440
#define PI 3.14159

class Scene3D
{
public:
	void Init(HWND*, Input*);	//initialse function
	void DrawScene(float);	// render scene
	void Resize();

protected:
	bool CreatePixelFormat(HDC);
	void ResizeGLWindow(int width, int height);	
	void InitializeOpenGL(int width, int height);
	void handleInput(float dt);
	void drawQuad();

	//vars
	HWND* hwnd;
	Input* input;
	RECT screenRect;
	HDC	hdc;
	HGLRC hrc;			//hardware RENDERING CONTEXT	

	Camera camera;

	GLuint Dlist;
	
	//Architecture variables
	Mandelbrot mandelbrot;
	thread mandelbrotThread;
	GLuint mandelbrotTex;
	imageArray image;
	double iterations, left, right, top, bottom, updateTex;
	bool mandebrotUpdate;
};

#endif