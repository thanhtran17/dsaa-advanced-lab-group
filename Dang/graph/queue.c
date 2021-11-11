#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#define MAX 100000
int rear = 0;
int front = 0;
void enqueue(int queue[], int val)
{
    queue[rear] = val;
    rear++;
}
// Tra ve vi tri cua node vua moi day ra khoi queue
int dequeue(int queue[])
{
    if (isEmpty(queue))
    {
        return -1;
    }
    int savedValue;
    savedValue = queue[front];
    queue[front] = -1;
    (front)++;
    return savedValue;
}
void printQueue(int queue[])
{
    if (isEmpty(queue))
    {
        return;
    }
    printf("Queue data: ");
    for (int i = front; i < rear; i++)
    {
        printf("%d ", queue[i]);
    }
    printf("\n");
}
int isEmpty(int queue[])
{
    if (front > rear || rear == -1)
    {
        printf("Queue is empty\n");
        return 1;
    }
    return 0;
}
int returnHead(int queue[])
{
    return queue[front];
}
// int main(){
//     int queue[10];
//     int front = 0;
//     int rear;
//     int choice;
//     int arraySize = 10;
//     do{
//         printf("1.Input elements\n");
//         printf("2.Enqueue\n");
//         printf("3.Dequeue\n");
//         printf("4.See first element\n");
//         printf("5.Print Queue\n");
//         printf("Your choice: \n");
//         scanf("%d",&choice);
//         switch(choice){
//             case 1: printf("how many elements you want to add\n");
//                     scanf("%d",&rear);
//                     for (int i = front;i < rear;i++){
//                         printf("Input queue[%d]\n",i+1);
//                         scanf("%d",&queue[i]);
//                     }
//                     break;
//             case 2: enqueue(queue,&rear,arraySize);
//                     break;
//             case 3: dequeue(queue,&front);
//                     break;
//             case 4: printf("First Element: %d\n",returnHead(queue,front));
//                     break;
//             case 5: printQueue(queue,&rear,&front);
//                     break;
//             case 6: return 0;
//         }

//     } while(1);

// }