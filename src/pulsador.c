#include "pulsador.h"

void Pulsador_init(Pulsador *self, 
                   Maquina *destino,
                   Evento evento,
                   SP_HPin pin,
                   bool nivelActivo,
                   uint8_t histeresis)
{
    self->destino = destino;
    self->evento  = evento;
    self->estado.nivanterior = !nivelActivo;
    if (nivelActivo == false){
        self->estado.cont = self->parametros.hist;
        SP_Pin_setModo(pin,SP_PIN_ENTRADA_PULLUP);
    }else{
        self->estado.cont = 0;
        SP_Pin_setModo(pin,SP_PIN_ENTRADA_PULLDN);
    }
    self->parametros.pin = pin;
    self->parametros.nivelact = nivelActivo;
    self->parametros.hist = histeresis;
    self->t0 = SP_Tiempo_getMilisegundos();
}

void pulsadorprocesa(Pulsador *self){
    uint32_t const t = SP_Tiempo_getMilisegundos();
    if (self->t0 != t){
        self->t0 = t;
        bool const nivelPin = SP_Pin_read(self->parametros.pin);
        bool nivelFlitrado = self->estado.nivanterior;
        if (nivelPin && (self->estado.cont < self->parametros.hist)){
            uint8_t const nuevoContador = self->estado.cont + 1;
            if (nuevoContador == self->parametros.hist){
                nivelFlitrado = 1;
            }
            self->estado.cont = nuevoContador;
        }else if(!nivelPin && (self->estado.cont > 0)){
            uint8_t const nuevoContador = self->estado.cont - 1;
            if (!nuevoContador){
                nivelFlitrado = 0;
            }
            self->estado.cont = nuevoContador;
        }
        if (nivelFlitrado != self->estado.nivanterior){
            self->estado.nivanterior = nivelFlitrado;
            if (self->parametros.nivelact == nivelFlitrado){ // Pulsador presionado
                Maquina_despacha(self->destino,self->evento);
            }
        }
    }   
}