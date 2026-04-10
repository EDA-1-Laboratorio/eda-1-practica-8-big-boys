/*
 * ==========================================================================
 * EJERCICIO 2 — Carrusel de inventario (lista doblemente ligada circular)
 * ==========================================================================
 *
 * En muchos videojuegos el jugador dispone de un inventario de herramientas
 * que puede recorrer cíclicamente con las teclas "siguiente" y "anterior".
 *
 * En este ejercicio se implementa un CARRUSEL CIRCULAR usando la estructura
 * dllista directamente (sin ListaDL), conectando el último elemento con el
 * primero en ambos sentidos. Un puntero "seleccion" indica la herramienta
 * actualmente equipada.
 *
 * ¿Por qué lista doblemente ligada?
 * • Avanzar y retroceder en el carrusel en O(1) gracias a los punteros
 * "siguiente" y "previo".
 * • Insertar y eliminar herramientas en cualquier posición sin perder
 * la capacidad de navegar en ambas direcciones.
 *
 * Compile con:
 * gcc -Wall -Wextra -o carrusel carrusel.c listadl.c
 *
 * ==========================================================================
 */

#include "listadl.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h> /* Necesario para free() si no está en listadl.h */

/* ---------- Herramientas del inventario ---------- */
/* Cada herramienta se identifica con un entero (DATO) */
#define ESPADA      0
#define ARCO        1
#define ESCUDO      2
#define POCION      3
#define BOMBA       4
#define ANTORCHA    5
#define GANCHO      6
#define LLAVE       7

#define NUM_HERRAMIENTAS 8

const char *nombre_herramienta(DATO id) {
    const char *nombres[] = {
        "Espada", "Arco", "Escudo", "Poción",
        "Bomba", "Antorcha", "Gancho", "Llave"
    };
    if (id >= 0 && id < NUM_HERRAMIENTAS)
        return nombres[id];
    return "???";
}

/* ================================================================
 * Funciones proporcionadas (no modificar)
 * ================================================================ */

/*
 * Muestra el carrusel centrado en la selección actual.
 * Muestra 2 herramientas a cada lado de la seleccionada.
 */
void mostrar_carrusel(dllista *seleccion) {
    if (seleccion == NULL) {
        printf("  [ Inventario vacío ]\n");
        return;
    }

    /* Retroceder 2 posiciones para mostrar contexto */
    dllista *inicio = seleccion->previo->previo;

    printf("  ");
    dllista *actual = inicio;
    for (int i = 0; i < 5; i++) {
        if (actual == seleccion)
            printf(" [> %s <] ", nombre_herramienta(actual->dato));
        else
            printf("   %s   ", nombre_herramienta(actual->dato));

        if (i < 4)
            printf("-");
        actual = actual->siguiente;
    }
    printf("\n");
}

/*
 * Cuenta los elementos del carrusel circular.
 */
int contar_carrusel(dllista *cualquiera) {
    if (cualquiera == NULL)
        return 0;
    int cuenta = 1;
    dllista *actual = cualquiera->siguiente;
    while (actual != cualquiera) {
        cuenta++;
        actual = actual->siguiente;
    }
    return cuenta;
}

/*
 * Imprime todo el inventario en orden desde un nodo dado.
 */
void imprimir_inventario(dllista *desde) {
    if (desde == NULL) {
        printf("  (vacío)\n");
        return;
    }
    printf("  Inventario: ");
    dllista *actual = desde;
    do {
        printf("%s", nombre_herramienta(actual->dato));
        actual = actual->siguiente;
        if (actual != desde)
            printf(" -> ");
    } while (actual != desde);
    printf(" -> (vuelve a %s)\n", nombre_herramienta(desde->dato));
}

/* ================================================================
 * Funciones completadas
 * ================================================================ */

/*
 * insertar_en_carrusel
 *
 * Inserta un nuevo elemento en el carrusel circular DESPUÉS del
 * nodo "despues_de".
 */
dllista *insertar_en_carrusel(dllista *despues_de, DATO dato) {
    dllista *nuevo = crear_elemento(dato);
    if (nuevo == NULL)
        return despues_de;

    if (despues_de == NULL) {
        /* El carrusel está vacío.
         * El nuevo nodo debe apuntar a sí mismo en ambos sentidos. */
        nuevo->siguiente = nuevo;
        nuevo->previo = nuevo;
        return nuevo;
    }

    /* Insertar "nuevo" entre "despues_de" y "despues_de->siguiente". */
    nuevo->siguiente = despues_de->siguiente;
    nuevo->previo = despues_de;
    
    despues_de->siguiente->previo = nuevo;
    despues_de->siguiente = nuevo;

    return nuevo;
}

/*
 * eliminar_del_carrusel
 *
 * Elimina el nodo "objetivo" del carrusel circular y libera su memoria.
 */
dllista *eliminar_del_carrusel(dllista *objetivo) {
    if (objetivo == NULL)
        return NULL;

    /* Caso 1: solo hay un elemento */
    if (objetivo->siguiente == objetivo) {
        free(objetivo);
        return NULL;
    }

    /* Caso 2: hay más elementos */
    dllista *siguiente_nodo = objetivo->siguiente;
    
    /* Conectar el nodo anterior con el siguiente, saltando el "objetivo" */
    objetivo->previo->siguiente = objetivo->siguiente;
    objetivo->siguiente->previo = objetivo->previo;
    
    free(objetivo);
    
    return siguiente_nodo;
}

