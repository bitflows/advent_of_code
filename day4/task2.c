#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_GUARD_COUNT 10000

struct log_date{
    uint32_t year, month, day, hour, minute;	
};

struct log_entry{
	struct log_entry * next;
	struct log_date timestamp;
	char * data;	
};

struct log{
	struct log_entry * head, * tail;
};

void add_sorted(struct log * l, struct log_entry * entry){
	struct log_entry * iter = l->head, * prev = NULL;
	while (iter){
		if (entry->timestamp.year < iter->timestamp.year){
			break;
		} else if (entry->timestamp.year == iter->timestamp.year){
			if (entry->timestamp.month < iter->timestamp.month){
				break;
			} else if (entry->timestamp.month == iter->timestamp.month){
				if (entry->timestamp.day < iter->timestamp.day){
					break;
				} else if (entry->timestamp.day == iter->timestamp.day){
					if (entry->timestamp.hour < iter->timestamp.hour){
						break;
					} else if (entry->timestamp.hour == iter->timestamp.hour){
						if (entry->timestamp.minute < iter->timestamp.minute){
							break;
						} else if (entry->timestamp.minute == iter->timestamp.minute){
							break;			
						}											
					}					
				}
			}
		}
		prev = iter;
		iter = iter->next;
	}
	if (iter){
		entry->next = iter;
		if (prev){
			prev->next = entry;
		} else {
			l->head = entry;
		}
	} else {
		l->tail = entry;
		if (prev){
			prev->next = entry;
		} else {
			l->head = entry;
		}
	}
}



int main(int argc, char * argv[]){
	FILE * f = fopen("input","r");

	struct log l = {0};

	uint32_t claims=0;
	char buf[1024];
	int count = 0;
	while (fgets(buf, sizeof(buf)-1, f)){
		struct log_entry * entry = calloc(1, sizeof(struct log_entry));
		assert(entry);

		int err = sscanf(buf, "[%u-%u-%u %u:%u", &entry->timestamp.year, &entry->timestamp.month, &entry->timestamp.day, &entry->timestamp.hour, &entry->timestamp.minute);
		assert(err == 5);

		entry->data = strdup(buf + 19);

		add_sorted(&l, entry);
		count++;
	}
	printf("Sorted %u logentries\n", count);

	uint32_t minutes = 1 * 12 * 31 * 1 * 60; //We count 31 days for all months to make it simpler. Only first hour is needed, so skip the 23 others. Only one year is used, so leave out.

	printf("Minutes per guard: %u\n", minutes);
	uint8_t * sheet [MAX_GUARD_COUNT] = {0};
	for (int i = 0; i< MAX_GUARD_COUNT; i++){
		sheet[i] = calloc(minutes, sizeof(uint8_t));
	}

	struct log_entry * iter = l.head;
    int32_t guard_id=-1;
	while (iter){
		//printf("%u-%u-%u %u:%u %s", iter->year, iter->month, iter->day, iter->hour, iter->minute , iter->data);
		if (sscanf(iter->data, "Guard #%d begins shift", &guard_id) == 1){
			assert(guard_id < MAX_GUARD_COUNT);
			//Mark hour as guarded (set 1)
			uint32_t start = ((iter->timestamp.month -1) * 31 * 1 * 60) + ((iter->timestamp.day -1) * 1 * 60) + (iter->timestamp.hour == 23 ? 60 : 0);
			for (uint32_t i = start; i < start + 60; i++){
				assert(sheet[guard_id][i] == 0);
				sheet[guard_id][i] = 1;
			}
		} else if (strncmp(iter->data, "falls asleep", strlen("falls asleep")) == 0){
			assert(guard_id != -1);
				//printf("Fell asleep\n");
			uint32_t start = ((iter->timestamp.month -1) * 31 * 1 * 60) + ((iter->timestamp.day -1) * 1 * 60) + iter->timestamp.minute ;
			for (uint32_t i = start; i < start + 60 - iter->timestamp.minute; i++){
				sheet[guard_id][i] = 2;
			}

		} else if (strncmp(iter->data, "wakes up", strlen("wakes up")) == 0){
			assert(guard_id != -1);
			//printf("Woke up\n");
			uint32_t start = ((iter->timestamp.month -1) * 31 * 1 * 60) + ((iter->timestamp.day -1) * 1 * 60) + iter->timestamp.minute;
			for (uint32_t i = start; i < start + 60 - iter->timestamp.minute; i++){
				sheet[guard_id][i] = 1;
			}
		} else {
			assert(0);
		}
		iter = iter->next;
	}


	int guard_max_sleep_id = -1;
	int guard_max_sleep_time = 0;
	for (int i = 0; i < MAX_GUARD_COUNT; i++){
		uint32_t sleep_time = 0, awake_time = 0;

		for (int j = 0; j < minutes; j++){
			if (sheet[i][j] == 1){
				awake_time++;
			} else if (sheet[i][j] == 2){
				sleep_time++;
			}
		}
		if (awake_time || sleep_time){
			printf ("Guard %u was awake for %u minutes, and slept %u minutes\n", i, awake_time, sleep_time);
			if (sleep_time > guard_max_sleep_time){
				guard_max_sleep_id = i;
				guard_max_sleep_time = sleep_time;
			}
		}
	}
	printf("Sleepiest guard is %d with %d minutes total.\n", guard_max_sleep_id, guard_max_sleep_time);

	uint32_t minutes_hist[MAX_GUARD_COUNT][60] = {0};
	for (int g = 0; g < MAX_GUARD_COUNT; g++){
		for (int i = 0; i < minutes; i+=60){
			for (int j = 0; j < 60; j++){
				if (sheet[g][i+j] == 2){
					minutes_hist[g][j]++;
				}
			}
		}
	}

	uint32_t sleepiest_minute = 0;
	uint32_t sleepiest_count = 0;
	uint32_t sleepiest_guard = 0;
	for (int g = 0; g < MAX_GUARD_COUNT; g++){
		for (int i = 0; i < 60; i++){
			if (minutes_hist[g][i] > sleepiest_count){
				sleepiest_count = minutes_hist[g][i];
				sleepiest_minute = i;
				sleepiest_guard = g;
			}
		}
	}

	printf("Sleepiest guard at any minute is %u @ minute %u (%u times)\nAnswer: %u\n", sleepiest_guard,sleepiest_minute, sleepiest_count, (sleepiest_guard * sleepiest_minute));

	//Cleanup
	for (int i = 0; i< MAX_GUARD_COUNT; i++){
		free(sheet[i]);
		sheet[i] = NULL;
	}

	iter = l.head;
	while (iter){
		struct log_entry * tmp = iter;
		iter = iter->next;
		free(tmp->data);
		free(tmp);
	}
}