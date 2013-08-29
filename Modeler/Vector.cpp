#include "Vector.h"

void Vector3D<GLdouble>::SendOGLVertex(){glVertex3dv(V);}
void Vector3D<GLdouble>::SendOGLNormal(){glNormal3dv(V);}

void Vector3D<GLfloat>::SendOGLVertex(){glVertex3fv(V);}
void Vector3D<GLfloat>::SendOGLNormal(){glNormal3fv(V);}

void Vector3D<GLint>::SendOGLVertex(){glVertex3iv(V);}
void Vector3D<GLint>::SendOGLNormal(){glNormal3iv(V);}

void Vector3D<GLshort>::SendOGLVertex(){glVertex3sv(V);}
void Vector3D<GLshort>::SendOGLNormal(){glNormal3sv(V);}
