/*
 *  Camera.h
 *
 *  Created by Nelson El-Hage on Wed Nov 27 2002.
 *	A camera class encapsulates a viewpoint for a scene
 */

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "ogl.h"
#include <math.h>
#include "vector.h"

class Camera
{
	private:
		Vector3D<> Pos;
		float YRot,						//our rotation about the Y axis
				Elevation;				//our elevation above the x-z plane
	public:
		Camera() : Pos(0,0,-10), YRot(0), Elevation(0) {}

		void Zoom(float dist){Pos.V[2] += dist;}			//Move the camera in or out
		void Move(const Vector3D<> & v){Pos += v;}
		void Swivel(float ang){YRot += ang;}			//swivels L-R around the Y Axis
		void Raise(float ang){Elevation += ang;}		//Adjust elevation above x-z plane

		void View()						//do the appropriate glTranslate() and glRotate() 's
		{
			glTranslated(Pos.V[0],Pos.V[1],Pos.V[2]);

			glRotated(Elevation,1.0,0.0,0.0);
			glRotated(YRot,0.0,1.0,0.0);
		}

		void Reset()
		{
			YRot = Elevation = 0;
			Pos.Set(0.0,0.0,-10.0);
		}
};

#endif
