#include "Grafo.h" 
#include <iostream>
#include <fstream>
#include "AVLArtista.h"
#include "AVLCurso.h"
#include "Artista.h"
#include <functional>
#include <sstream>
using namespace std;

void cargarArtistasDesdeArchivo(AVL& arbol, const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo) {
        cout << "No se pudo abrir el archivo: " << nombreArchivo << endl;
        return;
    }

    int cedula;
    string apellido, nombre, telefono, correo, provincia, canton, barrio;

    while (archivo >> cedula) {
        archivo.ignore();
        getline(archivo, apellido);
        getline(archivo, nombre);
        getline(archivo, telefono);
        getline(archivo, correo);
        getline(archivo, provincia);
        getline(archivo, canton);
        getline(archivo, barrio);

        Artista artista(cedula, apellido, nombre, telefono, correo, provincia, canton, barrio);
        arbol.agregar(artista);
    }
    archivo.close();
}

void guardarArtistasEnArchivo(Nodo* nodo, ofstream& archivo) {
    if (nodo) {
        guardarArtistasEnArchivo(nodo->izquierda, archivo);
        archivo << nodo->artista.cedula << '\n'
            << nodo->artista.apellido << '\n'
            << nodo->artista.nombre << '\n'
            << nodo->artista.telefono << '\n'
            << nodo->artista.correo << '\n'
            << nodo->artista.provincia << '\n'
            << nodo->artista.canton << '\n'
            << nodo->artista.barrio << '\n';
        guardarArtistasEnArchivo(nodo->derecha, archivo);
    }
}

void guardarCambios(const AVL& arbol, const string& nombreArchivo) {
    ofstream archivo(nombreArchivo);
    if (!archivo) {
        cout << "No se pudo abrir el archivo para escritura: " << nombreArchivo << endl;
        return;
    }
    guardarArtistasEnArchivo(arbol.raiz, archivo);
    archivo.close();
}

void agregarRelacionesDeCursos(Grafo& grafo, AVL& arbol) {
    int cedula1, cedula2;
    cout << "Ingrese la cedula del primer artista: ";
    cin >> cedula1;
    cout << "Ingrese la cedula del segundo artista: ";
    cin >> cedula2;

    if (cedula1 == cedula2) {
        cout << "Las cedulas ingresadas son iguales" << endl;
    }
    else {
        string nombre1 = arbol.buscarNombrePorCedula(arbol.raiz, cedula1);
        string nombre2 = arbol.buscarNombrePorCedula(arbol.raiz, cedula2);

        if (nombre1 == "No encontrado" || nombre2 == "No encontrado") {
            cout << "Verificar cedulas ya que una o las dos no corresponden a ningun artista registrado." << endl;
            return;
        }
        else {
            grafo.agregarArista(cedula1, cedula2);
            cout << "Relacion agregada exitosamente entre " << nombre1 << " y " << nombre2 << "." << endl;
        }
    }
}


void cargarCursosDesdeArchivo(AVLCurso& arbolCurso, const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo) {
        cout << "No se pudo abrir el archivo: " << nombreArchivo << endl;
        return;
    }

    int codigo;
    string nombre;

    while (archivo >> codigo) {
        archivo.ignore();
        getline(archivo, nombre);
        Curso curso(codigo, nombre);
        arbolCurso.agregar(curso);
    }
    archivo.close();
}

void guardarCursosEnArchivo(NodoCurso* nodo, ofstream& archivo) {
    if (nodo) {
        guardarCursosEnArchivo(nodo->izquierda, archivo);
        archivo << nodo->curso.codigo << '\n'
            << nodo->curso.nombre << '\n';
        guardarCursosEnArchivo(nodo->derecha, archivo);
    }
}

