#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "queue.h"

int main() {
	//Queue* q = createQueue();
	FILE* file;
	char filename[] = "text.txt";
	char word[MAX_WORD_SIZE];
    int totalWords = 0, totalChars = 0;

	Queue* queues[4];

	for (int i = 0; i < 4; i++) {
		queues[i] = createQueue();
	}

	if (fopen_s(&file, filename, "r") != 0) {
		printf("Error opening file: %s\n", filename);
		return 1;
	}
	else {
#pragma omp parallel num_threads(12) shared(file, queues) private(word)
        {
            int threadNum = omp_get_thread_num();

            if (threadNum < 8) {
                // Producer threads
                while (fscanf_s(file, "%s", word, sizeof(word)) == 1) {
                    enQueue(queues[threadNum % 4], word);
                }
            }
//            else {
//                // Consumer threads
//                while (!isQueueEmpty(queues[threadNum % 4])) {
//                    char* word = deQueue(queues[threadNum % 4]);
//                    if (word != NULL) {
//                        int words = 0, chars = 0;
//                        // Calculate number of words and characters in the sentence
//                        words++;
//                        chars = strlen(word);
//                        // Update the total number of words and characters
//#pragma omp critical
//                        {
//                            totalWords += words;
//                            totalChars += chars;
//                        }
//                    }
//                }
//            }
        }
		printf("Queue 1:\n");
		printQueue(queues[0]);
		printf("Queue 2:\n");
		printQueue(queues[1]);
		printf("\n");
	}
    printf("Total Words: %d\n", totalWords);
    printf("Total Characters: %d\n", totalChars);

	return 0;
}