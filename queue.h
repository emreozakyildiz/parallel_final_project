#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define MAX_WORD_SIZE 25

typedef struct {
	char word[MAX_WORD_SIZE];
	struct Node* next;
} Node;

typedef struct {
	Node* front;
	Node* rear;
	omp_lock_t lock;

} Queue;

Node* newNode(char* word) {
	Node* temp = (Node*)malloc(sizeof(Node));
	strcpy_s(temp->word, MAX_WORD_SIZE, word);
	temp->next = NULL;
	return temp;
}

Queue* createQueue() {
	Queue* q = (Queue*)malloc(sizeof(Queue));
	q->front = q->rear = NULL;
	omp_init_lock(&(q->lock));
	return q;
}

void enQueue(Queue* q, char* word) {
	omp_set_lock(&(q->lock));
	Node* temp = newNode(word);
	if (temp == NULL) {
		free(temp);
		printf("Memory is full!\n");
	}

	if (q->rear == NULL) {
		q->front = q->rear = temp;
		omp_unset_lock(&(q->lock));
		return;
	}

	q->rear->next = temp;
	q->rear = temp;
	omp_unset_lock(&(q->lock));
}

char* deQueue(Queue* q) {
	omp_set_lock(&(q->lock));
	char* word = NULL;

	if (isQueueEmpty(q)) {
		printf("Queue is empty. Cannot dequeue.\n");
		omp_unset_lock(&(q->lock));
		return word;
	}

	Node* temp = q->front;
	word = (char*)malloc(MAX_WORD_SIZE * sizeof(char));
	strcpy_s(word, MAX_WORD_SIZE, temp->word);

	q->front = q->front->next;
	free(temp);

	if (q->front == NULL)
		q->rear = NULL;

	omp_unset_lock(&(q->lock));
	return word;
}

int isQueueEmpty(Queue* q) {
	return (q->front == NULL);
}

void printQueue(Queue* q) {
	if (isQueueEmpty(q)) {
		printf("Queue is empty.\n");
		return;
	}

	int iterator = 0;
	Node* tmp = q->front;

	while (tmp != NULL) {
		iterator += 1;
		printf("Node: %d - Data: %s\n", iterator, tmp->word);
		tmp = tmp->next;
	}

	free(tmp);
}

void destroyQueue(Queue* q) {
	Node* current = q->front;
	while (current != NULL) {
		Node* next = current->next;
		free(current);
		current = next;
	}
	omp_destroy_lock(&(q->lock));
	free(q);
}