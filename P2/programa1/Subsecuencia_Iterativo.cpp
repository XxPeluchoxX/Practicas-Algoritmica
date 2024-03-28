#include <iostream>
#include <random>
#include <time.h>
#include <utility>



int SumaMaximaNum(int* arr, int n){
    int max=0, localmax=0;
    int* endpos = arr + n;
    for(int* pos = arr; pos < endpos; ++pos){
        int elem = *pos;
        if(localmax > 0){
            localmax += elem;
            
        } else{
            localmax = elem;
        }
        max = std::max(max, localmax);
        
    }
    return std::max(max,0);
}

std::pair<int,int> SumaMaximaPos(int* arr, int n){
    int max=0, localmax=0;
    std::pair<int,int> bounds(0,0);
    int localini=0, localfin=0;
    for(int i=0; i < n; ++i){
        
        if(localmax > 0){
            localmax += arr[i];
            localfin = i;
        } else{
            localmax = arr[i];
            localini = localfin = i;
        }
        //Si se quiere la subsecuencia mas pequeña posible, añadir:
        // || (localmax == max && localfin - localini < bounds.second - bounds.first)
        if (localmax > max){ 
            max = localmax;
            bounds.first = localini;
            bounds.second = localfin;
        }
        
    }
    return bounds;

}

int main(){
    srand((unsigned int)time(NULL));
    int n = 16;
    int arr[n] = {1,2,40,-50,7,-10,4,20,  30,-11,-1,0,0,0,-200,40};


    for(int i=0; i<n; ++i){
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    std::pair<int,int> bounds = SumaMaximaPos(arr,n);
    std::cout << SumaMaximaNum(arr, n) << "   " << bounds.first << " " << bounds.second << std::endl;
    return 0;
}