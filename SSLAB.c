1.a. Write a LEX program to recognize valid arithmetic expression. Identifiers in the
expression could be only integers and operators could be + and *. Count the
identifiers & operators present and print them separately.

%{
    int opnd = 0, ac = 0, sc = 0, mc = 0, dc = 0, valid = 0;
    void ext();
%}

%x OPER
%%

[a-zA-Z0-9]+ {valid = 1; opnd++; BEGIN OPER;}
<OPER>"+" {if(valid == 1) {valid = 0; ac++;} else {ext();}}
<OPER>"-" {if(valid == 1) {valid = 0; sc++;} else {ext();}}
<OPER>"*" {if(valid == 1) {valid = 0; mc++;} else {ext();}}
<OPER>"/" {if(valid == 1) {valid = 0; dc++;} else {ext();}}
<OPER>[a-zA-Z0-9]+ {if (valid == 0) {valid = 1; opnd++;} else {ext();}}
<OPER>. ext();
<OPER>\n return 0;
. ext();
\n return 0;

%%

void ext()
{
    printf("Invalid Expression");
    exit(0);
}
void main()
{
    printf("Enter Expression\n");
    yylex();
    printf("Valid Expression\n");
    printf("Count Operand=%d, Plus=%d, Minus=%d, Multiply=%d, Divide=%d\n", opnd, ac, sc, mc, dc);
}

Output:
>>> lex 01.a.Identify_Expression.l 
>>> gcc lex.yy.c -ll
>>> ./a.out 
Enter Expression
a+b*c
Valid Expression
Count Operand=3, Plus=1, Minus=0, Multiply=1, Divide=0
-----------------------------------------------------------------------------------------------------

1.b. Write YACC program to evaluate arithmetic expression involving operators: +, -, *, and /

LEX Part

%{
    #include "y.tab.h"
    extern int yylval;
%}

%%

[0-9]+ {yylval = atoi(yytext); return NUM;}
[+\-*/] return yytext[0];
. return yytext[0];
\n return 0;

%%

YACC Part

%{
    #include <stdio.h>
    #include <stdlib.h>
    int yylex();
    int yyerror();
%}

%left '+' '-'
%left '*' '/'
%token NUM

%%

expr: stmt {printf("Result=%d\n", $1);}
    ;
stmt: stmt '+' stmt {$$ = $1 + $3;}
    | stmt '-' stmt {$$ = $1 - $3;}
    | stmt '*' stmt {$$ = $1 * $3;}
    | stmt '/' stmt {if($3 != 0) {$$ = $1 / $3;} else {yyerror();}}
    | NUM {$$ = $1;}
    ;

%%
void main()
{
    if(yyparse() == 0)
    {
        // Do Nothing, Result is already printed
    }
    else
    {
        yyerror();
    }
}
int yyerror()
{
    printf("Invalid Expression\n");
    exit(0);
}

Output:
>>> lex 01.b.01.Evaluate_Expression_Lex.l 
>>> yacc -d 01.b.02.Evaluate_Expression_Yacc.y 
>>> gcc lex.yy.c y.tab.c -ll -ly
>>> ./a.out 
2+3*8
Result=26
-----------------------------------------------------------------------------------------------------

2. Develop, Implement and Execute a program using YACC tool to recognize all strings
ending with b preceded by n a’s using the grammar a
n b (note: input n value)

LEX Part

%{
    #include "y.tab.h"
%}

%%

a return A;
b return B;
. return yytext[0];
\n return 0;

%%

YACC Part

%{
    #include <stdio.h>
    #include <stdlib.h>
    int count=0;
    int yylex();
    int yyerror();
%}

%token A B
%%

str: T B
    ;
T: A T {++count;}
    |
    ;

%%
void main()
{
    int n;
    printf("Enter n value\n");
    scanf("%d\n", &n);
    if(yyparse() == 0 && n == count)
    {
        printf("Valid String\n");
    }
    else
    {
        yyerror();
    }
}
int yyerror()
{
    printf("Invalid String\n");
}

Output:
>>> lex 02.01.Recognize_AnB_Lex.l 
>>> yacc -d 02.02.Recognize_AnB_Yacc.y 
>>> gcc lex.yy.c y.tab.c -ll -ly
>>> ./a.out 
Enter n value
5
aaaaab
Valid String
-----------------------------------------------------------------------------------------------------

3. Design, develop and implement YACC/C program to construct Predictive / LL(1)
Parsing Table for the grammar rules: A ?aBa , B ?bB | e. Use this table to parse
the sentence: abba$

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char prod[3][10] = {"A->aBa", "B->bB", "B->@"};
char first[3][10] = {"a", "b", "@"};
char follow[3][10] = {"$", "a", "a"};
char table[3][4][10];
char input[10];
int top=-1;
char stack[25];
char curp[20];

