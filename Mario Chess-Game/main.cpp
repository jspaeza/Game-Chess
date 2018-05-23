#include <iostream> //Bliblioteca estandar de c++ utilizada para operaciones de entrada y salida
#include <stdlib.h>
#include <allegro.h> //Libreria usada para el diseño de la interfaz
#include <cstdlib> //Libreria que incluye la funcion rand()
#include <time.h>  //Libreria usada para el manejo de numeros aleatorios
#include <string> //Libreria utilizada para el manejo de cadenas

using namespace std;

//Declaracion de las variables de tipo Bitmap
BITMAP *buffer;
BITMAP *cursor;
BITMAP *menu;
BITMAP *jugar;
BITMAP *creditos;
BITMAP *salida;
BITMAP *jugadores;
BITMAP *tablero;
BITMAP *princesa;
BITMAP *madre;
BITMAP *padre;
BITMAP *pretendiente;
BITMAP *exnovio;
BITMAP *gexnovio;
BITMAP *gpretendiente;
BITMAP *gpadres;
BITMAP *finjuego;
MIDI *musica; // Musica de fondo
SAMPLE *muerte; //Musica de fin del juego

//Declaracion de la estructura personajes
struct personajes {
    int ronda;
    int dado;
}*pes;   //Declaracion del puntero de tipo struct personajes

//Declaracion de estructura MAPA que sirve de base para la lista encadenada
struct MAPA{
	int dato;
	char celda;
	MAPA *siguiente;
};

//Estructura que contiene los vectores de los nombres
struct nombres{
        char princes[5];
        int pos_prince;
        char padre[5];
        int pos_padre;
        char madre[5];
        int pos_madre;
        char preten[5];
        int pos_preten;
        char ex[5];
        int pos_ex;
};

//Clase que contiene las funciones relacionadas con la lista encadenada
class PRINCIPAL{
    MAPA *cab;
public:
    PRINCIPAL(){cab = NULL;}
    void insertar_letra(int,char);
    char b_ficha(int);
    void iniciar_mapa();
    void insertar(MAPA *, MAPA *, int);
    int desplazar(int,int,int);
    int direccion(int,int,char);
    int mover_padres(int,int,int);
    int mover_princesa(int,int);
    int mover_pretendiente(int,int);
    int mover_exnovio(int,int);
    int direccion_exnovio(int,int);
    int validar_mov(int,int,char);
    ~PRINCIPAL();
};

class INTERFAZ{
    nombres *aux;
public:
    INTERFAZ(){aux = new nombres;}
    char escribir();
    void prueba_img(int,char);
    void most_nombre(int,int);
    int fila(int);
    int columna(int);
    void nombre_personajes();
    void dibujar();
    void pos_ficha(int,char);
    int leer_pos(char);
    void nombres_pjs(int,char,char);
    char leer_nomb(int,char);
    void pantalla();
    void dib_tablero();
    ~INTERFAZ(){delete aux;};
};
INTERFAZ interfaz1; //Instanciacion de la clase interfaz
int posicion_in[5] = {0,0,0,0,0}; //Vector para la posicion inicial de las fichas

//Recorre la lista, e inserta la letra en la ubicacion
void PRINCIPAL::insertar_letra(int n, char let){
    MAPA *p, *q;
    int encontro = 0;
    p = NULL;
    q = cab;
    while(q != NULL && !encontro){
        if(n > q -> dato){
            p = q;
            q = q -> siguiente;
        }
        else {
            encontro = 1;
        }
    }
    if(encontro){
        if(n == q -> dato){
            q -> celda = let;
        }
    }
}

//Recorre la lista y devuelve la letra en la ubicacion
char PRINCIPAL::b_ficha(int n){
    MAPA *p, *q;
    int encontro = 0;
    p = NULL;
    q = cab;
    while(q != NULL && !encontro){
        if(n > q -> dato){
            p = q;
            q = q -> siguiente;
        }
        else {
            encontro = 1;
        }
    }
    if(encontro){
        if(n == q -> dato){
            return q -> celda;
        }
    }
}

//Crea el "vector" mapa como una lista encadenada
void PRINCIPAL::iniciar_mapa(){
    for(int n = 0; n < 64; n++) {
        MAPA *p, *q;
        int encontro = 0;
        p = NULL;
        q = cab;
        while(q != NULL && !encontro){
            if(n > q -> dato){
                p = q;
                q = q -> siguiente;
            }
        }
        insertar (p,q,n);
    }
}

