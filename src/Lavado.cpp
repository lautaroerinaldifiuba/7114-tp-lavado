#include "Lavado.h"
#include <algorithm>

Lavado::Lavado() :
		tiempo(0) {
}

unsigned int Lavado::getTiempo() const {
	return tiempo;
}

void Lavado::agregarPrenda(Prenda *prenda) {
	prendas[prenda->getNumero()] = prenda;

	if (prenda->getTiempoLavado() > tiempo)
		tiempo = prenda->getTiempoLavado();
}

void Lavado::agregarIncompatibilidad(unsigned int nro_prenda) {
	incompatibles.insert(nro_prenda);
}

bool Lavado::esCompatible(unsigned int nro_prenda) {
	return !(incompatibles.contains(nro_prenda));
}

map<unsigned int, Prenda*>& Lavado::getPrendas() {
	return prendas;
}

unsigned int Lavado::getCantidadPrendas() const {
	return prendas.size();
}

Lavado::~Lavado() {
}

string Lavado::getNroPrendas() {
	string concatena = "";
	map<unsigned int, Prenda*>::iterator it;

	for (it = prendas.begin(); it != prendas.end(); ++it)
		concatena += (to_string((it->second)->getNumero()) + " ");

	return concatena;
}
