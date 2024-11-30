#include "Grafo.h"

// Constructor
Grafo::Grafo(int tamano)
    : numNodos(tamano),
    matrizAdyacencia(tamano, vector<bool>(tamano, false)),
    matrizCursos(tamano, vector<int>(tamano, 0)) {}

// Agregar arista
void Grafo::agregarArista(int nodo1, int nodo2) {
    if (nodo1 < numNodos && nodo2 < numNodos) {
        matrizAdyacencia[nodo1][nodo2] = true;
        matrizAdyacencia[nodo2][nodo1] = true;
    }
}
// Funci�n auxiliar para encontrar el �ndice correspondiente a una c�dula en el vector de c�dulas
int encontrarIndicePorCedula(const vector<int>& cedulas, int cedula) {
    for (int i = 0; i < cedulas.size(); ++i) {
        if (cedulas[i] == cedula) {
            return i;  // Devuelve el �ndice correspondiente
        }
    }
    return -1;  // Si la c�dula no se encuentra, devuelve -1
}


// Eliminar arista
void Grafo::eliminarArista(int nodo1, int nodo2) {
    if (nodo1 < numNodos && nodo2 < numNodos) {
        matrizAdyacencia[nodo1][nodo2] = false;
        matrizAdyacencia[nodo2][nodo1] = false;
    }
}

// Obtener adyacentes
vector<int> Grafo::obtenerAdyacentes(int nodo) {
    vector<int> adyacentes;
    if (nodo < numNodos) {
        for (int i = 0; i < numNodos; ++i) {
            if (matrizAdyacencia[nodo][i]) {
                adyacentes.push_back(i);
            }
        }
    }
    return adyacentes;
}

// Mostrar relaciones
void Grafo::mostrarRelaciones() {
    for (int i = 0; i < numNodos; ++i) {
        cout << "Nodo " << i << " esta conectado con: ";
        for (int j = 0; j < numNodos; ++j) {
            if (matrizAdyacencia[i][j]) {
                cout << j << " ";
            }
        }
        cout << endl;
    }
}
// Agregar relaci�n de cursos entre dos estudiantes
void Grafo::agregarRelacionCurso(int nodo1, int nodo2) {
    if (nodo1 < numNodos && nodo2 < numNodos) {
        // Incrementa el n�mero de cursos compartidos
        matrizCursos[nodo1][nodo2]++;
        matrizCursos[nodo2][nodo1]++; 
        cout << "Relacion agregada entre " << nodo1 << " y " << nodo2
            << ", cursos compartidos: " << matrizCursos[nodo1][nodo2] << endl;// Sim�trico, ya que es una relaci�n bidireccional
    }
}


void Grafo::agregarRelacionCurso(int cedula1, int cedula2, const vector<int>& cedulas) {
    // Convierte las c�dulas a �ndices usando la funci�n auxiliar
    int nodo1 = encontrarIndicePorCedula(cedulas, cedula1);
    int nodo2 = encontrarIndicePorCedula(cedulas, cedula2);

    // Verifica que los �ndices est�n dentro del rango v�lido
    if (nodo1 >= 0 && nodo1 < numNodos && nodo2 >= 0 && nodo2 < numNodos) {
        // Si no existe relaci�n, se agrega con valor 1
        if (matrizCursos[nodo1][nodo2] == 0) {
            matrizCursos[nodo1][nodo2] = 1;
            matrizCursos[nodo2][nodo1] = 1;  // Relaci�n bidireccional
        }
        else {
            // Si ya existe la relaci�n, se suma
            matrizCursos[nodo1][nodo2]++;
            matrizCursos[nodo2][nodo1]++;  // Relaci�n bidireccional
        }

  
    }
    else {
        cout << "Indices fuera de rango. No se pudo agregar la relaci�n entre "
            << cedula1 << " y " << cedula2 << endl;
    }
}










// Eliminar relaci�n de cursos
void Grafo::eliminarRelacionCurso(int nodo1, int nodo2) {
    if (nodo1 < numNodos && nodo2 < numNodos) {
        matrizCursos[nodo1][nodo2] = 0;
        matrizCursos[nodo2][nodo1] = 0;
    }
}

// Obtener relaciones
vector<vector<bool>> Grafo::obtenerRelaciones() const {
    return matrizAdyacencia;
}

// Obtener relaciones de cursos
vector<vector<int>> Grafo::obtenerRelacionesCursos() const {
    return matrizCursos;
}

// Guardar relaciones en archivo
void Grafo::guardarRelacionesCursosEnArchivo(const string& nombreArchivo) const {
    ofstream archivo(nombreArchivo);
    if (!archivo) {
        cout << "No se pudo abrir el archivo para escritura.\n";
        return;
    }

    archivo << "[Relaciones]\n";  // Empezamos a escribir las relaciones

    // Guardar relaciones entre estudiantes
    for (int i = 0; i < numNodos; ++i) {
        for (int j = i + 1; j < numNodos; ++j) {  // Aseguramos que no haya duplicados
            if (matrizCursos[i][j] > 0) {  // Si hay relaci�n (cursos compartidos)
                archivo << i << " " << j << " " << matrizCursos[i][j] << "\n";
            }
        }
    }

    archivo.close();
    cout << "Relaciones guardadas en " << nombreArchivo << ".\n";
}


// Cargar relaciones desde archivo
void Grafo::cargarRelacionesCursosDesdeArchivo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo) {
        cout << "No se pudo abrir el archivo para lectura.\n";
        return;
    }

    int nodo1, nodo2, cantidad;
    while (archivo >> nodo1 >> nodo2 >> cantidad) {
        matrizCursos[nodo1][nodo2] = cantidad;
        matrizCursos[nodo2][nodo1] = cantidad;
    }
    archivo.close();
}
