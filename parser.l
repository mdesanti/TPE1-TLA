%{

#include "automata/automata.h"

int nodeDeclaration = 0;
int isFinal = 0;
int nodeNumber;
%}
%%

digraph[ ]*\{		{printf("Inicio del archivo\n");}
\/\/.*\n			{printf("Comentario\n");}
rankdir.*;	{}
node[ ]*\[shape=	{nodeDeclaration = 1; printf("Tengo un nodo\n");}
circle				{isFinal = 0; printf("\t Es un estado comun\n");}
doublecircle		{isFinal = 1; printf("\t Es un estado final\n");}
\][ ]*				{printf("Termina un nodo\n");}
Node[0-9][ ]*->[ ]*	{nodeDeclaration = 0; printf("Declaracion de transicion\n");}
Node
\[label=\"			{printf("Comienzo de etiqueta\n");}
[0-9]				{nodeNumber = *yytext - '0'; printf("Numero de nodo %s\n", yytext);}
[a-z]				{nodeNumber = *yytext; printf("Etiqueta %s\n", yytext);}
\"][ ]*;			{if(nodeDeclaration) {
						
						printf("Fin de etiqueta\n");}


%%


int main (int argc, char **argv)
{
  yylex();
  return 0;
}