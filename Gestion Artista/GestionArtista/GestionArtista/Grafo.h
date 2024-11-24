#ifndef GRAFO_H
#define GRAFO_H

#include "Artista.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>  // Para guardar/cargar relaciones

using namespace std;

class Grafo {
private:
    unordered_map<int, vector<int>> adyacencias;  // Relación entre artistas
    unordered_map<int, unordered_map<int, int>> relacionesCursos;  // Relación entre artistas por cursos

public:
    // Relacionar dos artistas
    void agregarArista(int cedula1, int cedula2);
    void eliminarArista(int cedula1, int cedula2);
    vector<int> obtenerAdyacentes(int cedula);

    // Mostrar las relaciones entre artistas
    void mostrarRelaciones();

    // Agregar o eliminar una relación de curso entre dos artistas
    void agregarRelacionCurso(int cedula1, int cedula2);  // Relaciona artistas en un curso
    void eliminarRelacionCurso(int cedula1, int cedula2); // Elimina la relación de curso entre artistas

    // Obtener relaciones de artistas y cursos
    unordered_map<int, vector<int>> obtenerRelaciones() const;  // Relación de artistas
    unordered_map<int, unordered_map<int, int>> obtenerRelacionesCursos() const;  // Relación de cursos entre artistas

    // Guardar relaciones de artistas y cursos en archivo
    void guardarRelacionesCursosEnArchivo(const string& nombreArchivo) const;

    // Cargar relaciones de artistas y cursos desde archivo
    void cargarRelacionesCursosDesdeArchivo(const string& nombreArchivo);
};

#endif
