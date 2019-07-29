#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include"istack.h"
#include "functions.h"
#include <limits.h>

#define OPERAND	100
#define OPERATOR 200
#define END	300
#define ERR 400

typedef struct token{
	int type;
	number *num;
	char op;
}token;
enum state { NUMBER, DOT, OP, FINISH, ERROR, SPC };
token gettoken(char *expr, int *reset) {
	static int i = 0;
	number *a = (number *)malloc(sizeof(number));
	initNumber(a);
	char currchar;
	static enum state currstate = SPC;
	enum state nextstate;
	token t;
	if(*reset == 1) {
		currstate = SPC;
		*reset = 0;
		i = 0;
		//t.dec = 0;
	}
	while(1) {
		currchar = expr[i];
		switch(currstate) {
			case NUMBER:
				switch(currchar) {
					case '0':case '1':case '2': case '3':
					case '4':case '5':case '6': case '7':
					case '8':case '9':
						nextstate = NUMBER;
						addDigit(a, currchar);
						i++;
						break;
					case '+': case '-': case '*': case '/': case '%': case '(' : case ')' :
					case 'S': case 'C' : case 'T':
						nextstate = OP;
						t.type = OPERAND;
						t.num = a;
						currstate = nextstate;
						i++;
						return t;
						break;
					case '\0':
						nextstate = FINISH;
						t.type = OPERAND;
						t.num = a;
						currstate = nextstate;
						return t;
						break;
					case ' ':
						nextstate = SPC;
						t.type = OPERAND;
						t.num = a;
						currstate = nextstate;
						i++;
						return t;
						break;
					
					case '.':
						nextstate = DOT;
						i++;
						currstate = nextstate;
						break;
					
					default: // anything else
						nextstate = ERROR;
						t.type = OPERAND;
						t.num = a;
						currstate = nextstate;
						return t;
						break;
				}
				break;
			
			case DOT:
				switch(currchar) {
					case '0':case '1':case '2': case '3':
					case '4':case '5':case '6': case '7':
					case '8':case '9':
						addDigit(a, currchar);
						a->dec++;
						nextstate = DOT;
						currstate = nextstate;
						i++;
						break;
					case '+': case '-': case '*': case '/': case '%': case '(' : case ')' :
					case 'S': case 'C' : case 'T':
						nextstate = OP;
						t.type = OPERAND;
						t.num = a;
						currstate = nextstate;
						i++;
						return t;
						break;
					case '\0':
						nextstate = FINISH;
						t.type = OPERAND;
						t.num = a;
						currstate = nextstate;
						return t;
						break;
					case ' ':
						nextstate = SPC;
						t.type = OPERAND;
						t.num = a;
						currstate = nextstate;
						i++;
						return t;
						break;
					
					case '.':
						nextstate = DOT;
						i++;
						currstate = nextstate;
						break;
					
					default:
						nextstate = ERROR;
						t.type = OPERAND;
						t.num = a;
						currstate = nextstate;
						return t;
						break;
				}
				break;
				
			case OP:
				switch(currchar) {
					case '0':case '1':case '2': case '3':
					case '4':case '5':case '6': case '7':
					case '8':case '9':
						t.type = OPERATOR;
						t.op = expr[i - 1];
						nextstate = NUMBER;
						currstate = nextstate;
						//i++;
						return t;
						break;
					case '+': case '-': case '*': case '/': case '%': case '(' : case ')' : 
					case 'S': case 'C' : case 'T':
						nextstate = OP;
						t.type = OPERATOR;
						t.op = expr[i - 1];
						currstate = nextstate;
						i++;
						return t;
						break;
					case '\0':
						nextstate = FINISH;
						t.type = OPERATOR;
						t.op = expr[i - 1];
						currstate = nextstate;
						return t;
						break;
					case ' ':
						nextstate = SPC;
						t.type = OPERATOR;
						t.op = expr[i - 1];
						currstate = nextstate;
						i++;
						return t;
						break;
					
					case '.':
						nextstate = DOT;
						t.type = OPERATOR;
						t.op = expr[i -1];
						currstate = nextstate;
						i++;
						return t;
						break;
					
					default: // anything else
						nextstate = ERROR;
						t.type = OPERATOR;
						t.op = expr[i - 1];
						currstate = nextstate;
						return t;
						break;
				}
				break;
			case FINISH:
				t.type = END;
				return t;
				break;
			case ERROR:
				t.type = ERR;
				return t;
				break;
			case SPC:
				switch(currchar) {
					case '0':case '1':case '2': case '3':
					case '4':case '5':case '6': case '7':
					case '8':case '9':
						//no = currchar - '0';
						addDigit(a, currchar);
						nextstate = NUMBER;
						i++;
						break;
					case '+': case '-': case '*': case '/': case '%': case '(' : case ')' :
					case 'S': case 'C' : case 'T':
						nextstate = OP;
						i++;
						break;
					case '\0':
						nextstate = FINISH;
						break;
					case ' ':
						nextstate = SPC;
						i++;
						break;
					
					case '.':
						nextstate = DOT;
						i++;
						break;
					
					default: // anything else
						nextstate = ERROR;
						break;
				}
				currstate = nextstate;
				break;
		}
	}
}
char ctop(cstack *s) {
	char ch;
	ch = cpop(s);
	cpush(s, ch);
	return ch;
}
int precedance(char o) {
	switch(o) {
		case 'S':
		case 'C':
		case 'T':
			return -1;
			break;
		case ')' :
			return 0;
			break;
		case '+' :
		case '-' :
			return 1;
			break;
		case '*' :
		case '/' :
			return 2;
			break;
		case '%' :
			return 3;
			break;
		case '(' :
			return 4;
			break;
		default :
			return 5;
			break;
	}
	return 5;
}
number *infix(char *exp) {
	token t;
	char curr_op, prev_op, ch;
	int /*setpow = 0, settrigo = 0,*/ cnt_ob = 0, cnt_cb = 0, reset  = 1, curr, prev;
	number *a, *b, *z;
	a = (number *)malloc(sizeof(number));
	b = (number *)malloc(sizeof(number));
	z = (number *)malloc(sizeof(number));
	initNumber(a);
	initNumber(b);
	initNumber(z);
	istack is;
	cstack cs;
	iinit(&is);
	cinit(&cs);
	prev = ERR;
	while(1) {
		t = gettoken(exp, &reset);
		curr = t.type;
		if(curr == prev && prev == OPERAND) {
			//return INT_MIN;
			return NULL;
		}
		if(t.type == OPERAND){
			ipush(&is, t.num);
							/*if(setpow == 1) {
								a = ipop(&is);
								if(t.num->dec != 0)
									return NULL;
								b = power(x, t.num);
								setpow = 0;
								push(&is, b);
							}
							else {
								ipush(&is, t.num);
							}*/
		}
		else if(t.type == OPERATOR){
			curr_op = t.op;
			if(curr_op == '(')
				cnt_ob++;
			if(curr_op == ')')
				cnt_cb++;
			if(cnt_cb > cnt_ob)
				//return INT_MIN;
				return NULL;
			if(!cisempty(&cs)) {
				prev_op = ctop(&cs);
				switch(prev_op) {
					case 'S':
						cpop(&cs);
						a = ipop(&is);
						z = sine(a);
						ipush(&is, z);
						//cpush(&cs, curr_op);
						break;
					case 'C':
						cpop(&cs);
						a = ipop(&is);
						z = cosine(a);
						ipush(&is, z);
						//cpush(&cs, curr_op);
						break;
					case 'T':
						cpop(&cs);
						a = ipop(&is);
						z = tangent(a);
						ipush(&is, z);
						//cpush(&cs, curr_op);
						break;
					default :
						break;
						
				}
				while(precedance(prev_op) >= precedance(curr_op)) {
					prev_op = cpop(&cs);
					if(!iisempty(&is)) {
						a = ipop(&is);
					}
					else{
						cpush(&cs, prev_op);
						break;
					}
					if(!iisempty(&is)) {
						b = ipop(&is);
					}
					else {
						cpush(&cs, prev_op);
						ipush(&is, a);
						break;
					}
					if((prev_op == '/' || prev_op == '%') && a == 0) {
                                                fprintf(stderr, "Mathematical error\n");
                                                //return INT_MIN;
                                        	return NULL;
                                        }
					switch (prev_op) {
						case '+' :
							//z = b + a;
							z = add(a, b);
							ipush(&is, z);
							break;
						case '-' :
							//z = b - a;
							z = sub(b, a);
							ipush(&is, z);
							break;
						case '*' :
							//z = b * a;
							z = mult(b, a);
							ipush(&is, z);
							break;
						case '/' :
							//z = b / a;
							z = division(b, a);
							ipush(&is, z);
							break;
						case '%' :
							//z = b % a;
							z = modulus(b, a);
							ipush(&is, z);
							break;
										/*case '^':
											setpow = 1;
											break;
										case 'S': 
											settrigo = 1;
											break;
										case 'C':
											settrigo = 2;
											break; 
										case 'T':
											settrigo = 3;
											break;
										case '!':
											a = pop(&is);
											//append(w, 1);
											if(a->dec != 0 || a->sign == MINUS)
												return NULL;
											else {	
												push(&is, factorial(a));
											}
											//remov(w, 0);
											break;*/	
											
						case '(' :
							cpush(&cs, prev_op);
							ipush(&is, b);
							ipush(&is, a);
							break;
						default :
							//return INT_MIN;
							return NULL;
					}
					if (prev_op == '(')
						break;
					if(!cisempty(&cs))
						prev_op = ctop(&cs);
					else
						break;
				}
			}
			cpush(&cs, t.op);
			if(curr_op == ')') {
				ch = cpop(&cs);
				cnt_cb--;
				ch = cpop(&cs);
				cnt_ob--;
			}
		}
		else if(t.type == END) {
			if(cnt_ob == cnt_cb) {
				while(!cisempty(&cs)) {
					if(!iisempty(&is)) {
						a = ipop(&is);
					}
					else {
						fprintf(stderr, "Less operands\n");
                                                //return INT_MIN;
                                                return NULL;
					}
					if(!iisempty(&is)) {
						b = ipop(&is);
					}
					else {
						fprintf(stderr, "Less operands\n");
                                                //return INT_MIN;
                                                return NULL;
					}
					ch = cpop(&cs);
					if((ch == '/' || ch == '%') && a == 0) {
						fprintf(stderr, "Mathematical error\n");
						//return INT_MIN;
						return NULL;
					}
					switch(ch) {
						case '+' :
							z = add(b, a);
							ipush(&is, z);
							break;
						case '-' :
							z = sub(b, a);
							ipush(&is, z);
							break;
						case '*' :
							z = mult(b, a);
							ipush(&is, z);
							break;
						case '/' :
							z = division(b, a);
							ipush(&is, z);
							break;
						case '%' :
							z = modulus(b, a);
							ipush(&is, z);
							break;
						default :
							//return INT_MIN;
							return NULL;
					}
				}
			}
			else {
				fprintf(stderr, "Error in Expression\n");
				//return INT_MIN;
				return NULL;
			}
			if(!iisempty(&is)){
				z = ipop(&is);
				if(iisempty(&is))
					return z;
				else {
                                	fprintf(stderr, "Less Operators\n");
                                	//return INT_MIN;
                                	return NULL;
                        	}
			}
			else {
				fprintf(stderr, "Less Operands\n");
				return NULL;
			}	
		}
		else if(t.type == ERR) {
			return NULL;
		}
		prev = curr;
	}
}

