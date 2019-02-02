#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include "list.h"
#include "functions.h"

//Function to equal the digits after decimal point.
void decimalEqual(number *a, number *b) {
	int i;
	if(a->dec > b->dec) {
		int diff = a->dec - b->dec;
		for(i = 0; i < diff; i++) {
			addDigit(b, '0');
			b->dec++;
		}
	}
	else if(b->dec > a->dec) {
		int diff = b->dec - a->dec;
		for(i = 0; i < diff; i++) {
			addDigit(a, '0');
			a->dec++;
		}
	}
}

//equals the length of both numbers by prepending zeros to small number.
void lengthEqual(number *a, number *b) {
	int gap; 
	gap = length(*a) - length(*b);
	if(gap > 0) {
		int i = 0;
		while(i < gap) {
			appendleft(b, 0);
			i++;
		}
	}
	else if(gap < 0) {
		int i = 0;
		gap = -gap;
		while(i < gap) {
			appendleft(a, 0);
			i++;
		}
	}
}

//checks whether the number is zero or not. returns 0 if it is zero number.
int zeroNumber(number a) {
	int i, flag = 0;
	node *p = a.head;
	for(i = 0; i < length(a); i++) {
		if(p->num != 0)
			flag = 1;
		 p = p->next;
	}
	return flag;
}

//copy number a in number b.
void copy(number *a, number *b) {
	int i, no, len;
	char ch;
	len = length(*a);
	node *t1 = a->head;
	for(i = 0; i < len; i++) {
		no = t1->num;
		ch = no + '0';
		addDigit(b, ch);
		t1 = t1->next;
	}
	b->dec = a->dec;
	b->sign = a->sign;
	return;
}
//Addition Operation
number *add(number *a, number *b) {
	number *ans;
	ans = (number *)malloc(sizeof(number));
	initNumber(ans);
	decimalEqual(a, b);
	if(a->sign != b->sign) {
		if(a->sign == MINUS) {
			a->sign = PLUS;
			ans = sub(b, a);
		}
		else if(b->sign == MINUS) {
			b->sign = PLUS;
			ans = sub(a, b);
		}
	}
	else if(a->sign == b->sign) {
		int i, n1, n2, carry = 0, sum;
		int len_a, len_b;
		node *t1 = a->tail;
		node *t2 = b->tail;
		len_a = length(*a);
		len_b = length(*b);
		if(a->sign == MINUS)
			ans->sign = MINUS;
		else
			ans->sign = PLUS;
		if(len_a >= len_b) {
			for(i = 1; i <= len_b; i++) {
				n1 = t1->num;
				n2 = t2->num;
				sum = n1 + n2 +carry;
				carry = sum / 10;
				sum = sum % 10;
				appendleft(ans, sum);
				t1 = t1->prev;
				t2 = t2->prev;
			}
			for(i = 1; i <= len_a - len_b; i++) {
				n1 = t1->num;
				sum = n1 + carry;
				carry = sum / 10;
				sum = sum % 10;
				appendleft(ans, sum);
				t1 = t1->prev;
			}
		}else {
			for(i = 1; i <= len_a; i++) {
				n1 = t1->num;
				n2 = t2->num;
				sum = n1 + n2 +carry;
				carry = sum / 10;
				sum = sum % 10;
				appendleft(ans, sum);
				t1 = t1->prev;
				t2 = t2->prev;
			}
			for(i = 1; i <= len_b - len_a; i++) {
				n1 = t2->num;
				sum = n1 + carry;
				carry = sum / 10;
				sum = sum % 10;
				appendleft(ans, sum);
				t2 = t2->prev;
			}
		}
		ans->dec = a->dec;	
		if(carry != 0)
			appendleft(ans, carry);
	}
	return ans;
}

