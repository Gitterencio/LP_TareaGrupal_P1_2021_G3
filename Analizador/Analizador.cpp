#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <sstream>


using namespace std;

void escribirDocumento(string texto);
void lecturaDocumento();
string nombreDocumento;

string codigo;


	//tabla de simbolos
	string TablaSimbolos[] = {"ONION","core","layer","import","var","func"};
	
	//tipo de dastos estado
	enum TEstado{q0,q1,q2,q3,q4,q5,qe,qx,qf};
	
		
	//variable estados
	TEstado Estado;

TEstado  definirEstado(string cadena);
bool palabraClave(string cadena);

string decLine();
int linea;

vector<vector<string> > listaElementos;

void guardarVariable(string cadena);
vector<vector<string> > listaVariables;


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
				//
				if ((Simbolo >= 'a' && Simbolo <= 'z' )||(Simbolo >= 'A' && Simbolo <= 'Z' )){
				 	
				 
				 	cadena+=Simbolo;
				 	Estado = q0;
				}
				else if((Simbolo==' ' ||Simbolo=='\n')&& cadena==""){
						Estado = q0;
				}
				else{
					
					if(Simbolo==' ' ||Simbolo=='\n'){
						
							
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
				
			
				// identificador 
				if (((Simbolo >= 'a' && Simbolo <= 'z' )||(Simbolo >= 'A' && Simbolo <= 'Z' ))&& cadena!="g{}uardada"){
				 	
				 
				 	cadena+=Simbolo;
				 	Estado = q1;
				}
				else if ((Simbolo >= '0' && Simbolo <= '9' )&& cadena!=""){
				 	
				 	cadena+=Simbolo;
				 	Estado = q1;
				}
				else if((Simbolo==' ' ||Simbolo=='\n')&& cadena==""){
						Estado = q1;
				}
				
				else{
					
					if((Simbolo==' ' ||Simbolo=='\n') && cadena!="g{}uardada"){
						
							//guardar indentificador
							guardarVariable(cadena);
							cadena="g{}uardada";
							Estado = q1;			
						 
					}
					else if((Simbolo==' ' ||Simbolo=='\n')){
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
				else if((Simbolo==' ' ||Simbolo=='\n')&& cadena==""){
						Estado = q2;
				}
				else{
					
					
					
					if(Simbolo==' ' ||Simbolo=='\n'){
						
						
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
				// estado de import ,replicando q0
				
				
			  	if ((Simbolo >= 'a' && Simbolo <= 'z' )||(Simbolo >= 'A' && Simbolo <= 'Z' )){
				 	
				 
				 	cadena+=Simbolo;
				 	Estado = q3;
				}
				else if((Simbolo==' ' ||Simbolo=='\n')&& cadena==""){
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
				// replicando q1
					
			  	if (((Simbolo >= 'a' && Simbolo <= 'z' )||(Simbolo >= 'A' && Simbolo <= 'Z' ))&& cadena!="g{}uardada"){
				 	
				 
				 	cadena+=Simbolo;
				 	Estado = q4;
				}
				else if ((Simbolo >= '0' && Simbolo <= '9' )&& cadena!=""){
				 	
				 	cadena+=Simbolo;
				 	Estado = q4;
				}
				
				else{
					
				
					if((Simbolo==' ' ||Simbolo=='\n') && cadena!="g{}uardada"){
						
						
							//guardar indentificador
							guardarVariable(cadena);
							cadena="g{}uardada";
							Estado = q4;
										
						 
					}
					else if((Simbolo==' ' ||Simbolo=='\n')){
							Estado = q4;
						
					}
					
				
					else if(Simbolo==';' && cadena!=""){ 
						
						Estado =qx; 
						cadena.clear();
					 }
					
					else{
							cout<<endl<<cadena<<endl;
					cout<<Simbolo;
							Estado = qe;
					}
					
				}
				//
				break;
				
			case qx:
				// estado de cierre
			  
				if(Simbolo==';'){ Estado =qx;  }
				
				else if(Simbolo=='}'){ Estado =qf;}
				//
				break;
				
			case qf:
				// estado de finalizacion
			
		
			  if ((Simbolo >= 'a' && Simbolo <= 'z' )||(Simbolo >= 'A' && Simbolo <= 'Z' )){
				 	
				 
				 	cadena+=Simbolo;
				 	Estado = qf;
				}
				else if((Simbolo==' ' ||Simbolo=='\n')&& cadena==""){
						Estado = qf;
						
							
				}
				else{
			
						
					if(Simbolo==' ' ||Simbolo=='\n'){
										
					
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
	
	
	// analisis de resultados
	
/*	if (Estado== q1){
		cout <<"Cadena aceptada" << endl;
		//buscar en la tabla de simbolos
		int j;
		int TamanioArreglo = sizeof TablaSimbolos / sizeof TablaSimbolos[0];
		for(j=0;j<TamanioArreglo;j++){
			if(Cadena.compare(TablaSimbolos[j]) == 0){
				cout << "palabra reservada: " << Cadena;
				break;	
			}
		}
	}
	else
		cout <<"Cadena NO valida";
	*/
	
	
	//salidas no
	
	//cout<< codigo;
	
//	escribirDocumento(codigo);

if (Estado== qe){
	
	cout<<"sin apertura \n";
}
else if(Estado==qf){
	
	cout<<endl<<"si hay \n";
}

else if(Estado==q1){
	
	cout<<"si hay q1 \n";
}
	
	system("pause");
	return 0;
}


TEstado definirEstado(string cadena){
	
    vector<string> elemento;
	
	if(palabraClave(cadena)){
	
	// estado q3
	if (Estado==q3){
		
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

			return q3;
		}
		
		else if(cadena=="func"){
			
			elemento.push_back(cadena);
			elemento.push_back("Reservado");
			elemento.push_back(decLine());
			listaElementos.push_back(elemento);

			return q3;
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
		
		return qe;
	}
	
}

bool palabraClave(string cadena){
	
	int i=0;
	int TamanioArreglo = sizeof TablaSimbolos / sizeof TablaSimbolos[0];
	
	while(i<TamanioArreglo){
		
		if(cadena == TablaSimbolos[i]){
			
				cout<<"palabra clave" <<endl;
			return true;
		}
		
		i++;
	}
	
	return false;
}

void guardarVariable(string cadena){
	
	vector<string> variable;

	
	if(Estado==q1){
		
		variable.push_back(cadena);
		variable.push_back(" identificador");
		variable.push_back(" clase");
		variable.push_back(decLine());
		
	}
	
	else if(Estado==q4){
		
		variable.push_back(cadena);
		variable.push_back(" identificador");
		variable.push_back(" referencia");
		variable.push_back(decLine());
		
	}
	
	
	
	 listaVariables.push_back(variable);
	 //pruebas
	 int i= 0;
	 while(i<variable.size()){
	 	
	 	cout<< variable[i];
	 	i++;
	 }
	
}

string decLine(){
	
	string ln =" Linea #";
	
	 stringstream ss;
	 ss<<ln; 
	 ss<<linea;
	
	return ss.str();
	
	
	
}

void lecturaDocumento(){
	
	ifstream archivo;
	
	cout<<"nombre del archivo \n";
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
