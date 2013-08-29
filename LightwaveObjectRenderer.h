// ----------------------------------------------------------------------------
// LightwaveObjectRenderer contains the code needed to render our 
// LightwaveObject models in OpenGL. It also contains state variables for the 
// different viewing modes.
// ----------------------------------------------------------------------------

//#include	<windows.h>
#include	<OpenGL/gl.h>
#include	<OpenGL/glu.h>
#include	"LightwaveObject.h"

#ifndef LIGHTWAVEOBJECTRENDERER_H
#define LIGHTWAVEOBJECTRENDERER_H

class LightwaveObject;

class LightwaveObjectRenderer {

public:
	LightwaveObjectRenderer(LightwaveObject *object = NULL);
	virtual ~LightwaveObjectRenderer() {};

	void setObject(LightwaveObject *object) { this->object = object; };
	void setScaleVariable(float *scaler) { this->scaler = scaler; };

	void setRenderTriangles() { renderType = GL_TRIANGLES; };
	void setRenderWireframe() { renderType = GL_LINE_LOOP; };

	void draw();

	// Pointer to the object we will render
	LightwaveObject *object;

	// We have to include a way to indepentantly scale the lighting normals 
	// because OpenGL doesn't do that for us.
	float *scaler;
	static float defaultScale;

	// Type of rendering, can be wireframe or solid
	int renderType;
};

#endif
