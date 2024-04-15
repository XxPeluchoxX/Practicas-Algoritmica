#include <iostream>
#include <fstream>
#include <set>
#include <list>
#include <cassert>
#include <string.h>
#include <climits>
#include <math.h>

//Si ejecutar en modo debug
#define NDEBUG

//Si ejecutar con el formato del generador de imágenes
#define drawFormat


class Ruta{
private:

    std::list<std::pair<double,double>> route;
    std::pair<double,double> ref_point;
public:
    Ruta(const std::list<std::pair<double,double>>& _route, std::pair<double,double> _ref_point)
    :route(_route)
    ,ref_point(_ref_point)
    {}

    Ruta(const Ruta& other)
    :route(other.route)
    ,ref_point(other.ref_point)
    {}

    /**
     * @brief Une una ruta ya existente con esta ruta.
     * Tras el último punto de la otra ruta se une el primero de esta, salvo
     * si reverse = true, entonces revierte la otra ruta para unir el 
     * primer punto con el primero
     * @param other la ruta con la que unir esta ruta 
     * 
     * @param reverse Si es true, invierte la lista para unir el primer punto
     * en vez del último
     * @post Se modifica other si reverse == true por cuestiones de eficiencia
     */
    void joinFront(Ruta& other, bool reverse = false){
        if(reverse){
            other.route.reverse();
        }
        route.insert(route.begin(), other.route.begin(),other.route.end());

        ref_point = midPoint(ref_point,other.ref_point);
    }

    /**
     * @brief Une una ruta ya existente con esta ruta.
     * Tras el último punto de esta ruta este se une el primero de la otra, salvo
     * si reverse = true, entonces revierte la otra ruta para unir el 
     * último punto con el último
     * @param other la ruta con la que unir esta ruta 
     * 
     * @param reverse Si es true, invierte la lista para unir el primer punto
     * en vez del último
     */
    void joinBack(Ruta& other, bool reverse = false){
        route.reverse();
        joinFront(other,reverse);
    }



    inline std::pair<double, double> front() const{
        return route.front();
    }

    inline std::pair<double, double> back() const{
        return route.back();
    }

    inline std::pair<double, double> refPoint() const{
        return ref_point;
    }

    inline int size() const{
        return route.size();
    }

    friend bool operator<(Ruta r1, Ruta r2){
        return  r1.size() <  r2.size() ||
               (r1.size() == r2.size() && r1.refPoint() < r2.refPoint());
    }

#ifdef drawFormat
    friend std::ostream& operator<<(std::ostream& os,const Ruta& ruta){
        for(auto p: ruta.route){
            os << p.first << " " << p.second<< "\n";
        }
        return os;
    }
#else
    friend std::ostream& operator<<(std::ostream& os,const Ruta& ruta){
        for(auto p: ruta.route){
            os << "(" << p.first << ", " << p.second<< ")" << " ";
        }
        return os;
    }
#endif

private:

    //TODO: Ver que es mejor, punto medio entre libres o el arrastrado
    //? Creo que el arrastrado es mejor para datos dispersos porque te centraliza un
    //? poco el punto ese medio, pensar si añadir pesos al problema según el número de conexiones en la ruta
    inline std::pair<double,double> midPoint(std::pair<double,double> p1, std::pair<double,double>p2){
        return std::pair<double,double>((p1.first+p2.first)/2,(p1.second+p2.second)/2);
    }

};

/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/


inline double getDistance(std::pair<double,double> orig, std::pair<double,double> dest){
    return sqrt((orig.first- dest.first) *(orig.first -dest.first ) 
              + (orig.second-dest.second)*(orig.second-dest.second));
}

/**
 * @brief Calcula la distancia eluclídea entre 2 puntos
 * @param orig 
 * @param dest 
 * @return 
 */
inline double getDistance(const Ruta& orig, const Ruta& dest){
    return getDistance(orig.refPoint(),dest.refPoint());    
}

/**
 * @brief 
 * @param begin 
 * @param end 
 * @param r 
 * @pre begin != end
 * @return 
 */
