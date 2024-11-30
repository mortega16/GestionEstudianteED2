#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Grafo {
private:
    int numNodos;                            // Número de artistas
    vector<vector<bool>> matrizAdyacencia;   // Matriz de adyacencia para relaciones
    vector<vector<int>> matrizCursos;        // Matriz para contar cursos compartidos
public:
    Grafo(int tamano);  // Constructor para inicializar la matriz con un tamaño

    void agregarArista(int nodo1, int nodo2);
    void eliminarArista(int nodo1, int nodo2);
    vector<int> obtenerAdyacentes(int nodo);

    void mostrarRelaciones();

    void agregarRelacionCurso(int nodo1, int nodo2);
    void agregarRelacionCurso(int cedula1, int cedula2, const vector<int>& cedulas);
    void eliminarRelacionCurso(int nodo1, int nodo2);

    vector<vector<bool>> obtenerRelaciones() const;
    vector<vector<int>> obtenerRelacionesCursos() const;
    vector<vector<int>>& obtenerRelacionesCursosMutable();

    void guardarRelacionesCursosEnArchivo(const string& nombreArchivo) const;
    void cargarRelacionesCursosDesdeArchivo(const string& nombreArchivo);
};

#endif
