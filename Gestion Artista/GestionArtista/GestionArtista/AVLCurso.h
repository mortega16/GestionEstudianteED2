#ifndef AVLCurso_H
#define AVLCurso_H

#include "Curso.h"
#include <string>
#include <vector>

class NodoCurso {
public:
    Curso curso;
    NodoCurso* izquierda;
    NodoCurso* derecha;
    int altura;

    NodoCurso(const Curso& curso);
};

class AVLCurso {
public:
    NodoCurso* raiz;

    AVLCurso();

    // Métodos de manejo de cursos
    void agregar(const Curso& curso);
    void mostrarAscendente();
    void mostrarDescendente();
    void buscarCurso(int codigo);
    bool existeCodigo(int codigo);
    void eliminar(int codigo);

    // Métodos de manejo de estudiantes
    bool agregarEstudianteACurso(int codigoCurso, int cedulaEstudiante);
    bool eliminarEstudianteDeCurso(int codigoCurso, int cedulaEstudiante);
    std::vector<int> obtenerEstudiantesDeCurso(int codigoCurso);

private:
    // Métodos internos de AVL
    NodoCurso* agregarRecursivo(NodoCurso* nodo, const Curso& curso);
    NodoCurso* rotacionDerecha(NodoCurso* nodo);
    NodoCurso* rotacionIzquierda(NodoCurso* nodo);
    int altura(NodoCurso* nodo);
    int balanceFactor(NodoCurso* nodo);
    void inOrden(NodoCurso* nodo);
    void inOrdenDescendente(NodoCurso* nodo);
    bool existeCodigo(NodoCurso* nodo, int codigoCurso);
    NodoCurso* buscarCursoRecursivo(NodoCurso* nodo, int codigo);
    NodoCurso* eliminarRecursivo(NodoCurso* nodo, int codigo);
    NodoCurso* obtenerMinimo(NodoCurso* nodo);
};

#endif
