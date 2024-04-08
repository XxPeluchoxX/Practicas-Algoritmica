#include <iostream> 
#include <string> 
#include <fstream> 
#include <cmath> 
#include <algorithm> 
#include <vector> 
#include <limits> 
#include <cfloat> 
#include <ctime>    
#include <chrono> 
#include <unistd.h>

using namespace std;
using namespace std::chrono;


#define UMBRAL_ITERATIVO 8

/**
 * @brief Calcula la distancia euclidea entre dos puntos en el plano.
 *
 * @param p1 El primer punto.
 * @param p2 El segundo punto.
 */
double dist(pair<int,int> p1, pair<int,int> p2){
    int diff_x = p1.first - p2.first;
    int diff_y = p1.second - p2.second;
    return sqrt(diff_x*diff_x + diff_y*diff_y);
}


/**
 * @brief Dadas sup - inf + 1 ciudades calcula el camino mas economico en cuanto a 
 * distancias. La primera ciudad se queda la primera siempre.
 *
 * @param m La matriz con las distancias entre las distintas ciudades.
 * @param v Las ciudades. Parámetro de entrada y salida, se devuelve con las 
 * ciudades ya ordenadas. 
 * @param inf La primera ciudad de la lista que tenemos en cuenta.
 * @param sup La ultima ciudad de la lista que tenemos en cuenta.
 */
void mejor_camino_Iterativo(vector<pair<int,int>> &v, int inf, int sup){
    if (inf >= sup){ // Caso base, no hay ciudades que considerar
        return;
    }

    sort(v.begin()+inf+1, v.begin()+sup+1); // El next_permutation necesita que este el vector en orden ascendente.

    int n = sup - inf + 1;  // Número de ciudades que consideramos
    double min = DBL_MAX;   // Distancia mínima inicial. La inicializamos a un valor muy grande para que cualquier distancia sea menor

    vector<pair<int, int>> perm; // Lista de las ciudades que vamos a considerar

    // Almacena en perm las ciudades que vamos a considerar. Permutación inicial
    for(int i = 0; i < n; ++i){
        perm.push_back(v[inf + i]);
    }

    double sum = 0; // Suma de las distancias de la permutación actual
    do{
        sum = 0;

        // Contabilizamos todas las distancias de la permutación actual
        for(int i = 0; i < n-1; ++i){
            sum += dist(perm[i], perm[i+1]);
        }
        sum += dist(perm[n-1], perm[0]); // Cierre del camino

        // Si la suma de distancias de la permutación actual es menor que la mínima encontrada hasta ahora, se actualiza
        if(sum < min){
            min = sum;      // Actualiza la distancia mínima
            for(int i = inf; i <= sup; ++i){ // Actualiza el vector de ciudades para ponerlo en el orden de la permutación actual
                v[i] = perm[i-inf];
            }
        }
        
    }while(next_permutation(perm.begin()+1, perm.end()));   // Genera la siguiente permutación, sin contabilizar la primera ciudad

}


/**
 * @brief Funcion que determina la mejor combinación de dos caminos.
 * 
 * El camino de la izq es un bucle y conecta con pivote de la forma:
 *         ... ---> izq2 -> pivote -> izq1 ---> ...
 * El camino de la dcha es un bucle y conecta con pivote de la forma:
 *        ... ---> dcha2 -> pivote -> dcha1 ---> ...
 * 
 * 
 * Para unir hay 4 posibilidades:
 * 1. izq2 --> ... --> izq1 --> pivote --> dcha1 --> ... --> dcha2
 *              Romper izq2 -> pivote y dcha2 -> pivote, y unirlos dcha2 -> izq2
 * 2. izq2 --> ... --> izq1 --> pivote --> dcha2 --> ... --> dcha1
 *              Romper izq2 -> pivote y dcha1 -> pivote, y unirlos dcha1 -> izq2
 * 3. izq1 --> ... --> izq2 --> pivote --> dcha1 --> ... --> dcha2
 *              Romper izq1 -> pivote y dcha2 -> pivote, y unirlos dcha2 -> izq1
 * 4. izq1 --> ... --> izq2 --> pivote --> dcha2 --> ... --> dcha1
 *              Romper izq1 -> pivote y dcha1 -> pivote, y unirlos dcha1 -> izq1
 * 
 * Informa sobre por donde se debe romper y unir para obtener el camino más corto
 * 
 * @param aux En aux[0] se guarda la que debe ser la última ciudad de la primera mitad (quedará justo antes de pivote)
 *            En aux[1] se guarda la que debe ser la última ciudad de la segunda mitad (quedará como la última ciudad) 
 *            Dato de salida
 * @param izq1  Primera ciudad de la primera mitad
 * @param izq2  Última ciudad de la primera mitad
 * @param dcha1  Primera ciudad de la segunda mitad
 * @param dcha2  Última ciudad de la segunda mitad
 * @param pivote  Ciudad que separa las dos mitades
 * 
 * @see une_caminos
 */
