#ifndef SSDL_H
#define SSDL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* SDL libs */
#include <SDL/SDL.h> // -lSDL
#include <SDL/SDL_image.h> // -lSDL_image
#include <SDL/SDL_ttf.h>  // -lSDL_ttf

#define RGB( ecran, a, b, c ) SDL_MapRGB(ecran->format, a, b, c)
	#define BLACK( ssdl ) RGB(ssdl->ecran, 0, 0, 0)
	#define RED( ssdl ) RGB(ssdl->ecran, 255, 0, 0)
	#define GREEN( ssdl ) RGB(ssdl->ecran, 0, 255, 0)
	#define BLUE( ssdl ) RGB(ssdl->ecran, 0, 0, 255)
	#define GREY( ssdl ) RGB(ssdl->ecran, 128, 128, 128 )
	#define WHITE( ssdl ) RGB(ssdl->ecran, 255, 255, 255)

typedef struct SSDL_Coord {
	int x, y;
} SSDL_Coord;

typedef struct SSDL {
	SDL_Surface **objects, *ecran;
	SSDL_Coord *pos;
	SDL_Rect rect;
	int nb_objects, width, height, bpp, flags, strokeWeight, ttf;
	Uint32 background, stroke, fill;
} SSDL;

/* SDL functions */
	SDL_Surface *init_Surface(int width, int height, int bpp, Uint32 flags);
	void set_Title(char *title);
	void set_Icon( char *path );
	void pause();
	void quit();
	SDL_Color u32_to_color( Uint32 i );
	Uint32 color_to_u32( SDL_Color c );
/* SDL */

/* SSDL functions */
	SSDL *SSDL_Init( int width, int height, int bpp, Uint32 flags );
	void SSDL_Add( SSDL *in, SDL_Surface *value, SSDL_Coord pos );
	void SSDL_AddSurface( SSDL *in, int x, int y, int width, int height, Uint32 color );
	void SSDL_AddImage( SSDL *in, int x, int y, char *path );
	void SSDL_AddText( SSDL *in, int x, int y, char *text, char *font, int size );
	void SSDL_Print( SSDL *in );
	void SSDL_SetColorKey( SSDL *in, int nPos, Uint32 color );
	void SSDL_SetAlpha( SSDL *in, int nPos, unsigned char value );
	void SSDL_Background( SSDL *in, Uint32 color );
	void SSDL_StrokeWeight( SSDL *in, int size );
	void SSDL_Stroke( SSDL *in, Uint32 color );
	void SSDL_Fill( SSDL *in, Uint32 color );
	void SSDL_Line(SSDL *in, int x0, int y0, int x2, int y2);
	void SSDL_Circle(SSDL *in, int x0, int y0, int radius);
	void SSDL_Free( SSDL *in );
	char SSDL_Wait4KeyboardPress();
	char SSDL_Wait4KeyboardRelease();
	void SSDL_EnableTTF( SSDL *in );
	void SSDL_Clear( SSDL *in );
/* SSDL */

#endif /* SSDL_H */
