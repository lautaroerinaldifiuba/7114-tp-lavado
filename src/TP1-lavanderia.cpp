#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <iomanip>

#include "Prenda.h"

using namespace std;

//#define PROBLEMA_1
#ifdef PROBLEMA_1
	#define ARCHIVO_ENTRADA "./primer_problema.txt"
	#define ARCHIVO_SALIDA "./resultado_primer_problema.txt"
#else
	#define ARCHIVO_ENTRADA "./segundo_problema.txt"
	#define ARCHIVO_SALIDA "./resultado_segundo_problema.txt"
#endif

#define DEFINICION "p"
#define INCOMPATIBILIDAD "e"
#define TIEMPO_LAVADO "n"

int calcularTiempoTotalLavadoConjunto(map<unsigned int, set<unsigned int>> *lavados,
		map<unsigned int, unsigned int> *tiempo_lavado_prendas) {
	unsigned int tiempo_total_lavado_conjunto = 0;

	for (unsigned int i = 0; i < lavados->size(); ++i) {
		set<unsigned int>::iterator it;
		cout << "Lavado " << setw(2) << i + 1 << ", " << setw(2) << (*lavados)[i].size() << " prendas, ";
		unsigned int tiempo_lavado_ciclo_actual = 0;

		string concatena = "";
		for (it = (*lavados)[i].begin(); it != (*lavados)[i].end(); ++it) {
			concatena += to_string(*it) + " ";
			if ((*tiempo_lavado_prendas)[*it] > tiempo_lavado_ciclo_actual)
				tiempo_lavado_ciclo_actual = (*tiempo_lavado_prendas)[*it];
		}
		cout << setw(2) << tiempo_lavado_ciclo_actual << " minutos, composición: " << concatena << endl;
		tiempo_total_lavado_conjunto += tiempo_lavado_ciclo_actual;
	}

	return tiempo_total_lavado_conjunto;
}

void exportarResultadoAArchivo(string nombre_archivo_salida, map<unsigned int, set<unsigned int>> *lavados) {
	ofstream archivo_salida(nombre_archivo_salida);

	for (unsigned int i = 0; i < lavados->size(); ++i) {
		set<unsigned int>::iterator it;
		for (it = (*lavados)[i].begin(); it != (*lavados)[i].end(); ++it)
			archivo_salida << *it << " " << i + 1 << endl;
	}
	archivo_salida.close();
}

void mostrarDefinicion(unsigned int cantidad_prendas, unsigned int cantidad_incompatibilidades) {
	cout << "Cantidad Prendas: " << cantidad_prendas << endl;
	cout << "Cantidad Incompatibilidades: " << cantidad_incompatibilidades << endl;
}

int calcularTiempoTotalLavadosIndividuales(map<unsigned int, unsigned int> *tiempo_lavado_prendas) {
	unsigned int tiempo_total_lavado = 0;

	for (unsigned int i = 0; i < tiempo_lavado_prendas->size(); ++i)
		tiempo_total_lavado += (*tiempo_lavado_prendas)[i];

	return tiempo_total_lavado;
}

void agregarTiempoLavadoIndividual(stringstream *datos, map<unsigned int, unsigned int> *tiempo_lavado_prendas) {
	string funcion = "";

	unsigned int prenda = 0;
	unsigned int tiempo = 0;

	*datos >> funcion >> prenda >> tiempo;

	(*tiempo_lavado_prendas)[prenda] = tiempo;
}

void mostrarReportes(map<unsigned int, set<unsigned int>> *lavados,
		map<unsigned int, unsigned int> *tiempo_lavado_prendas) {
	unsigned int tiempo_lavado_individual = calcularTiempoTotalLavadosIndividuales(tiempo_lavado_prendas);
	cout << "Tiempo total de lavado (prendas individuales): " << tiempo_lavado_individual << " minutos" << endl << endl;

	cout << "Lavados agrupados:" << endl;
	unsigned int tiempo_total_lavado_conjunto = calcularTiempoTotalLavadoConjunto(lavados, tiempo_lavado_prendas);
	cout << endl << "Cantidad de lavados conjuntos: " << lavados->size() << endl;
	cout << "Tiempo total de lavado (prendas agrupadas): " << tiempo_total_lavado_conjunto << " minutos" << endl;
}

void precargarLavadosCompatibles(set<unsigned int> *lavados_compatibles, unsigned int cantidad_lavados_confirmados) {
	lavados_compatibles->clear();
	for (unsigned int i = 0; i < cantidad_lavados_confirmados; ++i)
		lavados_compatibles->insert(i);
}

