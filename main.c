/*
* Karim El Kholy
* 0001041761
* Classe A
* karim.elkholy@studio.unibo.it
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int costoFisso;
int moltiplicatoreCosto;
int heapSize;
int arraySize;
int arrayCoordinate[8];
int nRighe;
int nColonne;

struct nodo {
        struct nodo * p;
        int d;
        int h;
        int i;
        int j;
};
typedef struct nodo nodo;

nodo visitati[];
int contaVisitati = 0;
nodo daVisitare[];

void initialization();
void dijkstra(nodo* graph);
void vicini(int i, int j);
nodo heapExtractMin(nodo* heap);
void minHeapify(nodo* heap, int i);
void buildMinHeap(nodo* array);
int weight(nodo n1, nodo n2);
void relax(nodo n1, nodo n2);
int parent(int i);
int left(int i);
int right(int i);

int main(int argc, char *argv[])
{
	int k;
    int i;
    int j;

    nodo **graph;

	FILE* file;

	file = fopen(argv[1], "r");
	if (file == NULL) {
		printf("Errore nell'aprire il file di input.\n");
		return 1;
	}

	fscanf(file, "%d %d %d %d", &costoFisso, &moltiplicatoreCosto, &nRighe, &nColonne);


        graph = (nodo**)malloc(nRighe * sizeof(nodo*));
        for(i = 0; i < nRighe; i++){
            graph[i] = (nodo*)malloc(nColonne * sizeof(nodo));
        }

        k = 0;

        for(i = 0; i < nRighe; i++){
            for (j = 0; j < nColonne; j++){
                daVisitare[k] = graph[i][j];
                graph[i][j].p = NULL;
                graph[i][j].d = INT_MAX;
                graph[i][j].i = i;
                graph[i][j].j = j;
                k++;
            }
        }
        graph[0][0].d = 0;

    /*Heap e array*/

    visitati = (nodo*)malloc(nRighe*nColonne *sizeof(nodo));
    daVisitare = (nodo*)malloc(nRighe*nColonne *sizeof(nodo));
    arraySize = nRighe * nColonne;
    heapSize = 0;

	fclose(file);
	return 0;
}
void dijkstra(nodo* graph){
    int i = 0;
    nodo minVertex;
    while(heapSize != 0){
        minVertex = heapExtractMin(graph);
        visitati[contaVisitati] = minVertex;
        vicini(minVertex.i, minVertex.j);
        do{
            relax(minVertex, graph[arrayCoordinate[i]][arrayCoordinate[i+1]]);
            i += 2;
        }
        while(arrayCoordinate[i] =! -1);
    }
}
void vicini(int i, int j){
    int direzioni[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    int d;
    int nj;
    int ni;
    int k;

    k = 0;
    arrayCoordinate = {-1, -1, -1, -1, -1, -1, -1, -1};

    for (d = 0; d < 4; d++) {
        ni = i + direzioni[d][0]; // coordinata i del vicino
        nj = j + direzioni[d][1]; // coordinata j del vicino

        // Verifichiamo se il vicino è all'interno dei limiti della matrice
        if (ni >= 0 && ni < nRighe && nj >= 0 && nj < nColonne) {
            arrayCoordinate[k] = ni;
            arrayCoordinate[k + 1] = nj;
            k += 2;
        }
    }
}

nodo heapExtractMin(nodo* heap){
    nodo min;
    if(heapSize < 1)
        printf("Errore: Heap vuoto");
        return NULL;
    min = heap[1];
    heap[1] = heap[heapSize];
    heapSize--;
    minHeapify(heap, 1);
    return min;
}

void minHeapify(nodo* heap, int i){
    int l;
    int r;
    int smallest;
    nodo temp;

    l = left(i);
    r = right(i);
    if(l <= heapSize && heap[l].d < heap[i].d){
        smallest = l;
    }
    else {
        smallest = i;
    }
    if( i<= heapSize && heap[r].d < heap[smallest].d){
        smallest = r;
    }
    if (smallest =! i){
        temp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = temp;

        minHeapify( heap, smallest);
    }
}

void buildMinHeap(nodo* array){
    int i;
    heapSize = arraySize;
    for(i = arraySize/2; i<= 1; i--){
        minHeapify(array, i);
    }
}

int parent(int i){
   return i/2;
}

int left(int i){
    return 2*i;
}

int right (int i){
    return 2*i+1;
}

int weight(nodo n1, nodo n2){
    int w;
    w = (n1.h - n2.h);
    return w*w*moltiplicatoreCosto + costoFisso;
}

void relax(nodo n1, nodo n2){
    if (n2.d > n1.d + weight(n1, n2)) {
        n2.d = n1.d + weight(n1, n2);
        n2.p = n1;
    }
}
