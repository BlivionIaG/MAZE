#ifndef MAZEMAKER_H
#define MAZEMAKER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct xyStack {
	int *x, *y;
	int length;
} xyStack;

void *malloc_safe( size_t size );
void stupidGen( char **map, int width, int height );
void perfectGen( char **map, int m, int n );

int randomInt( int nMin, int nMax );

xyStack *xyStack_init( );
void xyStack_push( xyStack *in, int x, int y );
void xyStack_pop( xyStack *in, int *x, int *y );
void xyStack_uPush( xyStack *in, int x, int y );
int xyStack_exist( xyStack *in, int x, int y );
void xyStack_free( xyStack *in );

int pair( int num );
void pls_give_me_a_name( char **map, int m, int n, char c );

#endif /* MAZEMAKER_H */