void donde_romper(pair<int, int>* aux,
                pair<int, int> izq1, pair<int, int> izq2,
                pair<int, int> dcha1,pair<int, int> dcha2,
                pair<int, int> pivote){

    // Calculo las 4 posibilidades
    double d1 = dist(izq1, pivote) + dist(pivote, dcha1) + dist(izq2, dcha2);
    double d2 = dist(izq1, pivote) + dist(pivote, dcha2) + dist(izq2, dcha1);
    double d3 = dist(izq2, pivote) + dist(pivote, dcha1) + dist(izq1, dcha2);
    double d4 = dist(izq2, pivote) + dist(pivote, dcha2) + dist(izq1, dcha1);


    // Modificamos aux para que tenga el camino más corto
    if(d1 <= d2 && d1 <= d3 && d1 <= d4){       // d1 = min {d1, d2, d3, d4}
        aux[0] = izq1;
        aux[1] = dcha2;
    }else if(d2 <= d1 && d2 <= d3 && d2 <= d4){ // d2 = min {d1, d2, d3, d4}
        aux[0] = izq1;
        aux[1] = dcha1;
    }else if(d3 <= d1 && d3 <= d2 && d3 <= d4){ // d3 = min {d1, d2, d3, d4}
        aux[0] = izq2;
        aux[1] = dcha2;
    }else{                                      // d4 = min {d1, d2, d3, d4}
        aux[0] = izq2;
        aux[1] = dcha1;
    }

}


/**
 * @brief Une los dos caminos en un solo camino.
 * 
 * En aux[0] se guarda la que debe ser la última ciudad de la primera mitad (quedará justo antes de pivote)
 *      Si se tiene que aux[0]=v[inf] (la primera ciudad),
 *      se debe invertir el camino [inf, pivote[ para que el orden sea correcto
 *      y pase de aux[0] a pivote.
 * 
 *              
 * En aux[1] se guarda la que debe ser la última ciudad de la segunda mitad (quedará como la última ciudad)
 *      Si se tiene que aux[1]=v[pivote+1] (la primera ciudad de la segunda mitad),
 *      se debe invertir el camino [pivote+1, sup] para que el orden sea correcto
 *      y aux[1] sea la última ciudad.
 * 
 * @param v  Vector de ciudades
 * @param aux  Vector auxiliar con las ciudades que se deben romper.
 * @param inf  La primera ciudad de la lista que tenemos en cuenta.
 * @param sup  La ultima ciudad de la lista que tenemos en cuenta.
 * @param pivote  Ciudad que separa las dos mitades
 * 
 * @see donde_romper
 */
void une_caminos(vector<pair<int, int>>& v, pair<int, int>* aux, int inf, int sup, int pivote){
    if(v[inf] == aux[0]){       // Debe ser la última ciudad de la primera mitad
        reverse(v.begin() + inf, v.begin() + pivote);  // revertir [inf, pivote[
    }

    if(v[pivote+1] == aux[1]){  // Debe ser la última ciudad de la segunda mitad
        reverse(v.begin() + pivote+1, v.begin() + sup + 1);   // revertir [pivote+1, s]
    }
}


/**
 * @brief Compara dos pares de enteros por la segunda componente
 * 
 * @param a Primer par
 * @param b Segundo par
 * @return true Si la segunda componente de a es menor que la de b
 * @return false  Si la segunda componente de a es mayor o igual que la de b
 */
