#include "Scene3D.h"

bool Scene3D::CreatePixelFormat(HDC hdc) 
{ 
    PIXELFORMATDESCRIPTOR pfd = {0}; 
    int pixelformat; 
 
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);	// Set the size of the structure
    pfd.nVersion = 1;							// Always set this to 1
	// Pass in the appropriate OpenGL flags
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
    pfd.dwLayerMask = PFD_MAIN_PLANE;			// standard mask (this is ignored anyway)
    pfd.iPixelType = PFD_TYPE_RGBA;				// RGB and Alpha pixel type
    pfd.cColorBits = COLOUR_DEPTH;				// Here we use our #define for the color bits
    pfd.cDepthBits = COLOUR_DEPTH;				// Ignored for RBA
    pfd.cAccumBits = 0;							// nothing for accumulation
    pfd.cStencilBits = COLOUR_DEPTH;						// nothing for stencil

	//Gets a best match on the pixel format as passed in from device
    if ( (pixelformat = ChoosePixelFormat(hdc, &pfd)) == false ) 
    { 
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
        return false; 
    } 
 
	//sets the pixel format if its ok. 
    if (SetPixelFormat(hdc, pixelformat, &pfd) == false) 
    { 
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
        return false; 
    } 
 
    return true;
}

void Scene3D::ResizeGLWindow(int width, int height)// Initialize The GL Window
{
	if (height == 0)// Prevent A Divide By Zero error
	{
		height = 1;// Make the Height Equal One
	}

	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//calculate aspect ratio	
	gluPerspective(45.0f, (float)width / (float)height, 0.1, 500.0f);

	glMatrixMode(GL_MODELVIEW);// Select The Modelview Matrix
	glLoadIdentity();// Reset The Modelview Matrix
}

void Scene3D::InitializeOpenGL(int width, int height) 
{  
    hdc = GetDC(*hwnd);//  sets  global HDC

    if (!CreatePixelFormat(hdc))//  sets  pixel format
        PostQuitMessage (0);

    hrc = wglCreateContext(hdc);	//  creates  rendering context from  hdc
    wglMakeCurrent(hdc, hrc);		//	Use this HRC.

	ResizeGLWindow(width, height);	// Setup the Screen
}

void Scene3D::Resize()
{
	if(hwnd == NULL)
		return;

	GetClientRect(*hwnd, &screenRect);	
	ResizeGLWindow(screenRect.right, screenRect.bottom);	
}

