#include "MandelbrotTask.h"

MandelbrotTask::MandelbrotTask(int maxIteration_, double left_, double right_, double top_, double bottom_, int startLine_, int endLine_, imageArray* image_, bool* stop_) 
{	
	// The number of times to iterate before we assume that a point isn't in the
	// Mandelbrot set.
	// (You may need to turn this up if you zoom further into the set.)
	maxIterations = maxIteration_;
	left = left_;
	right = right_;
	top = top_;
	bottom = bottom_;
	startLine = startLine_;
	endLine = endLine_;
	image = image_;
	stop = stop_;
}

MandelbrotTask::~MandelbrotTask()
{

}

// Render the Mandelbrot set into the image array.
// The parameters specify the region on the complex plane to plot.
void MandelbrotTask::run()
{
	for (int y = startLine; y < endLine; ++y)
	{
		if (*stop)
		{
			break;
		}
		for (int x = 0; x < WIDTH; ++x)
		{
			if (*stop)
			{
				break;
			}
			// Work out the point in the complex plane that
			// corresponds to this pixel in the output image.
			complex<double> c(left + (x * (right - left) / WIDTH),
				top + (y * (bottom - top) / HEIGHT));

			// Start off z at (0, 0).
			complex<double> z(0.0, 0.0);

			// Iterate z = z^2 + c until z moves more than 2 units
			// away from (0, 0), or we've iterated too many times.
			int iterations = 0;
			complex<double> orbitcheck;
			while (abs(z) < 2.0 && iterations < maxIterations)
			{
				z = (z * z) + c;
				++iterations;
				if (iterations % 10 == 0)
				{
					orbitcheck = z;
				}
				else if (orbitcheck == z)
				{
					iterations = maxIterations;
				}
			}

			if (iterations == maxIterations)
			{
				// z didn't escape from the circle.
				// This point is in the Mandelbrot set.
				(*image)[y][x] = 0xFFFFFF; // black
				//(*image)[y][x] = (iterations / 16 << 16) | (iterations / 16 << 8) | iterations / 16;
			}
			else
			{
				// z escaped within less than MAX_ITERATIONS
				// iterations. This point isn't in the set.
				//(*image)[y][x] = 0xFFFFFF;// white
				(*image)[y][x] = ((iterations / 2) << 16) | (iterations / 2) << 8 | (iterations / 2);
			}
		}
	}
}