void push(char item)
{
    stack[++top] = item;
}

void pop()
{
    top -= 1;
}

void display()
{
    int i;
    for(i = top; i>=0; i--)
        printf("%c", stack[i]);
}

int numr(char c)
{
    switch(c)
    {
        case 'A':
        case 'a':
                return 1;
        case 'B':
        case 'b':
                return 2;
        case '$':
                return 3;
    }
    return 1;
}

void main()
{
    char c;
    int i, j, k, n;
    for(i = 0; i<3; i++)
    {
        for(j = 0; j<4; j++)
        {
            strcpy(table[i][j], "e");
        }
    }
    
    printf("Grammar:\n");
    for(i=0; i<3; i++)
        printf("%s\n\n", prod[i]);
    
    printf("first={%s, %s, %s}\n", first[0], first[1], first[2]);
    printf("follow={%s, %s}\n", follow[0], follow[1]);
    
    strcpy(table[0][0], " ");
    strcpy(table[0][1], "a");
    strcpy(table[0][2], "b");
    strcpy(table[0][3], "$");
    strcpy(table[1][0], "A");
    strcpy(table[2][0], "B");
    
    for(i=0; i<3; i++)
    {
        if(first[i][0] != '@')
            strcpy(table[numr(prod[i][0])][numr(first[i][0])], prod[i]);
        else
            strcpy(table[numr(prod[i][0])][numr(follow[i][0])], prod[i]);
    }
    
    printf("Enter the input string\n");
    scanf("%s", input);
    
    for(i=0; input[i] != '\0'; i++)
    {
        if(input[i] != 'a' && input[i] != 'b'&& input[i] != '$')
        {
            printf("Invalid String\n");
            exit(0);
        }
    }
    
    if(input[i-1] != '$')
    {
        printf("No End Pointer\n");
        exit(0);
    }
    
    push('$');
    push('A');
    
    i=0;
    printf("\n\n");
    printf("Stack\t\tInput\t\tAction\n");
    while(input[i] != '$' && stack[top] != '$')
    {
        display();
        printf("\t\t%s\t", (input+i));
        if(stack[top] == input[i])
        {
            printf("\tMatched %c\n", input[i]);
            pop();
            i++;
        }
        else
        {
            if(stack[top]>=65 && stack[top] <92)
            {
                strcpy(curp, table[numr(stack[top])][numr(input[i])]);
                if(!strcmp(curp, "e"))
                {
                    printf("Invalid");
                    exit(0);
                }
                else
                {
                    printf("\tApply Production %s\n", curp);
                    if(curp[3] == '@')
                        pop();
                    else
                    {
                        pop();
                        n = strlen(curp);
                        for(j=n-1; j>=3; j--)
                            push(curp[j]);
                    }
                }
            }
        }
    }
    
    display();
    printf("\t\t%s\t\n", (input+i));
    if(stack[top] == '$' && input[i] == '$')
    {
        printf("Valid String\n");
    }
    else
    {
        printf("Invalid String\n");
    }
}

Output:
>>> gcc 03.LL1_Parser.c 
>>> ./a.out 

