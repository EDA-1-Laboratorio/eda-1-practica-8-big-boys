/*
 * ==========================================================================
 *  EJERCICIO 1 — Simulación del juego "Snake"
 * ==========================================================================
 *
 *  La víbora se representa como una lista doblemente ligada donde:
 *    - La CABEZA de la lista es la cabeza de la víbora.
 *    - Cada nodo almacena la posición (fila * 100 + columna) de un segmento.
 *    - El último nodo es la cola de la víbora.
 *
 *  ¿Por qué lista doblemente ligada?
 *    • Crecer: al comer comida, se agrega un segmento al final (insertar_final).
 *    • Moverse: se agrega un nuevo segmento en la cabeza (insertar_inicio)
 *      y se elimina el último segmento (eliminar_final). Ambas operaciones
 *      requieren recorrer eficientemente la lista.
 *    • Colisión consigo misma: se necesita recorrer el cuerpo para verificar
 *      si la nueva posición de la cabeza coincide con algún segmento.
 *    • El puntero "previo" permite recorrer la víbora desde la cola cuando
 *      es necesario (por ejemplo, para imprimirla en reversa).
 *
 *  Las y los alumnos deben completar las funciones marcadas con TODO.
 *  Compile con:
 *      gcc -Wall -Wextra -o snake snake.c listadl.c
 *
 * ==========================================================================
 */

#include "listadl.h"
#include <string.h>

/* ---------- Configuración del tablero ---------- */
#define FILAS    15
#define COLUMNAS 30

/* Codificación de posición: fila * 100 + columna */
#define POS(f, c) ((f) * 100 + (c))
#define FILA(p)   ((p) / 100)
#define COL(p)    ((p) % 100)

/* Direcciones */
#define ARRIBA    0
#define ABAJO     1
#define IZQUIERDA 2
#define DERECHA   3

/* Caracteres para dibujar */
#define CH_CABEZA 'O'
#define CH_CUERPO 'o'
#define CH_COMIDA '*'
#define CH_VACIO  '.'

#include "listadl.h"

dllista *crear_elemento(DATO dato) {
    dllista *nuevo = (dllista *)malloc(sizeof(dllista));
    if (nuevo == NULL)
        return NULL;
    nuevo->dato = dato;
    nuevo->previo = NULL;
    nuevo->siguiente = NULL;
    return nuevo;
}

ListaDL *crear_lista(void) {
    ListaDL *lista = (ListaDL *)malloc(sizeof(ListaDL));
    if (lista == NULL)
        return NULL;
    lista->cabeza = NULL;
    lista->longitud = 0;
    return lista;
}

void insertar_inicio(ListaDL *lista, DATO dato) {
    dllista *nuevo = crear_elemento(dato);
    if (nuevo == NULL)
        return;

    if (lista->cabeza == NULL) {
        lista->cabeza = nuevo;
    } else {
        nuevo->siguiente = lista->cabeza;
        lista->cabeza->previo = nuevo;
        lista->cabeza = nuevo;
    }
    lista->longitud++;
}

void insertar_final(ListaDL *lista, DATO dato) {
    dllista *nuevo = crear_elemento(dato);
    if (nuevo == NULL)
        return;

    if (lista->cabeza == NULL) {
        lista->cabeza = nuevo;
    } else {
        dllista *actual = lista->cabeza;
        while (actual->siguiente != NULL)
            actual = actual->siguiente;
        actual->siguiente = nuevo;
        nuevo->previo = actual;
    }
    lista->longitud++;
}

void insertar_en_posicion(ListaDL *lista, DATO dato, int posicion) {
    if (posicion < 0 || posicion > lista->longitud)
        return;

    if (posicion == 0) {
        insertar_inicio(lista, dato);
        return;
    }
    if (posicion == lista->longitud) {
        insertar_final(lista, dato);
        return;
    }

    dllista *nuevo = crear_elemento(dato);
    if (nuevo == NULL)
        return;

    dllista *actual = lista->cabeza;
    for (int i = 0; i < posicion; i++)
        actual = actual->siguiente;

    nuevo->previo = actual->previo;
    nuevo->siguiente = actual;
    actual->previo->siguiente = nuevo;
    actual->previo = nuevo;
    lista->longitud++;
}

