#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <sstream>


using namespace std;

void revisionImpresion();
void escribirDocumento(string texto);
void lecturaDocumento();
string nombreDocumento;

string codigo;


	//tabla de simbolos
	string TablaSimbolos[] = {"ONION","core","layer","import","func","var","if","while","elif","else","return","print"};
	
	//tipo de dastos estado
	enum TEstado{q0,q1,q2,q3,q4,q5,q6,q7,q8,q9,q10,q11,q12,q13,q14,qx,qf,qe};

		
	//variable estados
	TEstado Estado;

TEstado  definirEstado(string cadena);
bool palabraClave(string cadena);
vector<vector<string> > listaElementos;

string decLine();

string ultimoId(string tipo); // ultima funcion o clase
bool idUtilizado(vector<string> id );// identificador ya esta en uso

int linea;

void guardarIdentificador(string cadena);
bool existefuncion(string cadena);
vector<vector<string> > listaIdentificadores;

bool existeVariable(string cadena);
vector<vector<string> > listaVariables;


void guardarReferencia(string cadena, string tipo);
vector<vector<string> > listaReferencias;


//centinelas
	int CFP=0; //centinela "main"
	int CFA=0; // centinela de apertura de  func, while, if, else ,elif
	

	int CVV=0; //centinela para valor de la variable y valor condicional
	int CVP=0;  //centinela de parentesis dentro de el valor variable y para print
	
	int CCD=0; //centinela limitacion del condicioonal
	
	int CVF=0;  //centinela de variables y funciones utilizados directamente en q7
	
	vector<char> UA; //centinelas ultimo abierto fun,if,elif,else o while
		   char  UC=' '; //centinela ultimo cerrado