Grammar:
A->aBa
B->bB
B->@
first={a, b, @}
follow={$, a}
Enter the input string
abba$
Stack		Input		Action
A$		abba$		Apply Production A->aBa
aBa$		abba$		Matched a
Ba$		bba$		Apply Production B->bB
bBa$		bba$		Matched b
Ba$		ba$		Apply Production B->bB
bBa$		ba$		Matched b
Ba$		a$		Apply Production B->@
a$		a$		Matched a
$		$	
Valid String
-----------------------------------------------------------------------------------------------------
4. Design, develop and implement YACC/C program to demonstrate Shift Reduce
Parsing technique for the grammar rules: E →E+T | T, T →T*F | F, F →(E) | id
and parse the sentence: id + id * id. 

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
        printf("\t%s",input);
        printf("\t%s",action);
        printf("\n");
}
void check()
{
        while(1)
        {                     
                if(stack[top]=='T' && stack[top-1]=='+' && stack[top-2]=='E')
                {
                        if(input[i+1]=='*')
                                break;
                }
             	if(stack[top]=='T')
                {
                       if(input[i+1]=='*')
                               break;
                }
                if(stack[top]=='F' && stack[top-1]=='*' && stack[top-2]=='T')
                {
                        strcpy(action,"Reduce T->T*F");
                        stack[--top]=' ';
                        stack[--top]='T';
                        printfn();
                }
                if(stack[top]=='T' && stack[top-1]=='+' && stack[top-2]=='E')
                {
                       strcpy(action,"Reduce E->E+T");
                       stack[--top]=' ';
                       stack[--top]='E';
                       printfn();
                }  
			    if(stack[top]==')' && stack[top-1]=='E' && stack[top-2]=='(')
			    {
			   		 strcpy(action,"Reduce F->(E)");
			   		 stack[--top]=' ';
                     stack[--top]='F';
                     printfn();
			    }
			                 
                if(stack[top]=='d' && stack[top-1]=='i')
                {
                        strcpy(action,"Reduce F->id");
                        stack[--top]='F';
                        printfn();                       
                }       
             	else if(stack[top]=='F')
                {
                       strcpy(action,"Reduce T->F");
                       stack[top]='T';
                       printfn();
                }
                else if(stack[top]=='T')
                {
                       strcpy(action,"Reduce E->T");
                       stack[top]='E';
                       printfn();
                }
                else if(stack[top]=='E' || stack[top]=='*' || stack[top]=='+' || stack[top]=='(' || stack[top]==')')
                {
                        break;
                }              
        }
}
int main()
{
        printf("Enter the sentence\n");
        scanf("%s",input);
        int len=strlen(input);
        input[len] ='$';
        stack[++top]='$';
        printf("Stack\tInput\tAction\n");
        printf("$\t%s\t---\n",input);
        for(i=0;input[i]!='$';i++)
        {
                if(input[i]=='i' && input[i+1]=='d')
                {
                        input[i]=' ';
                        input[i+1] =' ';
                        stack[++top] ='i';
                        stack[++top]='d';
                        i=i+1;
                        strcpy(action,"Shift id");
                        printfn();
                        check();
                }
               else if(input[i]=='*')
                {
                        input[i]=' ';
                        stack[++top]='*';                        
                        strcpy(action,"Shift *");
                        printfn();
                        check();                        
                }
               else if(input[i]=='+')
               {
                       input[i]=' ';
                       stack[++top]='+';
                       strcpy(action,"Shift +");
                       printfn();
                       check();
               }
               else if(input[i]=='('){
               		    input[i]=' ';
                       stack[++top]='(';
                       strcpy(action,"Shift (");
                       printfn();
                       check();
               }
               else if(input[i]==')'){
               			input[i]=' ';
                       stack[++top]=')';
                       strcpy(action,"Shift )");
                       printfn();
                       check();
               }
               else{
               	        break;
               }
        }
        if(stack[top]=='E' && input[i]=='$')
        	strcpy(action,"ACCEPT");
        else
        	strcpy(action,"REJECT");
        printfn();
        return 0;
}

Output:
Grammar
E -> E + T | T
T -> T * F | F
F -> (E) | id 
Enter the input expression:
id+id*id
Stack			Input Dymbol		Action
$			    id+id*id$			--
$id			      +id*id$			Shift id
$F			      +id*id$			F->id
$T			      +id*id$			T->F
$E			      +id*id$			E->T
$E+			       id*id$			Shift +
$E+id			     *id$			Shift id
$E+F			     *id$			F->id
$E+T			     *id$			T->F
$E+T*			      id$			Shift *
$E+T*id			        $			Shift id
$E+T*F			        $			F->id
$E+T			        $			T->T*F
$E			            $			E->E+T
$E			            $			Accept
------------------------------------------------------------------------------------------------

5. Design, develop and implement a C/Java program to generate the machine code using triples for the statement A = -B * (C +D) whose intermediate code in three-address form: 
    T1 = -B 
    T2 = C + D 
    T3 = T1 + T2 
    A = T3

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;

class MachineCode {
    public static void main(String[] args) throws IOException {
        File file = new File("input.txt");
        Scanner scanner = new Scanner(file);
        PrintWriter printWriter = new PrintWriter("output.txt");
        while (scanner.hasNextLine()) {
            String[] fields = scanner.nextLine().split(" ");
            if (fields.length < 4) {
                printWriter.println(String.format("LOAD R1, %s", fields[2]));
                printWriter.println(String.format("STORE %s, R1\n", fields[0]));
            } else if (fields.length == 5) {
                printWriter.println(String.format("LOAD R2, %s", fields[2]));
                printWriter.println(String.format("LOAD R3, %s", fields[4]));
                switch (fields[3]) {
                    case "+":
                        printWriter.println("ADD R1,R3,R2");
                        break;
                    case "-":
                        printWriter.println("SUB R1,R3,R2");
                        break;
                    default:
                        printWriter.println("Unsupported Operator");
                }
                printWriter.println(String.format("STORE %s, R1\n", fields[0]));
            }
        }
        printWriter.close();
    }
}
