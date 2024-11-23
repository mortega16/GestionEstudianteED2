#ifndef GRAFO_H
#define GRAFO_H

#include "Artista.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Grafo {
private:
    unordered_map<int, vector<int>> adyacencias;  
    unordered_map<int, unordered_map<int, int>> relacionesCursos;  

public:
    
    void agregarArista(int cedula1, int cedula2);
    void eliminarArista(int cedula1, int cedula2);
    vector<int> obtenerAdyacentes(int cedula);

  
    void mostrarRelaciones();

 
    void agregarCursoAArtistas(int cedula1, int cedula2);
    void eliminarCursoDeArtistas(int cedula1, int cedula2); 

    unordered_map<int, vector<int>> obtenerRelaciones() const;
    unordered_map<int, unordered_map<int, int>> obtenerRelacionesCursos() const;
};

#endif
