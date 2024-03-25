#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <set>

using namespace std;

int MIN = -1000000;
int MAX = 1000000;

const int MINk = 1;
int MAXk = 30;

int I = 0;

int aleatorio(int min, int max){
    int values = max - min + 1;
    return (rand() % values) + min;
}

void texto(){
    cout << "---------------------------------------" << endl;
    cout << "   1. Subsecuencia de suma maxima      " << endl;
    cout << "   2. Enlosar un espacio               " << endl;
    cout << "   3. Viajante de comercio             " << endl;
    cout << "   0. Salir                            " << endl;
    cout << "---------------------------------------" << endl;
}

// devuelve entrada entre 0 y 3
int input(){
    int entrada;
    do{
        cout << "> ";
        cin >> entrada;
    }while(entrada < 0 || entrada > 3);

    return entrada;
}

void texto2(){
    cout << "---------------------------------------" << endl;
    cout << "   Introduzca el numero de elementos   " << endl;
    cout << "   o 0 para tamano aleatorio           " << endl;
    cout << "   o -1 para salir                     " << endl;
    cout << "---------------------------------------" << endl;
}

// devuelve entrada entrada >= -1
int input2(){
    int entrada;
    do{
        cout << "> ";
        cin >> entrada;
    }while(entrada < -1);

    return entrada;
}

string nextRuta(string r){
    return r + to_string(I++);
}

// recibe el nombre del archivo en el que lo guarda
int main(int argc, char** argv){
    if(argc < 2 || argc > 5){
        cerr << "generador <archivo>" << endl;
        cerr << "generador <archivo> <MAXk>" << endl;
        cerr << "generador <archivo> <MIN> <MAX>" << endl;
        cerr << "generador <archivo> <MIN> <MAX> <MAXk>" << endl;
        exit(1);
    }

    switch(argc){
        case 3:
            MAXk = strtol(argv[2], NULL, 10);
            break;
            
        case 5:
            MAXk = strtol(argv[4], NULL, 10);
        case 4:
            MIN = strtol(argv[2], NULL, 10);
            MAX = strtol(argv[3], NULL, 10);
            break;
    }

    string ruta(argv[1]);
    ofstream archivo;


    // argv

    time_t t;
    srand(time(&t));

    int n, k;
    int x, y;

    int entrada;
    do{
        texto();
        entrada = input();

        switch(entrada){
            case 1: // pide si genera numero aleatorio o numero fijo
                do{
                    texto2();            
                    n = input2();

                    if(n != -1){
                        if(n == 0)    // numero aleatorio
                            n = aleatorio(0, MAX);

                        archivo.open(nextRuta(ruta));
                        if(!archivo){
                            cerr << "No se pudo abrir el archivo" << endl;
                            exit(1);
                        }

                        cout << "Se generaron " << n << " numeros aleatorios" << endl;

                        archivo << n << endl;
                        // genera n numeros aleatorios
                        for(int i = 0; i < n; i++){
                            archivo << aleatorio(MIN, MAX) << " ";
                        }

                        archivo.close();
                    }

                }while(n != -1);
                break;
                
            case 2:
                archivo.open(nextRuta(ruta));
                if(!archivo){
                    cerr << "No se pudo abrir el archivo" << endl;
                    exit(1);
                }

                k = aleatorio(MINk, MAXk);
                n = pow(2, k);
                archivo << n << " ";

                x = aleatorio(0,n-1);
                y = aleatorio(0,n-1);

                archivo << x << " " << y;

                cout << "Tablero de " << n << " celdas, sumidero en (" << x << ", " << y << ")" << endl;
                archivo.close();
                break;
                
            case 3:
                do{
                    texto2();            
                    n = input2();

                    if(n != -1){
                        if(n == 0)    // numero aleatorio
                            n = aleatorio(0, MAX/2);

                        archivo.open(nextRuta(ruta));
                        if(!archivo){
                            cerr << "No se pudo abrir el archivo" << endl;
                            exit(1);
                        }

                        cout << "Se generaron " << n << " puntos aleatorios" << endl;

                        // genera n puntos aleatorios
                        set<pair<int, int>> conjunto;
                        pair<int, int> pareja;

                        while(conjunto.size() < n){
                            pareja.first = aleatorio(MIN, MAX);
                            pareja.second = aleatorio(MIN, MAX);
                            conjunto.insert(pareja);
                        }

                        archivo << n << endl;
                        set<pair<int, int>>::iterator it;
                        for(it = conjunto.begin(); it != conjunto.end(); ++it){
                            archivo << it->first << " " << it->second << endl;
                        }

                        archivo.close();
                    }

                }while(n != -1);
                break;
        }

    }while(entrada != 0);


    return 0;
}