DATO eliminar_inicio(ListaDL *lista) {
    if (lista->cabeza == NULL)
        return -1;

    dllista *eliminado = lista->cabeza;
    DATO dato = eliminado->dato;

    if (lista->cabeza->siguiente == NULL) {
        lista->cabeza = NULL;
    } else {
        lista->cabeza = lista->cabeza->siguiente;
        lista->cabeza->previo = NULL;
    }

    free(eliminado);
    lista->longitud--;
    return dato;
}

DATO eliminar_final(ListaDL *lista) {
    if (lista->cabeza == NULL)
        return -1;

    dllista *actual = lista->cabeza;
    while (actual->siguiente != NULL)
        actual = actual->siguiente;

    DATO dato = actual->dato;

    if (actual->previo == NULL) {
        lista->cabeza = NULL;
    } else {
        actual->previo->siguiente = NULL;
    }

    free(actual);
    lista->longitud--;
    return dato;
}

DATO eliminar_en_posicion(ListaDL *lista, int posicion) {
    if (posicion < 0 || posicion >= lista->longitud)
        return -1;

    if (posicion == 0)
        return eliminar_inicio(lista);
    if (posicion == lista->longitud - 1)
        return eliminar_final(lista);

    dllista *actual = lista->cabeza;
    for (int i = 0; i < posicion; i++)
        actual = actual->siguiente;

    DATO dato = actual->dato;
    actual->previo->siguiente = actual->siguiente;
    actual->siguiente->previo = actual->previo;
    free(actual);
    lista->longitud--;
    return dato;
}

int buscar(ListaDL *lista, DATO dato) {
    dllista *actual = lista->cabeza;
    int posicion = 0;
    while (actual != NULL) {
        if (actual->dato == dato)
            return posicion;
        actual = actual->siguiente;
        posicion++;
    }
    return -1;
}

DATO obtener(ListaDL *lista, int posicion) {
    if (posicion < 0 || posicion >= lista->longitud)
        return -1;

    dllista *actual = lista->cabeza;
    for (int i = 0; i < posicion; i++)
        actual = actual->siguiente;
    return actual->dato;
}

int esta_vacia(ListaDL *lista) {
    return lista->cabeza == NULL;
}

int longitud(ListaDL *lista) {
    return lista->longitud;
}

void imprimir_lista(ListaDL *lista) {
    dllista *actual = lista->cabeza;
    while (actual != NULL) {
        printf("[%d]", actual->dato);
        if (actual->siguiente != NULL)
            printf(" <-> ");
        actual = actual->siguiente;
    }
    printf(" -> NULL\n");
}

void imprimir_lista_reversa(ListaDL *lista) {
    dllista *actual = lista->cabeza;
    if (actual == NULL) {
        printf(" -> NULL\n");
        return;
    }
    while (actual->siguiente != NULL)
        actual = actual->siguiente;
    while (actual != NULL) {
        printf("[%d]", actual->dato);
        if (actual->previo != NULL)
            printf(" <-> ");
        actual = actual->previo;
    }
    printf(" -> NULL\n");
}

