%option noyywrap

%{
#include "defs.h"
#include "gramatica/gramatica2.h"
#include "libreria.h"

#define TUPLAS      0
#define NOTERMINALES   1
#define TERMINALES       2
#define SIMBOLOINICIAL       3
#define PRODUCCIONES 4

#define FORMA_I	     1
#define FORMA_F        2
#define NOMBRE_DE_ESTADO_I       3
#define NOMBRE_DE_ESTADO_F         4
#define ETIQUETA_I      5
#define ETIQUETA_F        6
#define ESTADO_F              7

#define TRANSICION            8
#define TRANSICION_ETIQUETA_I      9
#define TRANSICION_ETIQUETA_F        10

int tipoDeArchivo = 0;
int estado = 0;
int es_final = 0;
char * transicion;
char * nombreDeEstado;

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
                        	setearSimboloIncial(gramatica, *sinEspacios);
							estado++;
                        } else if (tipoDeArchivo == GRAMATICA && estado == NOTERMINALES) {
                        	char * sinComas = reemplazar(yytext, ",", "");
                        	char * sinEspacios = quitarEspacios(sinComas);
                        	agregarNoTerminal(gramatica, *sinEspacios);
                        } else { REJECT; }
                      }

[A-Za-z](,)?          { 
                        if (tipoDeArchivo == GRAMATICA && estado == NOTERMINALES) {
                        	char * sinComas = reemplazar(yytext, ",", "");
                        	char * sinEspacios = quitarEspacios(sinComas);
                        	agregarNoTerminal(gramatica, *sinEspacios);
                        } else if (tipoDeArchivo == GRAMATICA && estado == TERMINALES) { 
                        	char * sinComas = reemplazar(yytext, ",", "");
                        	char * sinEspacios = quitarEspacios(sinComas);
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
                        }  else { REJECT; }
                      }
(\{|\(|\))?          {}
                      
%%



int main(int argc, char ** argv) {
  analizarEntrada(argc, argv);
}