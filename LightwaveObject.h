// ----------------------------------------------------------------------------
// LightwaveObject contains the code needed to load and transform our object 
// from LWO2 format to our custom LightwaveObject format. It also holds all the 
// data associated with the object like vertices, polygons, and surfaces
// ----------------------------------------------------------------------------

#include <iostream>
//#include <conio.h>

#include "Texture.h"

// Lightwave SDK example is written in C
extern "C" {
	#include "lwo2.h"
}

using namespace std;

#ifndef LIGHTWAVEOBJECT_H
#define LIGHTWAVEOBJECT_H

// Vector classes
class LWVector3 {

public:
	float x, y, z;
};

class LWVector2 {

public:
	float x, y;
};

class LWVertex {

public:
	LWVector3 location;		// 3D location of vertex
	LWVector3 normal;		// Lighting normal
	int uvIndex;			// Texture coordinate index
};

class LWPolygon {

public:
	int vertexIndices[3];	// Index of component vertices
	LWVector3 normal;		// Polygon normal
	int surfaceIndex;		// Surface associated with polygon
};

class LWUVVertex {

public:
	LWVector2 coordinates;	// 2D location of vertex in texture
};

class LWVMap {

public:
	char name[255];
	int type;

	unsigned int size;		// Number of vertices in vertex map

	LWVector2 *vertices;	// Vertex array
};

class LWClip {

public:
	char sourceFile[255];	// Location of source image
	int index;				// Clip index
	int surfaceIndex;		// Index of surface associated with clip
	Texture tex;			// GL texture
};

class LWSurface {

public:
	bool vertexNormals;		// Vertex or polygon normals (smooth or flat shading)
	int index;
	char name[255];
	float color[4];
	float luminosity[4];
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float glossiness;

	int clipIndex;			// Index of clip associated with surface
	char uvMapName[255];	// Name of texture uv vertex map
	LWClip *surfaceMap;		// Pointer to clip image
	LWVMap *uvMap;			// Pointer to texture uv vertex map
};

class LightwaveObject {

public:
	LightwaveObject();
	virtual ~LightwaveObject();

	bool loadObject(char* filename);
	bool convertObject();
	void freeObject();

	char filepath[255];
	char filename[255];

	int numberOfVertices;
	int numberOfPolygons;
	int numberOfSurfaces;
	int numberOfVMaps;
	int numberOfClips;

	LWVertex*	vertices;
	LWPolygon*	polygons;
	LWSurface*	surfaces;
	LWClip*		clips;
	LWVMap*		vmaps;

private:
	lwObject *obj;
};

#endif