void guardarCambiosCursos(const AVLCurso& arbolCurso, const string& nombreArchivo) {
    ofstream archivo(nombreArchivo);
    if (!archivo) {
        cout << "No se pudo abrir el archivo para escritura: " << nombreArchivo << endl;
        return;
    }
    guardarCursosEnArchivo(arbolCurso.raiz, archivo);
    archivo.close();
}
void agregarEstudianteACurso(AVLCurso& arbolCurso, Grafo& grafo, AVL& arbolArtistas) {
    int codigoCurso, cedulaEstudiante;
    cout << "Ingrese el código del curso: ";
    cin >> codigoCurso;

    if (!arbolCurso.existeCodigo(codigoCurso)) {
        cout << "El curso con código " << codigoCurso << " no existe.\n";
        return;
    }

    cout << "Ingrese la cédula del estudiante: ";
    cin >> cedulaEstudiante;

    if (!arbolArtistas.existeCedula(cedulaEstudiante)) {
        cout << "El estudiante con cédula " << cedulaEstudiante << " no está registrado.\n";
        return;
    }

    if (arbolCurso.agregarEstudianteACurso(codigoCurso, cedulaEstudiante)) {
        // Relacionar al estudiante con otros en el curso
        vector<int> estudiantes = arbolCurso.obtenerEstudiantesDeCurso(codigoCurso);
        for (int otroEstudiante : estudiantes) {
            if (otroEstudiante != cedulaEstudiante) {
                grafo.agregarRelacionCurso(cedulaEstudiante, otroEstudiante);
            }
        }
        cout << "Estudiante agregado exitosamente al curso.\n";
    }
}
void consultarEstudiantesConCursosCompartidos(Grafo& grafo, AVL& arbolArtistas) {
    int cedula;
    cout << "Ingrese la cédula del estudiante: ";
    cin >> cedula;

    if (!arbolArtistas.existeCedula(cedula)) {
        cout << "La cédula ingresada no está registrada.\n";
        return;
    }

    auto relaciones = grafo.obtenerRelacionesCursos(); // Mapa completo de relaciones
    if (relaciones.find(cedula) == relaciones.end() || relaciones[cedula].empty()) {
        cout << "El estudiante no comparte cursos con nadie.\n";
        return;
    }

    cout << "Estudiantes con los que comparte al menos un curso:\n";
    for (const auto& par : relaciones[cedula]) { // Iterar sobre las relaciones del estudiante
        int cedulaCompanero = par.first;
        int cantidad = par.second;
        string nombreCompanero = arbolArtistas.buscarNombrePorCedula(arbolArtistas.raiz, cedulaCompanero);
        cout << "- " << nombreCompanero << " (Cédula: " << cedulaCompanero << ") - Cursos compartidos: " << cantidad << "\n";
    }
}




void mostrarArtistasConCursosCompartidos(Grafo& grafo, AVL& arbol) {
    int cedula;
    cout << "Ingrese la cedula del artista: ";
    cin >> cedula;

    if (!arbol.existeCedula(cedula)) {
        cout << "La cedula " << cedula << " no esta registrada en el sistema." << endl;
        return;
    }

    vector<int> adyacentes = grafo.obtenerAdyacentes(cedula);
    if (adyacentes.empty()) {
        cout << "El artista con cedula " << cedula << " no tiene relaciones registradas." << endl;
    }
    else {
        string nombreArtista = arbol.buscarNombrePorCedula(arbol.raiz, cedula);
        cout << "El artista " << nombreArtista << " ha compartido cursos con: " << endl;
        for (int vecino : adyacentes) {
            string nombreVecino = arbol.buscarNombrePorCedula(arbol.raiz, vecino);
            cout << "- " << nombreVecino << " (Cedula: " << vecino << ")" << endl;
        }
    }
}






void listarArtistasConCursosCompartidosPorCantidad(Grafo& grafo, AVL& arbol, int umbral) {
    auto relaciones = grafo.obtenerRelacionesCursos(); // Obtener las relaciones entre artistas por cursos

    cout << "Artistas que comparten al menos " << umbral << " cursos:\n";
    for (const auto& par1 : relaciones) { // Iterar sobre las relaciones
        int cedula1 = par1.first;
        for (const auto& par2 : par1.second) {
            int cedula2 = par2.first;
            int cantidad = par2.second;

            if (cantidad >= umbral && cedula1 < cedula2) {
                string nombreArtista1 = arbol.buscarNombrePorCedula(arbol.raiz, cedula1);
                string nombreArtista2 = arbol.buscarNombrePorCedula(arbol.raiz, cedula2);
                cout << "- " << nombreArtista1 << " y " << nombreArtista2 << " comparten " << cantidad << " cursos.\n";
            }
        }
    }
}
void guardarRelacionesYAsignaciones(const Grafo& grafo, const AVLCurso& arbolCurso, const string& nombreArchivo) {
    ofstream archivo(nombreArchivo);
    if (!archivo) {
        cout << "No se pudo abrir el archivo para guardar.\n";
        return;
    }

    // Guardar relaciones entre estudiantes
    archivo << "[Relaciones]\n";
    for (const auto& pareja : grafo.obtenerRelacionesCursos()) {
        int cedula1 = pareja.first; // Primera clave del map
        const auto& cursos = pareja.second;

        for (const auto& subpar : cursos) {
            int cedula2 = subpar.first; // Clave del submap
            int cantidad = subpar.second;
            archivo << cedula1 << " " << cedula2 << " " << cantidad << "\n";
        }
    }

    // Guardar asignaciones de estudiantes a cursos
    archivo << "[Asignaciones]\n";
    function<void(NodoCurso*)> guardarAsignaciones = [&](NodoCurso* nodo) {
        if (!nodo) return;
        guardarAsignaciones(nodo->izquierda);
        archivo << nodo->curso.codigo << ": ";
        for (int cedula : nodo->curso.obtenerEstudiantes()) {
            archivo << cedula << ",";
        }
        archivo.seekp(-1, ios_base::cur); // Elimina la última coma
        archivo << "\n";
        guardarAsignaciones(nodo->derecha);
        };
    guardarAsignaciones(arbolCurso.raiz);

    archivo.close();
    cout << "Relaciones y asignaciones guardadas en " << nombreArchivo << ".\n";
}

