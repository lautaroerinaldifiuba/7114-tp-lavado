#ifndef PRENDA_H_
#define PRENDA_H_

class Prenda {
	unsigned int numero;
	unsigned int tiempo_lavado;
	unsigned int cantidad_incompatibilidades;

public:
	Prenda(unsigned int numero, unsigned int tiempo_lavado, unsigned int cantidad_incompatibilidades);
	virtual ~Prenda();

	unsigned int getCantidadIncompatibilidades() const;
	unsigned int getNumero() const;
	unsigned int getTiempoLavado() const;
};

#endif /* PRENDA_H_ */