/*Function for finding bigger number among equal length numbers.*/
int compareEqual(number a, number b) {
	lengthEqual(&a, &b);
	decimalEqual(&a, &b);
	node *p, *q;
	int len;
	int i;
	len = length(a);
	p = a.head;
	q = b.head;
	for(i = 1; i <= len; i++) {
		if(p->num > q->num)
			return 1;  //i.e. number a greater than number b.
		else if(p->num < q->num)
			return -1;  //i.e. a is less than b.
		p = p->next;
		q = q->next;
	}
	return 0;    //i.e. both numbers are equal.
}
//Substraction operation.
number *sub(number *a, number *b) {
	number *ans;
	ans = (number *)malloc(sizeof(number));
	initNumber(ans);
	//decimal digits and length made equal.
	decimalEqual(a, b);
	lengthEqual(a, b);
	//zeroRemov(a);
	//zeroRemov(b);
	if(a->sign != b->sign) {
		if(a->sign == MINUS) {
			a->sign = PLUS;
			ans = add(a, b);
			ans->sign = MINUS;
		}
		else if(b->sign == MINUS) {
			b->sign = PLUS;
			ans = add(a, b);
			ans->sign = PLUS;
		}
	}
	else if(a->sign == b->sign) {
		if(a->sign == MINUS) {
			a->sign = b->sign = PLUS;
			ans = sub(b, a);
		}
		else if(a->sign == PLUS) {
			int n1, n2, diff, borrow = 0, i, len;
			node *t1 = a->tail;
			node *t2 = b->tail;
			//length of both numbers is same now because of lengthEqual.
			len = length(*b);	
			if(compareEqual(*a, *b) == 1) {
				for(i = 1; i <= len; i++) {
					n1 = t1->num;
					n2 = t2->num;
					n1 = n1 - borrow;
					if(n1 >= n2) {
						diff = n1 - n2;
						borrow = 0;
						appendleft(ans, diff);
					}
					else {
						n1 = n1 + 10;
						diff = n1 - n2;
						borrow = 1;
						appendleft(ans, diff);
					}
					t1 = t1->prev;
					t2 = t2->prev;
				}	
			}
			else if(compareEqual(*a, *b) == -1) {
				ans->sign = MINUS;
				for(i = 1; i <= len; i++) {
					n1 = t1->num;
					n2 = t2->num;
					n2 = n2 - borrow;
					if(n2 >= n1) {
						diff = n2 - n1;
						borrow = 0;
						appendleft(ans, diff);
					}
					else {
						n2 = n2 + 10;
						diff = n2 - n1;
						borrow = 1;
						appendleft(ans, diff);
					}
					t1 = t1->prev;
					t2 = t2->prev;
				}	
			}
			else {
				if(compareEqual(*a, *b) == 0) {
					appendleft(ans, 0);
				}
			}
		}
	}
	ans->dec = a->dec;
	return ans;
}
//Multiplication Operation.
number *mult(number *a, number *b) {
	number *ans = (number *)malloc(sizeof(number));
	initNumber(ans);
	//checks if any one of the numbers is zero.
	if((zeroNumber(*a) == 0) || (zeroNumber(*b) == 0)) {
		addDigit(ans, '0');
		return ans;
	}
	int lengthdiff;
	if(a->sign == b->sign) {
		ans->sign = PLUS;
		a->sign = b->sign = PLUS;
	}
	else {
		ans->sign = MINUS;
		a->sign = b->sign = PLUS;
	}
	lengthdiff = length(*a) - length(*b);
	if(lengthdiff < 0) {
		ans = mult(b, a);
		return ans;
	}
	else {
		node *t1, *t2;
		int len_a = length(*a);
		int len_b = length(*b);
		int i, j, n1 = 0, n2 = 0;
		int tempresult[2 * len_a];
		for(i = 0; i < 2 *len_a; i++)
			tempresult[i] = 0;
		int k = 2 * len_a - 1;
		t2 = b->tail;
		for(i = 0; i < len_b; i++) {
			t1 = a->tail;
			int carry1 = 0, carry2 = 0;
			for(j = k - i; j > len_a - 2; j--) {
				if(t1 != NULL && t2 != NULL) {
					n1 = t1->num * t2->num + carry1;
					t1 = t1->prev;
					carry1 = n1 / 10;
					n1 = n1 % 10;
					n2 = tempresult[j] + n1 + carry2;
					carry2 = n2 / 10;
					n2 = n2 % 10;
					tempresult[j] = n2;
				}
				else {
					break;
				}
			}
			tempresult[j] = carry1 + carry2 + tempresult[j];
			len_a--;
			t2 = t2->prev;	
		}
		for(i= k; i >= len_a - 1 && i >= 0; i--) {
			appendleft(ans, tempresult[i]);
		}
		ans->dec = a->dec + b->dec;
		return ans;
	}
}
//Division Operation.
number *division(number *m, number *n){
	if(zeroNumber(*n) == 0) {
		printf("Dividing by Zero is not allowed.\n");
		return NULL; 
	}
	zeroRemov(m);
	zeroRemov(n);
	
	int k = m->dec > n->dec ? m->dec : n->dec;
	int i = 0;
	
	while(i < k) {
		if(m->dec > 0)
			m->dec--;
		else
			addDigit(m, '0');
		if(n->dec > 0)
			n->dec--;
		else
			addDigit(n, '0');
		i++;
	}
	i = 9;
	number *c, *d, *ans, *q, *pro;
	c = (number *)malloc(sizeof(number));
	d = (number *)malloc(sizeof(number));
	ans = (number *)malloc(sizeof(number));
	pro = (number *)malloc(sizeof(number));
	q = (number *)malloc(sizeof(number));
	
	initNumber(ans);
	initNumber(c);
	initNumber(q);
	initNumber(d);
	if(m->sign == n->sign) {
		q->sign = PLUS;
		m->sign = n->sign = PLUS;
	}
	else {
		q->sign = MINUS;
		m->sign = n->sign = PLUS;
	}
	node *p = m->head;
	char ch = p->num + '0';
	addDigit(d, ch);
	while(q->dec < SCALE){
		while(i >= 0){	
			appendleft(c, i);
			pro = mult(n, c);
			ans = sub(d, pro);	
			if(ans->sign != MINUS) {
				addDigit(q, i + '0');
				node *tmp = c->head;
				free(tmp);
				c->head = c->tail = NULL;
				break;
			}
			else{
				node *tmp = c->head;
				free(tmp);
				c->head = c->tail = NULL;
				i--; 
			}
		}
		d = ans;
		if(p->next != NULL) {
			p = p->next;
			ch = p->num + '0';
			addDigit(d, ch);
		}
		else{	
			q->dec++;	
			addDigit(d, '0');
		}
		i = 9;
		node *tmp = c->head;
		free(tmp);
		c->head = c->tail = NULL;
	}
	q->dec--;
	return q;
}

