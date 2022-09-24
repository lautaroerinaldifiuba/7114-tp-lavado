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
	unsigned int getCantidadPrendas() const;
	unsigned int getCantidadIncompatibilidades() const;

	bool esCompatible(unsigned int nro_prenda);

	string getNroPrendas();

	void agregarPrenda(Prenda *prenda);
	void agregarIncompatibilidad(unsigned int nro_prenda);

	map<unsigned int, Prenda*>& getPrendas();
};

#endif /* LAVADO_H_ */
