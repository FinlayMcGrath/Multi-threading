#ifndef _MANDELBROT_H_
#define _MANDELBROT_H_

#include <windows.h>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <complex>
#include <fstream>
#include <thread>
#include <iostream>
#include <vector>
#include <string>
#include "Farm.h"
#include "MandelbrotTask.h"

// Import things we need from the standard library
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::complex;
using std::cout;
using std::endl;
using std::ofstream;
using namespace std;
using std::chrono::seconds;
using std::this_thread::sleep_for;
// Define the alias "the_clock" for the clock type we're going to use.
typedef std::chrono::steady_clock the_clock;

typedef uint32_t imageArray[HEIGHT][WIDTH];

#define slices 64

class Mandelbrot
{
public:
	Mandelbrot();
	void Mandelbrot::start(int iterations, double left, double right, double top, double bottom, imageArray* image_);
	bool isRunning() {return running;};
	bool isStopped() {return stop;};
	
	void end();
private:
	void Mandelbrot::write_tga(const char *filename);
	Farm farm;
	imageArray* image;
	int MAX_ITERATIONS;
	bool stop, running;
};

#endif