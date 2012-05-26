%option noyywrap

%{
#include "defs.h"
#include "automata/automata.h"
#include "gramatica/gramatica.h"
#include "libreria.h"

#define TUPLAS      0
#define NOTERMINALES   1
#define TERMINALES       2
#define SIMBOLOINICIAL       3
#define PRODUCCIONES 4

int tipoDeArchivo = 0;
int estado = 0;

automata_t automata;
gramatica_t gramatica;

%}

%%

([ ])				  {}
(\r\n|\/\/.*\r\n) 	  {}
(\t|\n|\/\/.*\n)         {}


(.+)[ ]?=   { if (tipoDeArchivo == GRAMATICA) { estado++; } else { REJECT; } } 

[A-Z\\][ ]*->[ ]*([ ]*[A-Za-z\\]+[ ]*\|)*[ ]*[A-Za-z\\]+[ ]*(,)?       { 

                        if (tipoDeArchivo == GRAMATICA && estado == PRODUCCIONES) {
                        	char * sinLlaves = reemplazar(yytext, "{", "");
                        	char * sinComas = reemplazar(sinLlaves, ",", "");
                        	char * sinEspacios = quitarEspacios(sinComas);
                        	printf("Produccion: %s\n", sinEspacios);
                        	agregarProduccionDesdeCadena(gramatica, sinEspacios);
                        	
                        } else { REJECT; }
                      }

\{[ ]*                {
                        if (tipoDeArchivo == GRAMATICA && estado == TERMINALES) {   
                        } else { REJECT; }
                      }

[ ]*\([ ]*\{[ ]*      { 
                        if (tipoDeArchivo == GRAMATICA && estado == NOTERMINALES) {  
                           
                        } else { REJECT; }
                      }

[A-Z][ ]*(,)          {
                        if (tipoDeArchivo == GRAMATICA && estado == SIMBOLOINICIAL) {
                        	char * sinComas = reemplazar(yytext, ",", "");
                        	char * sinEspacios = quitarEspacios(sinComas);
                        	printf("Simbolo inicial: %s\n", sinEspacios);
                        	setearSimboloIncial(gramatica, *sinEspacios);
							estado++;
                        } else if (tipoDeArchivo == GRAMATICA && estado == NOTERMINALES) {
                        	char * sinComas = reemplazar(yytext, ",", "");
                        	char * sinEspacios = quitarEspacios(sinComas);
                        	printf("No terminal: %s\n", sinEspacios);
                        	agregarNoTerminal(gramatica, *sinEspacios);
                        } else { REJECT; }
                      }

[A-Za-z](,)?          { 
                        if (tipoDeArchivo == GRAMATICA && estado == NOTERMINALES) {
                        	char * sinComas = reemplazar(yytext, ",", "");
                        	char * sinEspacios = quitarEspacios(sinComas);
                        	printf("No terminal: %s\n", sinEspacios);
                        	agregarNoTerminal(gramatica, *sinEspacios);
                        } else if (tipoDeArchivo == GRAMATICA && estado == TERMINALES) { 
                        	char * sinComas = reemplazar(yytext, ",", "");
                        	char * sinEspacios = quitarEspacios(sinComas);
                        	printf("Terminal: %s\n", sinEspacios);
                        	agregarTerminal(gramatica, *sinEspacios);
                        } else { REJECT; }
                      }

\}[ ]*,[ ]*           { 
                        if (tipoDeArchivo == GRAMATICA && (estado == NOTERMINALES || estado == TERMINALES || estado == PRODUCCIONES)) { 
                          estado++;
                        } else { REJECT; }
                      }

[ ]*\}[ ]*\)          {;
                        if (tipoDeArchivo == GRAMATICA && estado == PRODUCCIONES) { 
                        	printf("Termino la gramatica\n");
                        }  else { REJECT; }
                      }    

(\{|\(|\))?				  {}

%%



int main(int argc, char ** argv) {
  analizarEntrada(argc, argv);
}