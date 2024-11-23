
#ifndef ARTISTA_H
#define ARTISTA_H

#include <string>
using namespace std;

class Artista {
public:
    int cedula;
    string apellido;
    string nombre;
    string telefono;
    string correo;
    string provincia;
    string canton;
    string barrio;

    Artista(int cedula, string apellido, string nombre, string telefono,
        string correo, string provincia, string canton, string barrio);
    void mostrarInfo() const;
};

#endif 
