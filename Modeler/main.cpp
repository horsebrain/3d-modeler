#include <iostream>
#include "SDL.h"
#include "SDL_opengl.h"
#include <stdlib.h>

#include "App_Log.h"
#include "Vector.h"
#include "Macros.h"
#include "Texture.h"
#include "ConfigFile.h"
#include "GLUI2D.h"
#include "Model.h"
#include "Camera.h"

//constants
const char LOG_FILE[] = "Modeler.log";

//global variables
CApp_Log Log(LOG_FILE);
Model * model;
Camera view;

//functions declarations
bool Init_GL(int w, int h);
void Render_Scene();

int main(int argc, char *argv[])
{
	Log.Write("Initialization:");
	Log.Indent();
	SDL_ASSERT(SDL_Init(SDL_INIT_VIDEO) == 0);
	atexit(SDL_Quit);
	
	Log.Write("SDL Initialized OK!");
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	SDL_ASSERT(SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,16,SDL_ANYFORMAT | SDL_OPENGL) != NULL);
	SDL_WM_SetCaption("OpenGL",NULL);
	
	Log.Write("Video mode set OK!");
	
	if(!Init_GL(SCREEN_WIDTH,SCREEN_HEIGHT))
	{
		Log.Write("Init failed!");
		exit(-1);
	}

	model = new Model();
	
	SDL_Event Event;
	bool Done = false;
	bool Shifted = false;			//is shift pressed?
	
	Log.Write("Init OK!");
	Log.Unindent();
	
	while(!Done)
	{
		if(SDL_PollEvent(&Event))
		{
			switch(Event.type)
			{
				case SDL_QUIT:
					Done = true;
					break;
				case SDL_KEYDOWN:
					switch(Event.key.keysym.sym)
					{
						CASE(SDLK_ESCAPE,Done=true)
						CASE(SDLK_q,Done=true;)
						//View commands
						CASE(SDLK_PAGEUP,view.Zoom(1))				//Page Up/Down zooms
						CASE(SDLK_PAGEDOWN,view.Zoom(-1))
						CASE(SDLK_LEFT,view.Swivel(-15))				//Arrows rotate
						CASE(SDLK_RIGHT,view.Swivel(15))
						CASE(SDLK_UP,view.Raise(-15))
						CASE(SDLK_DOWN,view.Raise(15))
						CASE(SDLK_LSHIFT,Shifted=true);
						CASE(SDLK_RSHIFT,Shifted=true);
						default:break;
					}
					break;
				case SDL_KEYUP:
				{
					CASE(SDLK_LSHIFT,Shifted=false);
					CASE(SDLK_RSHIFT,Shifted=false);
				}
				case SDL_MOUSEBUTTONDOWN:
					switch(Event.button.button)
					{
						CASE(4,view.Zoom(.5))			//Wheel up
						CASE(5,view.Zoom(-.5))			//Wheel down
					}
					break;
				case SDL_MOUSEMOTION:
					if(Event.motion.state & SDL_BUTTON(SDL_BUTTON_RIGHT))
					{
						if(!Shifted)
						{
							view.Swivel(Event.motion.xrel);
							view.Raise(Event.motion.yrel);
						}
						else view.Move(Vector3D<>(Event.motion.xrel * 8./640.,-Event.motion.yrel * 8./640.,0));
					}
					break;
			}
			model->Event(Event);
		}
		else
		{
			Render_Scene();
		}
	}
	SDL_Quit();
	GLUI2D::Shutdown();
	delete model;
	return 0;
}

bool Init_GL(int width, int height)
{
	glShadeModel(GL_FLAT);
	
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glPolygonMode(GL_FRONT,GL_LINE);
		
	glClearColor(0.0,0.0,0.0,0);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_2D);

	glViewport(0,0,width,height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,((float)width)/((float)height),0.1f,100.0f);
		
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ZERO);
	
	glMatrixMode(GL_MODELVIEW);

	srand(SDL_GetTicks());

	if(!GLUI2D::Init())
	{
		Log.Write("Unable to initialize 2D engine");
		return false;
	}

	return true;
}

void Render_Scene()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	view.View();

	glBindTexture(GL_TEXTURE_2D,0);
	
	model->Draw();

	SDL_GL_SwapBuffers();
}
