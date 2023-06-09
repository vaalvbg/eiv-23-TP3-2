#ifndef CONTAINER_OF_H
#define CONTAINER_OF_H
#include <stddef.h>
#define container_of(puntero,TipoContenedor,rutaMiembro) (__extension__({\
    __typeof__(((TipoContenedor*)0)->rutaMiembro) *const pMiembro = (puntero);\
    TipoContenedor *const pContenedor = ((void*)pMiembro) - offsetof(TipoContenedor,rutaMiembro);\
    pContenedor;}))

#endif
