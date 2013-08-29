#ifndef __TEXTURE_H__
#define __TEXTURE_H__
#include "macros.h"

#ifdef USE_SDL_IMAGE
	#include "SDL.h"
	#if defined(__APPLE__) && defined(__MACH__)
	#include "SDL_image/SDL_Image.h"
	#else
	#include "SDL_Image.h"
	#endif
	#include "SDL_opengl.h"
#else		//if we're not using SDL, use wxWindows
	#include <wx/wx.h>
	#include <wx/image.h>
	#include "ogl.h"
#endif


class Texture
{
	private:
		GLuint texid;
	
	public:
		Texture();
		~Texture();
		
		bool Load(const char * file, GLenum MinFilter = GL_NEAREST, 
    				GLenum MagFilter = GL_NEAREST, bool UseMipMaps = false,
   					bool TileS = true, bool TileT = true);
#ifdef USE_SDL_IMAGE
		bool Load(SDL_Surface * data, GLenum MinFilter = GL_NEAREST, 
  					GLenum MagFilter = GL_NEAREST, bool UseMipMaps = false,
       				bool TileS = true, bool TileT = true);
#else
		bool Load(const wxImage & data, GLenum MinFilter = GL_NEAREST,
			GLenum MagFilter = GL_NEAREST, bool UseMipMaps = false,
			bool TileS = true, bool TileT = true);
#endif
		
		
		void Bind() {glBindTexture(GL_TEXTURE_2D, texid);}
};


#endif	//__TEXTURE_H__