//encadena la lista e inicializa sus miembros
void PRINCIPAL::insertar(MAPA *p, MAPA *q, int n){
 MAPA *nuevo;
 nuevo = new MAPA;
 nuevo -> dato = n;
 nuevo -> celda = 'O';
 nuevo -> siguiente = q;
 if(p != NULL){
  p -> siguiente = nuevo; // Al último nodo de la lista se le encadena el nodo nuevo
 }
 else {
  cab = nuevo;
 }
}

//Funcion que asigna posicion a las fichas
void INTERFAZ::pos_ficha(int n, char letra){
    if(letra == 'P')aux -> pos_prince = n;
    if(letra == 'K')aux -> pos_padre = n;
    if(letra == 'Q')aux -> pos_madre = n;
    if(letra == 'F')aux -> pos_preten = n;
    if(letra == 'E')aux -> pos_ex = n;
}

//Retorna la posicion de la ficha
int INTERFAZ::leer_pos(char letra){
    if(letra == 'P')return aux -> pos_prince;
    if(letra == 'K')return aux -> pos_padre;
    if(letra == 'Q')return aux -> pos_madre;
    if(letra == 'F')return aux -> pos_preten;
    if(letra == 'E')return aux -> pos_ex;
}

//Guarda el nombre de los personajes
void INTERFAZ::nombres_pjs(int j, char letra,char tecla){
    if(letra == 'P')aux -> princes[j] = tecla;
    if(letra == 'K')aux -> padre[j] = tecla;
    if(letra == 'Q')aux -> madre[j] = tecla;
    if(letra == 'F')aux -> preten[j] = tecla;
    if(letra == 'E')aux -> ex[j] = tecla;
}

//Lee el nombre del personaje
char INTERFAZ::leer_nomb(int j, char letra){
    if(letra == 'P')return aux -> princes[j];
    if(letra == 'K')return aux -> padre[j];
    if(letra == 'Q')return aux -> madre[j];
    if(letra == 'F')return aux -> preten[j] ;
    if(letra == 'E')return aux -> ex[j];
}


//Funcion que asigna columnas y filas
void INTERFAZ::prueba_img(int i, char pers){
    int columna, fila;
            if (i >= 0 && i <= 7){
                fila = 0;
                columna = i;
            }
            if (i >= 8 && i <= 15){
                fila = 1;
                columna = i % 8;
            }
            if (i >= 16 && i <= 23){
                fila = 2;
                columna = i % 8;
            }
            if (i >= 24 && i <= 31){
                fila = 3;
                columna = i % 8;
            }
            if (i >= 32 && i <= 39){
                fila = 4;
                columna = i % 8;
            }
            if (i >= 40 && i <= 47){
                fila = 5;
                columna = i % 8;
            }
            if (i >= 48 && i <= 55){
                fila = 6;
                columna = i % 8;
            }
            if (i >= 56 && i <= 63){
                fila = 7;
                columna = i % 8;
            }
            if(pers == 'P')masked_blit(princesa, buffer,0,0, columna*75, fila*75,75,75); // Dibuja el personaje en la ubicacion de la letra correspondiente
            else if(pers == 'Q')masked_blit(madre, buffer,0,0, columna*75, fila*75,75,75);
            else if(pers == 'K')masked_blit(padre, buffer,0,0, columna*75, fila*75,75,75);
            else if(pers == 'E')masked_blit(exnovio, buffer,0,0, columna*75, fila*75,75,75);
            else if(pers == 'F')masked_blit(pretendiente, buffer,0,0, columna*75, fila*75,75,75);
}

void INTERFAZ::pantalla(){
    blit(buffer, screen, 0,0,0,0, 850, 600); //Copia un área rectangular del bitmap origen(buffer) en el bitmap destino(screen)
}

void INTERFAZ::dib_tablero(){
    blit(tablero, buffer, 0,0,0,0,850,600); //Copia un área rectangular del bitmap origen(buffer) en el bitmap destino(tablero)
}

