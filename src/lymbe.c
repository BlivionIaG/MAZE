#include "lymbe.h"

/* Initialisation */

bot *init_bot( int x, int y, int wSize, int hSize, Uint32 color, Uint32 nc, Uint32 oc ){
	bot *out = malloc( sizeof(bot) );
	out->x = x;
	out->y = y;
	out->wSize = wSize;
	out->hSize = hSize;
	out->memory = NULL;
	out->length = 0;
	out->nodes = init_bot_memory( );
	out->color = color;
	out->nodeColor = nc;
	out->oldColor = oc;	
	out->left = 0;
	out->right = 0;
	out->up = 0;
	out->down = 0;
	out->finished = 0;

	return out;
}

bot_memory *init_bot_memory( ){
	bot_memory *out = malloc( sizeof(bot_memory) );
	out->x = NULL;
	out->y = NULL;
	out->left = NULL;
	out->right = NULL;
	out->up = NULL;
	out->down = NULL;
	out->length = 0;

	return out;
}

/* Memory Management */

void bot_memory_push( bot *in ){
	in->length++;
	in->memory = realloc( in->memory, (in->length)*sizeof(bot_coords) );
	
	bot_coords *tmp = in->memory;
	tmp[in->length-1].x = in->x;
	tmp[in->length-1].y = in->y;
}

void bot_memory_pop( bot *in ){
	if( in->length > 0 ){
		bot_coords *tmp = in->memory;
		in->x = tmp[in->length-1].x;
		in->y = tmp[in->length-1].y;

		in->length--;
		in->memory = realloc( in->memory, (in->length)*sizeof(bot_coords) );		
	}
}

void bot_nodes_push( bot *in ){
	bot_memory *tmp = in->nodes;
	int pos = position_in_bot_nodes( tmp, in->x, in->y );

	if( pos < 0 ){
		tmp->length++;
		tmp->x = realloc( tmp->x, tmp->length*sizeof(int) );
		tmp->y = realloc( tmp->y, tmp->length*sizeof(int) );
		tmp->left = realloc( tmp->left, tmp->length*sizeof(int) );
		tmp->right = realloc( tmp->right, tmp->length*sizeof(int) );
		tmp->up = realloc( tmp->up, tmp->length*sizeof(int) );
		tmp->down = realloc( tmp->down, tmp->length*sizeof(int) );

		tmp->x[tmp->length-1] = in->x;
		tmp->y[tmp->length-1] = in->y;
		tmp->left[tmp->length-1] = in->left;
		tmp->right[tmp->length-1] = in->right;
		tmp->up[tmp->length-1] = in->up;
		tmp->down[tmp->length-1] = in->down;
	}else{
		tmp->left[pos] = in->left;
		tmp->right[pos] = in->right;
		tmp->up[pos] = in->up;
		tmp->down[pos] = in->down;
	}
}

int position_in_bot_nodes( bot_memory *nodes, int x, int y ){
	if( nodes->length > 0 ){
		for( int i = 0; i < nodes->length; i++ ){
			if( x == nodes->x[i] && y == nodes->y[i] ){
				return i;
			}
		}
	}

	return -1;
}

/* Bot & Map view */
 
void draw_bot( SSDL *in, bot *obj ){
	bot_memory *tmp = obj->nodes;
	for( int i = 0; i < tmp->length; i++ ){
		if( tmp->left[i] + tmp->right[i] + tmp->up[i] + tmp->down[i] > 0 ){
			SSDL_AddSurface( in, tmp->x[i]*obj->wSize, tmp->y[i]*obj->hSize, obj->wSize, obj->hSize, obj->nodeColor );
		}else{
			SSDL_AddSurface( in, tmp->x[i]*obj->wSize, tmp->y[i]*obj->hSize, obj->wSize, obj->hSize, obj->oldColor );
		}
	}
	SSDL_AddSurface( in, obj->x*obj->wSize, obj->y*obj->hSize, obj->wSize, obj->hSize, obj->color );
}

void draw_bestPath( SSDL *in, bot *obj ){
	for( int i = 0; i < obj->length; i++ ){
			SSDL_AddSurface( in, obj->memory[i].x*obj->wSize, obj->memory[i].y*obj->hSize, obj->wSize, obj->hSize, RED( in ) );
	}
	SSDL_AddSurface( in, obj->x*obj->wSize, obj->y*obj->hSize, obj->wSize, obj->hSize, obj->color );
}

