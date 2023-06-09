#ifndef PULSADOR_H
#define PULSADOR_H
#include <maquina_estado.h>
#include <soporte_placa.h>


/**
 * @brief Objeto Pulsador.
 * Despacha un evento al detectar pulsación.
 * Implementado como máquina de estado de control
 * 
 */
typedef struct Pulsador{
    struct{
        bool nivelact;                   
        SP_HPin pin;                                
        uint8_t hist;                 
    }parametros;
    uint32_t t0;
    Maquina *destino;                      
    Evento evento;                         
    struct{
        bool nivanterior;                
        uint8_t cont;
    }estado;
} Pulsador;


/**
 * @brief Inicializa la lectura de pulsador y configura el evento
 * de pulsado y su destino
 
 */
void Pulsador_init(Pulsador *self, 
                   Maquina *destino,
                   Evento evento,
                   SP_HPin pin,
                   bool nivelact,
                   uint8_t histeresis);
/**
 * @brief Lee pulsador, actualiza la máquina de estado y
 * despacha evento si detecta pulsación
 * 
 * @param self 
 */
void pulsadorprocesa(Pulsador *self);

#endif