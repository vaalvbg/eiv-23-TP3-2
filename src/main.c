#include <soporte_placa.h>
#include "controlador_luz.h"
#include "controlador_de_pulsaciones.h"
#include "pulsador.h"
#include "despacho_retardado.h"
#include <stddef.h>


#define PIN_LUZ SP_PIN_LED
#define PIN_PULSADOR SP_PB9

#define HISTERESIS_ANTIRREBOTE 5

#define LUZ_ON 0

#define PULSADOR_NIVEL_ACTIVO 0

#define TIEMPO_ON 60000

#define TIEMPO_TRIPLE_PULSACION 1000


static Maquina * controladordeluz;
static Maquina * controladordepulsaciones;
static Pulsador pulsador[1];
static DespachoRetardado despachoretardado[1];

/**
 * @brief Inicializa el estado del programa para iniciar la ejecución
 * 
 */
static void setup(void);


int main(void){    
    setup();
    for (;;){
        Maquina_procesa(controladordepulsaciones);
        Maquina_procesa(controladordeluz);
        DespachoRetardado_procesarDespacho(despachoretardado);
        pulsadorprocesa(pulsador);
    }
    return 0;
}



static void setup(void){
    static ControladorLuz instanciacontrolador;
    static ControladorDePulsaciones instanciadepulsaciones;
    
    SP_init();
    
    DespachoRetardado_init(despachoretardado);

    ControladorLuz_init(&instanciacontrolador,TIEMPO_ON,PIN_LUZ,LUZ_ON,despachoretardado);
    controladordeluz = ControladorLuz_asMaquina(&instanciacontrolador);
    Maquina_procesa(controladordeluz); // Reset inicializa pin con luz apagada
    
    ControladorDePulsaciones_init(&instanciadepulsaciones,controladordeluz,despachoretardado,TIEMPO_TRIPLE_PULSACION);
    controladordepulsaciones = ControladorDePulsaciones_asMaquina(&instanciadepulsaciones);

    Pulsador_init(pulsador, 
                  controladordepulsaciones,
                  EV_BOTON_PULSADO,
                  PIN_PULSADOR,
                  PULSADOR_NIVEL_ACTIVO,
                  HISTERESIS_ANTIRREBOTE);
}