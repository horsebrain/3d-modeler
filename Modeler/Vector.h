#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "ogl.h"
#include <math.h>
#include "macros.h"

/*******************************************************
***********************VECTOR3D*************************
********************************************************
A class for the definition and manipulation of 3d vectors
********************************************************/

template<class T = GLdouble>
class Vector3D
{
	public:
		T V[3];

		Vector3D(){Set(0,0,0);}
		Vector3D(T x,T y,T z){Set(x,y,z);}
		~Vector3D() {}

		void Set(T x, T y, T z){V[0] = x; V[1] = y; V[2] = z;}

		Vector3D Normalize(){return *this /= Mag();} 				//reduce to a unit vector
		Vector3D Norm(){return *this / Mag();}						//return the norm (unit vector of the same direction)
		T Mag(){return sqrt(V[0]*V[0]+V[1]*V[1]+V[2]*V[2]);}	//returns the magnitude

		Vector3D operator*(T rhs){return Vector3D(V[0]*rhs,V[1]*rhs,V[2]*rhs);}
		Vector3D operator/(T rhs){return Vector3D(V[0]/rhs,V[1]/rhs,V[2]/rhs);}

		Vector3D operator+(T rhs){return Vector3D(V[0]+rhs,V[1]+rhs,V[2]+rhs);}
		Vector3D operator+(const Vector3D &rhs){return Vector3D(V[0]+rhs.V[0],V[1]+rhs.V[1],V[2]+rhs.V[2]);}
		Vector3D operator-(T rhs){return Vector3D(V[0]-rhs,V[1]-rhs,V[2]-rhs);}
		Vector3D operator-(const Vector3D &rhs){return Vector3D(V[0]-rhs.V[0],V[1]-rhs.V[1],V[2]-rhs.V[2]);}
		
		Vector3D operator*=(T rhs){return (*this = this->operator*(rhs));}
		Vector3D operator/=(T rhs){return (*this = this->operator/(rhs));}

		Vector3D operator+=(T rhs){return (*this = this->operator+(rhs));}
		Vector3D operator+=(const Vector3D &rhs){return (*this = this->operator+(rhs));}
		Vector3D operator-=(T rhs){return (*this = this->operator-(rhs));}
		Vector3D operator-=(const Vector3D &rhs){return (*this = this->operator-(rhs));}

		Vector3D operator-(){return Vector3D(-V[0],-V[1],-V[2]);}

		bool operator == (const Vector3D &rhs){ return V[0] == rhs.V[0] && V[1] == rhs.V[1] && V[2] == rhs.V[2];}
		bool operator != (const Vector3D &rhs){ return !(this->operator==(rhs));}

		const Vector3D & operator=(const Vector3D & rhs){V[0]=rhs.V[0];V[1]=rhs.V[1];V[2]=rhs.V[2];return *this;}

		T dot(const Vector3D &rhs){return V[0]*rhs.V[0] + V[1]*rhs.V[1] + V[2]*rhs.V[2];} 	  //dot product
		Vector3D cross(const Vector3D &rhs)	 //cross product
		{
			return Vector3D((V[1]*rhs.V[2])-(V[2]*rhs.V[1]),
								(V[2]*rhs.V[0])-(V[0]*rhs.V[2]),
								(V[0]*rhs.V[1])-(V[1]*rhs.V[0]));
		}

		void SendOGLVertex();
		void SendOGLNormal();

		static Vector3D RandV(const Vector3D & Min,const Vector3D & Max)		//return a vector between the two
		{
			return Vector3D(RANDFLOAT(Min.V[0],Max.V[0]),
							RANDFLOAT(Min.V[1],Max.V[1]),
							RANDFLOAT(Min.V[2],Max.V[2]));
		}
};

#endif
