// LightwaveObject.cpp: implementation of the LightwaveObject class.
//
//////////////////////////////////////////////////////////////////////

#include "LightwaveObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LightwaveObject::LightwaveObject() {

}

LightwaveObject::~LightwaveObject() {

}

bool LightwaveObject::loadObject(char *filename) {

	// Failure state variables for the lwo2 loader
	unsigned int failID;
	int failpos;

	obj = lwGetObject(filename, &failID, &failpos);

	if (obj == NULL) {
		printf("[Object Load Failed]\n");
		return false;
	} else {
		printf("[Object Loaded]\n");
		strncpy(this->filename, filename, 255);
	}

	// Convert the Lightwave Object to our custom format
	if (!convertObject()) {
		printf("[Object Conversion Failed]\n");
		lwFreeObject(obj);
		return false;
	} else {
		printf("[Object Converted To Internal Format]\n");
	}

	// We're done with the Lightwave Object, so free it
	lwFreeObject(obj);

	return true;
}

void LightwaveObject::freeObject() {

	// Delete our object
	delete [] vertices;
	delete [] polygons;
	delete [] surfaces;

	for (int i=0; i < numberOfVMaps; i++)
		delete (vmaps+i)->vertices;

	delete [] vmaps;
}

bool LightwaveObject::convertObject() {

	int totalVertices = 0;
	int totalPolygons = 0;
	int totalUVVertices = 0;
	int totalVMaps = 0;
	//int i,j;

	lwLayer *layer = obj->layer;

	// Count total vertices and polygons on all layers
	for (int i=0; i < obj->nlayers; i++) {

		totalVertices+= layer->point.count;
		totalPolygons += layer->polygon.count;
		totalVMaps += layer->nvmaps;

		// Move to next layer
		layer = layer->next;
	}

	printf("\tLoading:  \t"); printf(this->filename); printf("\n");
	printf("\tPoints:   \t%i\n", totalVertices);
	printf("\tPolygons:\t%i\n", totalPolygons);
	printf("\tSurfaces:\t%i\n", obj->nsurfs);

	// Update our object and allocate memory
	numberOfVertices = totalVertices;
	numberOfPolygons = totalPolygons;
	numberOfVMaps = totalVMaps;
	numberOfClips = obj->nclips;

	vertices = new LWVertex[totalVertices];
	polygons = new LWPolygon[totalPolygons];
	vmaps = new LWVMap[totalVMaps];

	// Read in vertices and polygons to object arrays
	int verticesIndex = 0, polygonsIndex = 0;
	layer = obj->layer;

	for (int i=0; i < obj->nlayers; i++) {

		int verticesLastLayer = verticesIndex;

		for (int j=0; j < layer->point.count; j++) {

			(vertices+verticesIndex)->location.x = (layer->point.pt+j)->pos[0];
			(vertices+verticesIndex)->location.y = (layer->point.pt+j)->pos[1];
			(vertices+verticesIndex)->location.z = (layer->point.pt+j)->pos[2];

			verticesIndex++;
		}

		for (int j=0; j < layer->polygon.count; j++) {

			LWPolygon *currentPolygon = (polygons+polygonsIndex);

			// For simplicity, we're only allowing triangles
			if ((layer->polygon.pol+j)->nverts != 3) {
				printf("\t[Object polys must be triangles]\n");
				return false;
			}

			currentPolygon->vertexIndices[0] = (layer->polygon.pol+j)->v[0].index;
			currentPolygon->vertexIndices[1] = (layer->polygon.pol+j)->v[1].index;
			currentPolygon->vertexIndices[2] = (layer->polygon.pol+j)->v[2].index;

			if (i > 0) {
				currentPolygon->vertexIndices[0] += verticesLastLayer;
				currentPolygon->vertexIndices[1] += verticesLastLayer;
				currentPolygon->vertexIndices[2] += verticesLastLayer;
			}

			// Figure out which surface the polygon belongs to
			lwSurface *surface = obj->surf;
			int surfaceIndex = 0;

			while (surface != NULL && (layer->polygon.pol+j)->surf != surface) {
				surface = surface->next;
				surfaceIndex++;
			}

			currentPolygon->surfaceIndex = surfaceIndex;

			bool vertexNormals = (surface->smooth > 0);

			if (vertexNormals) {

				for (int k=0; k < (layer->polygon.pol+j)->nverts; k++) {

					int vertexOffset = (((layer->polygon.pol+j)->v)+k)->index;

						(vertices+vertexOffset)->normal.x = ((layer->polygon.pol+j)->v+k)->norm[0];
						(vertices+vertexOffset)->normal.y = ((layer->polygon.pol+j)->v+k)->norm[1];
						(vertices+vertexOffset)->normal.z = ((layer->polygon.pol+j)->v+k)->norm[2];
				}

			} else {
				currentPolygon->normal.x = (layer->polygon.pol+j)->v[0].norm[0];
				currentPolygon->normal.y = (layer->polygon.pol+j)->v[0].norm[1];
				currentPolygon->normal.z = (layer->polygon.pol+j)->v[0].norm[2];
			}

			polygonsIndex++;
		}

		lwVMap *vmap = layer->vmap;

		// Are there any VMaps in this layer? For UV coordinates
		for (int i=0; i < layer->nvmaps; i++) {

			LWVMap *currentVMap = vmaps+i;

			// We'll only handle 2 dimensional vmaps
			if (vmap->dim != 2) continue;

			strcpy(currentVMap->name, vmap->name);
			currentVMap->size = vmap->nverts;
			
			currentVMap->vertices = new LWVector2[currentVMap->size];

			LWVector2 *vertex = currentVMap->vertices;

			for (int j=0; j < vmap->nverts; j++) {
				
				(vertices+*(vmap->vindex+j))->uvIndex = j;

				float *values = *(vmap->val+j);

				vertex->x = *(values);
				vertex->y = *(values+1);
				//printf("Tex Coord[%f,%f]\n",vertex->x,1-vertex->y);

				// Next vertex
				vertex++;
			}

			// Next VMap
			vmap = vmap->next;
		}

		// Next layer
		layer = layer->next;
	}

	// Allocate memory for and read in surfaces
	surfaces = new LWSurface[obj->nsurfs];

	LWSurface *currentSurface = surfaces;
	lwSurface *surface = obj->surf;

	for (int i=0; i < obj->nsurfs; i++) {

		currentSurface->clipIndex = -1; // Undefined

		//std::cout << "Surface: " << surface->name << "\n";

		strncpy(currentSurface->name, surface->name, 255);

		for (int j=0; j < 3; j++) {
			currentSurface->color[j] = surface->color.rgb[j];
			currentSurface->ambient[j] = (surface->diffuse.val / 3) * currentSurface->color[j];
			currentSurface->diffuse[j] = surface->diffuse.val * currentSurface->color[j];
			currentSurface->specular[j] = surface->specularity.val * currentSurface->color[j];
			currentSurface->luminosity[j] = surface->luminosity.val * currentSurface->color[j];
		}

		// Copy surface colors/parameters
		currentSurface->color[3] = surface->transparency.val.val;
		currentSurface->diffuse[3] = surface->diffuse.val;
		currentSurface->ambient[3] = surface->diffuse.val;
		currentSurface->specular[3] = surface->specularity.val;
		currentSurface->luminosity[3] = surface->luminosity.val;

		currentSurface->glossiness = surface->glossiness.val;
	
		if (surface->color.tex != NULL) {
			currentSurface->clipIndex = surface->color.tex->param.imap.cindex;
			strncpy(currentSurface->uvMapName, surface->color.tex->param.imap.vmap_name, 255);
		}

		currentSurface->vertexNormals = (surface->smooth > 0);

		surface = surface->next;
		currentSurface++;
	}

	// Allocate memory for and read in clips (images)
	clips = new LWClip[obj->nclips];

	LWClip *currentClip = clips;
	lwClip *clip = obj->clip;

	for (int i=0; i < obj->nclips; i++) {

		strncpy(currentClip->sourceFile, clip->source.still.name, 255);
		if(!currentClip->tex.Load(strrchr(currentClip->sourceFile,'/') + 1)) 
		{
			printf("[Texture Load Failed: "); printf(currentClip->sourceFile); printf("]\n");
			return false;
		}
		
		currentClip->index = clip->index;

		clip = clip->next;
		currentClip++;
	}

	// Reading done
	// Link necessary structures together

	// Link surfaces to clips
	for (int i=0; i < obj->nsurfs; i++)
	{
		for (int j=0; j < numberOfClips; j++)

			// If the surface's clip index equals the clip index, point the surface to the clip
			if ((surfaces+i)->clipIndex == (clips+j)->index) {

				(surfaces+i)->surfaceMap = (clips+j);
				j = numberOfClips;
			}
	}

	// Link surfaces to vmaps
	for (int i=0; i < obj->nsurfs; i++)
		for (int j=0; j < numberOfVMaps; j++)

			// If the vmap's name equals the surface's uv map's name, point the surface to the vmap
			if (strcmp((vmaps+j)->name, (surfaces+i)->uvMapName) == 0) {

				(surfaces+i)->uvMap = (vmaps+j);
				j = numberOfVMaps;
			}

	return true;
}