int main()
{	
	lecturaDocumento();
	

	
	//variables en general
	char Simbolo;  //un caracter o simbolo a validar en ASCII
	string cadena;

	int i; //posicion del caracter leido en la cadena
	int longitud; //longitud total de la cadena
	
	//valores iniciales
	i=longitud=0;
	Estado = q0;
	longitud = codigo.size();
	cadena=""; 
	linea=1;
		
	//maquina de estados
	
	
	while (longitud > i and Estado != qe){
		
		Simbolo = codigo[i];
	
		
		if(Simbolo=='\n'){linea++;}

		switch (Estado){
			case q0:
				// ONION core, layer
				if ((Simbolo >= 'a' && Simbolo <= 'z' )||(Simbolo >= 'A' && Simbolo <= 'Z' )){
				 	
				 
				 	cadena+=Simbolo;
				 	Estado = q0;
				}
				else if((Simbolo==' ' ||Simbolo=='\n'||Simbolo=='\t')&& cadena==""){
						Estado = q0;
				}
				else{
					
					if(Simbolo==' ' ||Simbolo=='\n'||Simbolo=='\t'){
						
							
							Estado = definirEstado(cadena);
							
								cadena.clear();
								
							
						 
					}
					
					else{
							Estado = qe;
					}
					
				}
				
				
				//
				break;
			
			case q1:
			
				// identificador  de clase
				if (((Simbolo >= 'a' && Simbolo <= 'z' )||(Simbolo >= 'A' && Simbolo <= 'Z' ))&& cadena!="g{}uardada"){
				 	
				 
				 	cadena+=Simbolo;
				 	Estado = q1;
				}
				else if ((Simbolo >= '0' && Simbolo <= '9' )&& cadena!=""){
				 	
				 	cadena+=Simbolo;
				 	Estado = q1;
				}
				else if((Simbolo==' ' ||Simbolo=='\n'||Simbolo=='\t')&& cadena==""){
						Estado = q1;
				}
				
				else{
					
					if((Simbolo==' ' ||Simbolo=='\n'||Simbolo=='\t'||Simbolo=='{') && cadena!="g{}uardada"){
						
							//guardar indentificador
						
							guardarIdentificador(cadena);
							cadena="g{}uardada";
							
							if(Estado==qe){ break;}
							
							else if(Simbolo=='{'){
								
									Estado =q2; 
						cadena.clear();
								
							}
							else{
								
									Estado = q1;
							}	
						 
					}
					else if((Simbolo==' ' ||Simbolo=='\n'||Simbolo=='\t')){
							Estado = q1;
						
					}
					else if(Simbolo=='{' && cadena=="g{}uardada"){ 
					
						Estado =q2; 
						cadena.clear();
					 }
					
					else{
							Estado = qe;
					}
					
				}
				

				
				break;
			
			
			case q2:
			
				
				// posibles son import , var, func , solo se pueden resibir estas claves
			  	if ((Simbolo >= 'a' && Simbolo <= 'z' )||(Simbolo >= 'A' && Simbolo <= 'Z' )){
				 
				 
				 	cadena+=Simbolo;
				 	Estado = q2;
				}
				else if((Simbolo==' ' ||Simbolo=='\n'||Simbolo=='\t')&& cadena==""){
						Estado = q2;
				}
				else{
					
					
					
					if(Simbolo==' ' ||Simbolo=='\n'||Simbolo=='\t'){
						
							
							Estado = definirEstado(cadena);
							
								cadena.clear();
								

						 
					}
					
					else{
						
							Estado = qe;
					}
					
				}
				//
				break;
				
			case q3:
				// estado de import , recibe layer.
				
				
			  	if ((Simbolo >= 'a' && Simbolo <= 'z' )||(Simbolo >= 'A' && Simbolo <= 'Z' )){
				 	
				 
				 	cadena+=Simbolo;
				 	Estado = q3;
				}
				else if((Simbolo==' ' ||Simbolo=='\n'||Simbolo=='\t')&& cadena==""){
						Estado = q3;
				}
				else{
					
					if(Simbolo=='.'){
							
					
							
							Estado = definirEstado(cadena);
							cadena.clear();
							 cout<<cadena<<endl;
					}
				
					else{
							Estado = qe;
					}
					
				}
				//
				break;
				
			case q4:
				//  identificador de import
					
			  	if (((Simbolo >= 'a' && Simbolo <= 'z' )||(Simbolo >= 'A' && Simbolo <= 'Z' ))&& cadena!="g{}uardada"){
				 	
				 
				 	cadena+=Simbolo;
				 	Estado = q4;
				}
				else if ((Simbolo >= '0' && Simbolo <= '9' )&& cadena!=""){
				 	
				 	cadena+=Simbolo;
				 	Estado = q4;
				}
				
				else{
					
				
					if((Simbolo==' ' ||Simbolo=='\n'||Simbolo=='\t'||Simbolo==';') && cadena!="g{}uardada" &&cadena!=""){
						
		
							//guardar indentificador
							guardarIdentificador(cadena);
							cadena="g{}uardada";
							
							if(Estado==qe){ break;}
							
							else  if(Simbolo==';'){ 
							
						
							Estado =qx; 
							cadena.clear();
								 }
								 else{
								 
							Estado = q4;}
										
						 
					}
					else if((Simbolo==' ' ||Simbolo=='\n'||Simbolo=='\t')){
							Estado = q4;
						
					}
					
				
					else if(Simbolo==';' && cadena!=""){ 
						
						Estado =qx; 
						cadena.clear();
					 }
					
					else{
					
							Estado = qe;
					}
					
				}
				//
				break;
				
			case q5:
				//  estado funcion identificador
					
			  	if (((Simbolo >= 'a' && Simbolo <= 'z' )||(Simbolo >= 'A' && Simbolo <= 'Z' ))&& cadena!="g{}uardada"){
				 	
				 
				 	cadena+=Simbolo;
				 	Estado = q5;
				}
				else if ((Simbolo >= '0' && Simbolo <= '9' )&& cadena!=""){
				 	
				 	cadena+=Simbolo;
				 	Estado = q5;
				}
				else if((Simbolo==' ' ||Simbolo=='\n'||Simbolo=='\t')&& cadena==""){
						Estado = q5;
				}
				
				else{
					
					if((Simbolo==' ' ||Simbolo=='\n'|| Simbolo=='('||Simbolo=='\t') && cadena!="g{}uardada"){
						
					
							//guardar indentificador
							guardarIdentificador(cadena);
							cadena="g{}uardada";
							
						    if(Estado==qe){ break;}
							else if(Simbolo=='('){
								Estado =q6; 
								cadena.clear();
							}
							else{
						
							Estado = q5;	}			
						 
					}
					else if((Simbolo==' ' ||Simbolo=='\n'||Simbolo=='\t')){
							Estado = q5;
						
					}
					else if(Simbolo=='(' && cadena=="g{}uardada"){ 
					
						Estado =q6; 
						cadena.clear();
					 }
					
					else{
							Estado = qe;
					}
					
				}
				

				
				break;
				
			case q6:
				//  paramentros de la funcion
						 	
			  	if (((Simbolo >= 'a' && Simbolo <= 'z' )||(Simbolo >= 'A' && Simbolo <= 'Z' ))&& cadena!="g{}uardada"){
				 	
				 	
				 	cadena+=Simbolo;
				 	
				 	Estado = q6;
				}
				else if ((Simbolo >= '0' && Simbolo <= '9' )&& cadena!=""){
				 	
				 	cadena+=Simbolo;
				 	Estado = q6;
				}
				else if((Simbolo==' ' ||Simbolo=='\n'||Simbolo=='\t'||Simbolo==')')&& cadena==""){
					
						if(Simbolo!=')'){
					
						Estado = q6;
							}
							else if(CFP!=0){
								CFP=3;
								Estado = q6;
								cadena="g{}uardada";
							}
				}
				
				else{
				
					
					if((Simbolo==' ' ||Simbolo=='\n'||Simbolo=='\t' || Simbolo==')'||Simbolo==',') && cadena!="g{}uardada" && cadena!=""){
				
							//guardar parametro
						

							guardarIdentificador(cadena);
						
							cadena="g{}uardada";
						
						 if(Estado==qe){ break;}
						
						else if(Simbolo==')'){ 
					    	CFP=3;
				
								 }
					
					else if (Simbolo==',' ){ 
						
						cadena.clear();
						
						}
						
				else if(Estado==qe){ break;}
						
						Estado =q6;
						
					 }
						
						
									
						 
				
					else if(Simbolo==' ' ||Simbolo=='\n'||Simbolo=='\t'){
							Estado = q6;
						
						
					}
					else if(Simbolo==',' && cadena=="g{}uardada" && CFP>1){
						
							cadena.clear();
							Estado = q6;
						
					}
					else if(Simbolo==')' && cadena=="g{}uardada"){ 
						CFP=3;
						Estado =q6; 
						
					 }
					 
					 	else if(Simbolo==':' && cadena=="g{}uardada" && CFP==3){
						 	 
							Estado = q7;
							CFP=2;
							CFA++; 
							UA.push_back('f');
						    cadena.clear();
					 }
					
					else{
				
							Estado = qe;
					}
					
				}
				

				
				break;
				
			// en el interior de la funcion:	
			case q7:
				
				// ENTRADA GENREAL DESPUES DE APERTURA ":"
				// posibles son IF , var, while,print ,return , CADENAS Y DEMAS
				
			  	if ((Simbolo >= 'a' && Simbolo <= 'z' )||(Simbolo >= 'A' && Simbolo <= 'Z' )){
				 	
				 
				 	cadena+=Simbolo;
				 	Estado = q7;
				}
					else if ((Simbolo >= '0' && Simbolo <= '9' )&& cadena!=""){
				 	
				 	cadena+=Simbolo;
				 	Estado = q7;
				}
				else if((Simbolo==' ' ||Simbolo=='\n'||Simbolo=='\t')&& cadena==""){
						Estado = q7;
				}
				else if(Simbolo==';'&& cadena==""){
						Estado = qx;
						CFA--;
						if(!UA.empty()){
						UC=UA[UA.size()-1];
						UA.pop_back(); 
							}
				}
				else{
					
				
					
					if(Simbolo==' ' ||Simbolo=='\n'||Simbolo=='\t'||Simbolo=='('||Simbolo=='.'||Simbolo=='='||Simbolo=='+'||Simbolo=='-'||Simbolo==':'){
					
							Estado = definirEstado(cadena);
								
								if(Simbolo=='('||Simbolo=='.'||Simbolo=='='||Simbolo==':'||Simbolo=='+'||Simbolo=='-'){
									
									i--;
									
									
								}
				
					
									cadena.clear();
						 
					}
					
					else{
				
						
							Estado = qe;
					}
					
				}
				//
				break;
				
				
				case q8:
				
			
				// identificador  de variable 
				if (((Simbolo >= 'a' && Simbolo <= 'z' )||(Simbolo >= 'A' && Simbolo <= 'Z' ))&& cadena!="g{}uardada"){
				 	
				 
				 	cadena+=Simbolo;
				 	Estado = q8;
				}
				else if ((Simbolo >= '0' && Simbolo <= '9' )&& cadena!=""){
				 	
				 	cadena+=Simbolo;
				 	Estado = q8;
				}
				else if((Simbolo==' ' ||Simbolo=='\n'||Simbolo=='\t')&& cadena==""){
						Estado = q8;
				}
				
				else{
					
					if((Simbolo==' ' ||Simbolo=='\n'||Simbolo=='\t'||Simbolo==';'||Simbolo=='=') && cadena!="g{}uardada"){
						
							//guardar indentificador
							guardarIdentificador(cadena);
							cadena="g{}uardada";
							
							if(Estado==qe){ break;}
							
							else if(Simbolo==';'){
								
								Estado = qx;
								
								cadena.clear();
							
							}
							else if(Simbolo=='='){
								//cambiar a numero//////////
								Estado = q12;
								cadena.clear();
							}
							else{
						
							Estado = q8;	}			
						 
					}
					else if((Simbolo==' ' ||Simbolo=='\n'||Simbolo=='\t')){
							Estado = q8;
						
					}
					else if(Simbolo==';' && cadena=="g{}uardada"){ 
						Estado =qx; 
						
						cadena.clear();
					 }
					 
					 	else if(Simbolo=='=' && cadena=="g{}uardada"){ 
							//cambiar a numero//////////
						Estado = q12; 
						cadena.clear();
					 }
					
					else{
							Estado = qe;
					}
					
				}
				

				
				break;
				
				
				case q9:
					
					// inicio de la codicion despues de while,if,elif
		
				if(Simbolo=='('){
					CVP++;
					Estado=q10;
					
				}
				else if(Simbolo==' '||Simbolo=='\n'||Simbolo=='\t'){
					
						Estado=q9;
				}
				
				else{
					
				
					Estado=qe;
				}
				

				break;
				
				
				
			case q10:
				// condicion
				 if(CVV==0&& CCD!=1) {
		   		
		   		
		   	    if(Simbolo=='"'&&CCD!=0){
		   	    	
		   		 	cadena+=Simbolo;
		   			Estado=q10;
		   			CVV=1;
				   }
				else if(Simbolo >= '0' && Simbolo <= '9'){
					
					if(CCD!=0){
				
					cadena+=Simbolo;
		   			Estado=q10;
		   			CVV=2;
		   				}
		   			else if((Simbolo=='1'||Simbolo=='0')&&CCD==0){
		   					CCD=1;
		   					Estado=q10;
		   			        CVV=100;
					   }
					   else{
					   	
					   	Estado=qe;
					   }
					
				}
				else if(Simbolo=='-'&& CCD!=0){
				//numero negativo
				
					cadena+=Simbolo;
		   			Estado=q10;
		   			CVV=2;
					
				}
				
				else if ((Simbolo >= 'a' && Simbolo <= 'z' )||(Simbolo >= 'A' && Simbolo <= 'Z' )){
				
					cadena+=Simbolo;
		   			Estado=q10;
		   			CVV=3;
					
				}
			else if(Simbolo==' '||Simbolo=='\n'||Simbolo=='\t'){
					
						Estado=q10;
				}
			else if(Simbolo=='('){
					
						Estado=q10;
						CVP++;
				}
				
				else{
				
						Estado=qe;
				}
		   	
		   	
		   }
		   else if(CVV==1){
			//strings
		   		if(Simbolo=='"'){
		   		    
					cadena+=Simbolo;
		   			Estado=q10;
		   			CVV=100;
		   			
				   }
				else{
					
					cadena+=Simbolo;
		   			Estado=q10;
					
				}
		   	
		   }
		   else if(CVV==2||CVV==21){
			//numeros
		   		if(Simbolo >= '0' && Simbolo <= '9'){
		   		    
					cadena+=Simbolo;
		   			Estado=q10;
		   			
		   			
				   }
				else if(Simbolo=='.'&& cadena[cadena.size()-1]!='-' && CVV==2){
						cadena+=Simbolo;
		   			    Estado=q10;
		   			    CVV=21;
						
				}
				else{
					CVV=100;
					i--;
		   			Estado=q10;
					
				}
		   	
		   }
		    else if(CVV==3||CVV==30||CVV==31||CVV==32||CVV==33){
			//variable o funcion
			
		   		if(((Simbolo >= 'a' && Simbolo <= 'z' )||(Simbolo >= 'A' && Simbolo <= 'Z' )||(Simbolo >= '0' && Simbolo <= '9'))&&CVV==3){
		   		    
					cadena+=Simbolo;
		   			Estado=q10;
		   			
		   			
				   }
			else if(Simbolo=='(' ){
						
						// existefuncion	
							guardarReferencia(cadena,"funcion");
						
						cadena.clear();
		   			    Estado=q10;
		   			    CVV=30;
		   			   
						
				}
				
				else if(Simbolo=='.'){
					//existeVariable
							guardarReferencia(cadena,"variable");
						
							CVV=3;
		   			        Estado=q10;
		   			        cadena.clear();
						
				
					
					
				}
				
				//funcion
			else if(CVV==30||CVV==31||CVV==32||CVV==33){
				
				//lista de parametros dentro de una funcion
				
				if((Simbolo >= 'a' && Simbolo <= 'z' )||(Simbolo >= 'A' && Simbolo <= 'Z' )){
					cadena+=Simbolo;
					Estado=q10;
					CVV=31;
					
				}
				
				else if ((Simbolo >= '0' && Simbolo <= '9')&&CVV==31){
					
					cadena+=Simbolo;
					Estado=q10;
					
				}
				else if ((Simbolo==' '||Simbolo=='\n'||Simbolo=='\t')&& (CVV==30||CVV==32||CVV==33)){
					
					 Estado=q10;
				}
				else if ((Simbolo==')')&& (CVV==30||CVV==33)){
					
					 Estado=q10;

					 CVV=100;
				}
				else if ((Simbolo==')')&& CVV==31){
					//existeVariable
						guardarReferencia(cadena,"variable");
						
							CVV=100;
		   			       Estado=q10;
						
				
					
			
				}
				
				else if(Simbolo==','&&(CVV==31||CVV==33)){
					
					//existeVariable
						guardarReferencia(cadena,"variable");
						
							CVV=32;
		   			        Estado=q10;
		   			        cadena.clear();
					
					
				}
				else if((Simbolo==' '||Simbolo=='\n'||Simbolo=='\t')&&CVV==31){
					//existeVariable
							guardarReferencia(cadena,"variable");
						
							CVV=33;
		   			        Estado=q10;
		   			        cadena.clear();
						
				
				}
				
				else{
					
					    CVV=0;
						cadena.clear();
						Estado=qe;
					
				}
			
				
				
				
				
				
			}
			//variable
				else{
					//existeVariable(cadena)
					guardarReferencia(cadena,"variable");
						
							CVV=100;
								i--;
		   			       Estado=q10;
			
		   		
					
				}
		   	
		   }
		   
		   else if(CVV==100||CVV==101||CVV==102){
		   
		   	  if((Simbolo=='!'||Simbolo=='='||Simbolo=='<'||Simbolo=='>')&&CVV==100&&CCD<1){
		   	  	  	
		   	  	 
		   	  	  	if(Simbolo=='<'||Simbolo=='>'){
		   	  	  		CVV=102;
		   	  	  		
						   }
						   else{
						   	CVV=101;
							   }
					CCD=2;
		   	  	    Estado=q10;
		   	  	    cadena.clear();
				 }
				 
				 else if((CVV==101||CVV==102)){
				 	
				 	
				 	if(Simbolo=='='){
				 	
				 		CVV=0;
				 		Estado=q10;
					 }
					 else if(CVV==102){
					 	 	
					 	CVV=0;
				 		Estado=q10;
					 }
					 
					 
				 	
				 	
				 }
		       	else if(Simbolo==':'&&CVP==0){
				
				//else if(Simbolo==';' && cadena=="g{}uardada"){ 
					    CVV=0;
						Estado =q11; 
						i--;
						cadena.clear();
						CCD=0;
						
					 }
					 
					else if(Simbolo==' '||Simbolo=='\n'||Simbolo=='\t'){
						 Estado=q10;
						
					}
					else if(Simbolo==')'||CVP>0 ){
						 CVV=100;
						 CVP--;
						 Estado=q10;
						
					}
					
					else {
						
						Estado =qe; 
						cadena.clear();
						
					}
					 
					 
				
			}
			else if(Simbolo==' '||Simbolo=='\n'||Simbolo=='\t'){
						
						 Estado=q10;
						
					}
					
			else {
				
					Estado =qe; 
					cadena.clear();
				
			}
					
		   
		   
		   		
		   	
				
		   	break;
				
				
				;
				
				case q11:
				
					if(Simbolo==':'){
						CFA++;
						Estado=q7;
					}
					else if (Simbolo==' '||Simbolo=='\n'||Simbolo=='\t'){
						
						Estado=q11;
					}
					
					else{
						
						
						Estado=qe;
					}
					
					break;
				
				
				;
	
	
	// interior de la funcion ;
	


			case  q12:
				////llenado variable/////
		
		
		   if(CVV==0) {
		   
		   	    if(Simbolo=='"'){
		   	    	
		   		 	cadena+=Simbolo;
		   			Estado= q12;
		   			CVV=1;
				   }
				else if(Simbolo >= '0' && Simbolo <= '9'){
					
					cadena+=Simbolo;
		   			Estado= q12;
		   			CVV=2;
					
				}
				else if(Simbolo=='-'){
				//numero negativo
					cadena+=Simbolo;
		   			Estado= q12;
		   			CVV=2;
					
				}
				
				else if ((Simbolo >= 'a' && Simbolo <= 'z' )||(Simbolo >= 'A' && Simbolo <= 'Z' )){
				
					cadena+=Simbolo;
		   			Estado= q12;
		   			CVV=3;
					
				}
			else if(Simbolo==' '||Simbolo=='\n'||Simbolo=='\t'){
					
						Estado= q12;
				}
			else if(Simbolo=='('){
					
						Estado= q12;
						CVP++;
				}
				
				else{
					
						Estado=qe;
				}
		   	
		   	
		   }
		   else if(CVV==1){
			//estrings
		   		if(Simbolo=='"'){
		   		    
					cadena+=Simbolo;
		   			Estado= q12;
		   			CVV=100;
		   			
				   }
				else{
					
					cadena+=Simbolo;
		   			Estado= q12;
					
				}
		   	
		   }
		   else if(CVV==2||CVV==21){
			//numeros
		   		if(Simbolo >= '0' && Simbolo <= '9'){
		   		    
					cadena+=Simbolo;
		   			Estado= q12;
		   			
		   			
				   }
				else if(Simbolo=='.'&& cadena[cadena.size()-1]!='-' && CVV==2){
						cadena+=Simbolo;
		   			    Estado= q12;
		   			    CVV=21;
						
				}
				else{
					CVV=100;
					i--;
		   			Estado= q12;
					
				}
		   	
		   }
		    else if(CVV==3||CVV==30||CVV==31||CVV==32||CVV==33){
			//variable o funcion
			
		   		if(((Simbolo >= 'a' && Simbolo <= 'z' )||(Simbolo >= 'A' && Simbolo <= 'Z' )||(Simbolo >= '0' && Simbolo <= '9'))&&CVV==3){
		   		    
					cadena+=Simbolo;
		   			Estado= q12;
		   			
		   			
				   }
			else if(Simbolo=='(' ){
						
						// existefuncion(cadena)	
							guardarReferencia(cadena,"funcion");
						
						cadena.clear();
		   			    Estado= q12;
		   			    CVV=30;
		   			 
						
				}
				
				else if(Simbolo=='.'){
					//existeVariable(cadena)
						guardarReferencia(cadena,"variable");
						
							CVV=3;
		   			        Estado= q12;
		   			        cadena.clear();
						
				
					
					
				}
				
				//funcion
			else if(CVV==30||CVV==31||CVV==32||CVV==33){
				
				//lista de parametros dentro de una funcion
				
				if((Simbolo >= 'a' && Simbolo <= 'z' )||(Simbolo >= 'A' && Simbolo <= 'Z' )){
					cadena+=Simbolo;
					Estado= q12;
					CVV=31;
					
				}
				
				else if ((Simbolo >= '0' && Simbolo <= '9')&&CVV==31){
					
					cadena+=Simbolo;
					Estado= q12;
					
				}
				else if ((Simbolo==' '||Simbolo=='\n'||Simbolo=='\t')&& (CVV==30||CVV==32||CVV==33)){
					
					 Estado= q12;
				}
				else if ((Simbolo==')')&& (CVV==30||CVV==33)){
					
					 Estado= q12;

					 CVV=100;
				}
				else if ((Simbolo==')')&& CVV==31){
					//existeVariable(cadena)
					
					guardarReferencia(cadena,"variable");
						
							CVV=100;
		   			       Estado= q12;
						
				
					
			
				}
				
				else if(Simbolo==','&&(CVV==31||CVV==33)){
					
					//existeVariable(cadena)
					 guardarReferencia(cadena,"variable");
						
							CVV=32;
		   			        Estado= q12;
		   			        cadena.clear();
						
				
					
				}
				else if((Simbolo==' '||Simbolo=='\n'||Simbolo=='\t')&&CVV==31){
					//existeVariable
					 guardarReferencia(cadena,"variable");
					 
							CVV=33;
		   			        Estado= q12;
		   			        cadena.clear();
						
				
				}
				
				else{
					
					    CVV=0;
						cadena.clear();
						Estado=qe;
					
				}
			
				
				
				
				
				
			}
			//variable
				else{
					//existeVariable
							
							guardarReferencia(cadena,"variable");
						
							CVV=100;
								i--;
		   			       Estado= q12;
				
				}
		   	
		   }
		   
		   else if(CVV==100){
		   	
		   	  if(Simbolo=='+'||Simbolo=='-'||Simbolo=='*'||Simbolo=='/'){
		   	  	   CVV=0;
		   	  	   //vector<vector<string> > lista;
		   	  	   Estado= q12;
		   	  	   cadena.clear();
				 }
		       	else if(Simbolo==';'&&CVP==0){
				
				//else if(Simbolo==';' && cadena=="g{}uardada"){ 
					   CVV=0;
						Estado =qx; 
						cadena.clear();
						
					 }
					 
					else if(Simbolo==' '||Simbolo=='\n'||Simbolo=='\t'){
						 Estado= q12;
						
					}
					else if(Simbolo==')'||CVP>0){
						 CVV=100;
						 CVP--;
						 Estado= q12;
						
					}
					
					else {
						
						Estado =qe; 
						cadena.clear();
						
					}
					 
					 
				
			}
			else if(Simbolo==' '||Simbolo=='\n'||Simbolo=='\t'){
						
						 Estado= q12;
						
					}
					
			else {
				
					Estado =qe; 
					cadena.clear();
				
			}
					
		   
		   
		   		
		   	
				
		   	break;
				
			;
			
			
			case q13:
			// cargar  print y return	
				if(Simbolo=='.'&&CVP==10){
						 Estado=q13;
						 CVP=11;
						
					}
				else if(Simbolo=='('&&CVP==11){
					
					     Estado= q12;
						 CVP=1;
				}
				
				else if(Simbolo!=' '){
					   	i--;	
						 Estado= q12;
					
				}
			
			   	break;
				
			;
			
			
			case q14:
				
			// recargar varible o usar funciones  dentro de una funcion
				
				if(Simbolo=='='&&(CVF==1||CVF==2)){
						 Estado= q12;
						 
						 CVF=0;
						
					}
					
				else if((Simbolo=='+'||Simbolo=='-')&&CVF==1){
					
						if(Simbolo=='+'){
					     CVF=30;	
						 }
					     else{
					     	
					     CVF=31;		
						 }
					     
					     Estado=q14;
					
				}
				
				else if(CVF==30||CVF==31){
						
						if(Simbolo=='+'&&CVF==30){
					
					     CVF=100;
					     Estado=q14;
					     	}
					    else if(Simbolo=='-'&&CVF==31){
					
					     CVF=100;
					     Estado=q14;
					     	}
					    else if(Simbolo=='='){
					
					     CVF=0;
					     Estado= q12;
					    }
					    else{
					    	
					    CVF=0;
						Estado=qe;
					    	
						}
					
				}
					
				else if(Simbolo==' '&&(CVF==1||CVF==2)){
					
						 CVF=2;
					     Estado=q14;
				}
				
				else if(Simbolo=='.'&&CVF==1){
					
					   CVF=10;
					    Estado=q14;
				}
				
				else if(CVF==10||CVF==11){
					
					
					if((Simbolo >= 'a' && Simbolo <= 'z' )||(Simbolo >= 'A' && Simbolo <= 'Z' )){
					cadena+=Simbolo;
					Estado=q14;
					CVF=11;
					
				   }
				
				   else if ((Simbolo >= '0' && Simbolo <= '9')&&CVF==11){
					
					cadena+=Simbolo;
				 	Estado=q14;
					
			    	}
				    else if ((Simbolo==' '||Simbolo=='\n'||Simbolo=='\t'||Simbolo=='(')&& CVF==11){
					
					 	
						 
						 
						 	if(Simbolo=='('){
								 	//existefuncion
								guardarReferencia(cadena,"funcion");
								
									cadena.clear();
						         	Estado=q14;
						 			CVF=01;
								
						
								 
							 }
							 
							 else {
							 	
							 	if(existeVariable(cadena)){
					 				
									cadena.clear();
						         	Estado=q14;
						 			CVF=2;
						       	
					 				
								 }
							 }
					 	
			     	}
			     	
			     	else{
			     		
			     		CVF=0;
						cadena.clear();
						Estado=qe;
			     		
					 }
					
					
				}
				
				else if(Simbolo=='(' &&CVF==0){	
						 Estado=q14;
						 CVF=01;
				}
				else if(CVF==01||CVF==02||CVF==03||CVF==04){
				
				//lista de parametros dentro de una funcion
				
				if((Simbolo >= 'a' && Simbolo <= 'z' )||(Simbolo >= 'A' && Simbolo <= 'Z' )){
					cadena+=Simbolo;
					Estado=q14;
					CVF=02;
					
				}
				
				else if ((Simbolo >= '0' && Simbolo <= '9')&&CVF==02){
					
					cadena+=Simbolo;
					Estado=q14;
					
				}
				else if ((Simbolo==' '||Simbolo=='\n'||Simbolo=='\t')&& (CVF==01||CVF==03||CVF==04)){
					
					 Estado= q12;
				}
				else if ((Simbolo==')')&& (CVF==01||CVF==04)){
					
					 Estado=q14;

					CVF=100;
				}
				else if ((Simbolo==')')&& CVF==02){
					//existeVariable
					
					guardarReferencia(cadena,"variable");
						
						CVF=100;
		   			       Estado=q14;
						
					
					
				
					
			
				}
				
				else if(Simbolo==','&&(	CVF==02||CVF==04)){
					
					//existeVariable
						guardarReferencia(cadena,"variable");
				
						
							CVF=03;
		   			        Estado=q14;
		   			        cadena.clear();
						
				
					
				}
				else if((Simbolo==' '||Simbolo=='\n'||Simbolo=='\t')&&CVF==02){
					//existeVariable
					 	guardarReferencia(cadena,"variable");
						
							CVF==04;
		   			        Estado=q14;
		   			        cadena.clear();
						
					
					
				}
				
				else{
					
					   	CVF=02;
						cadena.clear();
						Estado=qe;
					
				}
			
				
				
				
				
				
			}
			else if (CVF==100){
				
				if(Simbolo==';'){
			
					   CVF=0;
						Estado =qx; 
						cadena.clear();
						
					 }
					 
					else if(Simbolo==' '||Simbolo=='\n'||Simbolo=='\t'){
						 Estado=q14;
						
					}
					
					else{
						
						CVF=0;
						cadena.clear();
						 Estado=qe;
					}
			}
				
				else{
					
					Estado=qe;
					
				}
			
			   	break;
				
			;
	//////////////////////////////////////////////////////////////////////////////////////////
				
			case qx:
				// estado de cierre o returno a import,var,func ==q2
				
				if(Simbolo=='}' && CFA==0){ Estado=qf;}
			  
				else if(Simbolo==' '||Simbolo=='\n'||Simbolo=='\t'){ 
				
				    Estado =qx;  
				
				}
				else if(Simbolo==';' && CFA>0){
					
					if(!UA.empty()){
						UC=UA[UA.size()-1];
						UA.pop_back(); 
							}
	
					CFA--;
					Estado =qx;
				
				}
				
				else if (Simbolo >= 'a' && Simbolo <= 'z' ){
						
					
					if(CFA==0)
					{	
					
						//fuera de las funciones
							Estado=q2;
							
						
					
					}else {
						
						//en el interrior de una funcion q7
						  Estado=q7;
					}
				
					i--; //reevaluar el caracter en el siguiente estado
					
				
				}
				
				else{
					
					Estado=qe;
				}
				
			
				//
				break;
				
			case qf:
				// estado de finalizacion
			
		
			  if ((Simbolo >= 'a' && Simbolo <= 'z' )||(Simbolo >= 'A' && Simbolo <= 'Z' )){
				 	
				 
				 	cadena+=Simbolo;
				 	Estado = qf;
				}
				else if((Simbolo==' ' ||Simbolo=='\n'||Simbolo=='\t')&& cadena==""){
						Estado = qf;
						
							
				}
				else{
			
						
					if(Simbolo==' ' ||Simbolo=='\n'||Simbolo=='\t'){
										
					
							Estado = definirEstado(cadena);
							
								cadena.clear();
								
							
						 
					}
					
					else{
					
							Estado = qe;
					}
					
				}
				//
			break;
				
		
		}
		
			// avanza al siguiete caracter
		i++;
	}
	
	
	
	
	
//	resultados

if (Estado== qe){
	
	cout<<"\n Error de Sintaxis "<< decLine()<<endl;
}
else if(Estado==qf){
	
	cout<<endl<<"\n  Sintaxis valida, revisando referencias"<<endl;
}


revisionImpresion();

	
	system("pause");
	return 0;
}

