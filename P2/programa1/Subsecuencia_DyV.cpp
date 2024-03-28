#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <climits>
#include <cassert>

using namespace std;


#define PAIR

int suma(const vector<int> & v, int init, int fin){
    int result=0; 
    int middle=init+(fin-init)/2;
    int i=middle;
    int j=middle+1;
    
    while((i>=init)||(j<=fin)){
        if(i>=init){
            result+=v[i];
            --i;
        }
        if(j<=fin){
            result+=v[j];
            ++j;
        }
    }
    return result;
}

#ifdef PAIR

pair<int,int> subsecMax (const vector<int> &v, int init, int fin){
    int tam = fin-init+1;
    pair<int,int> result;
    int suma_result;
    if(tam<=1){
        result.first=result.second=init;
        return result;
    }

    else{
        int middle=init + (fin-init)/2;

        pair<int,int> result_i;
        pair<int,int> result_d;

        result_i=subsecMax(v,init,middle);
        result_d=subsecMax(v,middle+1,fin);

        //Unica posible combinacion 

        pair <int,int>result_comb(result_i.first,result_d.second);

        int suma_i,suma_d,suma_comb;

        //Combinacion de casos

        suma_i=suma(v,result_i.first,result_i.second);
        suma_d=suma(v,result_d.first,result_d.second);
        suma_comb=suma(v,result_comb.first,result_comb.second);

        if(suma_i>=suma_d){
            result.first=result_i.first;
            result.second=result_i.second;
            suma_result=suma_i;
        }
        else{
            result.first=result_d.first;
            result.second=result_d.second;
            suma_result=suma_d;
        }

        if(suma_comb>=suma_result){
            result.first=result_comb.first;
            result.second=result_comb.second;
        }
        return result;
    }
}

#endif

#ifndef PAIR

int SubsecSumaMax(const vector<int> , int init, int fin){
    return 0;
}

#endif

int main (int argc,char **argv){
    
    if(argc!=4){
        cout << " ./ejecutable <FILE>";
        return 1;
    }

    pair <int,int> result;
    srandom(time(0));

    string file = argv[1];
    
    chrono::high_resolution_clock::time_point tantes, tdespues;
    chrono::duration<double> transcurrido;
    int tam;

    ifstream input(file);
    
    if(!input){
        cout << "File" + file + "not found. Couldn't be opened.";
    }

    else {
        input >> tam;
        vector <int> v;
        int dato;
        for (int i=0; i<tam; ++i){
            input>>dato;
            v.push_back(dato);
        }

        tantes= chrono::high_resolution_clock::now();

        result = subsecMax(v,0,v.size()-1);

        tdespues = chrono::high_resolution_clock::now();

        transcurrido = chrono::duration_cast <chrono::duration<double>> (tdespues-tantes);
    }
    input.close();

    //Salida
    file=argv[2];
    ofstream output(file);

    if(!output){
        cout << "File" + file + "not found. Couldn't be opened.";
    }
    else{
        output << result.first << " " << result.second;
    }
    output.close();
    file=argv[3];
    output.open(file);

    if(!output){
        cout << "File" + file + "not found. Couldn't be opened.";
    }
    else{
        output << tam << " " << transcurrido.count() << endl;
    }
    output.close();
    return 0;

}