int PRINCIPAL::mover_princesa(int pos, int a){
	int ubi;
	ubi = pos + a;
	if(ubi >= 0 && ubi <= 63){
    	if(b_ficha(ubi) == 'O'){ 	   //verifica que la casilla este disponible
        	insertar_letra(ubi,'P');  //mueve la ficha a la nueva casilla
        	insertar_letra(pos,'O'); //borra la ubicacion anterior
        	interfaz1.pos_ficha(ubi,'P'); //Guarda la ubicacion
        	return 1;                 //indica que se realizo el movimiento
    	}
        else return 0;               //indica que no se realizo el movimiento
	}
	else return 0;
}
int PRINCIPAL::mover_pretendiente(int pos, int a){
    int ubi;
    ubi = pos + a;
    if(ubi >= 0 && ubi <= 63){
        if(b_ficha(ubi) == 'O'){          //verifica que la casilla este disponible
            insertar_letra(ubi,'F');      //mueve la ficha a la nueva casilla
            insertar_letra(pos,'O');      //borra la ubicacion anterior
            interfaz1.pos_ficha(ubi,'F'); //Guarda la ubicacion
            return 1;                     //indica que se realizo el movimiento
        }
        else{
            if(b_ficha(ubi)== 'P'){       //si la princesa esta en esa direccion acaba el juego
            insertar_letra(ubi,'F');      //mueve la ficha a la nueva casilla
            insertar_letra(pos,'O');
            interfaz1.pos_ficha(ubi,'F'); //Guarda la ubicacion
            return 7;                    //indica que gano el pretendiente
            }
        else return 0;                  //indica que no se pudo mover
        }
    }
    else return 0;
}
int PRINCIPAL::mover_padres(int dir,int pos, int a){
    int rango,ubi,pre,fin = 1,lim = 1, mod, desp, cuenta = 0;
    rango = pos + a;
    ubi = pos;
    if(rango >= 0 && rango <= 63){
        if(b_ficha(rango)=='O'){  //comprueba que haya al menos una casilla libre en la direccion
            do{
            cuenta ++;
            mod = ubi % 8;          //"columna" en la que esta ubicada la ficha
            ubi = ubi + a;
            pre = ubi % 8;
            switch(dir){            //determina los bordes del tablero para limitar el movimiento
            case 1:{if(mod==7) lim = 0;
                    if(pre==7) fin = 0;}
            break;
            case 2:{if(mod==7) lim = 0;
                    if(pre==7) fin = 0;}
            break;
            case 3:{if(mod==7) lim = 0;
                    if(pre==7) fin = 0;}
            break;
            case 4: lim = 1;
            break;
            case 5:{if(mod==0) lim = 0;
                    if(pre==0) fin = 0;}
            break;
            case 6:{if(mod==0) lim = 0;
                    if(pre==0) fin = 0;}
            break;
			case 7:{if(mod==0) lim = 0;
                    if(pre==0) fin = 0;}
            break;
            case 8: lim = 1;
            break;
            }
        }
        while(b_ficha(ubi) == 'O' && lim == 1);
        if(lim==1&&(b_ficha(ubi)=='P'||b_ficha(ubi)=='K'||b_ficha(ubi)=='Q'||b_ficha(ubi)=='E'||b_ficha(ubi)=='F')){  //determina que encontró un obstaculo
            if(b_ficha(ubi)=='K' || b_ficha(ubi)=='P'||b_ficha(ubi)=='Q'){     //determina si el obstaculo es uno de los padres o la princesa
                    insertar_letra(ubi-a,b_ficha(pos));            //se ubica una casilla antes del obstaculo
                    insertar_letra(pos,'O');                      //borra la ubicacion anterior
                    if(b_ficha(ubi-a)=='K') interfaz1.pos_ficha(ubi - a,'K'); //Guarda la ubicacion
                    else interfaz1.pos_ficha(ubi - a,'Q'); //Guarda la ubicacion
                    return 1;                                           //indica que se movio y hubo una colision
                }
            if(b_ficha(ubi)=='F'||b_ficha(ubi)=='E'){   //determina si el obstaculo es uno de los pretendientes
                if(b_ficha(ubi+a)=='O'&&fin==1){               //determina si hay espacio dispònible para desplazar al obstaculo
                        desp = desplazar(dir,ubi,a);        //llama a la funcion que movera al obstaculo
                        insertar_letra(ubi,b_ficha(pos));  //ubica la ficha en la posicion que tenia el obstaculo
                        insertar_letra(pos,'O');          //borra la ubicacion anterior
                        if(b_ficha(ubi)=='K') interfaz1.pos_ficha(ubi,'K'); //guarda la ubicacion
                        else interfaz1.pos_ficha(ubi,'Q'); //Guarda la ubicacion
                        return desp;                     //retorna el resultado del desplazamiento
                }
                else{insertar_letra(ubi-a,b_ficha(pos));  //si no habia un espacio disponible para desplazar al obstaculo ubica la ficha una casilla antes
                    insertar_letra(pos,'O');             // borra la ubicacion anterior
                    if(b_ficha(ubi-a)=='K') interfaz1.pos_ficha(ubi - a,'K'); //Guarda la ubicacion
                    else interfaz1.pos_ficha(ubi - a,'Q'); //Guarda la ubicacion
                    return 1;                             //indica que hubo una colision con pretendiente o exnovio pero que no hubo desplazamiento
                }
            }
        }
            else{               //si no encontro obstaculo, significa que llego al final del tablero
                insertar_letra(pos + ((cuenta - 1) *a),b_ficha(pos));//ubica la ficha en el movimiento maximo en la direccion
                if(b_ficha(pos)=='K')interfaz1.pos_ficha(pos + ((cuenta - 1) *a),'K'); //guarda la ubicacion
                else interfaz1.pos_ficha(pos + ((cuenta - 1) *a),'Q');
                insertar_letra(pos,'O');                            //borra la ubicacion anterior

                return 1;                                           //indica que se movio sin obstaculos
            }
        }
        else return 0;
    }
    else return 0;                                              //indica que no se realizo el movimiento al no haber al menos una casilla disponible
}
int PRINCIPAL::desplazar(int dir, int pos, int a){
    int cuenta = 0, ubi, lim = 1, mod;
    ubi = pos;
    do{
        cuenta ++;
        mod = ubi % 8;
        ubi = ubi + a;
        switch(dir){
            case 1:if(mod == 7) lim = 0;
            break;
            case 2:if(mod == 7) lim = 0;
            break;
            case 3:if(mod == 7) lim = 0;
            break;
            case 4: lim = 1;
            break;
            case 5:if(mod == 0) lim = 0;
            break;
            case 6:if(mod == 0) lim = 0;
            break;
			case 7:if(mod == 0) lim = 0;
            break;
            case 8: lim = 1;
            break;
        }
    }
    while(b_ficha(ubi) == 'O' && lim == 1);
    if(lim==1&&(b_ficha(ubi)=='P'||b_ficha(ubi)=='K'||b_ficha(ubi)=='Q'||b_ficha(ubi)=='E'||b_ficha(ubi)=='F')){  //determina que encontró un obstaculo
        if(b_ficha(ubi) == 'P'){                  //si la princesa esta en esa direccion acaba el juego
            if(b_ficha(pos)== 'F')return 7;         //gana el pretendiente
            else return 5;                                //gana el exnovio
        }
        else{
            insertar_letra(ubi-a,b_ficha(pos));
            insertar_letra(pos,'O');                    //borra la ubicacion anterior
            if(b_ficha(ubi-a)=='F') interfaz1.pos_ficha(ubi-a,'F');               //guarda la ubicacion
            else interfaz1.pos_ficha(ubi-a,'E');
        }
    }
    else{
        //insertar_letra(pos+((cuenta -1)*a),b_ficha(pos));//ubica la ficha en el movimiento maximo en la direccion
        insertar_letra(ubi-a,b_ficha(pos));
        insertar_letra(pos,'O');                    //borra la ubicacion anterior
        if(b_ficha(ubi-a)=='F') interfaz1.pos_ficha(ubi-a,'F');               //guarda la ubicacion
        else interfaz1.pos_ficha(ubi-a,'E');
        return 1;
    }
}
int PRINCIPAL::mover_exnovio(int pos, int a){
    int ubi;
    ubi=pos+a;
    if(ubi >= 0 && ubi <= 63){
        if(b_ficha(ubi)!= 'O' && b_ficha(ubi)!= 'P'){  //determina que hay un obstaculo en la casilla                                          //indica que no se pudo mover
            return 0;
        }
        else{
            if(b_ficha(ubi)=='O'){                       //verifica que la casilla este disponible
                insertar_letra(ubi,'E');       //mueve la ficha a la nueva casilla
                interfaz1.pos_ficha(ubi,'E');
                insertar_letra(pos,'O');                //borra la ubicacion anterior
                return 1;                               //indica que se realizo el movimiento
            }
            if(b_ficha(ubi)=='P'){
                insertar_letra(ubi,'E');       //mueve la ficha a la nueva casilla
                interfaz1.pos_ficha(ubi,'E');
                insertar_letra(pos,'O');                //borra la ubicacion anterior
                return 5;                               //si la princesa esta en esa direccion acaba el juego
            }
        }
    }
    else return 0;
}
//inicia el movimiento y cambia de direccion cuando la casilla esta bloqueada
int PRINCIPAL::validar_mov(int dir, int pos, char ficha){
    int val, contador = 0;
    if(ficha == 'E'){
        do{
        val = direccion_exnovio(dir, pos);//inicia el proceso de movimiento en la direccion indicada
        contador++;                         //registra el numero de intentos en una direccion prohibida
        if (dir < 8)dir++;                    //si la direccion es prohibida, prueba con la siguiente
        else dir = 1;                         //salta de la direccion 8 a la direccion 1 para seguir con el ciclo
    }
    while(val == 0 && contador < 9);            //termina el ciclo si el movimiento se realizo o si no encontro un movimeinto valido
    }
    else{
        do{
            val = direccion(dir, pos, ficha);
            contador++;                         //registra el numero de intentos en una direccion prohibida
            if (dir < 8)dir++;                    //si la direccion es prohibida, prueba con la siguiente
            else dir=1;                         //salta de la direccion 8 a la direccion 1 para seguir con el ciclo
        }
    while(val == 0 && contador < 9);            //termina el ciclo si el movimiento se realizo o si no encontro un movimeinto valido
    return val;								//devuelve el resultado
    }
}
int PRINCIPAL::direccion(int dir, int pos, char ficha){             //interpreta el numero de 1-8 como una direccion
    int result = 0, mod;
    mod = pos%8;
    if(ficha == 'P'){
        switch(dir){
            case 1:{
                if(mod==7) result = 0;
                else result = mover_princesa(pos,-7);
            }
            break;
            case 2:{
                if(mod==7) result = 0;
                else result=mover_princesa(pos,1);
            }
            break;
            case 3:{
                if(mod==7) result = 0;
                else result=mover_princesa(pos,9);
            }
            break;
            case 4:result=mover_princesa(pos, 8);
            break;
            case 5:{
            	if(pos%8)result=mover_princesa(pos, 7);
            	else result=0;
				break;
			}
            case 6:{
            	if(pos%8)result = mover_princesa(pos, -1);
            	else result=0;
				break;
			}
			case 7:{
            	if(pos%8)result = mover_princesa(pos, -9);
            	else result=0;
				break;
			}
            case 8:result=mover_princesa(pos, -8);
            break;
        }
        return result;
    }
    if(ficha=='K'||ficha == 'Q'){
       switch(dir){
            case 1:{
                if(mod==7) result = 0;
                else result=mover_padres(dir,pos,-7);
            }
            break;
            case 2:{
                if(mod==7) result = 0;
                else result=mover_padres(dir,pos,1);
            }
            break;
            case 3:{
                if(mod==7) result = 0;
                else result=mover_padres(dir,pos,9);
            }
            break;
            case 4: result=mover_padres(dir,pos, 8);
            break;
            case 5:{
            	if(pos%8)result=mover_padres(dir,pos, 7);
            	else result=0;
				break;
			}
            case 6:{
            	if(pos%8)result=mover_padres(dir,pos, -1);
            	else result=0;
				break;
			}
			case 7:{
            	if(pos%8)result=mover_padres(dir,pos, -9);
            	else result=0;
				break;
			}
            case 8:result=mover_padres(dir,pos, -8);
            break;
        }
        return result;
    }
    if(ficha == 'F'){
        switch(dir){
            case 1:{
                if(mod==7) result = 0;
                else result=mover_pretendiente(pos,-7);
            }
            break;
            case 2:{
                if(mod==7) result = 0;
                else result=mover_pretendiente(pos,1);
            }
            break;
            case 3:{
                if(mod==7) result = 0;
                else result=mover_pretendiente(pos,9);
            }
            break;
            case 4:result=mover_pretendiente(pos, 8);
            break;
            case 5:{
            	if(pos % 8)result=mover_pretendiente(pos, 7);
            	else result=0;
				break;
			}
            case 6:{
            	if(pos % 8)result = mover_pretendiente(pos, -1);
            	else result=0;
				break;
			}
			case 7:{
            	if(pos%8)result = mover_pretendiente(pos, -9);
            	else result=0;
				break;
			}
            case 8:result = mover_pretendiente(pos, -8);
            break;
        }
        return result;
    }
}
int PRINCIPAL::direccion_exnovio(int dir, int pos){             //interpreta el numero de 1-8 como una direccion para exnovio
    int result = 0, mod;
    mod = pos % 8;
        switch(dir){
            case 1:{
                if(mod==7) result = 0;
                else result=mover_exnovio(pos,-15);
            }
            break;
            case 2:{
                if(mod == 7||mod == 6) result = 0;
                else result = mover_exnovio(pos,-6);
            }
            break;
            case 3:{
                if(mod == 7||mod == 6) result = 0;
                else result=mover_exnovio(pos,10);
            }
            break;
            case 4:{
                if(mod == 7) result = 0;
                else result=mover_exnovio(pos,17);
            }
            break;
            case 5:{
                if(mod == 0) result = 0;
                else result=mover_exnovio(pos,15);
            }
            break;
            case 6:{
                if(mod == 0||mod == 1) result = 0;
                else result=mover_exnovio(pos,6);
            }
            break;
			case 7:{
                if(mod == 0||mod == 1) result = 0;
                else result=mover_exnovio(pos,-10);
            }
            break;
            case 8:{
                if(mod == 0) result = 0;
                else result = mover_exnovio(pos,- 17);
            }
            break;
        }
        return result;
}

