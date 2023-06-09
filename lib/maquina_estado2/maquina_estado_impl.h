#ifndef MAQUINA_ESTADO_IMPL_H
#define MAQUINA_ESTADO_IMPL_H

#include <maquina_estado.h>


typedef enum codigoResultado {
    RES_IGNORADO,
    RES_PROCESADO,
    RES_TRANSICION
} codigoResultado;


struct Resultado{
    codigoResultado codigo;
    Estado nuevoEstado; // Valido cuando resultado = RES_TRANSICION
};


void Maquina_init(Maquina *self, Estado estadoInicial);

#endif
