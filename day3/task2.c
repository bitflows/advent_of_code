#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

int main(int argc, char * argv[]){
	FILE * f = fopen("input","r");

	uint32_t line, offset_x, offset_y, size_x, size_y;
	uint32_t fabric[1000][1000] = {0};

	uint32_t claims1=0;
	while(fscanf(f, "#%u @ %u,%u: %ux%u\n", &line, &offset_x, &offset_y, &size_x, &size_y) == 5){
		//printf("Read: #%u @ %u,%u: %ux%u\n", line, offset_x, offset_y, size_x, size_y);
		assert(offset_x + size_x <= 1000);
		assert(offset_y + size_y <= 1000);
		claims1++;

		for (int x=offset_x; x < offset_x + size_x; x++){
			for (int y=offset_y; y < offset_y + size_y; y++){
				fabric[x][y]++;
			}
		}
	}

	rewind(f);
	uint32_t perfect_claim = 0;
	uint32_t perfect_claim_count = 0;
	uint32_t claims2=0;
	while(fscanf(f, "#%u @ %u,%u: %ux%u\n", &line, &offset_x, &offset_y, &size_x, &size_y) == 5){
		assert(offset_x + size_x <= 1000);
		assert(offset_y + size_y <= 1000);
		claims2++;

		bool collision = false;
		for (int x=offset_x; x < offset_x + size_x; x++){
			for (int y=offset_y; y < offset_y + size_y; y++){
				if (fabric[x][y] > 1){
					collision = true;
				}
			}
		}

		if (collision == false){
			assert(perfect_claim == 0);
			perfect_claim = line;		
		}
	}

	printf("Claims: %u/%u perfect: %u\n", claims1, claims2, perfect_claim);
}