//Funcion que redibuja el tablero con la posicion reciente de las fichas
void INTERFAZ::dibujar(){
    clear(buffer);
    dib_tablero();
    prueba_img(aux->pos_prince,'P');
    prueba_img(aux->pos_madre,'Q');
    prueba_img(aux->pos_padre,'K');
    prueba_img(aux->pos_preten,'F');
    prueba_img(aux->pos_ex,'E');
    nombre_personajes();
}

void INTERFAZ::nombre_personajes(){
    textprintf(buffer, font, 600,50,  palette_color[50],"    ¡¡¡¡TE AMO PRINCESA¡¡¡¡¡");
    textprintf(buffer, font, 700,280, palette_color[50],"RONDA: %d",pes -> ronda);
    textprintf(buffer, font, 660,500, palette_color[50],"NUMERO ALEATORIO: ");
    int pix = 0, cordx, cordy;
//Princesa
    cordx = columna(aux -> pos_prince);
    cordy = fila(aux -> pos_prince);
    for(int j = 0; j < 5; j++){
        textprintf_ex(buffer, font, (cordx*75) + 15 + pix, (cordy*75) + 7,makecol(1,1,1),-1,"%c",aux -> princes[j]);
        pix = pix + 10;
    }pix=0;
//padre
    cordx = columna(aux->pos_padre);
    cordy = fila(aux -> pos_padre);
    for(int j = 0; j < 5; j++){
        textprintf_ex(buffer, font, (cordx*75) + 15 + pix, (cordy*75) + 7,makecol(1,1,1),-1,"%c",aux -> padre[j]);
        pix = pix + 10;
    }pix=0;
//madre
    cordx = columna(aux -> pos_madre);
    cordy = fila(aux -> pos_madre);
    for(int j = 0; j < 5; j++){
        textprintf_ex(buffer, font, (cordx*75) + 15 + pix, (cordy*75) + 7,makecol(1,1,1),-1,"%c",aux -> madre[j]);
        pix = pix + 10;
    }pix=0;
//pretendiente
    cordx = columna(aux -> pos_preten);
    cordy = fila(aux -> pos_preten);
    for(int j = 0; j < 5; j++){
        textprintf_ex(buffer, font, (cordx*75) + 15 + pix, (cordy*75) + 7,makecol(1,1,1),-1,"%c",aux -> preten[j]);
        pix = pix + 10;
    }pix=0;
//ex
    cordx = columna(aux -> pos_ex);
    cordy = fila(aux -> pos_ex);
    for(int j = 0; j < 5; j++){
        textprintf_ex(buffer, font, (cordx*75) + 15 + pix, (cordy*75) + 7,makecol(1,1,1),-1,"%c", aux -> ex[j]);
        pix = pix + 10;
    }pix=0;
    blit(buffer, screen, 0,0,0,0, 850, 600);
}

