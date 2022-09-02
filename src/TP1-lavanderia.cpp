#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>

using namespace std;

#define ARCHIVO_ENTRADA "./primer_problema.txt"
#define ARCHIVO_SALIDA "./resultado.txt"

#define DEFINICION "p"
#define INCOMPATIBILIDAD "e"
#define TIEMPO_LAVADO "n"

int calcularTiempoTotalLavadoConjunto(map<unsigned int, set<unsigned int>> *lavados,
		map<unsigned int, unsigned int> *tiempo_lavado_prendas) {
	unsigned int tiempo_total_lavado_conjunto = 0;
	for (unsigned int i = 0; i < lavados->size(); ++i) {
		set<unsigned int>::iterator it;
		cout << "Lavado " << i + 1 << ", " << (*lavados)[i].size() << " prendas: ";
		unsigned int tiempo_lavado_ciclo_actual = 0;
		for (it = (*lavados)[i].begin(); it != (*lavados)[i].end(); ++it) {
			cout << *it << " ";
			if ((*tiempo_lavado_prendas)[*it] > tiempo_lavado_ciclo_actual)
				tiempo_lavado_ciclo_actual = (*tiempo_lavado_prendas)[*it];
		}
		cout << "       tiempo: " << tiempo_lavado_ciclo_actual;
		tiempo_total_lavado_conjunto += tiempo_lavado_ciclo_actual;
		cout << endl;
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

unsigned int obtenerTotalDePrendas(stringstream *datos) {
	string comentario = "";

	unsigned int cantidad_prendas = 0;
	unsigned int cantidad_incompatibilidades = 0;

	*datos >> comentario >> cantidad_prendas >> cantidad_incompatibilidades;
	//mostrarDefinicion(cantidad_prendas, cantidad_incompatibilidades);

	return cantidad_prendas;
}

int calcularTiempoTotalLavadosIndividuales(map<unsigned int, unsigned int> *tiempo_lavado_prendas) {
	unsigned int tiempo_total_lavado = 0;

	for (unsigned int i = 0; i < tiempo_lavado_prendas->size(); ++i)
		tiempo_total_lavado += (*tiempo_lavado_prendas)[i];

	return tiempo_total_lavado;
}

void agregarTiempoLavadoIndividual(stringstream *datos, map<unsigned int, unsigned int> *tiempo_lavado_prendas) {
	unsigned int prenda = 0;
	unsigned int tiempo = 0;
	*datos >> prenda >> tiempo;

	(*tiempo_lavado_prendas)[prenda] = tiempo;
}

void mostrarReportes(map<unsigned int, set<unsigned int>> *lavados,
		map<unsigned int, unsigned int> *tiempo_lavado_prendas) {
	unsigned int tiempo_lavado_individual = calcularTiempoTotalLavadosIndividuales(tiempo_lavado_prendas);
	cout << "Tiempo total de lavado (prendas individuales): " << tiempo_lavado_individual << " minutos" << endl;

	cout << endl << "Cantidad de lavados conjuntos: " << lavados->size() << endl;

	unsigned int tiempo_total_lavado_conjunto = calcularTiempoTotalLavadoConjunto(lavados, tiempo_lavado_prendas);
	cout << "Tiempo total de lavado (prendas agrupadas): " << tiempo_total_lavado_conjunto << " minutos" << endl;
}

void precargarLavadosCompatibles(set<unsigned int> *lavados_compatibles, unsigned int cantidad_lavados_confirmados) {
	lavados_compatibles->clear();
	for (unsigned int i = 0; i < cantidad_lavados_confirmados; ++i)
		lavados_compatibles->insert(i);
}

unsigned int obtenerNumeroLavadoCompatible(set<unsigned int> *lavados_compatibles,
		map<unsigned int, set<unsigned int>> *lavados) {
	if (lavados_compatibles->size() != 0)
		return *lavados_compatibles->begin();
	else
		return lavados->size();
}

void actualizarLavadosCompatibles(unsigned int prenda_incompatible, set<unsigned int> *lavados_compatibles,
		map<unsigned int, set<unsigned int>> *lavados) {

	bool salir = false;
	unsigned int lavado_en_analisis = 0;
	while (!salir && lavado_en_analisis < lavados->size()) {
		if ((*lavados)[lavado_en_analisis].find(prenda_incompatible) == (*lavados)[lavado_en_analisis].end()) {
			++lavado_en_analisis;

		} else {
			set<unsigned int>::iterator it;
			it = lavados_compatibles->find(lavado_en_analisis);

			if (it != lavados_compatibles->end())
				lavados_compatibles->erase(it);

			++lavado_en_analisis;
			salir = true;
		}
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

int main() {
	ifstream archivo_restricciones(ARCHIVO_ENTRADA);

	string leido = "";

	unsigned int prenda_actual = 1;
	unsigned int cantidad_prendas = 0;

	map<unsigned int, unsigned int> tiempo_lavado_prendas; // nro_prenda, tiempo
	map<unsigned int, set<unsigned int>> lavados; // nro_lavado, conjunto_prendas
	set<unsigned int> lavados_compatibles;
	set<unsigned int> prendas;

	while (getline(archivo_restricciones, leido)) {
		stringstream aux(leido);
		string funcion;

		aux >> funcion;
		if (funcion == DEFINICION) {
			cantidad_prendas = obtenerTotalDePrendas(&aux);
			precargarPrendas(&prendas, cantidad_prendas);
		}

		if (funcion == INCOMPATIBILIDAD) {
			unsigned int prenda1 = 0;
			unsigned int prenda2 = 0;
			aux >> prenda1 >> prenda2;

			if (prenda1 != prenda_actual) {
				unsigned int nro_lavado = obtenerNumeroLavadoCompatible(&lavados_compatibles, &lavados);

				lavados[nro_lavado].insert(prenda_actual);
				removerPrenda(&prendas, prenda_actual);
				prenda_actual = prenda1;

				precargarLavadosCompatibles(&lavados_compatibles, lavados.size());
			}

			actualizarLavadosCompatibles(prenda2, &lavados_compatibles, &lavados);
		}

		if (funcion == TIEMPO_LAVADO)
			agregarTiempoLavadoIndividual(&aux, &tiempo_lavado_prendas);

	}

	// Insertar última prenda con restricciones analizada
	unsigned int nro_lavado = obtenerNumeroLavadoCompatible(&lavados_compatibles, &lavados);
	lavados[nro_lavado].insert(prenda_actual);
	removerPrenda(&prendas, prenda_actual);

	asignarPrendasSinIncompatibilidades(&prendas, &lavados, 0);

	archivo_restricciones.close();

	exportarResultadoAArchivo(ARCHIVO_SALIDA, &lavados);
	//mostrarReportes(&lavados, &tiempo_lavado_prendas);

	return 0;
}