bool ordena_y(const pair<int, int> &a, const pair<int, int> &b) {
    return a.second < b.second;
}

/**
 * @brief Ordena un vector de ciudades en orden ascendente entre [inf, sup]
 *              por la componente x o y
 * 
 * @param v  Vector de ciudades
 * @param inf  La primera ciudad de la lista que tenemos en cuenta.
 * @param sup  La ultima ciudad de la lista que tenemos en cuenta.
 * @param sort_by_x  Si es true, ordena las ciudades por la componente x,
 *                   si es false, ordena las ciudades por la componente y
 */
void ordena(vector<pair<int,int>> &v, int inf, int sup, bool sort_by_x){
    if(sort_by_x){
        sort(v.begin() + inf, v.begin() + sup + 1); // [inf, sup]
    }else{
        sort(v.begin() + inf, v.begin() + sup + 1, ordena_y);    // [inf, sup]
    }
}

/**
 * @brief Rota un vector entre [inf, sup] pivote posiciones hacia la izquierda
 * 
 * @param v  Vector de ciudades
 * @param inf  La primera ciudad de la lista que tenemos en cuenta.
 * @param sup  La ultima ciudad de la lista que tenemos en cuenta.
 * @param pivote  Numero de posiciones que se rota
 */
void rota_ANTIGUO(vector<pair<int, int>> &v, int inf, int sup, int pivote){
    vector<pair<int, int>> temp(pivote);
    
    // Guarda los pivote primeros
    for(int i = inf; i < inf + pivote; ++i){
        temp[i-inf] = v[i];
    }

    // Rota todos pivote posiciones adelante
    for(int i = inf + pivote; i <= sup; ++i){
        v[i-pivote] = v[i];
    }

    // Pone los pivote primeros en las pivote ultimas posiciones
    for(int i = sup - pivote + 1; i <= sup; ++i){
        v[i] = temp[i - sup + pivote - 1];
    }
}


/**
 * @brief Rota un vector entre [inf, sup] pivote posiciones hacia la izquierda
 * 
 * @param v  Vector de ciudades
 * @param inf  La primera ciudad de la lista que tenemos en cuenta.
 * @param sup  La ultima ciudad de la lista que tenemos en cuenta.
 * @param pivote  Numero de posiciones que se rota
 */
void rota(vector<pair<int, int>> &v, int inf, int sup, int pivote){
    rotate(v.begin() + inf, v.begin() + inf + pivote, v.begin() + sup + 1);
}


/**
 * @brief Busca una ciudad en un vector de ciudades en un rango [inf, sup]
 * 
 * Implementa la búsqueda lineal
 * 
 * @param v  Vector de ciudades
 * @param inf  La primera ciudad de la lista que tenemos en cuenta.
 * @param sup  La ultima ciudad de la lista que tenemos en cuenta.
 * @param buscado  La ciudad que buscamos
 * @return int La posicion de la ciudad buscada en el vector
 * (si no se encuentra, devuelve INVALID_POS)
 * (si se encuentra en la posicion i, devuelve i)
 */
int busca(vector<pair<int, int>> &v, int inf, int sup, pair<int, int> buscado){

    const int INVALID_POS = -1;

    for(int i = inf; i <= sup; ++i)
        if(v[i] == buscado)
            return i;       // Se ha encontrado  

    return INVALID_POS;     // No se ha encontrado
}

/**
 * @brief Dadas sup - inf + 1 ciudades calcula el camino mas economico en cuanto a
 * distancias. La primera ciudad se queda la primera siempre.
 * 
 * @param v  Vector de ciudades
 * @param inf La primera ciudad de la lista que tenemos en cuenta.
 * @param sup  La ultima ciudad de la lista que tenemos en cuenta.
 * @param sort_by_x Si es true,  ordena por x (división horizontal),
 *                  si es false, ordena por y (división vertical)
 */
