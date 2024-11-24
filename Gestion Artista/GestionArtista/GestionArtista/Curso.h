#ifndef CURSO_H
#define CURSO_H

#include <string>
#include <set> 
#include <vector> 
using namespace std;

class Curso {
public:
    int codigo;
    string nombre;
    set<int> estudiantes;


    Curso(int codigo, string nombre);
    void mostrarInfo() const;
    int getCodigo() const { return codigo; }
    std::string getNombre() const { return nombre; }
    std::vector<int> obtenerEstudiantes() const {
        return std::vector<int>(estudiantes.begin(), estudiantes.end()); // Convierte el set a un vector
    }


    void agregarEstudiante(int cedula) {
        estudiantes.insert(cedula);
    }
    void eliminarEstudiante(int cedula) {
        estudiantes.erase(cedula);
    }
};


#endif 