unsigned int obtenerNumeroLavadoCompatible(set<unsigned int> *lavados_compatibles,
		map<unsigned int, set<unsigned int>> *lavados, map<unsigned int, unsigned int> *tiempo_lavado_prendas) {

	if (lavados_compatibles->size() != 0)
		return *lavados_compatibles->begin();
	else
		return lavados->size();	// Crear nuevo lavado
}

void actualizarLavadosCompatibles(unsigned int prenda_incompatible, set<unsigned int> *lavados_compatibles,
		map<unsigned int, set<unsigned int>> *lavados) {

	bool salir = false;
	unsigned int lavado_en_analisis = 0;
	while ((!salir) && (lavado_en_analisis < lavados->size())) {
		if ((*lavados)[lavado_en_analisis].find(prenda_incompatible) != (*lavados)[lavado_en_analisis].end()) {
			set<unsigned int>::iterator it;
			it = lavados_compatibles->find(lavado_en_analisis);

			if (it != lavados_compatibles->end())
				lavados_compatibles->erase(it);
			salir = true;	// Una vez que encontré la prenda en un lavado, no tiene sentido seguir buscando
		}
		++lavado_en_analisis;
	}
}

void precargarPrendas(set<unsigned int> *prendas, unsigned int cantidad_total_prendas) {
	for (unsigned int i = 1; i <= cantidad_total_prendas; ++i)
		prendas->insert(i);
}

void removerPrenda(set<unsigned int> *prendas, unsigned int nro_prenda) {
	set<unsigned int>::iterator it;
	it = prendas->find(nro_prenda);

	if (it != prendas->end())
		prendas->erase(it);
}

void asignarPrendasSinIncompatibilidades(set<unsigned int> *prendas, map<unsigned int, set<unsigned int>> *lavados,
		unsigned int nro_lavado) {
	set<unsigned int>::iterator it;
	for (it = prendas->begin(); it != prendas->end(); ++it)
		(*lavados)[nro_lavado].insert(*it);

	prendas->clear();
}

void cargarMatriz(int **matriz, int prenda1, int prenda2) {
	unsigned int fila = (prenda1 - 1);
	unsigned int columna = (prenda2 - 1);

	matriz[fila][columna] = 1;
	matriz[columna][fila] = 1;
}

void preprocesarRestricciones(ifstream &archivo_restricciones, int **&matriz, const unsigned int cantidad_prendas,
		unsigned int cantidad_incompatibilidades) {
	string funcion = "";
	string leido = "";

	int prenda1 = 0;
	int prenda2 = 0;

	matriz = new int*[cantidad_prendas];
	for (unsigned int i = 0; i < cantidad_prendas; ++i)
		matriz[i] = new int[cantidad_prendas];

	for (unsigned int j = 0; j < cantidad_incompatibilidades; ++j) {
		getline(archivo_restricciones, leido);
		stringstream aux(leido);
		aux >> funcion >> prenda1 >> prenda2;

		cargarMatriz(matriz, prenda1, prenda2);
	}
}

bool hayIncompatibilidad(int **matriz, unsigned int prenda1, unsigned int prenda2) {
	unsigned int fila = (prenda1 - 1);
	unsigned int columna = (prenda2 - 1);

	if ((fila != columna) && (matriz[fila][columna] == 1))
		return true;
	else
		return false;
}

void asignarLavados(const unsigned int cantidad_prendas, map<unsigned int, set<unsigned int>> *lavados,
		list<Prenda*> *cesto_ropa, map<unsigned int, unsigned int> *tiempo_lavado_prendas, int **matriz) {
	unsigned int prenda1 = 0;
	unsigned int prenda2 = 0;
	unsigned int prenda_actual = 1;

	set<unsigned int> lavados_compatibles;
	set<unsigned int> prendas;

	precargarPrendas(&prendas, cantidad_prendas);

	list<Prenda*>::iterator it;
	for (it = cesto_ropa->begin(); it != cesto_ropa->end(); ++it) {
		prenda1 = (*it)->getNumero();

		for (unsigned int columna = 0; columna < cantidad_prendas; ++columna) {
			prenda2 = columna + 1;

			if (hayIncompatibilidad(matriz, prenda1, prenda2)) {
				if (prenda1 != prenda_actual) {
					unsigned int nro_lavado = obtenerNumeroLavadoCompatible(&lavados_compatibles, lavados,
							tiempo_lavado_prendas);

					(*lavados)[nro_lavado].insert(prenda_actual);
					removerPrenda(&prendas, prenda_actual);
					prenda_actual = prenda1;

					precargarLavadosCompatibles(&lavados_compatibles, lavados->size());
				}
				actualizarLavadosCompatibles(prenda2, &lavados_compatibles, lavados);
			}
		}
	}

	// Insertar última prenda con restricciones analizada
	unsigned int nro_lavado = obtenerNumeroLavadoCompatible(&lavados_compatibles, lavados, tiempo_lavado_prendas);
	(*lavados)[nro_lavado].insert(prenda_actual);
	removerPrenda(&prendas, prenda_actual);

	asignarPrendasSinIncompatibilidades(&prendas, lavados, 0);
}

