/*
 *  ViewDialog.h
 *  wxModeler
 *
 *  Created by Nelson El-Hage on Mon Dec 23 2002.
 *
 */

#include <wx/wx.h>

class ViewDialog : public wxDialog
{
	public:
		ViewDialog(wxWindow * parent);
	protected:
		wxBitmapButton *Left, *Right,
						*Up, *Down,
						*ZoomIn, *ZoomOut;
		wxCheckBox * Move;
		
		enum
		{
			ID_LEFT,
			ID_RIGHT,
			ID_UP,
			ID_DOWN,
			ID_ZOOM_IN,
			ID_ZOOM_OUT
		};

		void OnLeft(wxCommandEvent & e);
		void OnRight(wxCommandEvent & e);
		void OnUp(wxCommandEvent & e);
		void OnDown(wxCommandEvent & e);
		void OnZoomIn(wxCommandEvent & e);
		void OnZoomOut(wxCommandEvent & e);

		DECLARE_EVENT_TABLE()
};