void draw_map( SSDL *in, char **map, int mapWidth, int mapHeight, int wSize, int hSize ){
	Uint32 color = 0;
	int i, j;

	for( i = 0; i < mapHeight; i++ ){
			for( j = 0; j < mapWidth; j++ ){
				switch( map[i][j] ){
					case '0':
						color = BLACK( in );
					break;
					case '1':		
						color = WHITE( in );
					break;
					case '2':
						color = RED( in );
					break;
					case '3':
						color = GREEN( in );
					break;
				}
			SSDL_AddSurface( in, j*wSize, i*hSize, wSize, hSize, color );
		}
	}
}

/* Map Management */

int findFirstCharX( char **in, int inLength, char iamtheone ){
	int limit = strlen( in[0] );

	for( int i = 0; i < inLength; i++ ){
		for( int j = 0; j < limit; j++ ){
			if( in[i][j] == iamtheone ) return j;
		}
	}

	return -1;
}

int findFirstCharY( char **in, int inLength, char iamtheone ){
	int limit = strlen( in[0] );

	for( int i = 0; i < inLength; i++ ){
		for( int j = 0; j < limit; j++ ){
			if( in[i][j] == iamtheone ) return i;
		}
	}

	return -1;
}

void move_bot( char **map, int mapLength, bot *in ){
	int pos;
	in->left = 0;
	in->right = 0;
	in->up = 0;
	in->down = 0;

	/* Look for free cases */
	if(in->x - 1 > 0 && map[in->y][in->x-1] == '1' && !already_been_there( in, in->x - 1, in->y ) ) in->left = 1;
	if(in->x + 1 < strlen( map[0] ) && map[in->y][in->x+1] == '1' && !already_been_there( in, in->x + 1, in->y ) ) in->right = 1;
	if(in->y - 1 > 0 && map[in->y-1][in->x] == '1' && !already_been_there( in, in->x, in->y - 1 ) ) in->up = 1;
	if(in->y + 1 < mapLength && map[in->y+1][in->x] == '1' && !already_been_there( in, in->x, in->y+1 ) ) in->down = 1;

	/* Look for older choices */
	pos = position_in_bot_nodes( in->nodes, in->x, in->y );
	if( pos >= 0 ){
		bot_memory *tmp = in->nodes;
		in->left = tmp->left[pos];
		in->right = tmp->right[pos];
		in->up = tmp->up[pos];
		in->down = tmp->down[pos];
	}



	/* Look for the end */
	if( in->x - 1 > 0 && map[in->y][in->x-1] == '3' ){
		in->left = 0;
		printf( "LEFT\n" );
		bot_nodes_push( in );
		bot_memory_push( in );
		in->x--;
		in->finished = 1;
	}else if( in->x + 1 < strlen( map[0] ) && map[in->y][in->x+1] == '3' ) {
		in->right = 0;
		printf( "RIGHT\n" );
		bot_nodes_push( in );
		bot_memory_push( in );
		in->x++;
		in->finished = 1;
	}else if( in->y - 1 > 0 && map[in->y-1][in->x] == '3' ){
		in->up = 0;
		printf( "UP\n" );
		bot_nodes_push( in );
		bot_memory_push( in );
		in->y--;
		in->finished = 1;
	}else if( in->y + 1 < mapLength && map[in->y+1][in->x] == '3' ){
		in->down = 0;
		printf( "DOWN\n" );
		bot_nodes_push( in );
		bot_memory_push( in );
		in->y++;
		in->finished = 1;
	}else{
		/* MOVE */
		if( in->left ){
			in->left = 0;
			printf( "LEFT\n" );
			bot_nodes_push( in );
			bot_memory_push( in );
			in->x--;
		}else if( in->right ){
			in->right = 0;
			printf( "RIGHT\n" );
			bot_nodes_push( in );
			bot_memory_push( in );
			in->x++;
		}else if( in->up ){
			in->up = 0;
			printf( "UP\n" );
			bot_nodes_push( in );
			bot_memory_push( in );
			in->y--;
		}else if( in->down ){
			in->down = 0;
			printf( "DOWN\n" );
			bot_nodes_push( in );
			bot_memory_push( in );
			in->y++;
		}else{
			printf( "BACK\n" );
			bot_nodes_push( in );
			bot_memory_pop( in );
		}
	}
}

int already_been_there( bot *in, int x, int y ){
	bot_coords *tmp = in->memory;
	for( int i = 0; i < in->length; i++ ){
		if( tmp[i].x == x && tmp[i].y == y ) return 1;
	}

	return 0;
}
