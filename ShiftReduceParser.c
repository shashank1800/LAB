#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int top=-1;
int i=0;
char input[20];
char stack[20];
char action[20];

void printfn()
{
	int j;
	for(j=0;j<=top;j++)
    		printf("%c",stack[j]);
	printf("\t\t%s",input);
	printf("\t\t%s\n",action);
}

void check(){

	while(1){
		if(stack[top] =='T' && stack[top-1]=='+' && stack[top-2]=='E' && input[i+1]!='*')
		{
			stack[--top]=' ';
			stack[--top]='E';
			strcpy(action,"E->E+T");
			printfn();
			check();
		}
		if(stack[top] =='F' && stack[top-1]=='*' && stack[top-2]=='T' && input[i+1]!='*')
		{
			stack[--top]=' ';
			stack[--top]='T';
			strcpy(action,"E->T+F");
			printfn();
			check();
		}
		if(stack[top]==')' && stack[top-1]=='E' && stack[top-2]=='('){
			stack[--top]=' ';
			stack[--top]='F';
			strcpy(action,"F->(E)");
			printfn();
			check();
		}
		if(stack[top]=='d' && stack[top-1]=='i'){
			stack[--top]='F';
			strcpy(action,"F->id");
			printfn();
			check();
		}
		if(stack[top]=='F'){
			stack[top]='T';
			strcpy(action,"T->F");
			printfn();
			check();
		}
		if(stack[top]=='T' && input[i+1]!='*' ){
			stack[top]='E';
			strcpy(action,"E->T");
			printfn();
			check();
		}
		else
			break;
	}
}

void main(){
	
	int len;
	printf("Enter the input expression\n");
	scanf("%s",input);
	
	len = strlen(input);
	input[len]='$';
	stack[++top] = '$';
	
	printf("Stack\t\t Input \t\t Action\n");
	printf("$\t\t %s \t\t --\n",input);
	
	for(i=0;input[i]!='$';i++)
	{
		if(input[i]=='i' && input[i+1]=='d'){
			input[i] =' ';
			input[i+1]= ' ';
			i++;
			stack[++top] = 'i';
			stack[++top] = 'd';
			strcpy(action,"Shift id");
			printfn();
			check();
			
		}
		if(input[i]=='*'){
			input[i] =' ';
			stack[++top] = '*';
			strcpy(action,"Shift *");
			printfn();
			check();
		}
		if(input[i]=='+'){
			input[i] =' ';
			stack[++top] = '+';
			strcpy(action,"Shift +");
			printfn();
			check();
		}
		if(input[i]=='('){
			input[i] =' ';
			stack[++top] = '(';
			strcpy(action,"Shift (");
			printfn();
			check();
		}
		if(input[i]==')'){
			input[i] =' ';
			stack[++top] = ')';
			strcpy(action,"Shift )");
			printfn();
			check();
		}
		
	}
	
	if(stack[top] =='E' && input[i] =='$'){
		strcpy(action,"Accept");
		printfn();
	}else{
		strcpy(action,"Reject");
		printfn();
	}
	
}