void cargarRelacionesYAsignaciones(Grafo& grafo, AVLCurso& arbolCurso, const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo) {
        cout << "No se pudo abrir el archivo para cargar.\n";
        return;
    }

    string linea;
    bool leyendoRelaciones = false;
    bool leyendoAsignaciones = false;

    while (getline(archivo, linea)) {
        if (linea == "[Relaciones]") {
            leyendoRelaciones = true;
            leyendoAsignaciones = false;
            continue;
        }
        else if (linea == "[Asignaciones]") {
            leyendoRelaciones = false;
            leyendoAsignaciones = true;
            continue;
        }

        if (leyendoRelaciones) {
            istringstream iss(linea);
            int cedula1, cedula2, cantidad;
            iss >> cedula1 >> cedula2 >> cantidad;
            grafo.agregarRelacionCurso(cedula1, cedula2);
        }
        else if (leyendoAsignaciones) {
            size_t pos = linea.find(":");
            int codigoCurso = stoi(linea.substr(0, pos));
            string estudiantes = linea.substr(pos + 1);

            istringstream iss(estudiantes);
            string cedulaStr;
            while (getline(iss, cedulaStr, ',')) {
                arbolCurso.agregarEstudianteACurso(codigoCurso, stoi(cedulaStr));
            }
        }
    }

    archivo.close();
    cout << "Relaciones y asignaciones cargadas desde " << nombreArchivo << ".\n";
}




int mostrarMenu() {
    int opcion;
    cout << "\n--- MENU GESTION DE ARTISTAS ---\n";
    cout << "1. Agregar Artista\n";
    cout << "2. Eliminar Artista\n";
    cout << "3. Mostrar Artistas en Orden Ascendente\n";
    cout << "4. Mostrar Artistas en Orden Descendente\n";
    cout << "5. Consultar Datos de un Artista\n";
    cout << "6. Agregar relacion de cursos\n";
    cout << "7. Mostrar relaciones de un artista\n";
    cout << "8. Agregar Curso\n";
    cout << "9. Eliminar Curso\n";
    cout << "10. Mostrar Cursos en Orden Ascendente\n";
    cout << "11. Mostrar Cursos en Orden Descendente\n";
    cout << "12. Consultar Datos de un Curso\n";
    cout << "13. Agregar estudiante a un curso\n";
    cout << "14. Consultar estudiantes con los que comparte cursos\n";
    cout << "15. Listar estudiantes que comparten al menos una cantidad de cursos\n";
    cout << "16. Guardar y Salir\n";
    cout << "Seleccione una opcion: ";
    cin >> opcion;
    return opcion;
}

