/*
 *  wxModeler.cpp
 *  wxModeler
 *
 *  Created by Nelson El-Hage on Sun Dec 22 2002.
 *
 */

#include "wxModeler.h"
#include "Camera.h"
#include "ViewDialog.h"
#include <wx/image.h>
//#include "LightwaveObjectRenderer.h"

//LightwaveObjectRenderer LOR;
//LightwaveObject Obj;

IMPLEMENT_APP(ModelerApp)

bool ModelerApp::OnInit()
{
	wxInitAllImageHandlers();
	
	modeler = new Modeler();

	canvas = new ModelerGLCanvas(modeler);
	modeler->Show();
	SetTopWindow(modeler);

	ViewDialog * ViewDlg = new ViewDialog(modeler);
	ViewDlg->Show(true);
	
	return true;
}

ModelerApp::~ModelerApp()
{
}

BEGIN_EVENT_TABLE(Modeler,wxFrame)
	EVT_CLOSE(Modeler::OnClose)
	EVT_TOOL(ID_NEW,Modeler::OnNew)
	EVT_TOOL(ID_SAVE,Modeler::OnSave)
	EVT_TOOL(ID_LOAD,Modeler::OnLoad)
	EVT_TOOL(ID_CUBE,Modeler::OnCube)
	EVT_TOOL(ID_FACE,Modeler::OnFace)
	EVT_TOOL(ID_POINT,Modeler::OnPoint)
END_EVENT_TABLE()

Modeler::Modeler() :
	wxFrame(NULL,-1,"Simple GL Modeler",wxDefaultPosition,
		 wxSize(800,600))
{
	ToolBar = CreateToolBar();

	ToolBar->AddTool(ID_NEW,"New",wxBitmap("data/new.gif",wxBITMAP_TYPE_GIF),"New model");
	ToolBar->AddTool(ID_SAVE,"Save",wxBitmap("data/save.gif",wxBITMAP_TYPE_GIF),"Save model to disk");
	ToolBar->AddTool(ID_LOAD,"Load",wxBitmap("data/load.gif",wxBITMAP_TYPE_GIF),"Load model from disk");
	ToolBar->AddSeparator();
	ToolBar->AddTool(ID_CUBE,"Cube",wxBitmap("data/cube.gif",wxBITMAP_TYPE_GIF),"Cube selection mode");
	ToolBar->AddTool(ID_FACE,"Face",wxBitmap("data/face.gif",wxBITMAP_TYPE_GIF),"Face selection mode");
	ToolBar->AddTool(ID_POINT,"Point",wxBitmap("data/point.gif",wxBITMAP_TYPE_GIF),"Point selection mode");

	ToolBar->Realize();

	Filename = "";
}

void Modeler::OnClose(wxCloseEvent & e)
{
	if(e.CanVeto())
	{
		if(!CanClose())
			return;
	}
	Destroy();
}

void Modeler::OnNew(wxCommandEvent & e)
{
	if(CanClose())
	{
		model.Reset();
		view.Reset();
		canvas->Refresh(false);
	}
}

void Modeler::OnSave(wxCommandEvent & e)
{
	if(Filename == "")
	{
		wxFileDialog * dlg = new wxFileDialog(this,"Save model",Filedir,Filename,
										"Model files(*.mdl)|*.mdl|All files(*.*)|*.*",wxSAVE);
		if(dlg->ShowModal() == wxID_OK)
		{
			Filedir = dlg->GetDirectory();
			Filename = dlg->GetFilename();
			Filepath = dlg->GetPath();
		}
		dlg->Destroy();
	}
	model.Save(Filepath);
}

void Modeler::OnLoad(wxCommandEvent & e)
{
	if(!CanClose()) return;
	wxFileDialog * dlg = new wxFileDialog(this,"Load model",Filedir,Filename,
									"Model files(*.mdl)|*.mdl|All files(*.*)|*.*",wxOPEN);
	if(dlg->ShowModal() == wxID_OK)
	{
		Filedir = dlg->GetDirectory();
		Filename = dlg->GetFilename();
		Filepath = dlg->GetPath();
	}
	dlg->Destroy();
	
	model.Load(Filepath);
}

void Modeler::OnCube(wxCommandEvent & e)
{
	model.SelCubes();
	canvas->Refresh(false);
}

void Modeler::OnFace(wxCommandEvent & e)
{
	model.SelFaces();
	canvas->Refresh(false);
}

void Modeler::OnPoint(wxCommandEvent & e)
{
	model.SelPoints();
	canvas->Refresh(false);
}

bool Modeler::CanClose()
{
	if(Filename == "") return true;
	switch(wxMessageBox("Save model before closing?","Are you sure?",wxYES_NO|wxCANCEL|wxICON_QUESTION))
	{
		case wxYES:
			model.Save(Filepath);
			return true;
		case wxNO:
			return true;
		case wxCANCEL:
		default:
			return false;
	}
}