//Funcion que asigna columnas y filas y retorna fila
int INTERFAZ::fila(int i){
    int fila;
            if (i >= 0 && i <= 7){
                fila = 0;
            }
            if (i >= 8 && i <= 15){
                fila = 1;
            }
            if (i >= 16 && i <= 23){
                fila = 2;
            }
            if (i >= 24 && i <= 31){
                fila = 3;
            }
            if (i >= 32 && i <= 39){
                fila = 4;
            }
            if (i >= 40 && i <= 47){
                fila = 5;
            }
            if (i >= 48 && i <= 55){
                fila = 6;
            }
            if (i >= 56 && i <= 63){
                fila = 7;
            }
            return fila;
}

//Funcion que asigna columnas y filas y retorna columna
int INTERFAZ::columna(int i){
    int columna;
            if (i >= 0 && i <= 7){
                columna = i;
            }
            else columna = i % 8;
            return columna;
 }

//Funcion para ingresar texto
char INTERFAZ::escribir(){
    while(true){
        char newkey = readkey() & 0xff;
        if(newkey >= 32 && newkey <= 126){
            return newkey;
        }
        else if(newkey == 13)return ' ';
    }
}
PRINCIPAL::~PRINCIPAL(){
    MAPA *p;
    if(cab==NULL);
    else{
        p=cab;
        while(cab!=NULL){
            cab=cab->siguiente;
            delete p;
            p=cab;
        }
    }
}

