#include "mandelbrot.h"

Mandelbrot::Mandelbrot() 
{	

}

void Mandelbrot::start(int iterations, double left, double right, double top, double bottom, imageArray* image_)
{
	image = image_;
	stop = false;
	running = true;
	// Start timing
	the_clock::time_point start = the_clock::now();
	
	//splits the image into multiple slices
	int endLine = HEIGHT / slices;
	int startLine = 0;

	for (int i = 0; i < slices; i++)
	{
		//sets up tasks to calculate a slice
		farm.addTask(new MandelbrotTask(iterations, left, right, top, bottom, startLine, endLine, image, &stop));

		startLine = endLine; 
		endLine += (HEIGHT / slices);
	}
	
	farm.run();

	// Stop timing
	the_clock::time_point end = the_clock::now();

	// Compute the difference between the two times in milliseconds
	auto time_taken = duration_cast<milliseconds>(end - start).count();
	cout << "Computing the Mandelbrot set took " << time_taken << " ms." << endl;

	//write_tga("textures/mandelbrot.tga");
	running = false;
}

// Write the image to a TGA file with the given name.
// Format specification: http://www.gamers.org/dEngine/quake3/TGA.txt
void Mandelbrot::write_tga(const char *filename)
{
	ofstream outfile(filename, ofstream::binary);

	uint8_t header[18] = {
		0, // no image ID
		0, // no colour map
		2, // uncompressed 24-bit image
		0, 0, 0, 0, 0, // empty colour map specification
		0, 0, // X origin
		0, 0, // Y origin
		WIDTH & 0xFF, (WIDTH >> 8) & 0xFF, // width
		HEIGHT & 0xFF, (HEIGHT >> 8) & 0xFF, // height
		24, // bits per pixel
		0, // image descriptor
	};
	outfile.write((const char *)header, 18);

	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			uint8_t pixel[3] = {
				*image[y][x] & 0xFF, // blue channel
				(*image[y][x] >> 8) & 0xFF, // green channel
				(*image[y][x] >> 16) & 0xFF, // red channel
			};
			outfile.write((const char *)pixel, 3);
		}
	}

	outfile.close();
	if (!outfile)
	{
		// An error has occurred at some point since we opened the file.
		cout << "Error writing to " << filename << endl;
		exit(1);
	}
}

void Mandelbrot::end()
{
	stop = true;
}