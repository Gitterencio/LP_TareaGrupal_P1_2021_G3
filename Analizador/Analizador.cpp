#include <iostream>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>


using namespace std;

void escribirDocumento(string texto);
void lecturaDocumento();
string nombreDocumento;

string codigo;

vector<vector<string> > listaElementos;
vector<string> elemento;

int main()
{	
	lecturaDocumento();
	
	cout<< codigo;
	
	escribirDocumento(codigo);
	
	return 0;
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
