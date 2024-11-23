#ifndef AVLArtista_H
#define AVLArtista_H

#include "Artista.h"

class Nodo {
public:
    Artista artista;
    Nodo* izquierda;
    Nodo* derecha;
    int altura;

    Nodo(const Artista& artista);
};

class AVL {
public:
    Nodo* raiz;

    AVL();
    void agregar(const Artista& artista);
    void eliminar(int cedula);
    void mostrarAscendente();
    void mostrarDescendente();
    void buscar(int cedula);
    string buscarNombrePorCedula(Nodo* nodo, int cedula);

    bool existeCedula(int cedula);

private:
    Nodo* agregarRecursivo(Nodo* nodo, const Artista& artista);
    Nodo* eliminarRecursivo(Nodo* nodo, int cedula);
    Nodo* buscarMinimo(Nodo* nodo);
    Nodo* rotacionDerecha(Nodo* nodo);
    Nodo* rotacionIzquierda(Nodo* nodo);
    int altura(Nodo* nodo);
    int balanceFactor(Nodo* nodo);
    void inOrden(Nodo* nodo);
    void inOrdenDescendente(Nodo* nodo);
    void mostrarArtista(Nodo* nodo, int cedula);

    bool existeCedula(Nodo* nodo, int cedula);
};

#endif