BEGIN_EVENT_TABLE(ModelerGLCanvas, wxGLCanvas)
	EVT_PAINT(ModelerGLCanvas::OnPaint)
	EVT_ERASE_BACKGROUND(ModelerGLCanvas::OnEraseBackground)
	EVT_KEY_DOWN(ModelerGLCanvas::OnKeyDown)
	EVT_KEY_UP(ModelerGLCanvas::OnKeyUp)
	EVT_LEFT_DOWN(ModelerGLCanvas::OnMouseDown)
	EVT_RIGHT_DOWN(ModelerGLCanvas::OnMouseDown)
	EVT_LEFT_UP(ModelerGLCanvas::OnMouseUp)
	EVT_RIGHT_UP(ModelerGLCanvas::OnMouseUp)
	EVT_MOTION(ModelerGLCanvas::OnMouseMove)
	EVT_ENTER_WINDOW(ModelerGLCanvas::OnMouseMove)
	EVT_MOUSEWHEEL(ModelerGLCanvas::OnMouseWheel)
	EVT_SIZE(ModelerGLCanvas::OnResize)
END_EVENT_TABLE()

ModelerGLCanvas::ModelerGLCanvas(wxWindow * parent) :
					wxGLCanvas(parent,(wxGLCanvas*)NULL)
{
	Init = false;
	RightButtonDown = false;
	SetCurrent();
	Refresh(false);
}

ModelerGLCanvas::~ModelerGLCanvas()
{
}

void ModelerGLCanvas::OnPaint(wxPaintEvent & e)
{
	//wxMessageBox("Paint");
	wxPaintDC dc(this);
	
	SetCurrent();

	if(!Init)
		InitGL();
	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	view.View();

	glBindTexture(GL_TEXTURE_2D,0);

	model.Draw();
	//LOR.setObject(&Obj);
	//LOR.draw();
		
	SwapBuffers();
}

void ModelerGLCanvas::OnEraseBackground(wxEraseEvent & e)
{
	//wxMessageBox("erasebg");
}

void ModelerGLCanvas::OnKeyDown(wxKeyEvent & e)
{
	//wxMessageBox("keydown");
	switch(e.GetKeyCode())
	{
		//View Commands
		CASE(WXK_HOME,view.Zoom(1))
		CASE(WXK_END,view.Zoom(-1))
		CASE(WXK_LEFT,view.Swivel(-15))
		CASE(WXK_RIGHT,view.Swivel(15))
		CASE(WXK_UP,view.Raise(-15))
		CASE(WXK_DOWN,view.Raise(15))
		//Modeling commands
		CASE('W',model.SelNext())
		CASE('S',model.SelPrev())
		CASE('E',model.AddCube())
		CASE('1',model.SelCubes())
		CASE('2',model.SelFaces())
		CASE('3',model.SelPoints())
		CASE('4',model.DrawLine())
		CASE('5',model.DrawFilled())
		//other
		CASE('=',model.GrowHandles())
		CASE('-',model.ShrinkHandles())
		default: return;
	}
	Refresh(false);
}

void ModelerGLCanvas::OnKeyUp(wxKeyEvent & e)
{
}

void ModelerGLCanvas::OnMouseDown(wxMouseEvent & e)
{
	//wxMessageBox("mousedown");
	if(e.LeftDown()) model.Click(e.GetX(),e.GetY());
	if(e.RightDown()) RightButtonDown = true;
	MouseX = e.GetX();
	MouseY = e.GetY();
	Refresh(false);
}

void ModelerGLCanvas::OnMouseUp(wxMouseEvent & e)
{
	//wxMessageBox("mouseup");
	if(e.LeftUp()) model.MouseUp(e.GetX(),e.GetY());
	if(e.RightUp()) RightButtonDown = false;
	MouseX = e.GetX();
	MouseY = e.GetY();
	Refresh(false);
}

void ModelerGLCanvas::OnMouseMove(wxMouseEvent & e)
{
	//wxMessageBox("mousemove");
	int xrel = e.GetX() - MouseX;
	int yrel = e.GetY() - MouseY;
	if(RightButtonDown)					//Right-Drag is used to move the camera
	{
		if(e.ShiftDown())				//with shift, it moves left/right
		{
			view.Move(Vector3D<>(xrel * 8./640.,-yrel * 8./640,0));
		}
		else if(e.ControlDown())			//crtl-Right-Drag zooms
		{
			view.Zoom(-yrel * 10./240.);
		}
		else							//with no modifiers, Right-Drag rotates the view
		{
			view.Swivel(xrel);
			view.Raise(yrel);
		}
	}
	else
		model.MouseMove(xrel,yrel);
	
	MouseX = e.GetX();
	MouseY = e.GetY();

	Refresh(false);
}

void ModelerGLCanvas::OnMouseWheel(wxMouseEvent & e)
{
	//wxMessageBox("Wheel");
	view.Zoom(e.GetWheelRotation() / e.GetWheelDelta());
}

void ModelerGLCanvas::OnResize(wxSizeEvent & e)
{
	Resize();	
	Refresh(false);
	e.Skip();
}

void ModelerGLCanvas::InitGL()
{
	glShadeModel(GL_SMOOTH);

	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glPolygonMode(GL_FRONT,GL_LINE);

	glClearColor(0.0,0.0,0.0,0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_2D);

	Resize();
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ZERO);

	glMatrixMode(GL_MODELVIEW);

	//srand(SDL_GetTicks());

	if(!GLUI2D::Init())
	{
		Log.Write("Unable to initialize 2D engine");
	}

	/*if(!Obj.loadObject("model.lwo"))
	{
		Log.Write("Unable to load model \"model.lwo\"");
	}*/

	model.Reset();
	Init = true;
}

void ModelerGLCanvas::Resize()
{
	SetCurrent();
	
	int width = GetClientSize().GetWidth();
	int height = GetClientSize().GetHeight();
	

	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,((float)width)/((float)height),0.1f,100.0f);
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
}