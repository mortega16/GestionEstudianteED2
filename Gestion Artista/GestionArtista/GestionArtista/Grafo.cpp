#include "Grafo.h"
#include <fstream>
#include <sstream>

void Grafo::agregarArista(int cedula1, int cedula2) {
    adyacencias[cedula1].push_back(cedula2);
    adyacencias[cedula2].push_back(cedula1);
}

void Grafo::eliminarArista(int cedula1, int cedula2) {
    auto& vecinos1 = adyacencias[cedula1];
    auto& vecinos2 = adyacencias[cedula2];

    vecinos1.erase(remove(vecinos1.begin(), vecinos1.end(), cedula2), vecinos1.end());
    vecinos2.erase(remove(vecinos2.begin(), vecinos2.end(), cedula1), vecinos2.end());
}

vector<int> Grafo::obtenerAdyacentes(int cedula) {
    if (adyacencias.find(cedula) != adyacencias.end()) {
        return adyacencias[cedula];
    }
    return {};
}

void Grafo::mostrarRelaciones() {
    for (const auto& par : adyacencias) {
        cout << "Artista con cédula " << par.first << " está conectado con: ";
        for (int vecino : par.second) {
            cout << vecino << " ";
        }
        cout << endl;
    }
}

void Grafo::agregarRelacionCurso(int cedula1, int cedula2) {
    // Si ya existe la relación, incrementa el contador de cursos compartidos
    if (relacionesCursos[cedula1].find(cedula2) != relacionesCursos[cedula1].end()) {
        relacionesCursos[cedula1][cedula2]++;
        relacionesCursos[cedula2][cedula1]++;
    }
    else {
        // Si no existe, la crea
        relacionesCursos[cedula1][cedula2] = 1;
        relacionesCursos[cedula2][cedula1] = 1;
    }
}

void Grafo::eliminarRelacionCurso(int cedula1, int cedula2) {
    // Elimina la relación de curso entre los dos artistas
    if (relacionesCursos[cedula1].find(cedula2) != relacionesCursos[cedula1].end()) {
        relacionesCursos[cedula1].erase(cedula2);
        relacionesCursos[cedula2].erase(cedula1);
    }
}

unordered_map<int, vector<int>> Grafo::obtenerRelaciones() const {
    return adyacencias;
}

unordered_map<int, unordered_map<int, int>> Grafo::obtenerRelacionesCursos() const {
    return relacionesCursos;
}

// Guardar las relaciones de cursos en un archivo
void Grafo::guardarRelacionesCursosEnArchivo(const string& nombreArchivo) const {
    ofstream archivo(nombreArchivo);
    if (!archivo) {
        cout << "No se pudo abrir el archivo para escritura: " << nombreArchivo << endl;
        return;
    }

    for (const auto& par : relacionesCursos) {
        for (const auto& subpar : par.second) {
            archivo << par.first << " " << subpar.first << " " << subpar.second << "\n";
        }
    }
    archivo.close();
}

// Cargar las relaciones de cursos desde un archivo
void Grafo::cargarRelacionesCursosDesdeArchivo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo) {
        cout << "No se pudo abrir el archivo: " << nombreArchivo << endl;
        return;
    }

    int cedula1, cedula2, cantidad;
    while (archivo >> cedula1 >> cedula2 >> cantidad) {
        // Asumimos que la relación entre artistas es bidireccional
        relacionesCursos[cedula1][cedula2] = cantidad;
        relacionesCursos[cedula2][cedula1] = cantidad;
    }
    archivo.close();
}