//Funcion principal del programa
int main()
{
    allegro_init(); // Llamada a la libreria allegro
    install_keyboard(); //Permite el manejo del teclado
    install_mouse(); //Permite usar el mouse
    install_timer(); //Permite el uso de sonidos

    // Titulo de la ventana del juego
    set_window_title("                                      TE AMO PRINCESA                                    ");

    // Inicializa la musica
    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0) {
        allegro_message("Error: inicializando sistema de sonido\n%s\n", allegro_error);
        return 1;
    }
    set_volume(70, 70); // Ajuste de volumen

    set_color_depth(32); //Determina la profundidad del color
    set_gfx_mode(GFX_AUTODETECT_WINDOWED,850,600, 0, 0);

    PRINCIPAL principal1; //Instanciacion de la clase principal
    principal1.iniciar_mapa(); //inicia la lista encadenada de 64 estructuras

    pes = new struct personajes; //Convierte la estructura personajes en estructura dinamica

    int aleatorio, i, flag, j, n = 1, mov,pix = 0, x = 0, cordx, cordy;// Declaracion de variables utilizadas en el programa
    time_t seconds;
    time(&seconds);
    srand(time(NULL));

    for(i = 0; i <= 4; i++){
            flag = 0;
            while(flag == 0){
                posicion_in[i] =  1 + rand() % (64 - 1);
                for(j = 0; j <= 4; j++){
                    if(posicion_in[i] == posicion_in[j] && i != j){
                        flag = 0;
                        break;
                    }
                    else if(posicion_in[i] != posicion_in[j] && i != j){
                        flag = 1;
                }
            }
        }
    }

    //posicion inicial aleatoria para las fichas
    principal1.insertar_letra(posicion_in[0], 'P');
    interfaz1.pos_ficha(posicion_in[0],'P');
    principal1.insertar_letra(posicion_in[1], 'Q');
    interfaz1.pos_ficha(posicion_in[1],'Q');
    principal1.insertar_letra(posicion_in[2], 'K');
    interfaz1.pos_ficha(posicion_in[2],'K');
    principal1.insertar_letra(posicion_in[3], 'F');
    interfaz1.pos_ficha(posicion_in[3],'F');
    principal1.insertar_letra(posicion_in[4], 'E');
    interfaz1.pos_ficha(posicion_in[4],'E');

    buffer = create_bitmap(SCREEN_W,SCREEN_H); //Crea el bitmap principal donde se dibuja todo
    cursor = load_bitmap("cursor.bmp",NULL);
    menu = load_bitmap("menu.bmp",NULL);
    jugar = load_bitmap("jugar.bmp",NULL);
    creditos = load_bitmap("nombres.bmp",NULL);
    salida = load_bitmap("salir.bmp",NULL);
    jugadores = load_bitmap("bienvenido.bmp",NULL);
    princesa = load_bitmap("princesa.bmp", NULL); //Carga la imagen de la princesa
    madre = load_bitmap("daisy.bmp", NULL);
    exnovio = load_bitmap("luigi.bmp", NULL);
    pretendiente = load_bitmap("mario.bmp", NULL);
    padre = load_bitmap("bowser.bmp", NULL);
    tablero =  load_bitmap("tablero.bmp",NULL);
    gexnovio = load_bitmap("ganoexnovio.bmp",NULL);
    gpretendiente = load_bitmap("ganopretendiente.bmp",NULL);
    gpadres = load_bitmap("gananpadres.bmp",NULL);
    musica = load_midi("mario.mid"); //Carga la cancion de fondo
    muerte = load_wav("muerte.wav");

    play_midi(musica,1); // Reproduce cancion de fondo

    //Menu
    bool salir = false;
    while(!salir){
      if(mouse_x > 228  && mouse_x < 625 && mouse_y > 300 && mouse_y < 359){
        draw_sprite(buffer, jugar, 0, 0);
        if(mouse_b & 1){
            salir = true;
         }
       }
       else if(mouse_x > 129 && mouse_x < 723  && mouse_y > 369 && mouse_y < 431){
        draw_sprite(buffer, creditos, 0, 0);
        }
       else if(mouse_x > 238 && mouse_x < 614 && mouse_y > 443 && mouse_y < 505 ){
        draw_sprite(buffer, salida, 0, 0);
        if(mouse_b & 1){
            break;
        }
       }
       else{
        draw_sprite(buffer, menu, 0, 0);
       }
       masked_blit(cursor,buffer,0,0,mouse_x,mouse_y,13,22);
       blit(buffer, screen, 0, 0, 0, 0, 850, 600);
    }

    blit(jugadores, screen, 0,0,0,0,850,600);

