#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#define INPUTFILE "input"

int main(int argc, char * argv[]){
	struct stat sb;
	if (stat(INPUTFILE, &sb)){
		printf("Could not stat input file '"INPUTFILE"'\n");
		exit(255);
	}

	int fd = open(INPUTFILE, O_RDONLY);
	if (fd == -1){
		printf("Could not read input file '"INPUTFILE"'\n");
		exit(255);		
	}

	char * buf = calloc(1,sb.st_size + 1);
	assert(buf);
	assert(read(fd, buf, sb.st_size) == sb.st_size);
	assert(close(fd) == 0);

	int buf_len = strlen(buf);
	assert(buf_len);
	if (buf[buf_len-1] == '\n'){
		buf[buf_len-1] = 0;
	}

	bool changed = false;
	do{
		changed = false;
		buf_len = strlen(buf);
		for (int i = 0; i < buf_len-1; i++){
			assert(isalpha(buf[i]));
			char comp = islower(buf[i]) ? buf[i] - 32 : buf[i] + 32;
			if (buf[i+1] == comp){
				buf[i] = 0;
				if (i+1 < buf_len){
					memmove(&buf[i], &buf[i+2], buf_len - (i+2) +1);
					changed = true;
					break;
				}
			}
		}
	} while (changed);
	printf("Resulting string: %s len:%lu\n", buf, strlen(buf));
}