void revisionImpresion(){
	
	string imp;
	int r=0;
	imp+="\n Referencias: ";
	cout<<"\n Referencias:";
	
	// revisar referncias
	int i=0;
	while(i<listaReferencias.size()){
		
		vector<string > tmp=listaReferencias[i];
		
		if (tmp[1]=="variable"){
			
			if(!existeVariable(tmp[0])){
				
				cout<<"\n referencia invalida :"<<tmp[0]<<" "<<tmp[1]<<" "<<tmp[2];
				
				imp+="\n referencia invalida :"+tmp[0]+" "+tmp[1]+" "+tmp[2];
				r++;
			}
		}
		else{
			
				if(!existefuncion(tmp[0])){
					
				cout<<"\n referencia invalida :"<<tmp[0]<<" "<<tmp[1]<<" "<<tmp[2];
				
				imp+="\n referencia invalida :"+tmp[0]+" "+tmp[1]+" "+tmp[2];
					r++;
				}
			
		}
		
		i++;
		
	}
	
	if(r==0){
		cout<<"\n Referencias validas";
	}

	
	imp+="\n\n Datos: ";
	cout<<"\n\n Datos: \n";
	imp+="\n";
	// lista de elmentos
	
	
   i=0;
	while(i<listaElementos.size()){
		
		vector<string > tmp=listaElementos[i];
		
		cout<<"\n Elemeto clave :"<<tmp[0]<<" "<<tmp[1]<<" "<<tmp[2];
				
		imp+="\n Elemeto clave :"+tmp[0]+" "+tmp[1]+" "+tmp[2];
  
		
		i++;
		
	}
	
		cout<<endl;
	imp+="\n";
	
	
		// lista de identificadores , funcion,clase , parametro
	
   i=0;
	while(i<listaIdentificadores.size()){
		
		vector<string > tmp=listaIdentificadores[i];
		
		if(tmp[2]=="funcion"){
			
		cout<<"\n ID :"<<tmp[0]<<" "<<tmp[1]<<" "<<tmp[2]<<" "<<tmp[3]<<" "<<tmp[4]<<" "<<tmp[5];
				
		 imp+="\n ID :"+tmp[0]+" "+tmp[1]+" "+tmp[2]+" "+tmp[3]+" "+tmp[4]+" "+tmp[5];
			
		}
		else{
		
		cout<<"\n ID :"<<tmp[0]<<" "<<tmp[1]<<" "<<tmp[2]<<" "<<tmp[3];
				
		 imp+="\n ID :"+tmp[0]+" "+tmp[1]+" "+tmp[2]+" "+tmp[3];
      }
		
		i++;
		
	}
	
	
		cout<<endl;
	imp+="\n";
	
	
		// lista de variables
	
   i=0;
	while(i<listaVariables.size()){
		
		vector<string > tmp=listaVariables[i];
	
	if(tmp[2]=="local"){
		
		cout<<"\n VARIABLE :"<<tmp[0]<<" "<<tmp[1]<<" "<<tmp[2]<<" "<<tmp[3]<<" "<<tmp[4]<<" "<<tmp[5]<<" "<<tmp[6]<<" "<<tmp[7];
				
		 imp+="\n VARIABLE :"+tmp[0]+" "+tmp[1]+" "+tmp[2]+" "+tmp[3]+" "+tmp[4]+" "+tmp[5]+" "+tmp[6]+" "+tmp[7];
	}
	
	else{
	
		cout<<"\n VARIABLE :"<<tmp[0]<<" "<<tmp[1]<<" "<<tmp[2]<<" "<<tmp[3]<<" "<<tmp[4]<<" "<<tmp[5];
				
		 imp+="\n VARIABLE :"+tmp[0]+" "+tmp[1]+" "+tmp[2]+" "+tmp[3]+" "+tmp[4]+" "+tmp[5];	
		}
	
		i++;
		
	}
	
	cout<<endl;
	imp+="\n";
	
	escribirDocumento(imp);
}