//FORMULA :- remainder = a - floor_division(a/b) * b
number *modulus(number *a, number *b) {
	if(zeroNumber(*b) == 0) {
		printf("ERROR : Modulo by Zero is not allowed.\n");
		return NULL;
	}
	int tempsign;
	if(a->sign == MINUS) {
		tempsign = MINUS;
		a->sign = b->sign = PLUS;
	}
	else {
		tempsign = PLUS;
		a->sign = b->sign = PLUS;
	}
	decimalEqual(a, b);
	int a_dec = a->dec;
	number *ans = (number *)malloc(sizeof(number));
	number *temp = (number *)malloc(sizeof(number));
	initNumber(ans);
	initNumber(temp);
	temp = division(a, b);
	if(temp->dec != 0) {
		int pos = length(*temp) - 1; 
		while(temp->dec != 0) {
			remov(temp, pos);
			temp->dec--;
			pos--;
		}
	}
	temp = mult(temp, b);
	ans = sub(a, temp);
	ans->sign = tempsign;
	ans->dec = a_dec;
	return ans;
}
//Factorial Operation.
number *factorial(number *a) {
	if(a->dec != 0 || a->sign == MINUS) {
		printf("ERROR : Factorial of given number is not possible\n");
		exit(1);
	}
	number *ans = (number *)malloc(sizeof(number));
	initNumber(ans);
	addDigit(ans, '1');
	number *b = (number *)malloc(sizeof(number));
	initNumber(b);
	addDigit(b, '1');
	while(compareEqual(*a, *b) > 0) {
		ans = mult(a, ans);
		a = sub(a, b);
	}
	return ans;
}

