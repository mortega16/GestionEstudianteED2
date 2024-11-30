#include "Grafo.h" 
#include <iostream>
#include <fstream>
#include "AVLArtista.h"
#include "AVLCurso.h"
#include "Artista.h"
#include <functional>
#include <sstream>
using namespace std;


int encontrarIndice(const vector<int>& cedulas, int cedula) {
    for (int i = 0; i < cedulas.size(); ++i) {
        if (cedulas[i] == cedula) {
            return i;
        }
    }
    return -1; // No encontrado
}

void cargarArtistasDesdeArchivo(AVL& arbol, vector<int>& cedulas, const string& nombreArchivo) {
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

        if (encontrarIndice(cedulas, cedula) == -1) {
            cedulas.push_back(cedula); // Agregar cédula única
        }
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
void agregarEstudianteACurso(AVLCurso& arbolCurso, Grafo& grafo, AVL& arbolArtistas, const vector<int>& cedulas) {
    int codigoCurso, cedulaEstudiante;
    cout << "Ingrese el código del curso: ";
    cin >> codigoCurso;

    if (!arbolCurso.existeCodigo(codigoCurso)) {
        cout << "El curso con código " << codigoCurso << " no existe.\n";
        return;
    }

    cout << "Ingrese la cedula del estudiante: ";
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
                // Aquí usamos la función para agregar la relación entre los estudiantes
                grafo.agregarRelacionCurso(cedulaEstudiante, otroEstudiante, cedulas);
            }
        }
        cout << "Estudiante agregado exitosamente al curso.\n";
    }
}