TEstado definirEstado(string cadena){
	
    vector<string> elemento;
	
	if(palabraClave(cadena)){
	
	//estado q7
	
	if(Estado==q7){
		
			if(cadena=="while"){
			
			elemento.push_back(cadena);
			elemento.push_back("Reservado");
			elemento.push_back(decLine());
			listaElementos.push_back(elemento);
			
			UA.push_back('w');
			return q9;
		}
		else if(cadena=="if"){
		
			elemento.push_back(cadena);
			elemento.push_back("Reservado");
			elemento.push_back(decLine());
			listaElementos.push_back(elemento);
			
			UA.push_back('i');
		
			return q9;
		}
		
		else if(cadena=="elif"){
			
			if(UC=='l'||UC=='i'){
		
			elemento.push_back(cadena);
			elemento.push_back("Reservado");
			elemento.push_back(decLine());
			listaElementos.push_back(elemento);
			UA.push_back('l');
			return q9;
			
				}
				else
				{
					return qe;
				}
		}
		
		else if(cadena=="else"){
		
				if(UC=='l'||UC=='i'){
		
			elemento.push_back(cadena);
			elemento.push_back("Reservado");
			elemento.push_back(decLine());
			listaElementos.push_back(elemento);
			UA.push_back('e');
			
			return q11;
			
				}
				else
				{
					return qe;
				}
		}
		
		else if(cadena=="return"){
			
			elemento.push_back(cadena);
			elemento.push_back("Reservado");
			elemento.push_back(decLine());
			listaElementos.push_back(elemento);

			return q13;
		}
		else if(cadena=="print"){
			
			elemento.push_back(cadena);
			elemento.push_back("Reservado");
			elemento.push_back(decLine());
			listaElementos.push_back(elemento);
			CVP=10;
			return q13;
		}
		
		else if(cadena=="var"){
			
			elemento.push_back(cadena);
			elemento.push_back("Reservado");
			elemento.push_back(decLine());
			listaElementos.push_back(elemento);

			return q8;
		}
		
		else{ return qe;}
	}
	
	// estado q3
	else if (Estado==q3){
		
			if(cadena=="layer"){
			
			elemento.push_back(cadena);
			elemento.push_back("Reservado");
			elemento.push_back(decLine());
			listaElementos.push_back(elemento);

			return q4;
		}
		else{
			
			return qe;
		}
	}
	// estados q2
	else if(Estado==q2){
	
			if(cadena=="import"){
			
			elemento.push_back(cadena);
			elemento.push_back("Reservado");
			elemento.push_back(decLine());
			listaElementos.push_back(elemento);

			return q3;
		}
		else if(cadena=="var"){
			
			elemento.push_back(cadena);
			elemento.push_back("Reservado");
			elemento.push_back(decLine());
			listaElementos.push_back(elemento);
	
			return q8;
		}
		
		else if(cadena=="func"){
			
			elemento.push_back(cadena);
			elemento.push_back("Reservado");
			elemento.push_back(decLine());
			listaElementos.push_back(elemento);

			return q5;
		}
		
		else{ return qe;}
		
	}
	
	//para estados q0 y qf
	else if(Estado==q0 || Estado ==qf) {

		if(cadena=="ONION" && listaElementos.empty()){
			
			elemento.push_back(cadena);
			elemento.push_back("Reservado");
			elemento.push_back(decLine());
			listaElementos.push_back(elemento);

			return q0;
		}
		else if (cadena=="core" && listaElementos.size()==1){
			
			elemento.push_back(cadena);
			elemento.push_back(" Reservado");
			elemento.push_back(decLine());
			listaElementos.push_back(elemento);

			return q1;
		}
		
		else if (cadena=="ONION" && Estado==qf){
			
			elemento.push_back(cadena);
			elemento.push_back(" Reservado");
			elemento.push_back(decLine());
			listaElementos.push_back(elemento);
  		
				
			return q0;
		}
		
	else if (cadena=="layer" && Estado==q0 &&listaElementos.size()>=3){
		
	     	elemento.push_back(cadena);
			elemento.push_back(" Reservado"); 
			elemento.push_back(decLine());
			listaElementos.push_back(elemento);
		
			return q1;
	}
					
		else{
			
			return qe;
		}
		
		
	}
	///
	
		}
	else{
		
		if(Estado==q7){
			
			
			if(existeVariable(cadena)){
				
			    CVF=1;
			    return q14;
			 //ES  VARIABLE q14
			}
			
			//debe ser una funcion
			else {
				
				guardarReferencia(cadena,"funcion");
		          return q14;
			}
			
		}
		
		else{
	
		return qe;	}
	}
	
}

