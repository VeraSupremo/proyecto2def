#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string.h>
#include <cstdlib>
#include <windows.h>
#include <queue>

using namespace std;

bool Jsupremo= false;     //esto verificara que exista solo 1 guardian de 100 y solo 3 de 90 en la lectura.
int maestros = 0;
struct ciudades{    //       la estructua ciudades se utilizara para la creacion de un grafo no definido luego se agregaran sus funciones
	string nombreC;
	string Nconexion;
	vector<ciudades* >caminoAdyacente; 
};
 struct Guardian{
 string nombre;
 int lvl;
 string maestro;
 string ciudad;
 Guardian* izquierda;
 Guardian* derecha;
  vector<Guardian *> aprendices;
  vector<Guardian *> candidatos;
};

//se crearan las funciones del arbol binario para colocar el ranking por niveles (k pro)
struct nodo{
 int dato_nivel;
 nodo* izquierda;
 nodo* derecha;
};
//estructuraarbol general

struct ArbolGeneral{
	//esta estructura solo debera de mostrar al guardian de 100 de vidas como el de jerarquia maxima
	// luego mostrara a sus 3 aprendices con 90 o 99 puntos de combate y por ultimo se mostrara a cada aprendiz de cada uno de ellos o se dejara 
	//solo el top 3
    Guardian* supremo;  // Nodo del guardián de nivel 100
};


Guardian* crearNodo(int dato_nivel){

    // Guardian* nuevo_guardia= new Guardian;
 // nuevo_guardia->lvl = dato_nivel;

 Guardian* nuevo_nodo = new Guardian;
 nuevo_nodo->lvl = dato_nivel;
	
 nuevo_nodo->derecha = NULL;
 nuevo_nodo->izquierda = NULL;
    
 return nuevo_nodo;
	
}

void imprimir_arbol_binario(Guardian* raiz){
 if(raiz == NULL){
  return;
 }

 cout << raiz->lvl << " ";
	
 imprimir_arbol_binario(raiz->izquierda);
 imprimir_arbol_binario(raiz->derecha);    
}

void agregar(Guardian*& raiz, int ingreso) {
 
 if(raiz == NULL) {
 	 raiz = crearNodo(ingreso);
     return;
 }

 if(ingreso > raiz->lvl) { //aca si el guardian tiene un nivel menor se ira a l aizquierda sino a la derecha 
  // raiz->izquierda = agregar(raiz->izquierda, ingreso); // aqui estas modificando la el lado de la raiz, pierdes conexiones. Conectas la raiz con el ultimo, no tiene sentido. 
        agregar(raiz->derecha, ingreso); 
 } else {
        agregar(raiz->izquierda, ingreso); 
 } 
}


//se crearan las distintas formasde recorrer el arbol



void rPreOR(Guardian* raiz){
	if(raiz == NULL){
		 return;
	}else{
		cout<<raiz->lvl<<"-";
		rPreOR(raiz->izquierda);
		rPreOR(raiz->derecha);
	}
	
}
void rInOr(Guardian* raiz){
	if(raiz == NULL){
		 return;
	}else{
		rInOr(raiz->izquierda);
		cout<<raiz->lvl<<"-";
		rInOr(raiz->derecha);		
	}
}
void rPsOr(Guardian* raiz){
	if(raiz== NULL){
		 return;
	}else{
		 rPsOr(raiz->izquierda);
		 rPsOr(raiz->derecha);
		 cout<<raiz->lvl<<"-";
	}
	
}


//eliminar guardian
Guardian* borrarnodo(Guardian* raiz, int nivel){
	int borrador = nivel;
	if(raiz == NULL){
		cout<<"NO EXISTE"<<std::endl;
		return raiz;
	}else if(borrador < raiz->lvl){
		raiz->izquierda = borrarnodo(raiz->izquierda,borrador);
	}else if(borrador> raiz->lvl){
		raiz->derecha = borrarnodo(raiz->derecha,borrador);
	}else{
		
		if(raiz->derecha == NULL && raiz->derecha == NULL){
			delete raiz;
			raiz = NULL;
			
		}else if(raiz->izquierda == NULL){
			Guardian* temp= raiz;
			raiz = raiz->derecha;
			delete temp;
		}else if(raiz->derecha ==NULL){
			Guardian* temp = raiz;
			raiz = raiz->izquierda;
			cout<<"baneando a :"<<temp->lvl<<endl;
			delete temp;
		}else{
			
			Guardian* temp 	= raiz->derecha;
			while(temp->izquierda != NULL){
				temp = temp->izquierda;
			}
			raiz->lvl = temp->lvl;
			raiz->derecha = borrarnodo(raiz->derecha, temp->lvl);
		}
		
		
	}
}
//ahora se creara una funcion que llamara a la funcion eliminar
void LLamadoBan(Guardian*&raiz, int nivel){
	raiz = borrarnodo(raiz,nivel);
}