void Scene3D::Init(HWND* wnd, Input* in)
{
	hwnd = wnd;
	input = in;

	GetClientRect(*hwnd, &screenRect);	//get rect into our handy global rect
	InitializeOpenGL(screenRect.right, screenRect.bottom); // initialise openGL

	//OpenGL settings
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glClearStencil(0);
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	
	//Also, do any other setting variables here for your app if you wish. 
	glEnable(GL_COLOR_MATERIAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	glEnable(GL_DEPTH_TEST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	glEnable(GL_TEXTURE_2D);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//variables for the threads
	iterations = 500;
	left = -1;
	right = 1;
	top = -1;
	bottom = 1;
	updateTex = 0;

	//starts the mandelbrot set in a thread
	mandelbrotThread = thread(&Mandelbrot::start, &mandelbrot, iterations, left, right, top, bottom, &image);
}

void Scene3D::DrawScene(float dt)
{
	handleInput(dt);
	// Clear The Screen And The Depth Buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 

	glLoadIdentity();// load Identity Matrix

	// Where we are, What we look at, and which way is up
	gluLookAt(camera.getPosition().getX(), camera.getPosition().getY(), camera.getPosition().getZ(), 
			  camera.getLook().getX(), camera.getLook().getY(), camera.getLook().getZ(),  
			  camera.getUp().getX(), camera.getUp().getY(), camera.getUp().getZ());

	//glGenTextures(1, &mandelbrotTex);
	//glBindTexture(GL_TEXTURE_2D, mandelbrotTex);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image);

	//if to be updated
	if (mandebrotUpdate)
	{
		//have mandelbrot stop and restart with new values
		mandelbrot.end();
		//if thread is still finished
		if (!mandelbrot.isRunning())
		{
			mandelbrotThread.join();
			//restart mandelbrot set thread with new data
			mandelbrotThread = thread(&Mandelbrot::start, &mandelbrot, iterations, left, right, top, bottom, &image);
			mandebrotUpdate = false;
		}
	}
	
	//draw the square the texture will be drawn onto

	glPushMatrix();
		glScalef(100.0f, 100.0f, 0.0f);
		glRotatef(90, -1, 0, 0);
		drawQuad();
	glPopMatrix();
	// reset colour
	glColor3f(1.0f, 1.0f, 1.0f);

	SwapBuffers(hdc);// Swap the frame buffers.
}	

void Scene3D::drawQuad()
{	
	glPushMatrix();
		glBegin (GL_TRIANGLES);
			glNormal3f( 0.0f, -1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-1, 0, -1);
                        
			glNormal3f( 0.0f, -1.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(1, 0, 1);

			glNormal3f( 0.0f, -1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(1, 0, -1);

			glNormal3f( 0.0f, -1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-1, 0, -1);
                        
			glNormal3f( 0.0f, -1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-1, 0, 1);
                        
			glNormal3f( 0.0f, -1.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(1, 0, 1);
		glEnd();
	glPopMatrix();
}

void Scene3D::handleInput(float dt)
{
	//disabled camera rotation controls
	/*if ((input->getMouseX() < (SCREEN_WIDTH * 0.25)) || (input->getMouseX() > (SCREEN_WIDTH * 0.75)))
	{
		camera.setRotation(camera.getRotation().getX() + 
							(dt * camera.getRotationalVelocity() * ((input->getMouseX() - (SCREEN_WIDTH / 2.0)) / (SCREEN_WIDTH / 2.0))), 
							camera.getRotation().getY(), 
							camera.getRotation().getZ());
	}


	if ((input->getMouseY() < (SCREEN_HEIGHT * 0.25)) || (input->getMouseY() > (SCREEN_HEIGHT * 0.75)))
	{
		camera.setRotation(camera.getRotation().getX(), 
							camera.getRotation().getY() - 
							(dt * camera.getRotationalVelocity() * ((input->getMouseY() - (SCREEN_HEIGHT / 2.0)) / (SCREEN_HEIGHT / 2.0))), 
							camera.getRotation().getZ());
	}*/
	
	//move up
	if (input->isKeyDown('W'))
	{
		top += dt;
		bottom += dt;
		mandebrotUpdate = true;
	}

	//move down
	if (input->isKeyDown('S'))
	{
		top -= dt;
		bottom -= dt;
		mandebrotUpdate = true;
	}

	//zoom in
	if (input->isKeyDown('Q'))
	{
		camera.setPosition(camera.getPosition().getX() + (camera.getForward().getX() * dt * camera.getVelocity()), 
						   camera.getPosition().getY() + (camera.getForward().getY() * dt * camera.getVelocity()), 
						   camera.getPosition().getZ() + (camera.getForward().getZ() * dt * camera.getVelocity()));
		top += dt;
		left += dt;
		bottom -= dt;
		right -= dt;
		mandebrotUpdate = true;
	}

	//zoom out
	if (input->isKeyDown('E'))
	{
		camera.setPosition(camera.getPosition().getX() + (-camera.getForward().getX() * dt * camera.getVelocity()), 
							camera.getPosition().getY() + (-camera.getForward().getY() * dt * camera.getVelocity()), 
							camera.getPosition().getZ() + (-camera.getForward().getZ() * dt * camera.getVelocity()));
		top -= dt;
		left -= dt;
		bottom += dt;
		right += dt;
		mandebrotUpdate = true;
	}
		
	//move left
	if (input->isKeyDown('A'))
	{
		left -= dt;
		right -= dt;
		mandebrotUpdate = true;
	}

	//move right
	if (input->isKeyDown('D'))
	{
		left += dt;
		right += dt;
		mandebrotUpdate = true;
	}
	
	//wait for threads to finish before quitting
	if (input->isKeyDown(VK_ESCAPE))
	{
		mandelbrot.end();
		mandelbrotThread.join();
	}
	
	//increase iterations
	if (input->isKeyDown('+'))
	{
		input->SetKeyUp('+');
		iterations += 10;
		mandebrotUpdate = true;
	}

	//decrease iterations
	if (input->isKeyDown('-'))
	{		
		input->SetKeyUp('-');
		iterations -= 10;
		mandebrotUpdate = true;
	}
	
	camera.CameraUpdate();

	//wireframe mode
	static bool enterPressed = false;
	static bool drawMode = true;
	if (input->isKeyDown(VK_RETURN) && !enterPressed)
	{
		drawMode = !drawMode;
	}

	if (drawMode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	enterPressed = input->isKeyDown(VK_RETURN);
}