void modify(char *line) {
	int i;
	for(i = 0; i < strlen(line); i++) {
		if(line[i] == '(') {
			int j = 1;
			while(1) {
				if(line[i + j] == ' ') {
					j++;
				}
				else {
					break;
				}
			}
			if((line[i + j] == '-') || (line[i + j] == '+')) {
				int temp = strlen(line);
				while(temp >= i + j) {
					line[temp + 1] = line[temp];
					temp--;
				}
				line[i + j] = '0'; 
			} 
		}
	}
}
int readline(char *line, int len) {
	int i;
	char ch;
	i = 0;
	while(i < len - 1) {
		ch = getchar();
		if(ch == '\n') {
			line[i++] = '\0';
			return i - 1;
		}
		else
			line[i++] = ch;
	}
	line[len - 1] = '\0';
	return len - 1;
}
int main(int argc, char *argv[]) {
	printf("\nbc 1.07.1\n");
	printf("Copyright 2018 ---> Shripad Mhetre -> 111708036\n");	
	printf("This is free software with ABSOLUTELY NO WARRANTY.\n\n");
	number *r;
	char line[2048];
	while(readline(line, 2048)) {
		modify(line);
		r = infix(line);
		if(r != NULL)
			printNumber(*r);
		else
			fprintf(stderr, "Error in expression\n");
	}
	return 0;
}
