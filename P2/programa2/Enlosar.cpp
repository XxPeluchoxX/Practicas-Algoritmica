#include <iostream>

void Escribir_baldosa(unsigned int **M, int first_row, int first_col, bool lower, bool right, int middle)
{
    // El array está mappeado para que la baldosa se ponga bien
    static unsigned int current_slab = 1;
    M[first_row - lower + middle][first_col - right + middle] = current_slab;
    M[first_row - lower + middle][first_col + right + middle-1] = current_slab;
    M[first_row + lower + middle-1][first_col - right + middle] = current_slab;
    ++current_slab;

}


/**
 * @brief Enlosa un cuadrado de 2^n filas y 2^n columnas con baldosas
 * con forma de L. Las enlosa enumerando las casillas con un número
 * único asociado a cada baldosa.
 * 
 * 
 * 
 * @param M El cuadrado que enlosar.
 * @param n 2^n es el número de filas y columnas
 * @param row fila en la que está el sumidero
 * @param col columna en la que esta el sumidero
 */
void Enlosar(unsigned int **M, int n, int row, int col, int first_row, int first_col)
{

    // Shift para exponenciar de forma eficiente
    int middle = 1 << (n - 1); // 1<<0 = 1
    bool lower = row >=middle+first_row;
    bool right = col >=middle+first_col;

    Escribir_baldosa(M, first_row, first_col, lower, right, middle);

    if (n <= 1)return; //finalizar caso base
    //Solo si n > 1

    //Sumidero en la esquina superior izquierda
    if(!(lower || right)) 
        Enlosar(M,n-1,row,col, first_row,first_col);
    else
        Enlosar(M,n-1,first_row + middle -1 ,first_col + middle -1, first_row, first_col);


    //Sumidero en la esquina superior derecha
    if(!lower && right)
        Enlosar(M,n-1,row,col, first_row,first_col + middle);
    else
        Enlosar(M,n-1,first_row + middle -1 ,first_col + middle, first_row , first_col + middle);


    //Sumidero en la esquina inferior izquierda
    if(lower && !right)
        Enlosar(M,n-1,row,col, first_row + middle,first_col);
    else
        Enlosar(M,n-1,first_row + middle,first_col + middle -1, first_row + middle, first_col);


    //Sumidero en la esquina inferior derecha
    if(lower && right)
        Enlosar(M,n-1,row,col, first_row + middle ,first_col + middle);
    else
        Enlosar(M,n-1,first_row + middle,first_col + middle, first_row + middle, first_col + middle);
}

/**
 * @brief Enlosa un cuadrado de 2^n filas y 2^n columnas con baldosas
 * con forma de L. Las enlosa enumerando las casillas con un número
 * único asociado a cada baldosa.
 * 
 * Wrapper 
 * 
 * @param M El cuadrado que enlosar.
 * @param n 2^n es el número de filas y columnas
 * @param row fila en la que está el sumidero
 * @param col columna en la que esta el sumidero
 */
void Enlosar(unsigned int **M, int n, int row, int col)
{
    Enlosar(M,n,row,col,0,0);
}

int main(int argc, char *argv[])
{

    // 2^n es el numero de filas y columnas, row y col es la posición del sumidero
    int n, row, col;


    if (argc < 2)
    {
        n = 2;
    }
    else if (argc < 4)
    {
        row = col = 0;
    }
    else
    {
        n = std::stoi(argv[1]);
        row = std::stoi(argv[2]);
        col = std::stoi(argv[3]);
    }
    int m = 1 << n;
    unsigned int** M = new unsigned int*[m];

    for(int i = 0; i < m; ++i){
        M[i] = new unsigned int[m];
        for(int j=0;j<m;++j){
            M[i][j] = 0;
        }
    }

    Enlosar(M,n,row,col);
    //Escribir_baldosa(M,0,0,1,1,1);

    std::cout << std::endl << std::endl << std::endl;
    for(int i=0; i < m; ++i){
        for(int j = 0; j < m; ++j){
            std::cout << M[i][j] << "\t"; 
        }
        std::cout << std::endl;
    }

    for(int i = 0; i < m; ++i){
        delete[] M[i];
    }

    delete[] M;

    return 0;
}