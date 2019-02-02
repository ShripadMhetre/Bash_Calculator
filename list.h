#define PLUS 1
#define MINUS -1

typedef struct node {
	int num;
	struct node *next, *prev;
}node;
typedef struct number {
	int sign, dec;
	node *head, *tail;
}number;

void initNumber(number *a);
void addDigit(number *a, char ch);
void printNumber(number a);
void destroyNumber(number *a);
void appendleft(number *a, int no);
int length(number a);
void remov(number *a, int pos);

void zeroRemov(number *a);
