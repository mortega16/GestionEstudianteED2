#include "AVLCurso.h"
#include <iostream>
#include <algorithm>
using namespace std;

NodoCurso::NodoCurso(const Curso& curso)
    : curso(curso), izquierda(nullptr), derecha(nullptr), altura(1) {}

AVLCurso::AVLCurso() : raiz(nullptr) {}

int AVLCurso::altura(NodoCurso* nodo) {
    return nodo ? nodo->altura : 0;
}

int AVLCurso::balanceFactor(NodoCurso* nodo) {
    return altura(nodo->izquierda) - altura(nodo->derecha);
}



NodoCurso* AVLCurso::rotacionDerecha(NodoCurso* y) {
    NodoCurso* x = y->izquierda;
    NodoCurso* T2 = x->derecha;

    x->derecha = y;
    y->izquierda = T2;

    y->altura = max(altura(y->izquierda), altura(y->derecha)) + 1;
    x->altura = max(altura(x->izquierda), altura(x->derecha)) + 1;

    return x;
}

NodoCurso* AVLCurso::rotacionIzquierda(NodoCurso* x) {
    NodoCurso* y = x->derecha;
    NodoCurso* T2 = y->izquierda;

    y->izquierda = x;
    x->derecha = T2;

    x->altura = max(altura(x->izquierda), altura(x->derecha)) + 1;
    y->altura = max(altura(y->izquierda), altura(y->derecha)) + 1;

    return y;
}

NodoCurso* AVLCurso::agregarRecursivo(NodoCurso* nodo, const Curso& curso) {
    if (!nodo) return new NodoCurso(curso);

    if (curso.codigo < nodo->curso.codigo)
        nodo->izquierda = agregarRecursivo(nodo->izquierda, curso);
    else if (curso.codigo > nodo->curso.codigo)
        nodo->derecha = agregarRecursivo(nodo->derecha, curso);
    else
        return nodo;

    nodo->altura = 1 + max(altura(nodo->izquierda), altura(nodo->derecha));
    int balance = balanceFactor(nodo);

    if (balance > 1 && curso.codigo < nodo->izquierda->curso.codigo)
        return rotacionDerecha(nodo);

    if (balance < -1 && curso.codigo > nodo->derecha->curso.codigo)
        return rotacionIzquierda(nodo);

    if (balance > 1 && curso.codigo > nodo->izquierda->curso.codigo) {
        nodo->izquierda = rotacionIzquierda(nodo->izquierda);
        return rotacionDerecha(nodo);
    }

    if (balance < -1 && curso.codigo < nodo->derecha->curso.codigo) {
        nodo->derecha = rotacionDerecha(nodo->derecha);
        return rotacionIzquierda(nodo);
    }

    return nodo;
}

void AVLCurso::agregar(const Curso& curso) {
    raiz = agregarRecursivo(raiz, curso);
}

void AVLCurso::inOrden(NodoCurso* nodo) {
    if (nodo) {
        inOrden(nodo->izquierda);
        nodo->curso.mostrarInfo();
        inOrden(nodo->derecha);
    }
}

void AVLCurso::mostrarAscendente() {
    inOrden(raiz);
}

void AVLCurso::inOrdenDescendente(NodoCurso* nodo) {
    if (nodo) {
        inOrdenDescendente(nodo->derecha);
        nodo->curso.mostrarInfo();
        inOrdenDescendente(nodo->izquierda);
    }
}

void AVLCurso::mostrarDescendente() {
    inOrdenDescendente(raiz);
}

NodoCurso* AVLCurso::buscarCursoRecursivo(NodoCurso* nodo, int codigo) {
    if (!nodo || nodo->curso.codigo == codigo) return nodo;

    if (codigo < nodo->curso.codigo)
        return buscarCursoRecursivo(nodo->izquierda, codigo);
    return buscarCursoRecursivo(nodo->derecha, codigo);
}

void AVLCurso::buscarCurso(int codigo) {
    NodoCurso* resultado = buscarCursoRecursivo(raiz, codigo);
    if (resultado)
        resultado->curso.mostrarInfo();
    else
        cout << "Curso no encontrado." << endl;
}
bool AVLCurso::existeCodigo(int codigoCurso) {
    return existeCodigo(raiz, codigoCurso);
}

bool AVLCurso::existeCodigo(NodoCurso* nodo, int codigoCurso) {
    if (nodo == nullptr) {
        return false;  
    }

    if (codigoCurso == nodo->curso.getCodigo()) {
        return true;  
    }

    if (codigoCurso < nodo->curso.getCodigo()) {
        return existeCodigo(nodo->izquierda, codigoCurso); 
    }
    else {
        return existeCodigo(nodo->derecha, codigoCurso);  
    }
}
void AVLCurso::eliminar(int codigo) {
    raiz = eliminarRecursivo(raiz, codigo);
}
NodoCurso* AVLCurso::eliminarRecursivo(NodoCurso* nodo, int codigo) {
    if (!nodo) return nullptr; 

    if (codigo < nodo->curso.codigo) {
        nodo->izquierda = eliminarRecursivo(nodo->izquierda, codigo);
    }
    else if (codigo > nodo->curso.codigo) {
        nodo->derecha = eliminarRecursivo(nodo->derecha, codigo);
    }
    else {

        if (!nodo->izquierda && !nodo->derecha) {
            delete nodo;
            return nullptr;
        }

        if (!nodo->izquierda) {
            NodoCurso* temp = nodo->derecha;
            delete nodo;
            return temp;
        }
        else if (!nodo->derecha) {
            NodoCurso* temp = nodo->izquierda;
            delete nodo;
            return temp;
        }

        NodoCurso* sucesor = obtenerMinimo(nodo->derecha);
        nodo->curso = sucesor->curso; 
        nodo->derecha = eliminarRecursivo(nodo->derecha, sucesor->curso.codigo);
    }

    nodo->altura = 1 + max(altura(nodo->izquierda), altura(nodo->derecha));

    int balance = balanceFactor(nodo);

    if (balance > 1 && balanceFactor(nodo->izquierda) >= 0)
        return rotacionDerecha(nodo);

    if (balance > 1 && balanceFactor(nodo->izquierda) < 0) {
        nodo->izquierda = rotacionIzquierda(nodo->izquierda);
        return rotacionDerecha(nodo);
    }

    if (balance < -1 && balanceFactor(nodo->derecha) <= 0)
        return rotacionIzquierda(nodo);

    if (balance < -1 && balanceFactor(nodo->derecha) > 0) {
        nodo->derecha = rotacionDerecha(nodo->derecha);
        return rotacionIzquierda(nodo);
    }

    return nodo;
}

NodoCurso* AVLCurso::obtenerMinimo(NodoCurso* nodo) {
    NodoCurso* actual = nodo;
    while (actual && actual->izquierda) {
        actual = actual->izquierda;
    }
    return actual;
}



