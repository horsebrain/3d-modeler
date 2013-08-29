/********************************************************************
 * FILENAME.: OpenGL.h
 * NICKNAME.: Doubt
 * NAME.....: Daniel Vivas
 * E-MAIL...: daniel@vivas.com.br
 * DATE.....: 17/06/2002
 * HOME-PAGE: http://www.danielvivas.cjb.net
 * GROUP....: Jogos Brasil
 ********************************************************************/

#ifndef OPENGL_H
#define OPENGL_H

#define PI 3.14159f

#include <windows.h>  // Header File For Windows
#include <gl\gl.h>    // Header File For The OpenGL32 Library
#include <gl\glu.h>   // Header File For The GLu32 Library
#include <gl\glaux.h> // Header File For The GLaux Library

#include "resource_definitions.h"

#include "TextureLoader.h"
#include "LightwaveObject.h"
#include "LightwaveObjectRenderer.h"
#include "ObjectView.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

class OpenGL {
   protected:
      bool      FullScreen; // Fullscreen Flag Set To Fullscreen Mode By Default
      HDC       hDC;        // Private GDI Device Context
      HGLRC     hRC;        // Permanent Rendering Context
      HWND      hWnd;       // Holds Our Window Handle
      HINSTANCE hInstance;  // Holds The Instance Of The Application
   public:
      OpenGL();
	  ~OpenGL();
      bool InitGL();
      bool CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag);
      bool DrawGLScene(GLvoid);
      void SetFullScreen(bool fullscreen);
      bool GetFullScreen() const;
      friend LRESULT CALLBACK WndProc(HWND   hWnd, UINT   uMsg, WPARAM wParam, LPARAM lParam);
      HDC GethDC() const;
      HWND GethWnd() const;
      GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
      GLvoid KillGLWindow();

	  // Loader
	  void loadModel(char *filename);
	  int windowWidth, windowHeight;
	  ObjectView myView;

   private:
	   char modelFilename[255];
	   LightwaveObject *myObject;
	   LightwaveObjectRenderer myRenderer;
};

#endif;