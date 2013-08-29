/*
 *  ViewDialog.cpp
 *  wxModeler
 *
 *  Created by Nelson El-Hage on Mon Dec 23 2002.
 *
 */

#include "ViewDialog.h"
#include "camera.h"
#include "wxModeler.h"
#include "vector.h"
#include <wx/image.h>

BEGIN_EVENT_TABLE(ViewDialog, wxDialog)
	EVT_BUTTON(ID_LEFT,ViewDialog::OnLeft)
	EVT_BUTTON(ID_RIGHT,ViewDialog::OnRight)
	EVT_BUTTON(ID_UP,ViewDialog::OnUp)
	EVT_BUTTON(ID_DOWN,ViewDialog::OnDown)
	EVT_BUTTON(ID_ZOOM_IN,ViewDialog::OnZoomIn)
	EVT_BUTTON(ID_ZOOM_OUT,ViewDialog::OnZoomOut)
END_EVENT_TABLE()

ViewDialog::ViewDialog( wxWindow * parent)
	: wxDialog(parent,-1,"Camera",wxDefaultPosition)
{
	Left = new wxBitmapButton(this,ID_LEFT,wxBitmap("data/left.gif",wxBITMAP_TYPE_GIF));
	Right = new wxBitmapButton(this,ID_RIGHT,wxBitmap("data/right.gif",wxBITMAP_TYPE_GIF));
	Up = new wxBitmapButton(this,ID_UP,wxBitmap("data/up.gif",wxBITMAP_TYPE_GIF));
	Down = new wxBitmapButton(this,ID_DOWN,wxBitmap("data/down.gif",wxBITMAP_TYPE_GIF));
	ZoomIn = new wxBitmapButton(this,ID_ZOOM_IN,wxBitmap("data/ZoomIn.gif",wxBITMAP_TYPE_GIF));
	ZoomOut = new wxBitmapButton(this,ID_ZOOM_OUT,wxBitmap("data/ZoomOut.gif",wxBITMAP_TYPE_GIF));

	Move = new wxCheckBox(this,-1,"");

	wxGridSizer * grd = new wxGridSizer(3,4,0,0);

	grd->Add(32,32);
	grd->Add(Up);
	grd->Add(32,32);
	grd->Add(ZoomIn);
	
	grd->Add(Left);
	grd->Add(Move,0,wxALIGN_CENTER);
	grd->Add(Right);
	grd->Add(32,32);

	grd->Add(32,32);
	grd->Add(Down);
	grd->Add(32,32);
	grd->Add(ZoomOut);

	SetSizer(grd);
	grd->SetSizeHints(this);
}

void ViewDialog::OnLeft(wxCommandEvent & e)
{
	if(Move->IsChecked())
		view.Move(Vector3D<>(-1,0,0));
	else
		view.Swivel(-15);
	canvas->Refresh(false);
}

void ViewDialog::OnRight(wxCommandEvent & e)
{
	if(Move->IsChecked())
		view.Move(Vector3D<>(1,0,0));
	else
		view.Swivel(15);
	canvas->Refresh(false);
}

void ViewDialog::OnUp(wxCommandEvent & e)
{
	if(Move->IsChecked())
		view.Move(Vector3D<>(0,1,0));
	else
		view.Raise(-15);
	canvas->Refresh(false);
}

void ViewDialog::OnDown(wxCommandEvent & e)
{
	if(Move->IsChecked())
		view.Move(Vector3D<>(0,-1,0));
	else
		view.Raise(15);
	canvas->Refresh(false);
}

void ViewDialog::OnZoomIn(wxCommandEvent & e)
{
	view.Zoom(1);
	canvas->Refresh(false);
}

void ViewDialog::OnZoomOut(wxCommandEvent & e)
{
	view.Zoom(-1);
	canvas->Refresh(false);
}