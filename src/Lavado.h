#ifndef LAVADO_H_
#define LAVADO_H_

#include "Prenda.h"
#include <map>
#include <set>

using namespace std;

class Lavado {
	unsigned int tiempo;
	map<unsigned int, Prenda*> prendas;
	set<unsigned int> incompatibles;

public:
	Lavado();
	virtual ~Lavado();

	unsigned int getTiempo() const;
	void agregarPrenda(Prenda *prenda);
	void agregarIncompatibilidad(unsigned int nro_prenda);
	bool esCompatible(unsigned int nro_prenda);
	unsigned int getCantidadPrendas() const;
	map<unsigned int, Prenda*>& getPrendas();

	string getNroPrendas();
};

#endif /* LAVADO_H_ */