bool palabraClave(string cadena){
	
	int i=0;
	int TamanioArreglo = sizeof TablaSimbolos / sizeof TablaSimbolos[0];
	
	while(i<TamanioArreglo){
		
		if(cadena == TablaSimbolos[i]){
			
			return true;
		}
		
		i++;
	}
	
	return false;
}
bool existeVariable(string cadena){
	vector<string> vr;
	int v=0;
	while(listaVariables.size()>v){
	
		vr=listaVariables[v];
		
		if(vr[0]==cadena){
		
			return true;
		}
		
		v++;
	}
	
	if(cadena=="json"||cadena=="true"||cadena=="false"){
		
		return true;
	}
	return false;
	
}

bool existefuncion(string cadena)
{
	vector<string> fc;
	int v=0;
	while(listaIdentificadores.size()>v){
	
		fc=listaIdentificadores[v];
		
		if(fc[0]==cadena&&fc[2]!="clase"){
	
			return true;
		}
		
		v++;
	}
	
	
	return false;
	
}

void guardarReferencia(string cadena, string tipo){
	
		vector<string> ref;
			ref.push_back(cadena);
			ref.push_back(tipo);
			ref.push_back(decLine());
	
		listaReferencias.push_back(ref);	
	
}


void guardarIdentificador(string cadena){
	
	//identificadores ,parametros y variables se guardan aqui
	
	vector<string> Identificador;
	if(palabraClave(cadena)){
		
		cout<<"\n Error Identificador:\n Palabra Reservada "<<cadena<<" "<<decLine()<<endl;
		
		Identificador.push_back(cadena);
		Identificador.push_back("error identificador");
		Identificador.push_back("palabra reservada");
		Identificador.push_back(decLine());
		listaIdentificadores.push_back(Identificador);
		Estado=qe;
		
		}
	else{

	
	if(Estado==q1){
		
		Identificador.push_back(cadena);
		Identificador.push_back("identificador de");
		Identificador.push_back("clase");
		Identificador.push_back(decLine());
		
		
	}
	
	else if(Estado==q4){
		
		Identificador.push_back(cadena);
		Identificador.push_back("identificador de");
		Identificador.push_back("referencia");
		Identificador.push_back(decLine());
		
	}
	
	else if(Estado==q5){
		
		Identificador.push_back(cadena);
		Identificador.push_back("identificador de");
		Identificador.push_back("funcion"); 
		Identificador.push_back("pertenece a la clase");
	    Identificador.push_back(ultimoId("clase"));
		Identificador.push_back(decLine());
		
	}
	
	else if(Estado==q6){
		
		
		if(CFP==0){
			if(cadena=="main"){
				CFP=1;
					Identificador.push_back(cadena);
					Identificador.push_back("parametro de");
					Identificador.push_back("funcion Principal");
					Identificador.push_back(decLine());
					
			}
			else {
				
			 Estado=qe;
			}
			
		}
		else if(cadena=="main"){
		
			        
				Estado=qe;
				
		}
		
		else{	
	
					Identificador.push_back(cadena);
					Identificador.push_back("parametro de");
					Identificador.push_back("funcion definida");
					Identificador.push_back(decLine());
		}
	}
	
	else if(Estado==q8){
		
		if(CFA==0){
			
					Identificador.push_back(cadena);
					Identificador.push_back("variable");
					Identificador.push_back("global");
					Identificador.push_back("pertenece a la clase");
	    			Identificador.push_back(ultimoId("clase"));
					Identificador.push_back(decLine());
			
		}
		else{
			
					Identificador.push_back(cadena);
					Identificador.push_back("variable");
					Identificador.push_back("local");
					Identificador.push_back("pertenece a la funcion");
	    			Identificador.push_back(ultimoId("funcion"));
	    			Identificador.push_back("clase");
	    			Identificador.push_back(ultimoId("clase"));
					Identificador.push_back(decLine());
			
			
		}
		

		
		
		if(!idUtilizado(Identificador)){
				
		 listaVariables.push_back(Identificador);
		 
		}
		
		else{
			
			cout<<"\n Identificador ya utilizado: "<<cadena<<"\n Error en"<<" "<<decLine()<<endl;
		    Identificador[4]="Incorrecto";
		    listaVariables.push_back(Identificador);
		
			
		}
		
	}
	
	if(Estado!=q8){   
	
		if(!idUtilizado(Identificador)){
				
		 listaIdentificadores.push_back(Identificador);	
		 
		}
		
		else{
			
			cout<<"\n Identificador ya utilizado: "<<cadena<<"\n Error en"<<" "<<decLine()<<endl;
		    Identificador[4]="Incorrecto";
		    listaIdentificadores.push_back(Identificador);	
			Estado=qe;
			
		}
		
		
	 
	 }
	
	}
}

