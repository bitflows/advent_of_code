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

	char * buf_orig = calloc(1,sb.st_size + 1);
	assert(buf_orig);
	assert(read(fd, buf_orig, sb.st_size) == sb.st_size);
	assert(close(fd) == 0);

	int buf_len = strlen(buf_orig);
	assert(buf_len);
	if (buf_orig[buf_len-1] == '\n'){
		buf_orig[buf_len-1] = 0;
	}

	for (int c = 'A'; c <= 'Z'; c++){
		char * buf = strdup(buf_orig);
		buf_len = strlen(buf);

		int i = 0;
		while (i < buf_len){
			if (buf[i] == c || buf[i] == c+32){
				memmove(&buf[i], &buf[i+1], buf_len - i + 1);
				buf_len--;
				continue;						
			}
			i++;
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
		printf("Filtering '%c' gives len:%lu\n", c, strlen(buf));		
		free(buf);
	}
}
