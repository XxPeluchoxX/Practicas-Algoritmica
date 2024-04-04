#include <iostream>
#include <fstream> 
#include <chrono> 
#include <ctime>



/**
 * @brief  Escribe una baldosa en la matriz M.
 * 
 * Coge las 4 casillas centrales (una de cada cuadrante) y la baldosa ocupará
 * las 3 casillas de los cuadrantes en los que no está el sumidero.
 * 
 * @param M  Matriz a enlosar
 * @param first_row  Fila de la esquina superior izquierda. Relativo a la matriz M completa
 * @param first_col Columna de la esquina superior izquierda. Relativo a la matriz M completa
 * @param lower true si el sumidero está en la mitad inferior
 * @param right true si el sumidero está en la mitad derecha
 * @param middle  mitad de la dimensión de la parte de M que se va a enlosar
 */
void Escribir_baldosa(unsigned int **M, int first_row, int first_col, bool lower, bool right, int middle)
{


    // Variable estática que lleva la cuenta de la baldosa actual
    static unsigned int current_slab = 1;

    // false = 0, true = 1
    // El array está mappeado para que la baldosa se ponga bien.
    M[first_row - lower + middle  ][first_col - right + middle]   = current_slab;
    M[first_row - lower + middle  ][first_col + right + middle-1] = current_slab;
    M[first_row + lower + middle-1][first_col - right + middle]   = current_slab;

    ++current_slab; // Incrementa el número de baldosa

    #ifdef DEBUG
    std::cout << std::endl << std::endl;
    std::cout << current_slab << std::endl;
    for(int i=0; i < m; ++i){
        for(int j = 0; j < m; ++j){
            std::cout << M[i][j] << "\t"; 
        }
        std::cout << std::endl;
    }
    #endif

}


/**
 * @brief Enlosa un cuadrado de 2^n filas y 2^n columnas con baldosas
 * 
 * @param M  Matriz a enlosar
 * @param n  2^n es el número de filas y columnas
 * @param row  Fila en la que está el sumidero. Relativo a la matriz M completa
 * @param col   Columna en la que está el sumidero. Relativo a la matriz M completa
 * @param first_row  Fila de la esquina superior izquierda. Relativo a la matriz M completa
 * @param first_col  Columna de la esquina superior izquierda. Relativo a la matriz M completa
 */
void Enlosar(unsigned int **M, int n, int row, int col, int first_row, int first_col)
{

    int middle = 1 << (n - 1); // middle = 2^(n-1) = 2^n/2 = dim/2

    // Determina en qué cuadrante se encuentra el sumidero
    bool lower = row >=middle+first_row;    // True si está en la mitad inferior
    bool right = col >=middle+first_col;    // True si está en la mitad derecha

    // Escribe la baldosa en la matriz que se ha determinado
    Escribir_baldosa(M, first_row, first_col, lower, right, middle);

    // Caso base. Si la matriz es de 2^1 x 2^1 = 2 x 2, no se puede dividir más; ya se habrá colocado la baldosa
    if (n <= 1)
        return;

    // Solo si n > 1


    // División para el cuadrante superior izquierdo
    if(!(lower || right))
        // El sumidero está en el cuadrante superior izquierdo. Enlosamos ahora ese cuadrante
        Enlosar(M,n-1, row,col, first_row,first_col);
    else
        // El sumidero no está en el cuadrante superior izquierdo.
        // Ya se ha enlosado la baldosa correspondiente a ese cuadrante (la esquina inferior derecha)
        // Se coloca un falso sumidero en la esquina inferior derecha para que se enlose el cuadrante superior izquierdo
        Enlosar(M,n-1, first_row + middle -1,first_col + middle -1, first_row, first_col);


    // División para el cuadrante superior derecho
    if(!lower && right)
        // El sumidero está en el cuadrante superior derecho. Enlosamos ahora ese cuadrante
        Enlosar(M,n-1,row,col, first_row,first_col + middle);
    else
        // El sumidero no está en el cuadrante superior derecho.
        // Ya se ha enlosado la baldosa correspondiente a ese cuadrante (la esquina inferior izquierda)
        // Se coloca un falso sumidero en la esquina inferior izquierda para que se enlose el cuadrante superior derecho
        Enlosar(M,n-1,first_row + middle -1 ,first_col + middle, first_row , first_col + middle);


    // División para el cuadrante inferior izquierdo
    if(lower && !right)
        // El sumidero está en el cuadrante inferior izquierdo. Enlosamos ahora ese cuadrante
        Enlosar(M,n-1,row,col, first_row + middle,first_col);
    else
        // El sumidero no está en el cuadrante inferior izquierdo.
        // Ya se ha enlosado la baldosa correspondiente a ese cuadrante (la esquina superior derecha)
        // Se coloca un falso sumidero en la esquina superior derecha para que se enlose el cuadrante inferior izquierdo
        Enlosar(M,n-1,first_row + middle,first_col + middle -1, first_row + middle, first_col);


    // División para el cuadrante inferior derecho
    if(lower && right)
        // El sumidero está en el cuadrante inferior derecho. Enlosamos ahora ese cuadrante
        Enlosar(M,n-1,row,col, first_row + middle ,first_col + middle);
    else
        // El sumidero no está en el cuadrante inferior derecho.
        // Ya se ha enlosado la baldosa correspondiente a ese cuadrante (la esquina superior izquierda)
        // Se coloca un falso sumidero en la esquina superior izquierda para que se enlose el cuadrante inferior derecho
        Enlosar(M,n-1,first_row + middle,first_col + middle, first_row + middle, first_col + middle);

}