string decLine(){
	
	string ln =" Linea #";
	
	 stringstream ss;
	 ss<<ln; 
	 ss<<linea;
	
	return ss.str();
	
	
	
}


bool idUtilizado(vector<string> id ){

	vector<string> tmp;
	if(id[2]=="clase"){
		
		int i=0;
		
		while(i<listaIdentificadores.size()){
			tmp=listaIdentificadores[i];
			
			if(id[0]==tmp[0] && tmp[2]=="clase"){
		
				return true;
			
			}
		
		i++;	
		}
	
		return false;
	}
	else if(id[2]=="funcion"){
		
		int i=0;
		
		while(i<listaIdentificadores.size()){
			tmp=listaIdentificadores[i];
			
			if(id[0]==tmp[0] && tmp[2]=="funcion" && id[4]==tmp[4]){
				
				return true;
			}
		
		i++;	
		}
		
		return false;
	
	}
	
	else if(id[2]=="global"){
		
			int i=0;
		
		while(i<listaVariables.size()){
			tmp=listaVariables[i];
			
			if(id[0]==tmp[0] && tmp[2]=="global" && id[4]==tmp[4]){
				
				return true;
			}
		
		i++;	
		}
		
		return false;
		
	}
	
		else if(id[2]=="local"){
		
			int i=0;
		
		while(i<listaVariables.size()){
			tmp=listaVariables[i];
			
			if(id[0]==tmp[0] && tmp[2]=="local" && id[4]==tmp[4] && id[6]==tmp[6]){
				
				return true;
			}
		
		i++;	
		}
		
		return false;
	}
	
	
}

