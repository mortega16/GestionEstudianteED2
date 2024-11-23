#include "Curso.h"
#include <iostream>
using namespace std;

Curso::Curso(int codigo, string nombre)
    : codigo(codigo), nombre(nombre) {}

void Curso::mostrarInfo() const {
    cout << "Codigo: " << codigo << endl;
    cout << "Nombre: " << nombre << endl;

}