//buscar guardian
Guardian* BuscarGuardian(Guardian* raiz,int nivel){

	if (raiz == NULL || raiz->lvl == nivel){
        return raiz;
    }else{
    	if(nivel < raiz->lvl){
			return BuscarGuardian(raiz->izquierda,nivel);
		}
		if(nivel > raiz->lvl){
			return BuscarGuardian(raiz->derecha,nivel);
		}
    	
	}

	
}
//ahora se creara una funcion dedicada a mostrar al guardian en caso de que se halla encontrado
void MostrarGbuscado(Guardian* raiz, int nivel){
	Guardian* hallado = BuscarGuardian(raiz,nivel);//   buscar solucion a estoooooooooooooooooooooo
	if(hallado != NULL){
		cout<<"Se ha hallado al guardian:"<<endl<<"NOMBRE: "<<hallado->nombre<<endl<<"NIVEL:"<<hallado->lvl<<endl;
		cout<<"MAESTRO:"<<hallado->maestro<<endl<<"CIUDAD:"<<hallado->ciudad<<endl;
	}else{
		cout<<"\n\nGuardian no hallado lo siento, intente mas tarde\n"<<endl;
	}
	
	
	delete hallado;
}




//funcion para leer archivos

int leerArchivo_arbolBinario(const string& nombreArchivo, Guardian* raiz_arbol) {
    ifstream archivo("archivo.txt");    

    if (!archivo) {
        cerr << "No se pudo abrir el archivo." << endl;
        return -1;
    } 
    string linea;
    while (getline(archivo, linea)) {
        Guardian* nuevoGuardian = new Guardian;

        istringstream ss(linea);
        string token;

        //nombre
        if (getline(ss, token, ',')) {
            nuevoGuardian->nombre = token;
        } else {
            cerr << "Error al leer el nombre." << endl;
            return -1;
        }

        //nivel
        if (getline(ss, token, ',')) {
            stringstream(token) >> nuevoGuardian->lvl;
        if(nuevoGuardian->lvl == 100){
        	if(Jsupremo == false){
        		Jsupremo = true;
			}else{
				return -1;
			}
		}
		if(nuevoGuardian->lvl >= 101){
			return -1;
		}
		if(nuevoGuardian->lvl <= 99 && nuevoGuardian->lvl >= 90){
			
        	if(maestros >= 0 && maestros < 3){
        		maestros ++;
			}else{
				cout<<"TIENE MAS DE 3 GUARDIANES CON 90 DE VIDA"<<std::endl;
				return -1;
			}
		}
        }else{
            cerr << "Error al leer el nivel." << endl;
            return -1;
        }

        //maestro
        if (getline(ss, token, ',')) {
            nuevoGuardian->maestro = token;
            if (nuevoGuardian->maestro == nuevoGuardian->nombre){
                cout << "NO PUEDES SER TU MAESTRO. REVISA EL ARCHIVO" << std::endl;
                return -1;
            }
        } else {
            cerr<<"Error al leer el maestro."<<endl;
            return -1;
        }
        if (getline(ss, token, ',')) {
            nuevoGuardian->ciudad = token;
        } else {
            cerr<<"Error al leer la ciudad."<<endl;
            return -1;
        }

        cout<<nuevoGuardian->nombre << ", " << nuevoGuardian->lvl << ", " << nuevoGuardian->maestro << ", " << nuevoGuardian->ciudad << endl;
        agregar(raiz_arbol, nuevoGuardian->lvl);
        agregarAlArbolGeneral(raiz_arbol, nuevoGuardian->lvl);

    }
      

    archivo.close(); 
	return 0;   
}


//ahora se agregaran las funciones de buscar y luego la de eliminar(esta eliminara cuando el puntaje sea 0 :D )

Guardian* BusquedaAmp(ArbolGeneral* raiz, int nivel){
	if(raiz == NULL || raiz->supremo == NULL){
		return 0;
	}
	queue<Guardian*> cols;
//	std::unordered_set<Guardian*> envista;
	
	cols.push(raiz->supremo);
//	envista.insert(raiz->supremo);  //marca que ya se vio al con mayor puntaje
	while(!cols.empty()){
		Guardian* gactual= cols.front();
		cols.pop();
		
		
		if(gactual->lvl == nivel){
			cout<<gactual->nombre<<endl<<gactual->lvl<<endl;
			return 0;
		}
		for(int i = 0; 	i < gactual->aprendices.size(); i++){
		/*	if(BusquedaAmp(gactual,nivel)){
				cout<<gactual->nombre<<endl<<gactual->lvl<<endl;
				return gactual;
			}else{
				cols.push(Guardian->aprendices);
			}*/
			Guardian* aprendiz = new Guardian;
			cols.push(aprendiz);
		}
		
		for(int i = 0; 	i < gactual->candidatos.size(); i++){
			/*if(BusquedaAmp(gactual,nivel)){
				cout<<gactual->nombre<<endl<<gactual->lvl<<endl;
				return gactual;
			}else{
				cols.push(Guardian->candidatos);
			}*/
			Guardian* candidato = new Guardian;
			cols.push(candidato);
		}
		return NULL;
	}
	
	
	
}






