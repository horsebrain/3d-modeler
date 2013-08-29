/*
 *  Model.h
 *
 *  Created by Nelson El-Hage on Wed Nov 27 2002.
 *	The Model class stores and manipulates the `model' edited
 */

#ifndef __MODEL_H__
#define __MODEL_H__

#include "vector.h"
#include "Macros.h"
#include <vector>
#include "ogl.h"
#include <wx/wx.h>

const unsigned int Faces[6][4] =
						{{0,1,2,3},			//Faces[i] contains the indices into the Verts array
						{1,5,6,2},			//of a Cube that make up the vertices of face i.
						{5,4,7,6},
						{4,0,3,7},
						{5,1,0,4},
						{2,6,7,3}};

class Model
{
	private:
		std::vector<Vector3D<> > Points;
	
		struct Cube
		{
			unsigned int Verts[8];		//indices into the Points array
			int Adjacent[6];			//indices into the Cubes array representing the adjacent cubes
			Cube();
		};
		
		std::vector<Cube> Cubes;
		
		unsigned int SelectedCube;		//the index of the currently selected cube
		unsigned char SelectedFace;		//the currently selected face (0-5)
		unsigned int SelectedPoint;
		
		enum EditMode
		{
			SEL_CUBE,
			SEL_FACE,
			SEL_POINT,
		};

		EditMode Mode;				//what editing mode are we in?

		enum DrawMode
		{
			DRW_FILLED,
			DRW_LINE
		};

		DrawMode DMode;

		enum DragMode
		{
			DRG_NONE,
			DRG_X = 6,
			DRG_Y,
			DRG_Z
		};

		DragMode DrgMode;

		double HandleLength;		//the length of the drag handles for moving

	public:
		Model();

		void Reset();				//Initialize us with the default new model

		void Draw();

		void DrawPicking();				//draw the geometry for the picking function
						 //select the next or previous item
		void SelNext();
		void SelPrev();

		//Change the selection mode
		void SelCubes(){Mode=SEL_CUBE;}
		void SelFaces(){Mode=SEL_FACE;}
		void SelPoints(){Mode=SEL_POINT;}

		//Change the Drawing mode
		void DrawLine(){DMode=DRW_LINE;}
		void DrawFilled(){DMode=DRW_FILLED;}

		//Add a cube onto the current face
		void AddCube();

		//Handle a mouse click
		void Click(int x, int y);

		//Handle a L button up
		void MouseUp(int x, int y);

		//Handle a mouse move
		void MouseMove(int xrel, int yrel);

		//void Event(SDL_Event &Event);
		void GrowHandles(){HandleLength *= 3./2.;}
		void ShrinkHandles(){HandleLength *= 2./3.;}

		void Load(const char * file);
		void Save(const char * file);
};

#endif			//__MODEL_H__


