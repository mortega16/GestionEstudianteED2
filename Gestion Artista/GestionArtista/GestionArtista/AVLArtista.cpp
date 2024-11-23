#include "AVLArtista.h"
#include "Artista.h"
#include <iostream>
#include <algorithm>
using namespace std;

Nodo::Nodo(const Artista& artista)
    : artista(artista), izquierda(nullptr), derecha(nullptr), altura(1) {}

AVL::AVL() : raiz(nullptr) {}

int AVL::altura(Nodo* nodo) {
    return nodo ? nodo->altura : 0;
}

int AVL::balanceFactor(Nodo* nodo) {
    return altura(nodo->izquierda) - altura(nodo->derecha);
}

Nodo* AVL::rotacionDerecha(Nodo* y) {
    Nodo* x = y->izquierda;
    Nodo* T2 = x->derecha;

    x->derecha = y;
    y->izquierda = T2;

    y->altura = max(altura(y->izquierda), altura(y->derecha)) + 1;
    x->altura = max(altura(x->izquierda), altura(x->derecha)) + 1;

    return x;
}

Nodo* AVL::rotacionIzquierda(Nodo* x) {
    Nodo* y = x->derecha;
    Nodo* T2 = y->izquierda;

    y->izquierda = x;
    x->derecha = T2;

    x->altura = max(altura(x->izquierda), altura(x->derecha)) + 1;
    y->altura = max(altura(y->izquierda), altura(y->derecha)) + 1;

    return y;
}

Nodo* AVL::agregarRecursivo(Nodo* nodo, const Artista& artista) {
    if (!nodo) return new Nodo(artista);

    if (artista.cedula < nodo->artista.cedula)
        nodo->izquierda = agregarRecursivo(nodo->izquierda, artista);
    else if (artista.cedula > nodo->artista.cedula)
        nodo->derecha = agregarRecursivo(nodo->derecha, artista);
    else
        return nodo;

    nodo->altura = 1 + max(altura(nodo->izquierda), altura(nodo->derecha));
    int balance = balanceFactor(nodo);

    if (balance > 1 && artista.cedula < nodo->izquierda->artista.cedula)
        return rotacionDerecha(nodo);

    if (balance < -1 && artista.cedula > nodo->derecha->artista.cedula)
        return rotacionIzquierda(nodo);

    if (balance > 1 && artista.cedula > nodo->izquierda->artista.cedula) {
        nodo->izquierda = rotacionIzquierda(nodo->izquierda);
        return rotacionDerecha(nodo);
    }

    if (balance < -1 && artista.cedula < nodo->derecha->artista.cedula) {
        nodo->derecha = rotacionDerecha(nodo->derecha);
        return rotacionIzquierda(nodo);
    }

    return nodo;
}

void AVL::agregar(const Artista& artista) {
    raiz = agregarRecursivo(raiz, artista);
}


Nodo* AVL::eliminarRecursivo(Nodo* nodo, int cedula) {
    if (nodo == nullptr) {
        return nodo;
    }

    if (cedula < nodo->artista.cedula) {
        nodo->izquierda = eliminarRecursivo(nodo->izquierda, cedula);
    }
    else if (cedula > nodo->artista.cedula) {
        nodo->derecha = eliminarRecursivo(nodo->derecha, cedula);
    }
    else {
        
        if ((nodo->izquierda == nullptr) || (nodo->derecha == nullptr)) {
            Nodo* temp = nodo->izquierda ? nodo->izquierda : nodo->derecha;

            if (temp == nullptr) {
                temp = nodo;
                nodo = nullptr;
            }
            else {
                *nodo = *temp;
            }
            delete temp;
        }
        else {
            Nodo* temp = buscarMinimo(nodo->derecha);

            nodo->artista = temp->artista;

            nodo->derecha = eliminarRecursivo(nodo->derecha, temp->artista.cedula);
        }
    }

    if (nodo == nullptr) {
        return nodo;
    }

    nodo->altura = 1 + std::max(altura(nodo->izquierda), altura(nodo->derecha));

    int balance = balanceFactor(nodo);


    if (balance > 1 && balanceFactor(nodo->izquierda) >= 0) {
        return rotacionDerecha(nodo);
    }

    if (balance > 1 && balanceFactor(nodo->izquierda) < 0) {
        nodo->izquierda = rotacionIzquierda(nodo->izquierda);
        return rotacionDerecha(nodo);
    }

    if (balance < -1 && balanceFactor(nodo->derecha) <= 0) {
        return rotacionIzquierda(nodo);
    }

    if (balance < -1 && balanceFactor(nodo->derecha) > 0) {
        nodo->derecha = rotacionDerecha(nodo->derecha);
        return rotacionIzquierda(nodo);
    }

    return nodo;
}

void AVL::eliminar(int cedula) {
    raiz = eliminarRecursivo(raiz, cedula);
}


void AVL::inOrden(Nodo* nodo) {
    if (nodo) {
        inOrden(nodo->izquierda);
        nodo->artista.mostrarInfo();
        inOrden(nodo->derecha);
    }
}

void AVL::mostrarAscendente() {
    inOrden(raiz);
}

void AVL::inOrdenDescendente(Nodo* nodo) {
    if (nodo) {
        inOrdenDescendente(nodo->derecha);
        nodo->artista.mostrarInfo();
        inOrdenDescendente(nodo->izquierda);
    }
}

void AVL::mostrarDescendente() {
    inOrdenDescendente(raiz);
}

void AVL::buscar(int cedula) {
    mostrarArtista(raiz, cedula);
}

void AVL::mostrarArtista(Nodo* nodo, int cedula) {
    if (!nodo) {
        cout << "Artista no encontrado.\n";
        return;
    }

    if (cedula < nodo->artista.cedula)
        mostrarArtista(nodo->izquierda, cedula);
    else if (cedula > nodo->artista.cedula)
        mostrarArtista(nodo->derecha, cedula);
    else
        nodo->artista.mostrarInfo();
}
Nodo* AVL::buscarMinimo(Nodo* nodo) {
    Nodo* actual = nodo;
    while (actual->izquierda != nullptr) {
        actual = actual->izquierda;
    }
    return actual;
}


string AVL::buscarNombrePorCedula(Nodo* nodo, int cedula) {
    if (!nodo) {
        return "No encontrado"; 
    }
    if (cedula == nodo->artista.cedula) {
        return nodo->artista.nombre;
    }
    if (cedula < nodo->artista.cedula) {
        return buscarNombrePorCedula(nodo->izquierda, cedula);
    }
    else {
        return buscarNombrePorCedula(nodo->derecha, cedula);
    }
}

bool AVL::existeCedula(Nodo* nodo, int cedula) {
    if (!nodo) return false; 
    if (cedula == nodo->artista.cedula) return true; 
    if (cedula < nodo->artista.cedula)
        return existeCedula(nodo->izquierda, cedula);
    return existeCedula(nodo->derecha, cedula);
}

bool AVL::existeCedula(int cedula) {
    return existeCedula(raiz, cedula);
}
