#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <list>
#include <algorithm>
#include <iomanip>

#include "Prenda.h"
#include "Lavado.h"

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

void exportarResultadoAArchivo(string nombre_archivo_salida, map<unsigned int, Lavado*> &lavados) {
	ofstream archivo_salida(nombre_archivo_salida);

	for (unsigned int i = 0; i < lavados.size(); ++i) {
		map<unsigned int, Prenda*>::iterator it = (lavados[i]->getPrendas()).begin();

		for (; it != (lavados[i]->getPrendas()).end(); ++it)
			archivo_salida << (it->second)->getNumero() << " " << i + 1 << endl;
	}
	archivo_salida.close();
}

void mostrarDefinicion(unsigned int cantidad_prendas, unsigned int cantidad_incompatibilidades) {
	cout << "Cantidad Prendas: " << cantidad_prendas << endl;
	cout << "Cantidad Incompatibilidades Informadas: " << cantidad_incompatibilidades << endl;
}

int calcularTiempoTotalLavadosIndividuales(map<unsigned int, unsigned int> &tiempo_lavado_prendas) {
	unsigned int tiempo_total_lavado = 0;

	for (unsigned int i = 0; i < tiempo_lavado_prendas.size(); ++i)
		tiempo_total_lavado += tiempo_lavado_prendas[i];

	return tiempo_total_lavado;
}

void agregarTiempoLavadoIndividual(stringstream *datos, map<unsigned int, unsigned int> &tiempo_lavado_prendas) {
	string funcion = "";

	unsigned int prenda = 0;
	unsigned int tiempo = 0;

	*datos >> funcion >> prenda >> tiempo;
	tiempo_lavado_prendas[prenda] = tiempo;
}

int calcularTiempoTotalLavadoConjunto(map<unsigned int, Lavado*> &lavados) {
	unsigned int tiempo_total = 0;

	for (unsigned int i = 0; i < lavados.size(); ++i) {
		cout << "Lavado " << setw(2) << i + 1 << ", " << setw(2) << lavados[i]->getCantidadPrendas()
				<< " prendas, ";
		cout << setw(2) << lavados[i]->getTiempo() << " minutos. Prendas: " << lavados[i]->getNroPrendas()
				<< endl;

		tiempo_total += lavados[i]->getTiempo();
	}
	return tiempo_total;
}

void mostrarReportes(map<unsigned int, Lavado*> &lavados, map<unsigned int, unsigned int> &tiempo_lavado_prendas) {
	unsigned int tiempo_lavados_individuales = calcularTiempoTotalLavadosIndividuales(tiempo_lavado_prendas);
	cout << "Tiempo total de lavado (prendas individuales): " << tiempo_lavados_individuales << " minutos" << endl
			<< endl;

	cout << "Lavados agrupados:" << endl;
	unsigned int tiempo_lavados_conjuntos = calcularTiempoTotalLavadoConjunto(lavados);
	cout << endl << "Cantidad de lavados conjuntos: " << lavados.size() << endl;
	cout << "Tiempo total de lavado (prendas agrupadas): " << tiempo_lavados_conjuntos << " minutos" << endl;
}

void cargarMatrizSimetrica(int **matriz, int prenda1, int prenda2) {
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

		cargarMatrizSimetrica(matriz, prenda1, prenda2);
	}
}

unsigned int getNumeroLavadoConMenosRestricciones(map<unsigned int, Lavado*> &lavados, list<unsigned int> &nros_lavados_compatibles) {

	unsigned int nro = *(nros_lavados_compatibles.begin());
	unsigned int cant_rest = lavados[*(nros_lavados_compatibles.begin())]->getCantidadIncompatibilidades();
	unsigned int tiempo = lavados[*(nros_lavados_compatibles.begin())]->getTiempo();

	list<unsigned int>::iterator it;
	for (it = nros_lavados_compatibles.begin(); it != nros_lavados_compatibles.end(); ++it) {
		if ((tiempo == lavados[*it]->getTiempo()) && (cant_rest < lavados[*it]->getCantidadIncompatibilidades())) {
			nro = *it;
			cant_rest = lavados[*it]->getCantidadIncompatibilidades();
		}
	}

	return nro;
}

