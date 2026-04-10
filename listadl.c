#include <stdio.h>
#include <stdlib.h>
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
        // Único elemento, se apunta a sí mismo
        nuevo->siguiente = nuevo;
        nuevo->previo = nuevo;
        lista->cabeza = nuevo;
    } else {
        dllista *ultimo = lista->cabeza->previo; // El último siempre es cabeza->previo
        
        nuevo->siguiente = lista->cabeza;
        nuevo->previo = ultimo;
        
        lista->cabeza->previo = nuevo;
        ultimo->siguiente = nuevo;
        
        lista->cabeza = nuevo;
    }
    lista->longitud++;
}

void insertar_final(ListaDL *lista, DATO dato) {
    dllista *nuevo = crear_elemento(dato);
    if (nuevo == NULL)
        return;

    if (lista->cabeza == NULL) {
        nuevo->siguiente = nuevo;
        nuevo->previo = nuevo;
        lista->cabeza = nuevo;
    } else {
        dllista *ultimo = lista->cabeza->previo;
        
        nuevo->siguiente = lista->cabeza;
        nuevo->previo = ultimo;
        
        ultimo->siguiente = nuevo;
        lista->cabeza->previo = nuevo;
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

    if (lista->longitud == 1) {
        lista->cabeza = NULL;
    } else {
        dllista *ultimo = lista->cabeza->previo;
        lista->cabeza = lista->cabeza->siguiente;
        
        lista->cabeza->previo = ultimo;
        ultimo->siguiente = lista->cabeza;
    }

    free(eliminado);
    lista->longitud--;
    return dato;
}

DATO eliminar_final(ListaDL *lista) {
    if (lista->cabeza == NULL)
        return -1;

    if (lista->longitud == 1) {
        return eliminar_inicio(lista);
    }

    dllista *ultimo = lista->cabeza->previo;
    DATO dato = ultimo->dato;

    ultimo->previo->siguiente = lista->cabeza;
    lista->cabeza->previo = ultimo->previo;

    free(ultimo);
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
    if (lista->cabeza == NULL)
        return -1;

    dllista *actual = lista->cabeza;
    int posicion = 0;
    
    do {
        if (actual->dato == dato)
            return posicion;
        actual = actual->siguiente;
        posicion++;
    } while (actual != lista->cabeza);
    
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
    if (lista->cabeza == NULL) {
        printf("Lista vacia -> NULL\n");
        return;
    }
    
    dllista *actual = lista->cabeza;
    do {
        printf("[%d]", actual->dato);
        printf(" <-> ");
        actual = actual->siguiente;
    } while (actual != lista->cabeza);
    
    printf("(vuelve a la cabeza)\n");
}

void imprimir_lista_reversa(ListaDL *lista) {
    if (lista->cabeza == NULL) {
        printf("Lista vacia -> NULL\n");
        return;
    }
    
    dllista *actual = lista->cabeza->previo; // Empezamos por el final
    do {
        printf("[%d]", actual->dato);
        printf(" <-> ");
        actual = actual->previo;
    } while (actual != lista->cabeza->previo); // Terminamos al dar toda la vuelta
    
    printf("(vuelve al final)\n");
}

void liberar_lista(ListaDL *lista) {
    if (lista->cabeza != NULL) {
        dllista *actual = lista->cabeza;
        
        // Usamos la longitud para asegurarnos de liberar exactamente N nodos
        for (int i = 0; i < lista->longitud; i++) {
            dllista *siguiente = actual->siguiente;
            free(actual);
            actual = siguiente;
        }
    }
    free(lista);
}

int main() {
    printf(" PRUEBA DE LISTA DOBLEMENTE ENLAZADA CIRCULAR \n\n");

    ListaDL *mi_lista = crear_lista();
    if (mi_lista == NULL) {
        printf("Error al crear la lista en memoria.\n");
        return 1;
    }
    printf("Lista creada exitosamente. Longitud: %d\n\n", longitud(mi_lista));

    printf("- Insertando elementos -\n");
    insertar_inicio(mi_lista, 10);
    insertar_final(mi_lista, 20);
    insertar_final(mi_lista, 30);
    insertar_inicio(mi_lista, 5);
    
    
    insertar_en_posicion(mi_lista, 15, 2); 
   
    printf("\nRecorrido normal (Hacia adelante):\n");
    imprimir_lista(mi_lista);

    printf("\nRecorrido inverso (Hacia atras):\n");
    imprimir_lista_reversa(mi_lista);

    printf("\n--- Buscando elementos ---\n");
    int valor_a_buscar = 15;
    int posicion = buscar(mi_lista, valor_a_buscar);
    if (posicion != -1) {
        printf("El valor %d se encuentra en la posicion (indice): %d\n", valor_a_buscar, posicion);
    } else {
        printf("El valor %d no se encontro en la lista.\n", valor_a_buscar);
    }

    printf("El elemento en la posicion 3 es: %d\n", obtener(mi_lista, 3));

    printf("\n--- Eliminando elementos ---\n");
    printf("Se elimino del inicio: %d\n", eliminar_inicio(mi_lista)); 
    printf("Se elimino del final: %d\n", eliminar_final(mi_lista));   
    printf("Se elimino de la posicion 1: %d\n", eliminar_en_posicion(mi_lista, 1)); 

    printf("\nEstado de la lista despues de las eliminaciones:\n");
    imprimir_lista(mi_lista);
    printf("Longitud final: %d\n", longitud(mi_lista));

    printf("\nLiberando la memoria de la lista...\n");
    liberar_lista(mi_lista);
    printf("Memoria liberada. Programa finalizado.\n");

    return 0;
}