void cargarTiemposDeLavadoPorPrendas(ifstream &archivo_restricciones, const unsigned int cantidad_prendas,
		map<unsigned int, unsigned int> *tiempo_lavado_prendas) {
	string leido = "";
	for (unsigned int fila = 0; fila < cantidad_prendas; ++fila) {
		getline(archivo_restricciones, leido);
		stringstream aux(leido);
		agregarTiempoLavadoIndividual(&aux, tiempo_lavado_prendas);
	}
}

unsigned int contarIncompatibilidades(int **matriz, unsigned int prenda, unsigned int cantidad_prendas) {
	unsigned int cantidad = 0;
	for (unsigned int i = 0; i < cantidad_prendas; ++i)
		cantidad += matriz[prenda - 1][i];

	return cantidad;
}

bool comparaPrendasTiempoDescendenteIncompatibilidadesDescendente(const Prenda *p1, const Prenda *p2) {
	if (p1->getTiempoLavado() > p2->getTiempoLavado())
		return true;
	else if (p1->getTiempoLavado() < p2->getTiempoLavado())
		return false;
	else
		return (p1->getCantidadIncompatibilidades() > p2->getCantidadIncompatibilidades());
}


bool comparaPrendasPorCantidadIncompatibilidadesDescendente(const Prenda *p1, const Prenda *p2) {
	return (p1->getCantidadIncompatibilidades() > p2->getCantidadIncompatibilidades());
}

void crearCestoRopa(list<Prenda*> *cesto_ropa, int **matriz, unsigned int cantidad_prendas,
		map<unsigned int, unsigned int> *tiempo_lavado_prendas) {

	for (unsigned int nro_prenda = 1; nro_prenda <= cantidad_prendas; ++nro_prenda) {
		unsigned int nro_incompatibilidades = contarIncompatibilidades(matriz, nro_prenda, cantidad_prendas);
		Prenda *prenda = new Prenda(nro_prenda, (*tiempo_lavado_prendas)[nro_prenda], nro_incompatibilidades);
		cesto_ropa->push_back(prenda);
	}

#ifdef PROBLEMA_1
	cesto_ropa->sort(comparaPrendasPorCantidadIncompatibilidadesDescendente);
#else
	cesto_ropa->sort(comparaPrendasTiempoDescendenteIncompatibilidadesDescendente);
#endif
}

void liberarMemoria(int** &matriz, unsigned int cantidad_de_prendas, list<Prenda*> * cesto_ropa) {
	for (unsigned int i = 0; i < cantidad_de_prendas; ++i)
	    delete [] matriz[i];
	delete [] matriz;

	for(auto& it : *cesto_ropa)
	    delete it;
}

int main() {
	ifstream archivo_restricciones(ARCHIVO_ENTRADA);

	string leido = "";

	unsigned int cantidad_prendas = 0;
	unsigned int cantidad_incompatibilidades = 0;

	int **matriz = NULL;

	map<unsigned int, unsigned int> tiempo_lavado_prendas; // nro_prenda, tiempo
	map<unsigned int, set<unsigned int>> lavados; // nro_lavado, conjunto_prendas
	list<Prenda*> cesto_ropa;

	getline(archivo_restricciones, leido);		// Primer Comentario
#ifdef PROBLEMA_1
	getline(archivo_restricciones, leido);	// Segundo Comentario (Solo para problema 1)
#endif

	getline(archivo_restricciones, leido);	// Definicion del problema
	stringstream aux(leido);
	string funcion = "";
	string comentario = "";
	aux >> funcion >> comentario >> cantidad_prendas >> cantidad_incompatibilidades;
	mostrarDefinicion(cantidad_prendas, cantidad_incompatibilidades);

	preprocesarRestricciones(archivo_restricciones, matriz, cantidad_prendas, cantidad_incompatibilidades);
	cargarTiemposDeLavadoPorPrendas(archivo_restricciones, cantidad_prendas, &tiempo_lavado_prendas);
	crearCestoRopa(&cesto_ropa, matriz, cantidad_prendas, &tiempo_lavado_prendas);
	asignarLavados(cantidad_prendas, &lavados, &cesto_ropa, &tiempo_lavado_prendas, matriz);

	archivo_restricciones.close();

	exportarResultadoAArchivo(ARCHIVO_SALIDA, &lavados);
	mostrarReportes(&lavados, &tiempo_lavado_prendas);

	liberarMemoria(matriz, cantidad_prendas, &cesto_ropa);

	return 0;
}

