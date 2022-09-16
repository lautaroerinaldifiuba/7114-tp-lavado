#include "Prenda.h"

Prenda::Prenda(unsigned int numero, unsigned int tiempo_lavado, unsigned int cantidad_incompatibilidades) :
		numero(numero), tiempo_lavado(tiempo_lavado), cantidad_incompatibilidades(cantidad_incompatibilidades) {
}

unsigned int Prenda::getCantidadIncompatibilidades() const {
	return cantidad_incompatibilidades;
}

unsigned int Prenda::getNumero() const {
	return numero;
}

unsigned int Prenda::getTiempoLavado() const {
	return tiempo_lavado;
}

Prenda::~Prenda() {
}