void mejor_caminoDyV(vector<pair<int,int>> &v, int inf, int sup, bool sort_by_x){

    // Caso base
    if(sup - inf + 1 <= UMBRAL_ITERATIVO){
        mejor_camino_Iterativo(v, inf, sup);
    }else{
        // Guardo el pivote de la anterior llamada (primer elemento, ya que el pivote siempre se deja en el primer elemento)
        pair<int, int> old_pivote = v[inf];

        // Ordenamos y cogemos el valor medio (pivote)
        ordena(v, inf, sup, sort_by_x); // Notemos que va a descolocar el pivote, por eso lo guardamos antes
        int pivote = inf + (sup - inf+1)/2;

        // Obtenemos el mejor camino en [pivote, sup]
        // Notemos que si la division es horizontal (sort_by_x = true),
        //      la siguiente llamada se hace con sort_by_x = false, para dividir ahora verticalmente.
        mejor_caminoDyV(v, pivote, sup, !sort_by_x);

        // Obtenemos el mejor camino en [inf, pivote]
        // Como se empieza y termina en el primer elemento y,
        //      en este caso queremos que empiece y termine por pivote (el último elemento),
        //      se intercambia para dejar la pivote el primero.
        //      Al final se vuelve a dejar el pivote en su sitio original, pero
        //              ya no basta con un swap, porque perdería el orden de las ciudades.
        //              Hay que hacer un reverse para invertir las ciudades.
        swap(v[inf], v[pivote]);
        mejor_caminoDyV(v, inf, pivote, !sort_by_x);
        reverse(v.begin() + inf, v.begin() + pivote + 1); // ponemos el pivote en pivote

        
        
        // Combinación de los dos caminos
        pair<int, int> info_roturas[2]; // Vector auxiliar para guardar las ciudades que se deben romper
        donde_romper(info_roturas, v[inf], v[pivote-1], v[pivote+1], v[sup], v[pivote]);
        une_caminos(v, info_roturas, inf, sup, pivote);

        // Devuelvo la solucion con el pivote antiguo en inf, ya que se ha descolocado
        //     (siempre ha de empezarse y terminarse por la primera ciudad)
        //     pero en este caso ya estará el tramo [inf, sup] ordenado, por lo que mantenemos el orden
        int desfase = busca(v, inf, sup, old_pivote) - inf; // Restamos inf para obtener el desfase
        rota(v, inf, sup, desfase);
    }
}

/**
 * @brief Sobrecarga del operador << para imprimir un vector de pares de enteros
 * 
 * @param os Flujo de salida
 * @param v Vector de pares de enteros
 * @return ostream& Referencia al flujo de salida
 */
ostream& operator<<(ostream &os, vector<pair<int,int>> v){
    vector<pair<int,int>>::iterator it;
    for(it = v.begin(); it != v.end(); ++it){
        os << it->first << " " << it->second << endl;
    }
    return os;
}

/**
 * Programa principal
 *
 * @param argc Numero de argumentos
 * @param argv Array con los argumentos
 *
 * Uso: programa <entrada> <salida>
 * Formato de entrada
 * n
 * x y
 * x y
 * .
 * .
 * .
 */
int main(int argc, char **argv){
    // Comprobacion de parametros
    if (argc != 3){
        cout << "Uso: ejecutable <entrada> <salida>\n";
        return 1;
    }    


    // Apertura del archivo de entrada
    ifstream entrada(argv[1]);
    if (!entrada.is_open()){
        cout << "Error al abrir el archivo de entrada\n";
        return 1;
    }
    
    // Apertura del archivo de salida
    ofstream salida(argv[2]);
    if (!salida.is_open()){
        cout << "No se puede abrir el archivo de salida\n"; 
        return 1;
    }

    // Lectura del archivo
    int n;
    entrada >> n;
    vector<pair<int,int>> ciudades(n); 

    for(int i=0; i < n; ++i){
        entrada >> ciudades[i].first >> ciudades[i].second;
    }
    entrada.close();

    high_resolution_clock::time_point tantes, tdespues;
    duration<double> transcurrido;

    // Busco el mejor camino
    tantes = high_resolution_clock::now();
    mejor_caminoDyV(ciudades, 0, n-1, true);
    tdespues = high_resolution_clock::now();
    
    transcurrido = duration_cast<duration<double>>(tdespues - tantes);

    // Añadimos la primera ciudad (volver a ella)
    ciudades.push_back(ciudades[0]);

    // Salida. Imprimimos el tiempo y las ciudades por orden
    cout << n << " " << transcurrido.count() << endl;
    salida << ciudades;

    salida.close();

    return 0;
}