int main() {
    AVL arbol;
    Grafo grafo;
    string nombreArchivo = "artistas.txt";
    cargarArtistasDesdeArchivo(arbol, nombreArchivo);
    AVLCurso arbolCurso;
    string archivoCursos = "cursos.txt";
    cargarCursosDesdeArchivo(arbolCurso, archivoCursos);
    string archivoRelaciones = "relaciones_y_asignaciones.txt";
    cargarRelacionesYAsignaciones(grafo, arbolCurso, archivoRelaciones);

    int opcion;
    do {
        opcion = mostrarMenu();
        switch (opcion) {
        case 1: {
            int cedula;
            string apellido, nombre, telefono, correo, provincia, canton, barrio;

            cout << "Ingrese cedula: ";
            cin >> cedula;

            if (arbol.existeCedula(cedula)) {
                cout << "Error: Ya existe un artista con la cedula " << cedula << ".\n";
                break;
            }

            cin.ignore();
            cout << "Ingrese apellido: ";
            getline(cin, apellido);
            cout << "Ingrese nombre: ";
            getline(cin, nombre);
            cout << "Ingrese telefono: ";
            getline(cin, telefono);
            cout << "Ingrese correo: ";
            getline(cin, correo);
            cout << "Ingrese provincia: ";
            getline(cin, provincia);
            cout << "Ingrese canton: ";
            getline(cin, canton);
            cout << "Ingrese barrio: ";
            getline(cin, barrio);

            Artista nuevoArtista(cedula, apellido, nombre, telefono, correo, provincia, canton, barrio);
            arbol.agregar(nuevoArtista);
            guardarCambios(arbol, "artistas.txt");
            cout << "Artista agregado con exito.\n";
            break;
        }

        case 2: {
            int cedula;
            cout << "Ingrese la cedula del artista a eliminar: ";
            cin >> cedula;
            arbol.eliminar(cedula);
            cout << "Artista eliminado (si existia).\n";
            break;
        }
        case 3:
            cout << "Artistas en Orden Ascendente:\n";
            arbol.mostrarAscendente();
            break;
        case 4:
            cout << "Artistas en Orden Descendente:\n";
            arbol.mostrarDescendente();
            break;
        case 5: {
            int cedula;
            cout << "Ingrese la cedula del artista: ";
            cin >> cedula;
            arbol.buscar(cedula);
            break;
        }
        case 6:
            agregarRelacionesDeCursos(grafo, arbol);
            break;
        case 7:
            mostrarArtistasConCursosCompartidos(grafo, arbol);
            break;
        case 8: {
            int codigo;
            string nombre;

            cout << "Ingrese codigo del curso: ";
            cin >> codigo;

            if (arbolCurso.existeCodigo(codigo)) {
                cout << "Error: Ya existe un curso con el ID " << codigo << ".\n";
                break;
            }

            cin.ignore();
            cout << "Ingrese nombre del curso: ";
            getline(cin, nombre);

            Curso nuevoCurso(codigo, nombre);
            arbolCurso.agregar(nuevoCurso);
            guardarCambiosCursos(arbolCurso, "cursos.txt");
            cout << "Curso agregado con exito.\n";
            break;
        }
        case 9: {
            int codigo;
            cout << "Ingrese el codigo del curso a eliminar: ";
            cin >> codigo;
            arbolCurso.eliminar(codigo);
            guardarCambiosCursos(arbolCurso, archivoCursos);
            cout << "Curso eliminado (si existia).\n";
            break;
        }

        case 10:
            cout << "Cursos en Orden Ascendente:\n";
            arbolCurso.mostrarAscendente();
            break;

        case 11:
            cout << "Cursos en Orden Descendente:\n";
            arbolCurso.mostrarDescendente();
            break;


        case 12:

            int codigo;
            cout << "Ingrese el codigo del curso a buscar: ";
            cin >> codigo;

            arbolCurso.buscarCurso(codigo);
            break;
        case 13:
           

            agregarEstudianteACurso(arbolCurso, grafo, arbol);
            guardarRelacionesYAsignaciones(grafo, arbolCurso, "relaciones_y_asignaciones.txt");
            guardarCambios(arbol, nombreArchivo);
            guardarCambiosCursos(arbolCurso, archivoCursos);
            break;
        case 14:
            consultarEstudiantesConCursosCompartidos(grafo, arbol);
            guardarRelacionesYAsignaciones(grafo, arbolCurso, "relaciones_y_asignaciones.txt");
            guardarCambios(arbol, nombreArchivo);
            guardarCambiosCursos(arbolCurso, archivoCursos);
            break;
        case 15:
            int umbral;
            cout << "Ingrese el numero minimo de cursos compartidos para mostrar la relacion: ";
            cin >> umbral;

            listarArtistasConCursosCompartidosPorCantidad(grafo, arbol, umbral);
            break;

        case 16:
            guardarCambios(arbol, "artistas.txt");
            guardarCambiosCursos(arbolCurso, "cursos.txt");
            guardarRelacionesYAsignaciones(grafo, arbolCurso, "relaciones_y_asignaciones.txt");
            guardarCambios(arbol, nombreArchivo);
            guardarCambiosCursos(arbolCurso, archivoCursos);
            cout << "Cambios guardados. Saliendo...\n";
            break;
        default:
            cout << "Opción no valida.\n";
        }
    } while (opcion != 16);

    return 0;
}
