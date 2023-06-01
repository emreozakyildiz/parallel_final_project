#include <string.h>
#include <ctype.h>
#include <windows.h>
#include "queue.h"

int main() {
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
#pragma omp parallel num_threads(12) shared(file, queues) firstprivate(word)
		{
			int threadNum = omp_get_thread_num();

			if (threadNum < 8) {
				// Producer threads
				while (fscanf_s(file, "%s", word, (unsigned)_countof(word)) == 1) {
					// Remove non-alphabetic characters from word
					int len = strlen(word);
					int j = 0;
					for (int i = 0; i < len; i++) {
						if (isalpha(word[i])) {
							word[j] = tolower(word[i]);  // Convert to lowercase if needed
							j++;
						}
					}
					word[j] = '\0';  // Null-terminate the modified word

					// Enqueue the modified word
					enQueue(queues[threadNum % 4], word);
				}
#pragma omp barrier
			}
			else {
#pragma omp barrier
				// Consumer threads
				while (!isQueueEmpty(queues[threadNum % 4])) {
					int words = 0, chars = 0;
					char* word = deQueue(queues[threadNum % 4]);
					//printf("%s dequeued from thread-%d\n", word, omp_get_thread_num());
					if (word != NULL) {
						// Calculate number of words and characters in the sentence
						words++;
						chars = strlen(word);
						// Update the total number of words and characters
#pragma omp critical
						{
							totalWords += words;
							totalChars += chars;
						}
					}
				}
			}
		}
	}
	printf("Total Words: %d\n", totalWords);
	printf("Total Characters: %d\n", totalChars);

	return 0;
}