/*
 *  GLUI2D.h
 *
 *  Created by Nelson El-Hage on Thu Sep 19 2002.
 *  The GLUI2D namespace contains methods for drawing 2D graphics with OpenGL
 */

#ifndef __GLUI2D_H__
#define __GLUI2D_H__

namespace GLUI2D
{
	//Texture Font;
	//GLuint FontBase;
	bool Init();			//initialize the font engine
	void Shutdown();		//shutdown	  "   "     "

	void Begin2D();			//Enter 2D (ortho) mode
	void End2D();			//Exit 2D (ortho) mode

	void Text(int x, int y, const char * txt,...);	//print characters to the screen

	int Length(const char * str);	//return the pixel length of a string

	//Rect() is like glRect*(), but it takes a width and a height,
	//not two corners, and it adds texture coordinates
	void Rect(int x, int y, int w, int h);
	//void Rect(GLUIRect & r);
};

#endif

