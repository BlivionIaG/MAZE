#include "ssdl.h"

SDL_Surface *init_Surface( int width, int height, int bpp, Uint32 flags ){
    SDL_Surface *out = NULL;
    if( SDL_Init( SDL_INIT_VIDEO == -1 ) ){
        fprintf( stderr, "Erreur init SDL : %s\n", SDL_GetError( ) );
        exit( EXIT_FAILURE );
    }
    
    out = SDL_SetVideoMode( width, height, bpp, flags );
    if( out == NULL ){
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError( ) );
        exit( EXIT_FAILURE );
    }
    
    return out;
}

void set_Title( char *title ){
    SDL_WM_SetCaption( title, NULL );
}

void set_Icon( char *path ){
	SDL_WM_SetIcon( IMG_Load( path ), NULL );
}

void pause( ){
    SDL_Event event;
    int continuer = 1; 
    while( continuer ){
        SDL_WaitEvent( &event );
        switch( event.type ){
            case SDL_QUIT:
                continuer = 0;
                break;
        }
    }
}

char SSDL_Wait4KeyboardPress( ){
	SDL_Event event;
	int continuer = 1; 
	while( continuer ){
		SDL_WaitEvent( &event );
		switch( event.type ){
			case SDL_KEYDOWN:
				return event.key.keysym.sym;
			break;
		}
	}
	
	return 0;
}

char SSDL_Wait4KeyboardRelease( ){
	SDL_Event event;
	int continuer = 1; 
	while( continuer ){
		SDL_WaitEvent( &event );
		switch( event.type ){
			case SDL_KEYUP:
				return event.key.keysym.sym;
			break;
		}
	}
	
	return 0;
}

void quit( ){
    SDL_Quit( );
}

SSDL *SSDL_Init( int width, int height, int bpp, Uint32 flags ){
	SSDL *out = malloc( sizeof(SSDL) );
	out->objects = NULL;
	out->pos = NULL;
	out->rect.x = 0;
	out->rect.y = 0;
	out->nb_objects = 0;
	
	out->ecran = init_Surface( width, height, bpp, flags );
	out->width = width;
	out->height = height;
	out->bpp = bpp;
	out->flags = flags;
	out->strokeWeight = 1;
	out->stroke = 0;
	out->fill = 0;
	out->background = 0;
	out->ttf = 0;
	
	return out;
}

void SSDL_Add( SSDL *in, SDL_Surface *value, SSDL_Coord pos ){
	in->nb_objects++;
	in->objects = realloc( in->objects, sizeof(SDL_Surface *)*(in->nb_objects) );
	in->objects[in->nb_objects-1] = value;
	in->pos[in->nb_objects-1] = pos;
}

void SSDL_AddSurface( SSDL *in, int x, int y, int width, int height, Uint32 color ){
	SDL_Surface *value = SDL_CreateRGBSurface( SDL_HWSURFACE, width, height, 32, 0, 0, 0, 0 );
	SDL_FillRect( value, NULL, color );
	SSDL_Coord pos;
	pos.x = x;
	pos.y = y;
	
	in->nb_objects++;
	in->objects = realloc( in->objects, sizeof(SDL_Surface *)*(in->nb_objects) );
	in->objects[in->nb_objects-1] = value;
	in->pos = realloc( in->pos, sizeof(SSDL_Coord)*(in->nb_objects) );
	in->pos[in->nb_objects-1] = pos;
}

void SSDL_AddText( SSDL *in, int x, int y, char *text, char *font, int size ){
	if( !in->ttf ){
		printf( "TTF not enabled, skipped\n" );
	}else{
		TTF_Font *ttfont = TTF_OpenFont( font, size );
		SDL_Surface *value = TTF_RenderUTF8_Blended( ttfont, text, u32_to_color(in->stroke) );	
		SSDL_Coord pos;
		pos.x = x;
		pos.y = y;
		
		in->nb_objects++;
		in->objects = realloc( in->objects, sizeof(SDL_Surface *)*(in->nb_objects) );
		in->objects[in->nb_objects-1] = value;
		in->pos = realloc( in->pos, sizeof(SSDL_Coord)*(in->nb_objects) );
		in->pos[in->nb_objects-1] = pos;
		
		TTF_CloseFont( ttfont );
	}
	
}

void SSDL_Print( SSDL *in ){
	//printf( "SSDL Print Event : " );
	int i;
	
	SDL_FillRect( in->ecran, NULL, in->background );
	//SDL_Flip( in->ecran );
	for( i = 0; i < in->nb_objects; i++ ){
		in->rect.x = in->pos[i].x;
		in->rect.y = in->pos[i].y;
		SDL_BlitSurface( in->objects[i], NULL, in->ecran, &(in->rect) );
	}

	SDL_Flip( in->ecran );
	//printf( "%d objects\n", i );
}

