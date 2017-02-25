#pragma once

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <complex>
#include <fstream>
#include <thread>
#include <iostream>
#include <vector>
#include <string>
#include "Task.h"

using namespace std;

// The size of the image to generate.
const int WIDTH = 1440;
const int HEIGHT = 900;

typedef uint32_t imageArray[HEIGHT][WIDTH];

class MandelbrotTask : public Task
{
public:
	MandelbrotTask(int maxIteration_, double left_, double right_, double top_, double bottom_, int startLine_, int endLine_, imageArray* image_, bool* stop_);
	~MandelbrotTask();
	void run();
private:
	// The image data.
	// Each pixel is represented as 0xRRGGBB.
	imageArray* image;
	int maxIterations, startLine, endLine;
	double left, right, top, bottom;
	const bool* stop;
};

