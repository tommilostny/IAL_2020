#include <stdio.h>
#include <stdlib.h>
#include "c203/c203.c"
#include "c206/c206.c"

///Queue print from c203-test.c
void queuePrint ( tQueue* q ) {
/* Prints a queue with the following index meaning:
 * F = Front
 * B = Back
 * E = queue is Empty (F == B)
 */
	putchar('\n');
                                                   /* Prints a queue content. */
	for ( int i=0; i<QUEUE_SIZE; i++ )
		putchar(q->arr[i]);
	putchar('\n');
                                                 /* Prints a queue index(es). */
	for ( int i=0; i<QUEUE_SIZE; i++ ) {
		if ( ( i == q->f_index ) || ( i == q->b_index ) )
			putchar('^');
		else
			putchar(' ');
	}
	putchar('\n');
                                                 /* Prints index(es) meaning. */
	for ( int i=0; i<QUEUE_SIZE; i++ ) {
		if ( (i == q->f_index) && (i==q->b_index))
			putchar('E');
		else if ( i == q->f_index )
			putchar ('F');
		else if ( i == q->b_index )
			putchar ('B');
		else
			putchar (' ');
	}
	putchar('\n');
}

tQueue *myq;
int QUEUE_SIZE;
int solved;
int err_flag;

void queueTests()
{
    QUEUE_SIZE = 12;

    myq = malloc(sizeof(tQueue));
    queueInit(myq);
    queuePrint(myq);

    char hw[] = "ahoj svete!";

    for (int i = 0; i < 12; i++)
        queueUp(myq, hw[i]);
    queuePrint(myq);

    for (int i = 0; i < 5; i++)
        queueRemove(myq);
    queuePrint(myq);

    char t2[] = "zdar";
    for (int i = 0; i < 5; i++)
        queueUp(myq, t2[i]);
    queuePrint(myq);

     for (int i = 0; i < 11; i++)
        queueRemove(myq);
    queuePrint(myq);

    for (int i = 0; i < 12; i++)
        queueUp(myq, hw[i]);
    queuePrint(myq);

    char c;
    for (int i = 0; i < 2; i++)
    {
        queueGet(myq, &c);
        printf("%c\n", c);
    }
    queuePrint(myq);

    queueUp(myq, 'Z');
    queueUp(myq, 'Z');
    queuePrint(myq);
    queueUp(myq, 'G');
    queuePrint(myq);

    free(myq);
}

void dllistTests()
{
    tDLList list;
    DLInitList(&list);
    DLInsertFirst(&list, 5);
    DLDeleteFirst(&list);
    DLInsertLast(&list, 5);
    DLDeleteLast(&list);
    
    for (size_t i = 0; i < 5; i++)
    {
        DLInsertFirst(&list, i * 7);
    }
    for (size_t i = 100; i < 105; i++)
    {
        DLInsertLast(&list, i);
    }
    
    for (size_t i = 0; i < 5; i++)
    {
        if (i % 2 == 0)
            DLDeleteFirst(&list);
        else
            DLDeleteLast(&list);
    }

    DLLast(&list);
    DLPostInsert(&list, 200);
    DLFirst(&list);
    DLPreInsert(&list, 300);
    DLDisposeList(&list);
}

int main()
{
    queueTests();
    dllistTests();
    return 0;
}