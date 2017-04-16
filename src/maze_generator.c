#include "mazemaker.h"

int main(int argc, char **argv){
	if(argc != 5){
		printf("USAGE : ./maze_generator filename width height genAlgorithm ( width and height are multiples of 3 )\n");
		exit(-1);
	}else{
		if(!atoi(argv[2])){
		printf("USAGE : ./maze_generator filename width height genAlgorithm ( width and height are multiples of 3 )\n");
			exit(-2);
		}else if(!atoi(argv[3])){
		printf("USAGE : ./maze_generator filename width height genAlgorithm ( width and height are multiples of 3 )\n");
			exit(-2);
		}
	}
	if((int)atoi(argv[2])/3 != (double)atoi(argv[2])/3){
			printf("USAGE : ./maze_generator filename width height genAlgorithm ( width and height are multiples of 3 )\n");
			exit(-9);
	}
	if((int)atoi(argv[3])/3 != (double)atoi(argv[3])/3){
			printf("USAGE : ./maze_generator filename width height genAlgorithm ( width and height are multiples of 3 )\n");
			exit(-9);
	}


	FILE *file = fopen(argv[1], "w");
	if(file == NULL){
		printf("ERROR: cannot create %s\n", argv[1] );
		exit(-3);
	}
	srand(time(NULL));

	fprintf(file,"%s\n", argv[2]);
	int width = atoi(argv[2]);
	int height = atoi(argv[3]);
	int i;

	char **map = malloc(height*sizeof(char *));
	for(i=0; i<height; i++){
		map[i] = malloc((width+1)*sizeof(char));
	}

	for(int j=0; j<height; j++){
		for(int k=0; k<width; k++){
			map[j][k] = '1';
		}
		map[j][width] = 0;
	}

	switch(atoi(argv[4])){
		case 1:
			printf("StupidGen method selected\n");
			stupidGen(map, width, height);
		break;
		default:
			printf("PerfectGen method selected\n");
			perfectGen(map, width, height);
		break;
	}

	/* WRITE */
	for(i=0; i<height; i++){
		fprintf(file, "%s\n", map[i]);
	}

	/* FREE and CLOSE */
	for(i=0; i<height; i++){
		free(map[i]);
	}
	free(map);
	fclose(file);

	return 0;
}

void *malloc_safe(size_t size)
{
    void* ptr = malloc(size);
    if(ptr == NULL)
        return malloc_safe(size); 
    else
        return ptr;
}