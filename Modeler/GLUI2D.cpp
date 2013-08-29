/*
 *  GLUI2D.cpp
 *  GLUI
 *
 *  Created by Nelson El-Hage on Thu Sep 19 2002.
 *
 */

#include "Macros.h"
#include "App_Log.h"
#include "texture.h"
#include "GLUI2D.h"
#include <stdarg.h>
#include <string.h>

extern CApp_Log Log;

namespace GLUI2D
{
	Texture Font;
	GLuint FontBase;
	GLint PolyMode;
};

bool GLUI2D::Init()
{
	if(!Font.Load("font.png"))
	{
		Log.Write("Unable to load font.png!");
		return false;
	}
	
	FontBase = glGenLists(256);
	Font.Bind();
	float tx,ty;
	for(int i=0;i<256;i++)
	{
		tx = (i%16)/16.0f;			//find the tex coords of the lower left
		ty = 1 - (i/16)/16.0f;
		glNewList(FontBase+i,GL_COMPILE);
			glBegin(GL_QUADS);
				glTexCoord2f(tx,ty-1./16.);		//Lower Left
				glVertex2i(0,0);

				glTexCoord2f(tx+1./16.,ty-1./16.);	//Lower Right
				glVertex2i(16,0);

				glTexCoord2f(tx+1./16.,ty);	//Upper Right
				glVertex2i(16,16);

				glTexCoord2f(tx,ty);	//Upper Left
				glVertex2i(0,16);					
			glEnd();
			glTranslated(10,0,0);						//move to the right to be in position for next char
		glEndList();
	}
	return true;
}

void GLUI2D::Shutdown()
{
	glDeleteLists(256,FontBase);
}

void GLUI2D::Begin2D()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();					//save the old projection matrix

	glLoadIdentity();
	gluOrtho2D(0,SCREEN_WIDTH,0,SCREEN_HEIGHT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glGetIntegerv(GL_POLYGON_MODE,&PolyMode);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}

void GLUI2D::End2D()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPolygonMode(GL_FRONT_AND_BACK,PolyMode);
}

void GLUI2D::Text(int x, int y, const char  * text,...)
{
	if(text == NULL)
		return;
	
	GLint src,dst;
	//Save the old BlendFunc
	glGetIntegerv(GL_BLEND_SRC,&src);
	glGetIntegerv(GL_BLEND_DST,&dst);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	char buffer[1024];				//buffer overflow!!
	va_list va;

	va_start(va, text);
	vsprintf(buffer,text,va);
	va_end(va);
	//Log.Write("printing %s",buffer);
	glTranslated(x,y,0);
	glListBase(FontBase-32);
	Font.Bind();
	glCallLists(strlen(buffer),GL_BYTE,buffer);
	glPopMatrix();

	//restore the old BlendFunc
	glBlendFunc(src,dst);
}

int GLUI2D::Length(const char * str)
{
	//Each character is 16x16, but there is only 10px placed between the start of
	//one char and the next. Thus, the length of string is simply 10*strlen(s);
	return 10*strlen(str);
}

void GLUI2D::Rect(int x, int y, int w, int h)
{
	glBegin(GL_QUADS);
		glTexCoord2f(0.0,0.0);
		glVertex2i(x,y);

		glTexCoord2f(1.0,0.0);
		glVertex2i(x+w,y);

		glTexCoord2f(1.0,1.0);
		glVertex2i(x+w,y+h);

		glTexCoord2f(0.0,1.0);
		glVertex2i(x,y+h);
	glEnd();
}


