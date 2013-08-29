// LightwaveObjectRenderer.cpp: implementation of the LightwaveObjectRenderer class.
//
//////////////////////////////////////////////////////////////////////

#include "LightwaveObjectRenderer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

float LightwaveObjectRenderer::defaultScale = 1;
const float white[] = {1.0, 1.0, 1.0, 1.0};
const float black[] = {0.0, 0.0, 0.0};

LightwaveObjectRenderer::LightwaveObjectRenderer(LightwaveObject *object) {

	this->object = object;
	scaler = &defaultScale;

	// Set view mode
	this->setRenderTriangles();
}

void LightwaveObjectRenderer::draw() {
	// Make sure an object has been set
	if (object == NULL) return;

	bool drawSurface = false;
	float normalScaler = *scaler;

	LWSurface *oldSurface = NULL;

	for (int i=0; i < object->numberOfPolygons; i++) {

		LWPolygon *currentPolygon = object->polygons + i;
		LWSurface *currentSurface = object->surfaces+currentPolygon->surfaceIndex;
		
		// We only want to change surfaces if the surface associated with the current polygon has changed
		if (currentSurface != oldSurface) {

			glMaterialfv(GL_FRONT, GL_DIFFUSE, currentSurface->diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, currentSurface->specular);
			glMaterialfv(GL_FRONT, GL_SHININESS, &currentSurface->glossiness);

			// If surface has a clip texture associated, bind it
			if (currentSurface->clipIndex != -1) {

				glEnable(GL_TEXTURE_2D);
				currentSurface->surfaceMap->tex.Bind();
				drawSurface = true;

				// Make sure the texture color is solid white, so only the texture color will appear
				glColor4fv(white);
			} else {
				glDisable(GL_TEXTURE_2D);
				glColor4fv(currentSurface->color);
				drawSurface = false;
			}

			oldSurface = currentSurface;
		}

		// If we're using polygon normals for this surface, set it 
		if (!currentSurface->vertexNormals)
			glNormal3f(currentPolygon->normal.x * normalScaler, currentPolygon->normal.y * normalScaler, currentPolygon->normal.z * normalScaler);

		glBegin(renderType);

		// Loop through the points on this polygon
		for (int j=0; j < 3; j++) {
			
			// Access the current vertex
			LWVertex *currentVertex = object->vertices + currentPolygon->vertexIndices[j];

			// If we're using vertex normals for this surface, set them
			if (currentSurface->vertexNormals) 
				glNormal3f(currentVertex->normal.x * normalScaler, currentVertex->normal.y * normalScaler, currentVertex->normal.z * normalScaler);

			// Is there a texture being drawn?
			if (drawSurface) {

				// Get the UV coordinate vector
				LWVector2 *uv = currentSurface->uvMap->vertices + currentVertex->uvIndex;

				// Set the GL coordinates (y-axis is inverted in the Lightwave VMap)
				glTexCoord2f(uv->x, (1-uv->y));
			}
			
			// Set the vertex location
			glVertex3f(currentVertex->location.x, currentVertex->location.y, currentVertex->location.z);
		}

		glEnd();
	}
}