void consultarEstudiantesConCursosCompartidos(Grafo& grafo, AVL& arbolArtistas, const vector<int>& cedulas) {
    int cedula;
    cout << "Ingrese la cedula del estudiante: ";
    cin >> cedula;

    int index = encontrarIndice(cedulas, cedula);
    if (index == -1) {
        cout << "La cédula ingresada no está registrada.\n";
        return;
    }

    auto relaciones = grafo.obtenerRelacionesCursos();
    bool tieneRelaciones = false;

    cout << "Estudiantes con los que comparte al menos un curso:\n";
    for (int i = 0; i < relaciones.size(); ++i) {
        if (relaciones[index][i] > 0) {  // Relación encontrada (cursos compartidos)
            tieneRelaciones = true;
            int cedulaCompanero = cedulas[i]; // Convertir índice de vuelta a cédula
            string nombreCompanero = arbolArtistas.buscarNombrePorCedula(arbolArtistas.raiz, cedulaCompanero);
            cout << "- " << nombreCompanero << " (Cédula: " << cedulaCompanero
                << ") - Cursos compartidos: " << relaciones[index][i] << "\n";
        }
    }

    if (!tieneRelaciones) {
        cout << "El estudiante no comparte cursos con nadie.\n";
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



void listarArtistasConCursosCompartidosPorCantidad(Grafo& grafo, AVL& arbol, const vector<int>& cedulas, int umbral) {
    auto relaciones = grafo.obtenerRelacionesCursos();  // Obtenemos la matriz de relaciones de cursos
    cout << "Artistas que comparten al menos " << umbral << " cursos:\n";

    bool hayRelaciones = false;

    // Recorremos la matriz de relaciones de cursos
    for (int i = 0; i < relaciones.size(); ++i) {
        for (int j = i + 1; j < relaciones.size(); ++j) {  // Evitar duplicados
            // Solo mostrar relaciones que cumplen con el umbral
            if (relaciones[i][j] >= umbral) {
                // Mostrar nombres y cédulas de los artistas
               
                string nombreArtista1 = arbol.buscarNombrePorCedula(arbol.raiz, i);  // Buscar el nombre del primer artista
                string nombreArtista2 = arbol.buscarNombrePorCedula(arbol.raiz, j);  // Buscar el nombre del segundo artista

                cout << "- " << "Artista con cedula" << " (Cedula: " << cedulas[i] << ") y "
                    << "Artista con cedula" << " (Cedula: " << cedulas[j] << ") comparten "
                    << relaciones[i][j] << " cursos.\n";

                hayRelaciones = true;  // Al menos una relación que cumple con el umbral
            }
        }
    }

    if (!hayRelaciones) {
        cout << "No se encontraron artistas que compartan al menos " << umbral << " cursos.\n";
    }
}







void guardarRelacionesYAsignaciones(const Grafo& grafo, const AVLCurso& arbolCurso, const vector<int>& cedulas, const string& nombreArchivo) {
    ofstream archivo(nombreArchivo);
    if (!archivo) {
        cout << "No se pudo abrir el archivo para guardar.\n";
        return;
    }

    // Guardar relaciones entre estudiantes
    auto relaciones = grafo.obtenerRelacionesCursos();
    archivo << "[Relaciones]\n";
    for (int i = 0; i < relaciones.size(); ++i) {
        for (int j = i + 1; j < relaciones.size(); ++j) { // Evitar duplicados
            if (relaciones[i][j] > 0) { // Solo escribir relaciones existentes
                archivo << cedulas[i] << " " << cedulas[j] << " " << relaciones[i][j] << "\n";
            }
        }
    }

    // Guardar asignaciones de estudiantes a cursos
    archivo << "[Asignaciones]\n";
    function<void(NodoCurso*)> guardarAsignaciones = [&](NodoCurso* nodo) {
        if (!nodo) return;
        guardarAsignaciones(nodo->izquierda);
        archivo << nodo->curso.codigo << ": ";
        for (int cedula : nodo->curso.estudiantes) {
            archivo << cedula << ",";
        }
        archivo.seekp(-1, ios_base::cur); // Elimina la última coma
        archivo << "\n";
        guardarAsignaciones(nodo->derecha);
        };
    guardarAsignaciones(arbolCurso.raiz);

    archivo.close();
    //cout << "Relaciones y asignaciones guardadas en " << nombreArchivo << ".\n";
}





void cargarRelacionesYAsignaciones(Grafo& grafo, AVLCurso& arbolCurso, const vector<int>& cedulas, const string& nombreArchivo) {
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

            // Convertir las cédulas a índices
            int index1 = encontrarIndice(cedulas, cedula1);
            int index2 = encontrarIndice(cedulas, cedula2);

            if (index1 != -1 && index2 != -1) {
                // Acceder directamente a la matriz mutable
                auto& matrizCursos = grafo.obtenerRelacionesCursosMutable();

                // Si la relación no existe, inicializar con el valor del archivo
                if (matrizCursos[index1][index2] == 0) {
                    matrizCursos[index1][index2] = cantidad;
                    matrizCursos[index2][index1] = cantidad;  // Relación simétrica
                }
                else {
                    // Si ya existe la relación, suma el valor del archivo
                    matrizCursos[index1][index2] += cantidad;
                    matrizCursos[index2][index1] += cantidad;  // Relación simétrica
                }

                
            }
            else {
                cout << "Relación no válida entre " << cedula1 << " y " << cedula2
                    << ". Cédulas no encontradas en el sistema.\n";
            }
        }
        else if (leyendoAsignaciones) {
            size_t pos = linea.find(":");
            int codigoCurso = stoi(linea.substr(0, pos));
            string estudiantes = linea.substr(pos + 1);

            istringstream iss(estudiantes);
            string cedulaStr;
            while (getline(iss, cedulaStr, ',')) {
                int cedula = stoi(cedulaStr);
                arbolCurso.agregarEstudianteACurso(codigoCurso, cedula);
            }
        }
    }

    archivo.close();
}