std::set<Ruta>::iterator findNearest(std::set<Ruta>::iterator begin,const std::set<Ruta>::iterator& end, const Ruta& r){

    auto nearest = begin;
    double minDist = getDistance(*begin,r);
    for(++begin; begin != end; ++begin){
        double dist = getDistance(*begin,r);
        if( dist < minDist){
            minDist = dist;
            nearest = begin;
        }
    }
    return nearest;
}

void effInsertEnd(std::set<Ruta>& rutas, const Ruta& r){
    static int maxSize = 0;
    if(maxSize < r.size()){
        maxSize = r.size();
        rutas.insert(rutas.end(),r); 
    } else{
        rutas.insert(r);
    }
}

/**
 * @brief 
 * @param rutas 
 * @param posR1 
 * @param posR2 
 */
void joinRoutes(std::set<Ruta>& rutas, std::set<Ruta>::iterator posR1, std::set<Ruta>::iterator posR2){
    Ruta r1 = *posR1, r2 = *posR2;
    rutas.erase(posR1);
    rutas.erase(posR2);

    //Calcular mejor unión posible
    std::pair<double,double> ptsR1[4] = {r1.front(), r1.front(), r1.back(), r1.back()};
    std::pair<double,double> ptsR2[4] = {r2.front(), r2.back(), r2.front(), r2.back()};

    double currMin = getDistance(ptsR1[0],ptsR2[0]);
    int posCurrMin=0;
    for(int i = 1; i < 4; ++i){
        double d = getDistance(ptsR1[i],ptsR2[i]);
        if( d < currMin){
            currMin = d;
            posCurrMin = i;
        }
    }

    //insertar nueva ruta 
    switch(posCurrMin){
        case 0:
            r1.joinFront(r2,true);
            effInsertEnd(rutas,r1);
            break;
        case 1:
            r1.joinFront(r2,false);
            effInsertEnd(rutas,r1);
            break;
        case 2:
            r2.joinFront(r1,false);
            effInsertEnd(rutas,r2);
            break;
        case 3:
            r1.joinBack(r2,false);
            effInsertEnd(rutas,r1);
            break;
    }

}



int main(int argc, char* argv[]){
    std::ofstream outputFile;
    std::ifstream inputFile;

    //Configurar la entrada y la salida
    switch(argc){
        case 3:
            //ofstream, redirige la salida de std::cout a la proporcionada por el archivo
            //Si se cierra el buffer del archivo de salida, cout queda en estado inválido
            outputFile.open(argv[2]);

            //comprueba que se abriese el flujo
            if(!outputFile) throw std::ios_base::failure("No se pudo abrir el archivo de salida");

            //redirige el flujo de cout
            std::cout.rdbuf(outputFile.rdbuf()); //redirige cout al ofstream

            //fallthrough intencional, no falta el break
        case 2:

            //ifstream, redirige la entrada de std::cin a la proporcionada por el archivo
            //Si se cierra el buffer del archivo de entrada, cin queda en estado inválido
            inputFile.open(argv[1]);

            //comprueba que se abriese el flujo
            if(!inputFile) throw std::ios_base::failure("No se pudo abrir el archivo de entrada");

            //redirige el flujo de cin
            std::cin.rdbuf(inputFile.rdbuf());
            break;

        default:
            #ifdef NDEBUG
                //Caso no debug (Compatible con assert)
                std::cerr << "Uso: " << argv[0] << " <Entrada> [Salida]" << std::endl;
                return 1;
            #else
                //caso debug
                std::cerr << "Se esta ejecutando en modo debug" << std::endl << std::endl; 
            #endif

            break;

    }


    //inicialización del problema
    int n,x,y;
    std::cin >> n;
    std::set<Ruta> rutas;
    while(n){
        std::cin >> x;
        std::cin >> y;
        rutas.insert(Ruta({{x,y}},{x,y})); 
        --n;
    }

    int maxSize = 0;
    //resolución del problema
    //TODO ver si hacer una version en la que elija los dos puntos más cercanos
    while(rutas.size() > 1){ 
        auto setIt = findNearest(++rutas.begin(),rutas.end(), *rutas.begin());
        joinRoutes(rutas, rutas.begin(), setIt);
    }
    
    std::cout << *rutas.begin() << (*rutas.begin()).front().first << " " << (*rutas.begin()).front().second << std::endl;


    outputFile.close();
    inputFile.close();
    return 0;
}