string ultimoId(string tipo){
	
	vector<string> tmp;

	if(tipo=="clase"){
		
		int i =listaIdentificadores.size()-1;
		
		while(i>=0){
			tmp=listaIdentificadores[i];
			
			if(tmp[2]=="clase"){
			
				return tmp[0];
			}
		
		i--;	
		}
	}
	if(tipo=="funcion"){
		
		int i =listaIdentificadores.size()-1;
		
		while(i>=0){
			tmp=listaIdentificadores[i];
			
			if(tmp[2]=="funcion"){
			
				return tmp[0];
			}
		
		i--;	
		}
	}
	
}

void lecturaDocumento(){
	
	ifstream archivo;
	
	cout<<"Ingrese Nombre Del Archivo :\n";
	getline(cin, nombreDocumento);
	nombreDocumento+=".txt";
	
	archivo.open(nombreDocumento.c_str(),ios::in);//lectura
	
		if(archivo.fail()){
		cout<<"no se abrio el archivo";
		exit(1);
	}
	
		
	string linea;
	while(!archivo.eof()){
	
		getline(archivo,linea); //ingresar los datos dentro del string
		
		 codigo+=linea+"\n"; //guardar codigo en el programa incluye el salto de linea

	}
	archivo.close();
		
	nombreDocumento ="Resultados de "+ nombreDocumento ;
	
		
}

void escribirDocumento(string texto){

	ofstream archivo;
	
	archivo.open(nombreDocumento.c_str(),ios::out);//escritura
	
	if(archivo.fail()){
		cout<<"no se abrio el archivo";
		exit(1);
	}

	archivo <<texto;
	
	archivo.close();
	
}