void asignarLavados(const unsigned int cantidad_prendas, map<unsigned int, Lavado*> &lavados,
		list<Prenda*> &cesto_ropa, int **matriz) {

	unsigned int prenda = 0;

	list<Prenda*>::iterator it;
	for (it = cesto_ropa.begin(); it != cesto_ropa.end(); ++it) {
		prenda = (*it)->getNumero();
		list<unsigned int> nros_lavados_compatibles;

		bool encontro_lavado = false;
		unsigned int nro_lavado_actual = 0;

		for (nro_lavado_actual = 0; nro_lavado_actual < lavados.size(); ++nro_lavado_actual) {
			if (lavados[nro_lavado_actual]->esCompatible(prenda)) {
				nros_lavados_compatibles.push_back(nro_lavado_actual);
				encontro_lavado = true;
			}
		}

		unsigned int pos_ins = 0;
		if (encontro_lavado) {
			pos_ins = getNumeroLavadoConMenosRestricciones(lavados, nros_lavados_compatibles);
		} else {
			// Significa que ya revisé todos los lavados existentes, hay que empezar un lavado nuevo
			pos_ins = lavados.size();
			lavados[pos_ins] = new Lavado();
		}

		// Agrego todas las incompatibilidades que tenìa la prenda al lavado
		lavados[pos_ins]->agregarPrenda((*it));
		for (unsigned int j = 0; j < cantidad_prendas; ++j)
			if (matriz[prenda - 1][j] == 1)
				lavados[pos_ins]->agregarIncompatibilidad((j + 1));
	}
}

void cargarTiemposDeLavadoPorPrendas(ifstream &archivo_restricciones, const unsigned int cantidad_prendas,
		map<unsigned int, unsigned int> &tiempo_lavado_prendas) {
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

bool comparaPrendasPorCantidadIncompatibilidadesDescendente(const Prenda *p1, const Prenda *p2) {
	return (p1->getCantidadIncompatibilidades() > p2->getCantidadIncompatibilidades());
}

bool comparaPrendasTiempoDescendenteIncompatibilidadesDescendente(const Prenda *p1, const Prenda *p2) {
	if (p1->getTiempoLavado() > p2->getTiempoLavado())
		return true;
	else if (p1->getTiempoLavado() < p2->getTiempoLavado())
		return false;
	else
		return (p1->getCantidadIncompatibilidades() > p2->getCantidadIncompatibilidades());
}

void crearCestoRopa(list<Prenda*> &cesto_ropa, int **matriz, unsigned int cantidad_prendas,
		map<unsigned int, unsigned int> &tiempo_lavado_prendas) {

	for (unsigned int nro_prenda = 1; nro_prenda <= cantidad_prendas; ++nro_prenda) {
		unsigned int nro_incompatibilidades = contarIncompatibilidades(matriz, nro_prenda, cantidad_prendas);
		Prenda *prenda = new Prenda(nro_prenda, tiempo_lavado_prendas[nro_prenda], nro_incompatibilidades);
		cesto_ropa.push_back(prenda);
	}

	#ifdef PROBLEMA_1
		cesto_ropa.sort(comparaPrendasPorCantidadIncompatibilidadesDescendente);
	#else
		cesto_ropa.sort(comparaPrendasTiempoDescendenteIncompatibilidadesDescendente);
	#endif
}

void liberarMemoria(int **&matriz, unsigned int cantidad_de_prendas, list<Prenda*> &cesto_ropa) {
	for (unsigned int i = 0; i < cantidad_de_prendas; ++i)
		delete[] matriz[i];
	delete[] matriz;

	for (auto &it : cesto_ropa)
		delete it;
}

void procesarDefinicionProblema(ifstream &archivo_restricciones, unsigned int & cantidad_prendas, unsigned int & cantidad_incompatibilidades) {
	string leido = "";
	string funcion = "";
	string comentario = "";

	getline(archivo_restricciones, leido);		// Primer Comentario, descartar
	#ifdef PROBLEMA_1
		getline(archivo_restricciones, leido);	// Segundo Comentario (Solo para problema 1), descartar
	#endif
	getline(archivo_restricciones, leido);	// Definicion del problema
	stringstream aux(leido);

	aux >> funcion >> comentario >> cantidad_prendas >> cantidad_incompatibilidades;
}

int main() {
	int **matriz = NULL;

	unsigned int cantidad_prendas = 0;
	unsigned int cantidad_incompatibilidades = 0;

	map<unsigned int, unsigned int> tiempo_lavado_prendas; // nro_prenda, tiempo_lavado
	map<unsigned int, Lavado*> lavados;
	list<Prenda*> cesto_ropa;	// prendas ordenadas por algún criterio

	ifstream archivo_restricciones(ARCHIVO_ENTRADA);


	procesarDefinicionProblema(archivo_restricciones, cantidad_prendas, cantidad_incompatibilidades);
	mostrarDefinicion(cantidad_prendas, cantidad_incompatibilidades);

	preprocesarRestricciones(archivo_restricciones, matriz, cantidad_prendas, cantidad_incompatibilidades);
	cargarTiemposDeLavadoPorPrendas(archivo_restricciones, cantidad_prendas, tiempo_lavado_prendas);
	archivo_restricciones.close();


	crearCestoRopa(cesto_ropa, matriz, cantidad_prendas, tiempo_lavado_prendas);
	asignarLavados(cantidad_prendas, lavados, cesto_ropa, matriz);


	exportarResultadoAArchivo(ARCHIVO_SALIDA, lavados);
	mostrarReportes(lavados, tiempo_lavado_prendas);

	liberarMemoria(matriz, cantidad_prendas, cesto_ropa);
	return 0;
}
