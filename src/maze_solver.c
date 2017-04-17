#include "lymbe.h"

int main( int argc, char **argv ){
	if( argc != 2 ) return -1; //If no file specified

	FILE *labyrinthe = fopen( argv[1], "r" ); //Open maze file
	if( labyrinthe == NULL ) { //If failed, exit
		printf( "ERREUR donne un fichier qui existe !!!\n" );
		return -2;
	}

	char *taille = malloc( ( MAX_VAL + 1 )*sizeof(char) ); 
	int largeur = 0;

	if( !fgets( taille, MAX_VAL, labyrinthe ) ){
		printf( "ERREUR le fichier est invalide !\n" );
		return -3;
	}else{
		largeur = atoi( taille );
		printf( "largeur:%d\n", largeur );
		free( taille );
		taille = malloc( ( largeur + 1 )*sizeof(char) );
	}

	char **terrain = NULL;
	int terrainLength = 0;

	while( fgets( taille, largeur + 2, labyrinthe ) ){
		taille[strlen( taille )] = 0;

		terrainLength++;
		terrain = realloc( terrain, terrainLength*sizeof(char *) );
		terrain[terrainLength-1] = malloc( ( largeur + 1 )*sizeof(char) );
		strcpy( terrain[terrainLength-1], taille );
	}
	printf( "terrainLength:%d\n", terrainLength );

	int lw = BLOCK_SIZE, lh = BLOCK_SIZE;
	int tw = largeur*BLOCK_SIZE, th = terrainLength*BLOCK_SIZE;

	SSDL *window = SSDL_Init( tw, th, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE );
	SSDL_Background( window, RGB( window->ecran, 192, 192, 192 ) );
	set_Title( "Labyrinthe" );

	bot *boy = init_bot(
		findFirstCharX( terrain, terrainLength, '2' ),
		findFirstCharY( terrain, terrainLength, '2' ),
		lw,
		lh,
		RGB( window->ecran, 255, 255, 0 ),
		RGB( window->ecran, 0, 255, 255 ),
		RGB( window->ecran, 255, 0, 255 )
	);

	Uint32 last_time = SDL_GetTicks( );
	Uint32 current_time,ellapsed_time;
	Uint32 start_time;
	SDL_Event event;

	draw_map( window, terrain, largeur, terrainLength, lw, lh );
	draw_bot( window, boy );
	SSDL_Print( window );

	while( 's' != SSDL_Wait4KeyboardPress( ) );

	while( !boy->finished ){
		start_time = SDL_GetTicks( );
		/* I/O */
		while( SDL_PollEvent( &event ) ){
			switch( event.type ){
				case SDL_QUIT:
					SSDL_Free( window );
					quit( );
					exit( 0 );
				break;
				case SDL_KEYDOWN:
					switch( event.key.keysym.sym ){
						case 'q':
							SSDL_Free( window );
							quit( );
							exit( 0 );
						break;
						case 'p':
							boy->wSize++;
							boy->hSize++;
							lw++;
							lh++;
						break;
						case 'm':
							if( boy->wSize > 0 ) boy->wSize--;
							if( boy->hSize > 0 ) boy->hSize--;
							if( lw > 0 ) lw--;
							if( lh > 0 ) lh--;
						break;
						default:
						break;
					}
				break;
			}
		}
		current_time = SDL_GetTicks( );
		ellapsed_time = current_time - last_time;
		last_time = current_time;

		/* DRAW */
		SSDL_Clear(window);
		draw_map(window, terrain, largeur, terrainLength, lw, lh );
		draw_bot(window, boy);
		move_bot(terrain, terrainLength, boy);
		SSDL_Print(window);

		/* END of Loop */
		ellapsed_time = SDL_GetTicks() - start_time;
		if (ellapsed_time < PRGM_DELAY){
			SDL_Delay(PRGM_DELAY - ellapsed_time);
		}
	}

	printf("PATH FOUNDED !!!\n");
	SSDL_Clear(window);
	draw_map(window, terrain, largeur, terrainLength, lw, lh );
	draw_bot(window, boy);
	SSDL_Print(window);
	
	pause();
	
	SSDL_Free(window);
	quit();

	return 0;
}

