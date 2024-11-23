#ifndef CURSO_H
#define CURSO_H

#include <string>
using namespace std;

class Curso {
public:
    int codigo;
    string nombre;


    Curso(int codigo, string nombre);
    void mostrarInfo() const;
    int getCodigo() const { return codigo; }
    std::string getNombre() const { return nombre; }
};


#endif 
