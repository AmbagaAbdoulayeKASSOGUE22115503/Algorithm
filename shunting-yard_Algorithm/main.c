// Need this to use the getline C function on Linux. Works without this on MacOs. Not tested on Windows.
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#include "token.h"
#include "queue.h"
#include "stack.h"


/** Main function for testing.
 * The main function expects one parameter that is the file where expressions to translate are
 * to be read.
 *
 * This file must contain a valid expression on each line
 *
 */

/*libère la mémoire de toutes les valeurs de la fille ainsi que tous ses éléments*/
void deleteQueueToken(Queue *queue)
{
	Token *token=NULL;
	while (!queueEmpty(queue))
		{
			token=queueTop(queue);
			queuePop(queue);
			deleteToken(&token);
		}
	deleteQueue(&queue);
}



/*isSymbol*/
bool isSymbol(char c)
{
	return (c=='+' || c=='-' || c=='*' || c=='/' || c=='^' || c=='(' || c==')');
}

/*printToken*/
void printToken(FILE *f, void *e)
{
	Token *token= (Token *)e;
	tokenDump(f,token);
}

/*stringToTokenQueue*/
Queue *stringToTokenQueue(const char *expression) 
{
	Queue *tokenQueue= createQueue();
	int finEl;
	const char *curpos = expression;
	while (*curpos!='\0')
	{
		while (isspace(*curpos)) ++curpos;

		if(isSymbol(*curpos))
		{
			queuePush(tokenQueue,createTokenFromString(curpos,1));
			++curpos;
		}
		else if(isdigit(*curpos))
		{
			finEl=1;
			while (isdigit(curpos[finEl])) ++finEl;
			queuePush(tokenQueue,createTokenFromString(curpos,finEl));
			curpos+=finEl;
		}
		
	}

	return tokenQueue;
}

/**shuntingYard*/
Queue *shuntingYard(Queue* infix)
{
	Queue *shuntingYardQueue= createQueue();
	Token *token;
	Stack *operatorStack= createStack(0);

	while(!queueEmpty(infix))
	{
		token= queueTop(infix); queuePop(infix);
		if (tokenIsNumber(token)) queuePush(shuntingYardQueue,token);
		else if(tokenIsOperator(token))
		{
			while( !stackEmpty(operatorStack) && ((tokenGetOperatorPriority(stackTop(operatorStack))>tokenGetOperatorPriority(token)) || ( (tokenGetOperatorPriority(stackTop(operatorStack))==tokenGetOperatorPriority(token)) && (tokenOperatorIsLeftAssociative(stackTop(operatorStack))) )) && (tokenGetParenthesisSymbol(token)!='(') ) 
			{
				queuePush(shuntingYardQueue,stackTop(operatorStack));
				stackPop(operatorStack);
			}
			stackPush(operatorStack,token);
		}
		else if(tokenGetParenthesisSymbol(token)=='(')
			stackPush(operatorStack,token);
		else if(tokenGetParenthesisSymbol(token)==')')
		{
			while( !stackEmpty(operatorStack) && tokenGetParenthesisSymbol(stackTop(operatorStack))!='(')
			{
				queuePush(shuntingYardQueue,stackTop(operatorStack));
				stackPop(operatorStack);
			}
			/*on libère la mémoire pour les deux parenthèses tokens*/
			deleteToken(&token);
			token= stackTop(operatorStack);
			deleteToken(&token);
			stackPop(operatorStack);
		}
	}

	
	while(!stackEmpty(operatorStack))
	{
	queuePush(shuntingYardQueue,stackTop(operatorStack));
	stackPop(operatorStack);	
	}

	/*on libère la pile operatorStack*/
	deleteStack(&operatorStack);
	return shuntingYardQueue;
}

/*evaluateOperator*/
Token *evaluateOperator(Token *arg1, Token *op, Token *arg2)
{
	float operande1= tokenGetValue(arg1);
	float operande2= tokenGetValue(arg2);
	switch (tokenGetOperatorSymbol(op))
	{
	case '+':
		return createTokenFromValue(operande1+operande2);
	case '-':
		return createTokenFromValue(operande1-operande2);
	case '/':
		return createTokenFromValue(operande1/operande2);
	case '*':
		return createTokenFromValue(operande1*operande2);
	case '^':
		return createTokenFromValue(pow(operande1,operande2));
	default: 
		perror("erreur evaluateOperator");
		exit(1);
	}
}


/*evaluateExpression*/
float evaluateExpression(Queue* postfix)
{
	Token *token= NULL;
	Stack *operandeStack= createStack(0);
	Queue *tokenToFree= createQueue();
	Token *opr1=NULL; Token *opr2=NULL; Token *result=NULL;
	while (!queueEmpty(postfix))
	{
		token= queueTop(postfix); queuePop(postfix);
		if (tokenIsOperator(token))
		{
			opr2= stackTop(operandeStack); stackPop(operandeStack);
			opr1= stackTop(operandeStack); stackPop(operandeStack);

			result= evaluateOperator(opr1,token,opr2);
			stackPush(operandeStack,result);

			queuePush(tokenToFree,opr1); queuePush(tokenToFree,opr2); queuePush(tokenToFree,token);
		}
		else if (tokenIsNumber(token))
			stackPush(operandeStack,token);
	}

	result= stackTop(operandeStack);  stackPop(operandeStack); queuePush(tokenToFree,result);
	float resultat= tokenGetValue(result); /*valeur à retourner*/
	deleteQueueToken(tokenToFree); deleteStack(&operandeStack);
	return resultat;
}

/*computeExpressions*/
void computeExpressions(FILE *input) {
	char *buffer=NULL;
	size_t readSize=0;
	Queue *tokenQueue;
	Queue *shuntingQueue;
	while (getline(&buffer, &readSize, input)>0)
	{
		tokenQueue= stringToTokenQueue(buffer);
		if (!queueEmpty(tokenQueue))  //si le token queue est vide on a pas besoin de faire l'affichage
		{
			printf("Input    :%s",buffer);  //exo1....
			printf("Infix    :");           //exo2....
			queueDump(stdout,tokenQueue,printToken);
			printf("\n");

			shuntingQueue= shuntingYard(tokenQueue);
			printf("Postfix  :");         //exo3....
			queueDump(stdout,shuntingQueue,printToken);
			printf("\n");

			printf("Evaluate :%f",evaluateExpression(shuntingQueue));

			printf("\n");
			printf("\n");
			
			deleteQueueToken(shuntingQueue);
			free(buffer);
		}

		deleteQueueToken(tokenQueue);
	}
}

int main(int argc, char **argv){
	FILE *input;
	
	if (argc<2) {
		fprintf(stderr,"usage : %s filename\n", argv[0]);
		return 1;
	}
	
	input = fopen(argv[1], "r");

	if ( !input ) {
		perror(argv[1]);
		return 1;
	}

	computeExpressions(input);

	fclose(input);
	return 0;
} 
                                            