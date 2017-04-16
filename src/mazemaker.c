#include "mazemaker.h"

void stupidGen(char **map, int width, int height){
	int i;
	srand(time(NULL)); 
	
	for(int j=1; j<height-1; j++){
		for(int k=1; k<width-1; k++){
			if(rand()%(0-10) +0 > 2) {
				map[j][k] = '1';
			}else{
				map[j][k] = '0';
			}
		}
	}


	for(i=1; i<width-1 && map[1][i] != '1'; i++);
	map[0][i] = '2';
	for(i=width-2; i>0 && map[height-2][i] != '1'; i--);
	map[height-1][i] = '3';
}

void perfectGen(char **map, int m, int n){
	for(int j=0; j<n; j+=2){
		for(int i=0; i<m; i++){
			map[j][i] = '0';
		}
	}

	for(int j=1; j<n; j+=2){
		for(int i=0; i<m; i+=2){
			map[j][i] = '0';
		}
	}

	int ry = randomInt(0, n);
	int rx = randomInt(0, m);
	while(map[ry][rx] != '1'){
		ry = randomInt(0, n);
		rx = randomInt(0, m);		
	}
	//map[ry][rx] = '2';


	xyStack *mem = xyStack_init();
	xyStack *history = xyStack_init();
	int left = 0, right = 0, up = 0, down = 0;
	int choice = -1;
	char cache[4];
	int cacheLength = 0;
	int nCases;
	if(pair(m)){
		nCases = m/2;
	}else{
		nCases = (m-1)/2;
	}
	if(pair(n)){
		nCases*= n/2;
	}else{
		nCases*= (n-1)/2;
	}

	do{ //Tant que l'on a pas visité toutes les cases
		xyStack_push(history, rx, ry);
		xyStack_uPush(mem ,rx ,ry );
		
		left = 0;
		right = 0;
		up = 0;
		down = 0;

		if(rx > 1 && map[ry][rx-1] == '0') left = 1;
		if(rx+1 < m-1 && map[ry][rx+1] == '0') right = 1;
		if(ry > 1 && map[ry-1][rx] == '0') up = 1;
		if(ry+1 < n-1 && map[ry+1][rx] == '0') down = 1;

		if(rx-2 > 0){
			if(xyStack_exist(mem, rx-2, ry)) left = 0;
		}
		if(rx+2 < m){
			if(xyStack_exist(mem, rx+2, ry)) right = 0;
		}
		if(ry-2 > 0){
			if(xyStack_exist(mem, rx, ry-2)) up = 0;
		}
		if(ry+2 < n){
			if(xyStack_exist(mem, rx, ry+2)) down = 0;
		}

		if(left+right+up+down > 0){
			choice = randomInt(0, left+right+up+down);
			cacheLength = 0;
			if(left){
				cache[cacheLength] = 'L';
				cacheLength++;
			}
			if(right){
				cache[cacheLength] = 'R';
				cacheLength++;
			}
			if(up){
				cache[cacheLength] = 'U';
				cacheLength++;
			}
			if(down){
				cache[cacheLength] = 'D';
				cacheLength++;
			}

			switch(cache[choice]){
				case 'L':
					map[ry][rx-1] = '1';
					if(rx-2 > 0)rx-= 2;
				break;
				case 'R':
					map[ry][rx+1] = '1';
					if(rx+2 < m)rx+= 2;
				break;
				case 'U':
					map[ry-1][rx] = '1';
					if(ry-2 > 0)ry-= 2;
				break;
				case 'D':
					map[ry+1][rx] = '1';
					if(ry+2 < n)ry+= 2;
				break;
			}		
		}else{
			xyStack_pop(history, &rx, &ry);
			xyStack_pop(history, &rx, &ry);
		}
	}while(mem->length < nCases && history->length > 0);
	pls_give_me_a_name(map, m, n, '2');
	pls_give_me_a_name(map, m, n, '3');	

	xyStack_free(history);
	xyStack_free(mem);
}

void pls_give_me_a_name(char **map, int m, int n, char c){ //Do things
	int entry = randomInt(0, 4);
	int coordTmp;
	switch(entry){
		case 0:
			do{
				coordTmp = randomInt(1, m-2);		
			}while(map[1][coordTmp] != '1');
			map[0][coordTmp] = c;
		break;
		case 1:
			do{
				coordTmp = randomInt(1, m-2);		
			}while(map[n-2][coordTmp] != '1');
			map[n-1][coordTmp] = c;
		break;
		case 2:
			do{
				coordTmp = randomInt(1, n-2);		
			}while(map[coordTmp][1] != '1');
			map[coordTmp][0] = c;
		break;
		case 3:
			do{
				coordTmp = randomInt(1, n-2);		
			}while(map[coordTmp][m-2] != '1');
			map[coordTmp][m-1] = c;
		break;
	}
}
int randomInt(int nMin, int nMax){
	return rand()%(nMax-nMin) + nMin;
}

xyStack *xyStack_init(){
	xyStack *out = malloc(sizeof(xyStack));
	out->x = NULL;
	out->y = NULL;
	out->length = 0;

	return out;
}
void xyStack_push(xyStack *in, int x, int y){
	in->length++;
	in->x = realloc(in->x, in->length*sizeof(int));
	in->y = realloc(in->y, in->length*sizeof(int));
	in->x[in->length-1] = x;
	in->y[in->length-1] = y;
}

void xyStack_uPush(xyStack *in, int x, int y){
	if(!xyStack_exist(in, x, y)){
		xyStack_push(in, x, y);
	}
}

void xyStack_pop(xyStack *in, int *x, int *y){
	*x = in->x[in->length-1];
	*y = in->y[in->length-1];
	in->length--;
	in->x = realloc(in->x, in->length*sizeof(int));
	in->y = realloc(in->y, in->length*sizeof(int));
}

int xyStack_exist(xyStack *in, int x, int y){
	for(int i = 0; i < in->length; i++){
		if(in->x[i] == x && in->y[i] == y){
			return 1;
		}
	}

	return 0;
}

void xyStack_free(xyStack *in){
	free(in->x);
	free(in->y);
	free(in);
}

int pair(int num){
	if((int)num/2 == (double)num/2){
		return 1;
	}else{
		return 0;
	}
}