/*
 * avanzar
 *
 * Avanza la selección "n" posiciones hacia adelante (siguiente).
 */
dllista *avanzar(dllista *seleccion, int n) {
    if (seleccion == NULL) return NULL;
    
    /* Recorre "n" veces hacia adelante */
    for (int i = 0; i < n; i++) {
        seleccion = seleccion->siguiente;
    }
    return seleccion;
}

/*
 * retroceder
 *
 * Retrocede la selección "n" posiciones hacia atrás (previo).
 */
dllista *retroceder(dllista *seleccion, int n) {
    if (seleccion == NULL) return NULL;
    
    /* Recorre "n" veces hacia atrás */
    for (int i = 0; i < n; i++) {
        seleccion = seleccion->previo;
    }
    return seleccion;
}

/* ================================================================
 * Simulación del carrusel de inventario
 * ================================================================ */

int main(void) {
    printf("╔══════════════════════════════════════╗\n");
    printf("║    CARRUSEL DE INVENTARIO            ║\n");
    printf("╚══════════════════════════════════════╝\n\n");

    /* --- Construir el inventario inicial --- */
    printf(">> Construyendo inventario inicial...\n\n");

    dllista *seleccion = NULL;
    seleccion = insertar_en_carrusel(seleccion, ESPADA);
    seleccion = insertar_en_carrusel(seleccion, ARCO);
    seleccion = insertar_en_carrusel(seleccion, ESCUDO);
    seleccion = insertar_en_carrusel(seleccion, POCION);
    seleccion = insertar_en_carrusel(seleccion, BOMBA);

    /* Volver a la Espada como selección inicial */
    seleccion = avanzar(seleccion, 1);

    printf("  Inventario inicial (%d objetos):\n", contar_carrusel(seleccion));
    imprimir_inventario(seleccion);
    printf("\n");

    /* --- Navegar hacia adelante --- */
    printf(">> Avanzar 1 posición (siguiente):\n");
    seleccion = avanzar(seleccion, 1);
    mostrar_carrusel(seleccion);
    printf("  Equipado: %s\n\n", nombre_herramienta(seleccion->dato));

    printf(">> Avanzar 2 posiciones:\n");
    seleccion = avanzar(seleccion, 2);
    mostrar_carrusel(seleccion);
    printf("  Equipado: %s\n\n", nombre_herramienta(seleccion->dato));

    /* --- Navegar hacia atrás --- */
    printf(">> Retroceder 1 posición (anterior):\n");
    seleccion = retroceder(seleccion, 1);
    mostrar_carrusel(seleccion);
    printf("  Equipado: %s\n\n", nombre_herramienta(seleccion->dato));

    printf(">> Retroceder 3 posiciones:\n");
    seleccion = retroceder(seleccion, 3);
    mostrar_carrusel(seleccion);
    printf("  Equipado: %s\n\n", nombre_herramienta(seleccion->dato));

    /* --- Agregar herramienta --- */
    printf(">> Recoger Antorcha (se agrega después de la selección actual):\n");
    insertar_en_carrusel(seleccion, ANTORCHA);
    printf("  Inventario (%d objetos):\n", contar_carrusel(seleccion));
    imprimir_inventario(seleccion);
    mostrar_carrusel(seleccion);
    printf("\n");

    /* --- Eliminar herramienta --- */
    printf(">> Usar Bomba (se elimina del inventario):\n");
    /* Navegar hasta la bomba */
    while (seleccion->dato != BOMBA)
        seleccion = avanzar(seleccion, 1);
    printf("  Eliminando: %s\n", nombre_herramienta(seleccion->dato));
    seleccion = eliminar_del_carrusel(seleccion);
    printf("  Nueva selección: %s\n", nombre_herramienta(seleccion->dato));
    printf("  Inventario (%d objetos):\n", contar_carrusel(seleccion));
    imprimir_inventario(seleccion);
    mostrar_carrusel(seleccion);
    printf("\n");

    /* --- Dar una vuelta completa --- */
    printf(">> Vuelta completa al carrusel:\n");
    int total = contar_carrusel(seleccion);
    dllista *recorrido = seleccion;
    printf("  ");
    for (int i = 0; i < total; i++) {
        printf("%s", nombre_herramienta(recorrido->dato));
        recorrido = avanzar(recorrido, 1);
        if (i < total - 1) printf(" -> ");
    }
    printf(" -> (vuelta)\n\n");

    printf(">> Vuelta completa en reversa:\n");
    recorrido = seleccion;
    printf("  ");
    for (int i = 0; i < total; i++) {
        printf("%s", nombre_herramienta(recorrido->dato));
        recorrido = retroceder(recorrido, 1);
        if (i < total - 1) printf(" -> ");
    }
    printf(" -> (vuelta)\n\n");

    /* --- Liberar memoria --- */
    printf(">> Vaciando inventario...\n");
    while (seleccion != NULL)
        seleccion = eliminar_del_carrusel(seleccion);
    printf("  Inventario final: %d objetos\n", contar_carrusel(seleccion));

    return 0;
}
