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
                      
digraph[ ]*\{         { 
                        estado++; 
                      }

rankdir.*;            {}

node[ ]*\[shape=      { if (tipoDeArchivo == AUTOMATA) { estado = FORMA_I; } }
circle                { 
						if (tipoDeArchivo == AUTOMATA && estado == FORMA_I) { 
							es_final = 0; estado++; 
						} else { REJECT; } 

					  }
doublecircle          { 
						if (tipoDeArchivo == AUTOMATA && estado == FORMA_I) { 
							es_final = 1; estado++; 
						} else { REJECT; } 
					  }
\][ ]*                { if (tipoDeArchivo == AUTOMATA && estado == FORMA_F) { estado = NOMBRE_DE_ESTADO_I; } else { REJECT; } }

[A-Za-z0-9]+[ ]*      { 
						if (tipoDeArchivo == AUTOMATA && estado == NOMBRE_DE_ESTADO_I) 
						{ 
							nombreDeEstado = copiar(yytext);
							estado = ETIQUETA_I; 
						} else { REJECT; } 
					  }
\[label=\"            { if (tipoDeArchivo == AUTOMATA && estado == ETIQUETA_I) { estado = ETIQUETA_F; } 
                   else if (tipoDeArchivo == AUTOMATA && estado == TRANSICION) { estado = TRANSICION_ETIQUETA_I; } else { REJECT; } }
[A-Za-z0-9/\\]+        { 
						if (tipoDeArchivo == AUTOMATA && estado == ETIQUETA_F) 
						{ 
							agregarEstado(automata, quitarEspacios(nombreDeEstado), *yytext-'0', es_final);
							estado = ESTADO_F; 							
						} 
						else if (tipoDeArchivo == AUTOMATA && estado == TRANSICION_ETIQUETA_I) { 
							char * origen = primeraParte(transicion);
							char * destino = segundaParte(transicion);
							agregarTransicion(automata, origen, destino, *yytext);
							estado = TRANSICION_ETIQUETA_F; 
						} else { REJECT; } 
					}
\"][ ]*;              { if (tipoDeArchivo == AUTOMATA && estado == ESTADO_F) { estado = 0; }
                   else if (tipoDeArchivo == AUTOMATA && estado == TRANSICION_ETIQUETA_F) { estado = 0; } else { REJECT; } }

\}                    {  }

[A-Za-z0-9]+[ ]*->[A-Za-z0-9]+[ ]*[ ] { if (tipoDeArchivo == AUTOMATA && estado == 0) { 
							transicion = copiar(yytext);

							estado = TRANSICION; } else { REJECT; } }

(\{|\(|\))?				  {}

%%



int main(int argc, char ** argv) {
  analizarEntrada(argc, argv);
}