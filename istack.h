#include "list.h"

//  Stack for storing number list.
typedef struct inode {
	number *a;
	struct inode *p;
}inode;
typedef inode *istack;
void iinit(istack *s);

void ipush(istack *s, number *a);

number *ipop(istack *s);

int iisempty(istack *s);


//##############################################################################################

// Stack for storing character Operators.
typedef struct cnode {
	char a;
	struct cnode *p;
}cnode;
typedef cnode *cstack;
void cinit(cstack *s);

void cpush(cstack *s, char t);

char cpop(cstack *s);

int cisempty(cstack *s);
