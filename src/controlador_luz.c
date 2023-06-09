#include "controlador_luz.h"
#include <maquina_estado_impl.h>

static Resultado estadooff(Maquina *contexto,Evento evento);
static Resultado estadoEncendido(Maquina *contexto,Evento evento);
static Resultado estadoMudanza (Maquina *contexto, Evento evento);

void ControladorLuz_init(ControladorLuz *self,uint32_t tiempoOn,SP_HPin pindeluz,bool niveldeluzon,DespachoRetardado *despachoRetardado){

    Maquina_init(&self->maquina,estadooff);
    self->timeon = tiempoOn;
    self->interfazLuz.pin=pindeluz;
    self->interfazLuz.nivelOn=niveldeluzon;
    self->despachoRetardado = despachoRetardado;
}

Maquina * ControladorLuz_asMaquina(ControladorLuz *self){
    return &self->maquina;
}

static void Controladordeluzapagaluz(ControladorLuz *self){
    SP_Pin_write(self->interfazLuz.pin,!self->interfazLuz.nivelOn);
}

static void Controladordeluzenciendeluz(ControladorLuz *self){
    SP_Pin_write(self->interfazLuz.pin,self->interfazLuz.nivelOn);
}

static Resultado estadooff(Maquina *contexto,Evento evento){
    ControladorLuz *self = (ControladorLuz*)contexto;               
    Resultado r = {0};                                              
    switch (evento){
    case EV_RESET:                                                  //Si se produce "RESET" en apagado
        SP_Pin_setModo(self->interfazLuz.pin,SP_PIN_ENTRADA);       
        Controladordeluzapagaluz(self);                             //Apago la luz
        SP_Pin_setModo(self->interfazLuz.pin,SP_PIN_SALIDA);
        r.codigo = RES_PROCESADO;                               //Indico que hay un cambio de estado
    break; case EV_BOTON_PULSADO:                                   //Si se pulsa el boton
        Controladordeluzenciendeluz(self);                          //Enciendo la luz
        DespachoRetardado_programarDespacho(self->despachoRetardado,contexto,EV_TIMEOUT,self->timeon); //Configuro el Timeout           
        r.codigo = RES_TRANSICION;                               //Indico un cambio de estado
        r.nuevoEstado = estadoEncendido;                            //El nuevo estado será encendido
    break;default:
        r.codigo = RES_IGNORADO;                                 //Si pasa algun otro evento lo ignora
    break;
    }
    return r;                                                       //Retorno el resultado con el código del mismo y el nuevo estado (de ser necesario)
}
static Resultado estadoEncendido(Maquina *contexto,Evento evento){
    ControladorLuz *self = (ControladorLuz*)contexto;
    Resultado r = {0};
    switch (evento){
    case EV_TIMEOUT:                                                //Si ocurre el TIMEOUT
        Controladordeluzapagaluz(self);                             //Apago la luz
        r.codigo = RES_TRANSICION;                             
        r.nuevoEstado = estadooff;                              //Cambio a estado apagado                              
    break; case EV_TRIPLE_PULSACION:                                //Si llego a las tres pulsaciones
        r.codigo = RES_TRANSICION;                               
        r.nuevoEstado = estadoMudanza;                              //Paso al estado mudanza
    break;default:
        r.codigo = RES_IGNORADO;
    break;
    }
    return r;
}

static Resultado estadoMudanza(Maquina *contexto,Evento evento){
    ControladorLuz *self = (ControladorLuz*)contexto;
    Resultado r = {0};
    switch (evento){
    case EV_TRIPLE_PULSACION:
        Controladordeluzapagaluz(self);
        r.codigo = RES_TRANSICION;
        r.nuevoEstado = estadooff;
    break;default:
        r.codigo = RES_IGNORADO;
    break;
    }
    return r;
}