int mostrarMenu() {
    int opcion;
    cout << "\n--- MENU GESTION DE ARTISTAS ---\n";
    cout << "1. Agregar Artista\n";
    cout << "2. Eliminar Artista\n";
    cout << "3. Mostrar Artistas en Orden Ascendente\n";
    cout << "4. Mostrar Artistas en Orden Descendente\n";
    cout << "5. Consultar Datos de un Artista\n";
    cout << "6. Agregar Curso\n";
    cout << "7. Eliminar Curso\n";
    cout << "8. Mostrar Cursos en Orden Ascendente\n";
    cout << "9. Mostrar Cursos en Orden Descendente\n";
    cout << "10. Consultar Datos de un Curso\n";
    cout << "11. Agregar estudiante a un curso\n";
    cout << "12. Consultar estudiantes con los que comparte cursos\n";
    cout << "13. Listar estudiantes que comparten al menos una cantidad de cursos\n";
    cout << "14. Guardar y Salir\n";
    cout << "Seleccione una opcion: ";
    cin >> opcion;
    return opcion;
}


int main() {
    
   
    
    

    AVL arbol;
    vector<int> cedulas;

    string nombreArchivo = "artistas.txt";
    cargarArtistasDesdeArchivo(arbol, cedulas, nombreArchivo);

    Grafo grafo(cedulas.size());
    AVLCurso arbolCurso;
    string archivoCursos = "cursos.txt";
    cargarCursosDesdeArchivo(arbolCurso, archivoCursos);

    string archivoRelaciones = "relaciones_y_asignaciones.txt";
    cargarRelacionesYAsignaciones(grafo, arbolCurso, cedulas, archivoRelaciones);


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
       
        case 6: {
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
        case 7: {
            int codigo;
            cout << "Ingrese el codigo del curso a eliminar: ";
            cin >> codigo;
            arbolCurso.eliminar(codigo);
            guardarCambiosCursos(arbolCurso, archivoCursos);
            cout << "Curso eliminado (si existia).\n";
            break;
        }

        case 8:
            cout << "Cursos en Orden Ascendente:\n";
            arbolCurso.mostrarAscendente();
            break;

        case 9:
            cout << "Cursos en Orden Descendente:\n";
            arbolCurso.mostrarDescendente();
            break;


        case 10:

            int codigo;
            cout << "Ingrese el codigo del curso a buscar: ";
            cin >> codigo;

            arbolCurso.buscarCurso(codigo);
            break;
        case 11:
           
            agregarEstudianteACurso(arbolCurso, grafo, arbol, cedulas);  // Pasamos cedulas al agregar
            guardarRelacionesYAsignaciones(grafo, arbolCurso, cedulas, "relaciones_y_asignaciones.txt");
            guardarCambios(arbol, nombreArchivo);
            guardarCambiosCursos(arbolCurso, archivoCursos);
            break;
        case 12:
            consultarEstudiantesConCursosCompartidos(grafo, arbol, cedulas);
            
            guardarRelacionesYAsignaciones(grafo, arbolCurso, cedulas, "relaciones_y_asignaciones.txt");
            guardarCambios(arbol, nombreArchivo);
            guardarCambiosCursos(arbolCurso, archivoCursos);
            break;
        case 13:
            int umbral;
            cout << "Ingrese el numero minimo de cursos compartidos para mostrar la relacion: ";
            cin >> umbral;

            listarArtistasConCursosCompartidosPorCantidad(grafo, arbol, cedulas, umbral);
            break;

        case 14:
            guardarRelacionesYAsignaciones(grafo, arbolCurso, cedulas, "relaciones_y_asignaciones.txt");
            
            guardarCambios(arbol, "artistas.txt");
            guardarCambiosCursos(arbolCurso, "cursos.txt");
            guardarCambios(arbol, nombreArchivo);
            guardarCambiosCursos(arbolCurso, archivoCursos);
            cout << "Cambios guardados. Saliendo...\n";
            break;
        default:
            cout << "Opción no valida.\n";
        }
    } while (opcion != 14);

    return 0;
}
