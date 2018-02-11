

#ifndef STACKOPS_H_
#define STACKOPS_H_

typedef struct
{
    int front, rear, size;
    int capacity;
    float* array;
}Queue_t;

void setQueue(Queue_t *queue, int capacity);
int qisFull( Queue_t *queue);
int qisEmpty( Queue_t *queue);
void enqueue(Queue_t *queue, float item);
float dequeue( Queue_t *queue);
float front( Queue_t *queue);
float rear( Queue_t *queue);


#endif /* STACKOPS_H_ */
