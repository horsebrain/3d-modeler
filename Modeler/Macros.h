#ifndef __MACROS_H__
#define __MACROS_H__

#include <stdlib.h>					//for rand()

#define ABS(x) ((x)<0 ? (-(x)) : (x))
#define MAX(a,b) (((a)<(b))?(b):(a))
#define MAXABS(a,b) ((ABS(a)<ABS(b))?(b):(a))
//returns a random integer r such that low <= r <= hi
#define RANDINT(low,hi) ((rand() % ABS(((int)((hi)-(low)+1)))) + (low))

//returns a random float 0 <= RAND <= 1
#define RAND (((float)rand())/RAND_MAX)
//returns a random float x <= f <= y
#define RANDFLOAT(x,y) (RAND * ABS((y)-(x)) + x)

#define SDL_ASSERT(x) if(!(x)) \
			{ \
				Log.Write("%s: %s",#x,SDL_GetError()); \
				exit(1);\
			}

//#define USE_SDL_IMAGE

#define CASE(c,a) case c: a; break;

extern int SCREEN_WIDTH, SCREEN_HEIGHT;
			
typedef unsigned char byte;
typedef unsigned short word;
#endif
