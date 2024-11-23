#include "Artista.h"
#include <iostream>
using namespace std;

Artista::Artista(int cedula, string apellido, string nombre, string telefono,
    string correo, string provincia, string canton, string barrio)
    : cedula(cedula), apellido(apellido), nombre(nombre),
    telefono(telefono), correo(correo), provincia(provincia),
    canton(canton), barrio(barrio) {}

void Artista::mostrarInfo() const {
    cout << "Cedula: " << cedula << endl;
    cout << "Nombre: " << nombre << " " << apellido << endl;
    cout << "Telefono: " << telefono << endl;
    cout << "Correo: " << correo << endl;
    cout << "Provincia: " << provincia << ", Canton: " << canton
        << ", Barrio: " << barrio << endl;
}