//nombre princesa
     for(int j = 0;j < 5;j++){
        interfaz1.nombres_pjs(j,'P', interfaz1.escribir());
        if( interfaz1.leer_nomb(j, 'P') == ' '){
            j = 4;
     }
     else {
         textprintf_ex(jugadores, font, 220 + pix, 250,makecol(1,1,1),-1,"%c",interfaz1.leer_nomb(j, 'P'));
         blit(jugadores, screen, 0,0,0,0,850,600);
         pix = pix + 10;
         }
    }pix = 0;
//nombre padre
    for(int j = 0;j < 5;j++){
     interfaz1.nombres_pjs(j,'K', interfaz1.escribir());
      if(interfaz1.leer_nomb(j, 'K') == ' '){
        j = 4;
     }
     else {
         textprintf_ex(jugadores, font, 160 + pix, 315,makecol(1,1,1),-1,"%c",interfaz1.leer_nomb(j, 'K'));
         blit(jugadores, screen, 0,0,0,0,850,600);
         pix = pix + 10;
         }
    }pix = 0;
//nombre madre
    for(int j = 0;j < 5;j++){
     interfaz1.nombres_pjs(j,'Q', interfaz1.escribir());
      if(interfaz1.leer_nomb(j, 'Q')== ' '){
        j = 4;
     }
     else {
         textprintf_ex(jugadores, font, 160 + pix, 380,makecol(1,1,1),-1,"%c",interfaz1.leer_nomb(j, 'Q'));
         blit(jugadores, screen, 0,0,0,0,850,600);
         pix = pix + 10;
         }
    }pix = 0;
