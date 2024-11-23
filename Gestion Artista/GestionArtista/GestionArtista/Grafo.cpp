#include "Grafo.h"

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

void Grafo::agregarCursoAArtistas(int cedula1, int cedula2) {
    if (relacionesCursos[cedula1].find(cedula2) != relacionesCursos[cedula1].end()) {
        relacionesCursos[cedula1][cedula2]++;
        relacionesCursos[cedula2][cedula1]++;  
    }
    else {
        relacionesCursos[cedula1][cedula2] = 1;
        relacionesCursos[cedula2][cedula1] = 1;  
    }
}

void Grafo::eliminarCursoDeArtistas(int cedula1, int cedula2) {
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
