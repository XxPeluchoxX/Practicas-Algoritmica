#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

// Funcion para calcular la distancia entre dos puntos
double dist(pair<int,int> p1, pair<int,int> p2){
    return sqrt((p1.first - p2.first)*(p1.first - p2.first) + (p1.second - p2.second)*(p1.second - p2.second));
}

// Calcula la suma de un camino dado en un archivo con el formato:
// x y
// x y
// x y
// .
// .
// .
// x y
int main(int argc, char **argv){

    if(argc != 2){
        cout << "Uso: ./calculaDist <archivo_de_distancias>" << endl;
        exit(-1);
    }

    // Abrimos el archivo
    string ruta = argv[1];

    ifstream archivo(ruta);
    if (!archivo.is_open()){
        cout << "Error al abrir el archivo\n";
        return 1;
    }

    // Lectura del archivo
    vector<pair<int,int>> ciudades; 

    int x, y;
    while(archivo){
        archivo >> x >> y;
        ciudades.push_back(pair(x,y));
    }

    archivo.close();

    // Calculo de la distancia
    int n = ciudades.size()-1;
    double sum = 0;
    for(int i = 0; i < n-1; i++){
        sum += dist(ciudades[i], ciudades[i+1]);
    }

    cout << n << endl;
    for(int i = 0; i < n-1; i++){
        cout << i << endl;
        cout << dist(ciudades[i], ciudades[i+1]) << endl;
    }

    cout << sum << endl;

    return 0;
}