/**
 * @brief Enlosa un cuadrado de 2^n filas y 2^n columnas con baldosas
 * con forma de L. Las enlosa enumerando las casillas con un número
 * único asociado a cada baldosa.
 * @param M La matriz a enlosar.
 * @param n 2^n es el número de filas y columnas
 * @param row fila en la que está el sumidero
 * @param col columna en la que esta el sumidero
 */
void Enlosar(unsigned int **M, int n, int row, int col)
{
    Enlosar(M,n,row,col,0,0);
}


/**
 * Uso ejecutable <entrada> <salida>
 * Formato de entrada: n x y
 * n exponente de la potencia de 2. La matriz será de 2^n x 2^n
 * (x,y) es la posición del sumidero
 */
int main(int argc, char *argv[])
{

    // n es el exponente, (row, col) es la posición del sumidero
    int n, row, col;

    if (argc =! 3){
        std::cout << "Uso: ejecutable <entrada> <salida>\n";
        return 1;
    }


    // Apertura del archivo de entrada
    std::ifstream entrada(argv[1]);
    if (!entrada.is_open()){
        std::cout << "No se puede abrir el archivo de entrada\n"; 
        return 1;
    }

    // Apertura del archivo de salida
    std::ofstream salida(argv[2]);
    if (!salida.is_open()){
        std::cout << "No se puede abrir el archivo de salida\n"; 
        return 1;
    }

    // Lectura de los datos de entrada
    entrada >> n >> row >> col;


    int dim = 1 << n; // dim = 2^n

    // Creación de la matriz e inicialización a 0
    unsigned int** M = new unsigned int*[dim];
    for(int i = 0; i < dim; ++i){
        M[i] = new unsigned int[dim];
        for(int j=0;j<dim;++j){
            M[i][j] = 0;
        }
    }

    std::chrono::high_resolution_clock::time_point tantes, tdespues;
    std::chrono::duration<double> transcurrido;

    tantes = std::chrono::high_resolution_clock::now();
    Enlosar(M,n,row,col);
    tdespues = std::chrono::high_resolution_clock::now();

    transcurrido = std::chrono::duration_cast<std::chrono::duration<double>>(tdespues-tantes);


    // Salida del tamaño de la matriz y el tiempo de ejecución
    std::cout << dim << " " << transcurrido.count() << std::endl;

    // Escritura de la matriz en el archivo de salida
    for(int i=0; i < dim; ++i){
        for(int j = 0; j < dim; ++j){
            salida << M[i][j] << "\t"; 
        }
        salida << std::endl;
    }


    // Liberación de la memoria de la matriz
    for(int i = 0; i < dim; ++i){
        delete[] M[i];
    }
    delete[] M;

    return 0;
}