//              grafos
/*
int VerCiudad(const vector<ciudades>& grafo_ciudades, const string& nombre_ciudad){
    for (int i = 0; i < grafo_ciudades.size(); ++i) {
        if (grafo_ciudades[i].nombreC == nombre_ciudad) {
            return i;
        }
    }
    return -1; // Si no se encuentra la ciudad
}
void AgregarNodo(vector<vector<nodo> > & grafo, const vector<ciudades> & grafo_ciudades, const string& nombre_origen, const string& nombre_destino) {
   int indice_origen = ObtenerIndiceCiudad(grafo_ciudades, nombre_origen);
   int indice_destino = ObtenerIndiceCiudad(grafo_ciudades, nombre_destino);
   
   
   
  
}*/




int Leerciudad(const string & nombreArchivo, Guardian *&raiz_arbol) {
    ifstream archivo("citys.txt");    //aca se dberan leer las ciudades :D

    if (!archivo) {
        cerr << "No se pudo abrir el archivo." << endl;
        return -1;
    } 
    string linea;
    vector<ciudades> ciudadesG;
    while (getline(archivo, linea)) {
    	ciudades nueva_ciudad;
    	istringstream ss(linea);
        string token2;

       if (getline(ss, token2, ',')) {        //lector ciudad 
            nueva_ciudad.nombreC = token2;
        } else {
            cerr << "Error al leer el nombre." << endl;
            return -1;
        }
        
        if (getline(ss, token2, ',')) {        //lector ciudad 
            nueva_ciudad.Nconexion = token2;
        } else {
            cerr << "Error al leer el nombre." << endl;
            return -1;
        }
       
    }
    
    archivo.close(); 
	return 0;   
}


//funciones del A general

void agregarAlArbolGeneral(ArbolGeneral* arbol, Guardian* nuevoGuardian){
    if (nuevoGuardian->lvl == 100){
        if (arbol->supremo == NULL){
            arbol->supremo = nuevoGuardian;
        } else{
            // Manejar el caso donde ya hay un supremo
            cout<<"YA EXISTE UN GUARDIAN SUPREMO"<<std::endl;
        }
    } else if(nuevoGuardian->lvl >= 90 && nuevoGuardian->lvl <= 99){
        if(maestros >= 0 && maestros < 3){
            // Agregar al árbol general
            arbol->supremo->aprendices.push_back(nuevoGuardian);
            maestros++;
        }else{
            cout <<"El top de guardianes está lleno, seras posicionado en candidatos"<<std::endl;
            arbol->supremo->candidatos.push_back(nuevoGuardian);
        }
    }
    
}

void agregarAprendices(Guardian* raiz){
	if(raiz == NULL){
		return;
	}else{
		for(int i = 0; i < raiz->aprendices.size(); i++){
			agregarAprendices(raiz);
		}
	}
}




int main(){ 
	int eliminarnivel = 0;      
    Guardian* raiz_arbol = NULL;  
//	ArbolGeneral* arbol = NULL;  
    int aseguradora = leerArchivo_arbolBinario("archivo.txt",raiz_arbol);
    if(aseguradora == 0){
    	cout<<"Analisis de datos de guardianes terminados no se encontraron fallas"<<std::endl;
	}else if(aseguradora <= -1){
		cerr<<"ANALISIS FALLIDO REVISE EL ARCHIVO"<<std::endl;
		return 1;
	}
    imprimir_arbol_binario(raiz_arbol);
    eliminarnivel = 92;
    if(eliminarnivel >= 100){
    	cout<<"NO PUEDE ELIMINAR AL SUPREMO"<<endl;
	}else{
		LLamadoBan(raiz_arbol,eliminarnivel);
	}
	
	
	
cout<<"___________________________________________________"<<endl;
cout<<"________________Bienvenido guardian________________"<<endl;
Sleep(1000);
cout<<"      deberas elegir que guardian deseas ser       "<<endl;
cout<<"  tu mision sera llegar al rango entre 90 y 99     \n     para ser de los mejores guardianes del reino"<<endl;
cout<<"___________________________________________________"<<endl;
Sleep(1000);
cout<<"."<<endl;
Sleep(1000);
cout<<"."<<endl;
Sleep(1000);
cout<<"."<<endl;
Sleep(1000);
cout<<"."<<endl;
Sleep(1000);
cout<<"."<<endl;
Sleep(1000);
system("cls");

cout<<"______________Que opcion desea elegir______________"<<endl;
cout<<"           Opcion____ver los candidatos__1         "<<endl;  //acas edaran las opciones de que deben elegirr suceptible a cambio
cout<<"           Opcion____ver  aun guardian___2         "<<endl;
cout<<"           Opcion____Conoce el reino_____3         "<<endl;
cout<<"           Opcion________Batalla_________4         "<<endl;
cout<<"           Opcion____________________5             "<<endl;
cout<<"___________________________________________________"<<std::endl;


	
	
	
	
	
	
    return 0;
}
