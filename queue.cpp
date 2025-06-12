#include <iostream>
#include "queue.h"
#include <cstdlib>  // malloc, free


Queue* init(void) {
	Queue* queue = (Queue*)malloc(sizeof(Queue));
	if (queue == NULL) {
		return NULL;
	}
	queue->head = NULL;
	queue->tail = NULL;
	return queue;
}



void release(Queue* queue) {
	if (queue == NULL) return;

	Node* current = queue->head;
	while (current != NULL) {
		Node* next = current->next;
		free(current);
		current = next;
	}

	free(queue);
}


Node* nalloc(Item item) {
	Node* node = (Node*)malloc(sizeof(Node));
	if (node == NULL) return NULL;  

	node->item = item;
	node->next = NULL;
	return node;
}


void nfree(Node* node) {
	return;
}


Node* nclone(Node* node) {
	return NULL;
}


Reply enqueue(Queue* queue, Item item) {
    Reply reply = { false, item };

    Node* new_node = nalloc(item);
    if (new_node == NULL) return reply;

    // Case 1: �� ť�� ���
    if (queue->head == NULL) {
        queue->head = queue->tail = new_node;
        reply.success = true;
        return reply;
    }

    // Case 2: ������ item�� key�� head���� Ŭ ��� (�տ� ����)
    if (item.key > queue->head->item.key) {
        new_node->next = queue->head;
        queue->head = new_node;
        reply.success = true;
        return reply;
    }

    // Case 3: �߰� �Ǵ� ���� ����
    Node* current = queue->head;
    while (current->next != NULL && current->next->item.key >= item.key) {
        current = current->next;
    }

    new_node->next = current->next;
    current->next = new_node;

    if (new_node->next == NULL) {
        queue->tail = new_node;
    }

    reply.success = true;
    return reply;
}


Reply dequeue(Queue* queue) {
	Reply reply = { false, NULL };
	return reply;
}

Queue* range(Queue* queue, Key start, Key end) {
	return NULL;
}