//nombre exnovio
    for(int j = 0;j < 5;j++){
      interfaz1.nombres_pjs(j,'E', interfaz1.escribir());
      if(interfaz1.leer_nomb(j, 'E') == ' '){
        j = 4;
     }
     else {
         textprintf_ex(jugadores, font, 210 + pix, 445,makecol(1,1,1),-1,"%c",interfaz1.leer_nomb(j, 'E'));
         blit(jugadores, screen, 0,0,0,0,850,600);
         pix = pix + 10;
         }
    }pix = 0;
//nombre pretendiente
    for(int j = 0;j < 5;j++){
      interfaz1.nombres_pjs(j,'F', interfaz1.escribir());
      if(interfaz1.leer_nomb(j, 'F') == ' '){
        j = 4;
     }
     else {
         textprintf_ex(jugadores, font, 290 + pix, 505,makecol(1,1,1),-1,"%c",interfaz1.leer_nomb(j, 'F'));
         blit(jugadores, screen, 0,0,0,0,850,600);
         pix = pix + 10;
         }
    }pix = 0;

    pes -> ronda = 1;

    while(!key[KEY_ESC] && n <= 10){
        if(keypressed() && key[KEY_DOWN]){
            x = x + 1;
            aleatorio =  1 + rand() % (9 - 1); //Genera un numero aleatorio de 1 a 8
            textprintf(screen, font,795,500, palette_color[50]," %d",aleatorio);
            if(x == 1){
               principal1.validar_mov(aleatorio,interfaz1.leer_pos('P'),'P');
               pes -> dado = aleatorio;
               rest(500);
               interfaz1.dibujar();
            }else if (x == 2){
              principal1.validar_mov(aleatorio,interfaz1.leer_pos('K'),'K');
              rest(500);
              interfaz1.dibujar();
            }else if (x == 3){
              principal1.validar_mov(aleatorio,interfaz1.leer_pos('Q'),'Q');
              rest(500);
              interfaz1.dibujar();
            }else if (x == 4){
              mov=principal1.validar_mov(aleatorio,interfaz1.leer_pos('F'),'F');
              rest(500);
              interfaz1.dibujar();
            }else if (x == 5){
              mov=principal1.validar_mov(aleatorio,interfaz1.leer_pos('E'),'E');
              rest(500);
              interfaz1.dibujar();
              x = 0;
              n ++;
              pes -> ronda = n;
            }

            if ( mov == 7){
                rest(500);
                clear_bitmap(buffer);
                draw_sprite(buffer, gpretendiente, 0, 0);
                blit(buffer, screen, 0, 0, 0, 0, 850, 600);
                play_sample(muerte,250,150,1000,0); // Reproduce cancion de fin de juego
                rest(1000);
                break;
            }
            else if ( mov == 5){
                rest(500);
                clear_bitmap(buffer);
                draw_sprite(buffer, gexnovio, 0, 0);
                blit(buffer, screen, 0, 0, 0, 0, 850, 600);
                play_sample(muerte,250,150,1000,0);
                rest(1000);
                break;
            }
            else if (n == 11){
                rest(500);
                clear_bitmap(buffer);
                draw_sprite(buffer, gpadres, 0, 0);
                blit(buffer, screen, 0, 0, 0, 0, 850, 600);
                play_sample(muerte,250,150,1000,0);
                rest(1000);
                break;
            }
        }
    interfaz1.dibujar();
    }
}
END_OF_MAIN(); //Esta sentencia permite el funcionamiento de la libreria en Windows Y Linux
