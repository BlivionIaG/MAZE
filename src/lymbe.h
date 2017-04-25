#ifndef LYMBE_H
#define LYMBE_H

#include "ssdl.h" 
#define MAX_VAL 8192
#define PRGM_DELAY 0
#define BLOCK_SIZE 8

/* Struct for coordinates */
typedef struct bot_coords {
	int x, y;
} bot_coords;

/* Struct for bot memory */
typedef struct bot_memory {
	int *x, *y;
	int *left, *right, *up, *down;
	int length;
} bot_memory;

/* Struct for a bot */
typedef struct bot {
	int x, y, wSize, hSize;
	int left, right, up, down;
	bot_memory *nodes;
	bot_coords *memory;  // memory stack
	int length; // memory stack length
	Uint32 color, oldColor, nodeColor;

	int finished; //If bot is at the end of the maze
} bot;

/* Initialize a bot */ 
bot *init_bot( int x, int y, int wSize, int hSize, Uint32 color, Uint32 nc, Uint32 oc );
/* Initialize a bot memory */
bot_memory *init_bot_memory( );

/* Push current coordinates to memory stack */
void bot_memory_push( bot *in );
/* Pop last coordinates from memory stack to current coordinates */
void bot_memory_pop( bot *in );

void bot_nodes_push( bot *in );
int already_been_there( bot *in, int x, int y );
int position_in_bot_nodes( bot_memory *nodes, int x, int y );
void draw_bot( SSDL *in, bot *obj );
void draw_bestPath( SSDL *in, bot *obj );
void draw_map( SSDL *in, char **map, int mapWidth, int mapHeight, int wSize, int hSize );
void move_bot( char **map, int mapLength, bot *in );

int findFirstCharX( char **in, int inLength, char iamtheone );
int findFirstCharY( char **in, int inLength, char iamtheone );

#endif /* LYMBE_H */
