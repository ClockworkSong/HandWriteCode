#include <stdio.h>
#include <string.h>

#define SIZE 10

typedef struct Table
{
	int arr[SIZE];
	int top;
}table_t;


int push(table_t* t, int data) 
{
	if(t->top >= 9) {
		printf("table full!\n");
		return -1;
	}

	t->arr[++t->top] = data;

	return 0;
}

int pop(table_t* t)
{
	if(t->top <= -1) {
		printf("table is empty!\n");
		return -1;
	}
	t->top --;

	return 0;
}


void display(table_t t) 
{

	while(t.top != -1) {
		printf("%d ", t.arr[t.top--]);
	}
	printf("\n");

	return;
}

int main()
{
	table_t t;

	memset(t.arr, 0, sizeof(t.arr));
 	t.top = -1;

	push(&t, 10);
	push(&t, 40);
	push(&t, 60);
	push(&t, 100);
	display(t);
	
	pop(&t);
	pop(&t);
	pop(&t);
	
	display(t);

	return 0;
}
