#include <stdio.h>
#include <stdint.h>
#include <assert.h>

int main(int argc, char * argv[]){
	FILE * f = fopen("input","r");

	uint32_t line, offset_x, offset_y, size_x, size_y;
	uint32_t fabric[1000][1000] = {0};

	uint32_t claims=0;
	while(fscanf(f, "#%u @ %u,%u: %ux%u\n", &line, &offset_x, &offset_y, &size_x, &size_y) == 5){
		//printf("Read: #%u @ %u,%u: %ux%u\n", line, offset_x, offset_y, size_x, size_y);
		assert(offset_x + size_x <= 1000);
		assert(offset_y + size_y <= 1000);

		for (int x=offset_x; x < offset_x + size_x; x++){
			for (int y=offset_y; y < offset_y + size_y; y++){
				fabric[x][y]++;
			}
		}
		claims++;
	}

	uint32_t overlaps = 0;
	for (int x=0; x < 1000; x++){
		for (int y=0; y < 1000; y++){
			if (fabric[x][y] > 1){
				overlaps++;
			}
		}
	}

	printf("Claims: %u Overlaps: %u\n", claims, overlaps);
}