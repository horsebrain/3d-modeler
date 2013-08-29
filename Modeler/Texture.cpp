#include "Texture.h"
#include "App_Log.h"

extern CApp_Log Log;

Texture::Texture() : texid(0) {}

Texture::~Texture()
{
	glDeleteTextures(1,&texid);
}


bool Texture::Load(const char * file, GLenum MinFilter, GLenum MagFilter,
					bool UseMipMaps, bool TileS, bool TileT)
{
#ifdef USE_SDL_IMAGE
	SDL_Surface * tmp;
	tmp = IMG_Load(file);
	bool OK = Load(tmp, MinFilter, MagFilter, UseMipMaps, TileS, TileT);
	SDL_FreeSurface(tmp);
	return OK;
#else
	wxInitAllImageHandlers();
	wxImage tmp;
	tmp.LoadFile(file);
	return Load(tmp,  MinFilter, MagFilter, UseMipMaps, TileS, TileT);
#endif
}

#ifdef USE_SDL_IMAGE
bool Texture::Load(SDL_Surface * data, GLenum MinFilter, GLenum MagFilter,
					bool UseMipMaps, bool TileS, bool TileT)
#else
bool Texture::Load(const wxImage & data, GLenum MinFilter, GLenum MagFilter,
				   bool UseMipMaps, bool TileS, bool TileT)
#endif
{
	bool HasAlphaChannel;
	unsigned char * ImgData;					//the raw image data, converted to RGB or RGBA
	int width,									//these are used later, when creating the texture
		height,
		Bpp;
#ifdef USE_SDL_IMAGE
	Log.Write("Loading Texture from SDL_Surface:");
	Log.Indent();
	if(data == NULL)
	{
		Log.Write("Can't load from a null pointer!");
		Log.Unindent();
		return false;
	}

	//read format/size information
	HasAlphaChannel = (data->format->BitsPerPixel == 32);
	width = data->w;
	height = data->h;
	Bpp = HasAlphaChannel ? 4 : 3;
	
	SDL_Surface * tmp;					//the image data as an RGB(A) SDL_Surface with pitch == width*BPP
	SDL_PixelFormat fmt;				//the format of the above surface
	
	fmt.palette = NULL;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	if(!HasAlphaChannel)
	{
		fmt.BitsPerPixel = 24;
		fmt.BytesPerPixel = 3;
	
		fmt.Rmask = 0x00FF0000;
		fmt.Gmask = 0x0000FF00;
		fmt.Bmask = 0x000000FF;
		fmt.Amask = 0;
	
		fmt.Rshift = 16;
		fmt.Gshift = 8;
		fmt.Bshift = 0;
		fmt.Ashift = 24;
	
		fmt.Rloss = fmt.Bloss = fmt.Gloss = 0;
		fmt.Aloss = 8;
	
		fmt.colorkey = 0;
		fmt.alpha = 0xFF;
	}
	else
	{
		fmt.BitsPerPixel = 32;
		fmt.BytesPerPixel = 4;

		fmt.Rmask = 0xFF000000;
		fmt.Gmask = 0x00FF0000;
		fmt.Bmask = 0x0000FF00;
		fmt.Amask = 0x000000FF;

		fmt.Rshift = 24;
		fmt.Gshift = 16;
		fmt.Bshift = 8;
		fmt.Ashift = 0;

		fmt.Rloss = fmt.Bloss = fmt.Gloss = fmt.Aloss = 0;

		fmt.colorkey = 0;
		fmt.alpha = 0xFF;
	}
#else
	if(!HasAlphaChannel)
	{
		fmt.BitsPerPixel = 24;
		fmt.BytesPerPixel = 3;
	
		fmt.Rmask = 0x000000FF;
		fmt.Gmask = 0x0000FF00;
		fmt.Bmask = 0x00FF0000;
		fmt.Amask = 0;
	
		fmt.Rshift = 0;
		fmt.Gshift = 8;
		fmt.Bshift = 16;
		fmt.Ashift = 0;
	
		fmt.Rloss = fmt.Bloss = fmt.Gloss = 0;
		fmt.Aloss = 8;
	
		fmt.colorkey = 0;
		fmt.alpha = 0xFF;
	}
	else
	{
		fmt.BitsPerPixel = 32;
		fmt.BytesPerPixel = 4;

		fmt.Rmask = 0x000000FF;
		fmt.Gmask = 0x0000FF00;
		fmt.Bmask = 0x00FF0000;
		fmt.Amask = 0xFF000000;

		fmt.Rshift = 0;
		fmt.Gshift = 8;
		fmt.Bshift = 16;
		fmt.Ashift = 24;

		fmt.Rloss = fmt.Bloss = fmt.Gloss = fmt.Aloss = 0;

		fmt.colorkey = 0;
		fmt.alpha = 0xFF;
	}
#endif			//SDL_BYTEORDER == SDL_BIG_ENDIAN

	tmp = SDL_ConvertSurface(data,&fmt,SDL_SWSURFACE);
	if(!tmp)
	{
		Log.Write("Unable to create temporary RGB surface!");
		Log.Unindent();
		return false;
	}
	//SDL_SaveBMP(tmp,"test.bmp");
	Log.Write("Succesfully converted to a %dx%dx%d %s format",tmp->w, tmp->h, fmt.BitsPerPixel, HasAlphaChannel ? "RGBA" : "RGB");
	//OGL stores textures upside-down, so we need to flip the SDL_Surface data
	ImgData = new GLubyte [tmp->w * tmp->h * Bpp];
	for(int i=0;i<tmp->h;i++)
	{
		memcpy(ImgData+(tmp->pitch * (tmp->h - i - 1)),((GLubyte*)tmp->pixels) + tmp->pitch * i,tmp->pitch);
	}
	Log.Write("Flipped OK!");
	SDL_FreeSurface(tmp);	

#else
	width = data.GetWidth();
	height = data.GetHeight();
	Bpp = 3;
	HasAlphaChannel = false;
	ImgData = new GLubyte [width * height * Bpp];
	for(int i=0;i<height;i++)
	{
		memcpy(ImgData+((Bpp*width) * (height - i - 1)),((GLubyte*)data.GetData()) + (Bpp*width) * i,(Bpp*width));
	}
	Log.Write("Flipped OK!");
#endif		//USE_SDL_IMAGE
	glDeleteTextures(1,&texid);		//In case Load() gets called twice
	glGenTextures(1,&texid);
	Bind();
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,MagFilter);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,MinFilter);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, 
 						TileS ? GL_REPEAT : GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,
 						TileT ? GL_REPEAT : GL_CLAMP);
	if(UseMipMaps)
	{
		Log.Write("Building MipMapped texture.");
		gluBuild2DMipmaps(GL_TEXTURE_2D, HasAlphaChannel ? 4 : 3,
							width, height, HasAlphaChannel ? GL_RGBA : GL_RGB,
							GL_UNSIGNED_BYTE, ImgData);
	}
	else
	{
		Log.Write("Building texture.");
		glTexImage2D(GL_TEXTURE_2D, 0, HasAlphaChannel ? 4 : 3,
						width, height, 0, HasAlphaChannel ? GL_RGBA : GL_RGB,
						GL_UNSIGNED_BYTE, ImgData);
	}
	delete [] ImgData;
	Log.Write("Success!");
	Log.Unindent();
 	return true;
}
