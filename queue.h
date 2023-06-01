#include <stdio.h>
#include <stdlib.h>
#define MAX_WORD_SIZE 25

typedef struct {
	char word[MAX_WORD_SIZE];
	struct Node* next;
} Node;

typedef struct {
	Node* front;
	Node* rear;
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
	return q;
}

void enQueue(Queue* q, char* word) {
	Node* temp = newNode(word);
	if (temp == NULL) {
		free(temp);
		printf("Memory is full!\n");
	}

	if (q->rear == NULL) {
		q->front = q->rear = temp;
		return;
	}

	q->rear->next = temp;
	q->rear = temp;
}

char* deQueue(Queue* q) {
	char* word = NULL;

	if (isQueueEmpty(q)) {
		printf("Queue is empty. Cannot dequeue.\n");
		return word;
	}

	Node* temp = q->front;
	word = temp->word;

	q->front = q->front->next;
	free(temp);

	if (q->front == NULL)
		q->rear = NULL;

	return word;
}

int isQueueEmpty(Queue* q) {
	return (q->front == NULL);
}

void printQueue(Queue* q) {
	int iterator = 0;
	Node* tmp = q->front;

	while (tmp != NULL) {
		iterator += 1;
		printf("Node: %d - Data: %s\n", iterator, tmp->word);
		tmp = tmp->next;
	}

	free(tmp);
}