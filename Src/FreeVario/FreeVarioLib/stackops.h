

#ifndef STACKOPS_H_
#define STACKOPS_H_

typedef struct
{
    int front, rear, size;
    int capacity;
    float* array;
}Queue_t;

void SO_setQueue(Queue_t *queue, int capacity);
int SO_qisFull( Queue_t *queue);
int SO_qisEmpty( Queue_t *queue);
void SO_enqueue(Queue_t *queue, float item);
float SO_dequeue( Queue_t *queue);
float SO_front( Queue_t *queue);
float SO_rear( Queue_t *queue);
float SO_getAvarage(Queue_t *queue);

#endif /* STACKOPS_H_ */