void SSDL_AddImage( SSDL *in, int x, int y, char *path ){
	SDL_Surface *value = IMG_Load( path );
	SSDL_Coord pos;
	pos.x = x;
	pos.y = y;
	
	in->nb_objects++;
	in->objects = realloc( in->objects, sizeof(SDL_Surface *)*(in->nb_objects) );
	in->objects[in->nb_objects-1] = value;
	in->pos = realloc( in->pos, sizeof(SSDL_Coord)*(in->nb_objects) );
	in->pos[in->nb_objects-1] = pos;
}

void SSDL_SetColorKey( SSDL *in, int nPos, Uint32 color ){
	printf( "SSDL Chroma Set Event : Object n°%d\n", nPos );
	SDL_SetColorKey( in->objects[nPos], SDL_SRCCOLORKEY, color );
}

void SSDL_SetAlpha( SSDL *in, int nPos, unsigned char value ){
	printf( "SSDL Set Transparency Event : Object n°%d, Strength = %d\n", nPos, value );
	SDL_SetAlpha( in->objects[nPos], SDL_SRCALPHA, value );
}

void SSDL_Background( SSDL *in, Uint32 color ){
	SDL_FillRect( in->ecran, NULL, color );
	in->background = color;
	SDL_Flip( in->ecran );
}

void SSDL_Stroke( SSDL *in, Uint32 color ){
	in->stroke = color;
}

void SSDL_Fill( SSDL *in, Uint32 color ){
	in->fill = color;
}

void SSDL_StrokeWeight( SSDL *in, int size ){
	in->strokeWeight = size;
}

void SSDL_Line( SSDL *in, int x0, int y0, int x2, int y2 ) {
	
	int x1 = x0, y1 = y0;
	int dx, dy;
	int e; // valeur d’erreur
	e = x2 - x1 ;        // -e(0,1)
	dx = e * 2 ;          // -e(0,1)
	dy = ( y2 - y1 ) * 2 ;  // e(1,0)
	while( x1 <= x2 ){
		SSDL_AddSurface( in, x1, y1, in->strokeWeight, in->strokeWeight, in->stroke );
		x1++;  // colonne du pixel suivant
		if( ( e -= dy ) <= 0 ) {  // erreur pour le pixel suivant de même rangée
			y1++;  // choisir plutôt le pixel suivant dans la rangée supérieure
			e += dx ;  // ajuste l’erreur commise dans cette nouvelle rangée
		}
	}
	SSDL_AddSurface( in, x2, y2, in->strokeWeight, in->strokeWeight, in->stroke );
}

void SSDL_Circle( SSDL *in, int x0, int y0, int radius ){
    int x = radius;
    int y = 0;
    int e = 0;

    while ( x >= y ){
    	SSDL_AddSurface( in, x0 + x, y0 + y, in->strokeWeight, in->strokeWeight, in->stroke );
        SSDL_AddSurface( in, x0 + x, y0 + y, in->strokeWeight, in->strokeWeight, in->stroke );
        SSDL_AddSurface( in, x0 + y, y0 + x, in->strokeWeight, in->strokeWeight, in->stroke );
        SSDL_AddSurface( in, x0 - y, y0 + x, in->strokeWeight, in->strokeWeight, in->stroke );
        SSDL_AddSurface( in, x0 - x, y0 + y, in->strokeWeight, in->strokeWeight, in->stroke );
        SSDL_AddSurface( in, x0 - x, y0 - y, in->strokeWeight, in->strokeWeight, in->stroke );
        SSDL_AddSurface( in, x0 - y, y0 - x, in->strokeWeight, in->strokeWeight, in->stroke );
        SSDL_AddSurface( in, x0 + y, y0 - x, in->strokeWeight, in->strokeWeight, in->stroke );
        SSDL_AddSurface( in, x0 + x, y0 - y, in->strokeWeight, in->strokeWeight, in->stroke );

        if ( e <= 0 ){
            y += 1;
            e += 2 * y + 1;
        }
        if ( e > 0 ){
            x -= 1;
            e -= 2 * x + 1;
        }
    }
}

void SSDL_Free( SSDL *in ){
	if( in->ecran != NULL ){
		SDL_FreeSurface( in->ecran );
	}
	for( int i = 0; i < in->nb_objects; i++ ){
		if( in->objects[i] != NULL ){
			SDL_FreeSurface( in->objects[i] );
		}
	}
	if( in->objects != NULL ){
		free( in->objects );
	}
	if( in->pos != NULL ){
		free( in->pos );
	}
	if( in->ttf ){
		TTF_Quit( );
	}
	
	free( in );
}

void SSDL_Clear( SSDL *in ){
	for( int i = 0; i < in->nb_objects; i++ ){
		if( in->objects[i] != NULL ){
			SDL_FreeSurface( in->objects[i] );
		}
	}
	
	in->nb_objects = 0;
}

void SSDL_EnableTTF( SSDL *in ){
	TTF_Init( );
	in->ttf = 1;
}

SDL_Color u32_to_color( Uint32 i ){
	SDL_Color c;
	c.r = i >> 16;
	c.g = i >> 8;
	c.b = i;
	
	return c;
}

Uint32 color_to_u32( SDL_Color c ){
	return  ( c.r << 16 ) | ( c.g << 8 ) | c.b;
}