void liberar_lista(ListaDL *lista) {
    dllista *actual = lista->cabeza;
    while (actual != NULL) {
        dllista *siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
    free(lista);
}

/* ================================================================
 *  Funciones proporcionadas (no modificar)
 * ================================================================ */

DATO generar_comida(ListaDL *vibora) {
    DATO pos;
    do {
        int f = 1 + rand() % (FILAS - 2);
        int c = 1 + rand() % (COLUMNAS - 2);
        pos = POS(f, c);
    } while (buscar(vibora, pos) != -1);
    return pos;
}

void dibujar_tablero(ListaDL *vibora, DATO comida) {
    char tablero[FILAS][COLUMNAS + 1];

    for (int f = 0; f < FILAS; f++) {
        for (int c = 0; c < COLUMNAS; c++) {
            if (f == 0 || f == FILAS - 1 || c == 0 || c == COLUMNAS - 1)
                tablero[f][c] = '#';
            else
                tablero[f][c] = CH_VACIO;
        }
        tablero[f][COLUMNAS] = '\0';
    }

    tablero[FILA(comida)][COL(comida)] = CH_COMIDA;

    dllista *seg = vibora->cabeza;
    int primero = 1;
    while (seg != NULL) {
        int f = FILA(seg->dato);
        int c = COL(seg->dato);
        tablero[f][c] = primero ? CH_CABEZA : CH_CUERPO;
        primero = 0;
        seg = seg->siguiente;
    }

    printf("\n");
    for (int f = 0; f < FILAS; f++)
        printf("  %s\n", tablero[f]);
    printf("\n");
}

void mostrar_estado(int turno, int puntaje, int dir) {
    const char *nombres[] = {"ARRIBA", "ABAJO", "IZQUIERDA", "DERECHA"};
    printf("  Turno: %d | Puntaje: %d | Dirección: %s\n",
           turno, puntaje, nombres[dir]);
}

/* ================================================================
 *  Funciones por completar
 * ================================================================ */

DATO calcular_nueva_cabeza(DATO cabeza_actual, int direccion) {
    int f = FILA(cabeza_actual);
    int c = COL(cabeza_actual);

    if (direccion == ARRIBA) f--;
    else if (direccion == ABAJO) f++;
    else if (direccion == IZQUIERDA) c--;
    else if (direccion == DERECHA) c++;

    return POS(f, c);
}

int colision_pared(DATO posicion) {
    int f = FILA(posicion);
    int c = COL(posicion);

    if (f <= 0 || f >= FILAS - 1 || c <= 0 || c >= COLUMNAS - 1)
        return 1;

    return 0;
}

int colision_cuerpo(ListaDL *vibora, DATO nueva_pos) {
    if (buscar(vibora, nueva_pos) != -1)
        return 1;

    return 0;
}

int mover_vibora(ListaDL *vibora, int direccion, DATO comida) {
    DATO nueva_pos = calcular_nueva_cabeza(vibora->cabeza->dato, direccion);

    insertar_inicio(vibora, nueva_pos);

    if (nueva_pos == comida) {
        return 1;
    } else {
        eliminar_final(vibora);
        return 0;
    }
}

/* ================================================================
 *  Secuencia de movimientos predefinida (simulación automática)
 * ================================================================ */

int main(void) {
    srand(42);

    ListaDL *vibora = crear_lista();
    int f_ini = FILAS / 2;
    int c_ini = COLUMNAS / 2;
    insertar_final(vibora, POS(f_ini, c_ini));
    insertar_final(vibora, POS(f_ini, c_ini - 1));
    insertar_final(vibora, POS(f_ini, c_ini - 2));

    DATO comida = generar_comida(vibora);
    int puntaje = 0;

    int movimientos[] = {
        DERECHA, DERECHA, DERECHA, DERECHA, DERECHA,
        ABAJO, ABAJO, ABAJO,
        IZQUIERDA, IZQUIERDA, IZQUIERDA, IZQUIERDA,
        ARRIBA, ARRIBA,
        DERECHA, DERECHA, DERECHA,
        ABAJO, ABAJO, ABAJO, ABAJO,
        IZQUIERDA, IZQUIERDA,
        ARRIBA, ARRIBA, ARRIBA,
        DERECHA, DERECHA, DERECHA, DERECHA
    };
    int total_movimientos = sizeof(movimientos) / sizeof(movimientos[0]);

    printf("╔══════════════════════════════════════╗\n");
    printf("║        SIMULACIÓN DE SNAKE           ║\n");
    printf("╚══════════════════════════════════════╝\n");

    dibujar_tablero(vibora, comida);
    mostrar_estado(0, puntaje, DERECHA);

    for (int i = 0; i < total_movimientos; i++) {
        int dir = movimientos[i];
        DATO nueva = calcular_nueva_cabeza(vibora->cabeza->dato, dir);

        if (colision_pared(nueva)) {
            printf("  ¡GAME OVER! La víbora chocó con la pared.\n");
            break;
        }
        if (colision_cuerpo(vibora, nueva)) {
            printf("  ¡GAME OVER! La víbora se mordió a sí misma.\n");
            break;
        }

        int comio = mover_vibora(vibora, dir, comida);
        if (comio) {
            puntaje += 10;
            comida = generar_comida(vibora);
            printf("  ¡Ñam! +10 puntos\n");
        }

        dibujar_tablero(vibora, comida);
        mostrar_estado(i + 1, puntaje, dir);
        printf("  Longitud de la víbora: %d segmentos\n", longitud(vibora));
    }

    printf("\n  Puntaje final: %d\n", puntaje);
    printf("  Longitud final: %d segmentos\n", longitud(vibora));

    printf("\n  Víbora (cabeza -> cola): ");
    imprimir_lista(vibora);
    printf("  Víbora (cola -> cabeza): ");
    imprimir_lista_reversa(vibora);

    liberar_lista(vibora);
    return 0;
}
