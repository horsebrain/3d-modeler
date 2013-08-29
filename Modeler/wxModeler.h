/*
 *  wxModeler.h
 *  wxModeler
 *
 *  Created by Nelson El-Hage on Sun Dec 22 2002.
 *
 */

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include "App_Log.h"
#include "Model.h"
#include "Macros.h"
#include "GLUI2D.h"
#include "camera.h"

#ifndef __WX_MODELER__
#define __WX_MODELER__

class ModelerApp : public wxApp
{
	public:
		virtual bool OnInit();
		~ModelerApp();
};

DECLARE_APP(ModelerApp)

class ModelerGLCanvas;

class Modeler : public wxFrame
{
	friend class ModelerApp;
	public:
		Modeler();
	protected:
		wxString Filename, Filedir, Filepath;
	
		wxToolBar * ToolBar;

		void OnClose(wxCloseEvent & e);

		enum
		{
			ID_NEW,
			ID_SAVE,
			ID_LOAD,
			ID_CUBE,
			ID_FACE,
			ID_POINT
		};

		void OnNew(wxCommandEvent & e);
		void OnSave(wxCommandEvent & e);
		void OnLoad(wxCommandEvent & e);
		void OnCube(wxCommandEvent & e);
		void OnFace(wxCommandEvent & e);
		void OnPoint(wxCommandEvent & e);

		bool CanClose();			//Can we close an open document?

	DECLARE_EVENT_TABLE()
};

class ModelerGLCanvas : public wxGLCanvas
{
	public:
		ModelerGLCanvas(wxWindow * parent);
		~ModelerGLCanvas();

		void InitGL();
		void Resize();
	private:
		bool Init;
		int MouseX, MouseY;
		bool RightButtonDown;			//RightIsDown() doesn't work for me for some reason
	protected:
		void OnPaint(wxPaintEvent & e);
		void OnEraseBackground(wxEraseEvent & e);

		void OnKeyDown(wxKeyEvent & e);
		void OnKeyUp(wxKeyEvent & e);

		void OnMouseDown(wxMouseEvent & e);
		void OnMouseUp(wxMouseEvent & e);
		void OnMouseMove(wxMouseEvent & e);
		void OnMouseWheel(wxMouseEvent & e);

		void OnResize(wxSizeEvent & e);
		
	DECLARE_EVENT_TABLE()
};

extern CApp_Log Log;
extern Camera view;
extern Model model;
extern Modeler * modeler;
extern ModelerGLCanvas * canvas;

#endif