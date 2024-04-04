#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

// Funcion para calcular la distancia entre dos puntos
double dist(pair<int,int> p1, pair<int,int> p2){
    return sqrt((p1.first - p2.first)*(p1.first - p2.first) + (p1.second - p2.second)*(p1.second - p2.second));
}

int main(int argc, char **argv){
    // Abrimos el archivo
    string ruta = argv[1];
    int n = strtol(argv[2], NULL, 10);

    ifstream archivo(ruta);
    if (!archivo.is_open()){
        cout << "Error al abrir el archivo\n";
        return 1;
    }

    // Lectura del archivo
    int x, y;
    vector<pair<int,int>> ciudades(n); 

    for(int i=0; i < n; i++){
        archivo >> ciudades[i].first >> ciudades[i].second;
    }

    archivo.close();


    // Calculo de la distancia
    double sum = dist(ciudades[0], ciudades[n-1]); // Distancia entre la primera y la ultima ciudad, cerrando el ciclo
    for(int i = 0; i < n-1; i++){
        sum += dist(ciudades[i], ciudades[i+1]);
    }

    cout << sum << endl;

    return 0;
}

