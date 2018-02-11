

/*
 *        Author: Robert I. Pitts <rip@cs.bu.edu>
 * Last Modified: March 7, 2000
 *         Topic: Stack - Array Implementation
 * ----------------------------------------------------------------
 *
 * This is an array implementation of a character stack.
 */

//#include <stdio.h>
#include <stdlib.h>  /* for dynamic allocation */
#include "stackops.h"

/************************ Function Definitions **********************/

void StackInit(stackT *stackP, int maxSize)
{
  stackElementT *newContents;

  /* Allocate a new array to hold the contents. */

  newContents = (stackElementT *)malloc(sizeof(stackElementT) * maxSize);

  if (newContents == NULL) {
  //  fprintf(stderr, "Insufficient memory to initialize stack.\n");
    exit(1);  /* Exit, returning error code. */
  }

  stackP->contents = newContents;
  stackP->maxSize = maxSize;
  stackP->top = -1;  /* I.e., empty */
}

void StackDestroy(stackT *stackP)
{
  /* Get rid of array. */
  free(stackP->contents);

  stackP->contents = NULL;
  stackP->maxSize = 0;
  stackP->top = -1;  /* I.e., empty */
}

void StackPush(stackT *stackP, stackElementT element)
{
  if (!StackIsFull(stackP)) {

	  stackP->contents[++stackP->top] = element;
  }

}

void StackPushPop(stackT *stackP, stackElementT element)
{
  if (StackIsFull(stackP)) {
	  StackPop(stackP);
  }

  /* Put information in array; update top. */

  stackP->contents[++stackP->top] = element;
}

void StackPop(stackT *stackP)
{
  if (!StackIsEmpty(stackP)) {

	 stackP->top--;
  }


}

float getAvarage(stackT *stackP)  {
	float sum=0;
	int i;

	for(i = 0; i < stackP->top; ++i) {
			sum += stackP->contents[i];
		}

	return sum / stackP->top;
}

int StackIsEmpty(stackT *stackP)
{
  return stackP->top < 0;
}

int StackIsFull(stackT *stackP)
{
  return stackP->top >= stackP->maxSize - 1;
}