//Power Operation.....gives ans = a^b 
number *power(number *a, number *b) {
	number *ans = (number *)malloc(sizeof(number));
	initNumber(ans);
	addDigit(ans, '1');
	number *n1 = (number *)malloc(sizeof(number));
	initNumber(n1);
	addDigit(n1, '1'); 
	number *n2 = (number *)malloc(sizeof(number));
	initNumber(n2);
	addDigit(n2, '2');
	number *n0 = (number *)malloc(sizeof(number));
	initNumber(n0);
	addDigit(n0, '0');
	number *c = (number *)malloc(sizeof(number));
	initNumber(c);
	copy(b, c);
	int sign = a->sign;
	if(b->sign == PLUS) {
		while(zeroNumber(*c) != 0) {
			ans = mult(ans, a);
			c = sub(c, n1);
		}
	}
	else if(b->sign == MINUS) {
		while(zeroNumber(*c) != 0) {
			ans = division(ans, a);
			c = add(c, n1);
		}
	}
	a->sign = sign;
	if(a->sign == MINUS) {
		number *temp;
		temp = modulus(b, n2);
		if(compareEqual(*temp, *n0) == 0)
			ans->sign = PLUS;
		else
			ans->sign = MINUS;
	}
	return ans;
}
//Function to get the value of 2 PI...... PI is approximately 355/113.
number *TwoPI() {
	number *ans = (number *)malloc(sizeof(number));
	number *n710 = (number *)malloc(sizeof(number));
	number *n113 = (number *)malloc(sizeof(number));
	initNumber(n710);
	initNumber(n113);
	addDigit(n710, '7');
	addDigit(n710, '1');
	addDigit(n710, '0');
	addDigit(n113, '1');
	addDigit(n113, '1');
	addDigit(n113, '3');
	ans = division(n710, n113);
	return ans;
}
//converts number list to double data type.
double convertToDouble(number *a) {
	int i;
	char ch, str[128];
	double ans;
	node *p = a->head;
	
	i = 0;
	while(i != (length(*a) - a->dec)) {
		ch = p->num + '0';
		str[i] = ch;
		p = p->next;
		i++;
	}
	str[i] = '.';
	i++;
	while(p != NULL) {
		ch = p->num + '0';
		str[i] = ch;
		p = p->next;
		i++;
	}
	str[i] = '\0';
	sscanf(str, "%lf", &ans);
	if(a->sign == MINUS) {
		ans = -1 * ans;
	}
	return ans; 
}
//converts double data type in number list.
number *doubletoNumber(double a) {
	number *ans = (number *)malloc(sizeof(number));
	initNumber(ans);
	char str[128];
	sprintf(str, "%lf", a);
	int i = 0;
	if(str[i] == '-') {
		ans->sign = MINUS;
		i++;
	}
	while(str[i] != '\0') {
		if(str[i] == '.') {
			ans->dec = strlen(str) - i - 1;
		}
		else {
			addDigit(ans, str[i]);
		}
		i++;
	}
	return ans;
}
//Sine Operation.
number *sine(number *a) {
	double no, val;
	number *ans = (number *)malloc(sizeof(number));
	number *temp = (number *)malloc(sizeof(number));
	temp = modulus(a, TwoPI());
	no = convertToDouble(temp);
	val = sin(no);
	ans = doubletoNumber(val);
	return ans;	
}
//Cosine Operation.
number *cosine(number *a) {
	double no, val;
	number *ans = (number *)malloc(sizeof(number));
	number *temp = (number *)malloc(sizeof(number));
	temp = modulus(a, TwoPI());
	no = convertToDouble(temp);
	val = cos(no);
	ans = doubletoNumber(val);
	return ans;
}
//Tangent Operation.
number *tangent(number *a) {
	double no, val;
	number *ans = (number *)malloc(sizeof(number));
	number *temp = (number *)malloc(sizeof(number));
	temp = modulus(a, TwoPI());
	no = convertToDouble(temp);
	val = tan(no);
	ans = doubletoNumber(val);
	return ans;
}
