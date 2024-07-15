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
int arrayCoordinate[9];
int nRighe;
int nColonne;

struct nodo {
        struct nodo * p;
        long int d;
        int h;
        int i;
        int j;
};
typedef struct nodo nodo;

nodo *visitati;
int contaVisitati = 0;
nodo *daVisitare;

void dijkstra(nodo** graph);
void vicini(int i, int j);
nodo heapExtractMin(nodo* heap);
void minHeapify(nodo* heap, int i);
void heap_decrease(nodo* heap, int i, int d);
void buildMinHeap(nodo* array);
int weight(nodo n1, nodo n2);
void relax(nodo *n1, nodo *n2, nodo ** graph);
int parent(int i);
int left(int i);
int right(int i);
int isVisited(nodo* visitati, int i, int j);
int findIndex(nodo* heap, int i, int j);
void printPath(nodo* end);

int main(int argc, char *argv[])
{
	int k;
    int i;
    int j;

    nodo empty;

    nodo end;

    nodo **graph;

	FILE* file;

	empty.d = -1;
	empty.h = -1;
	empty.i = -1;
	empty.j = -1;
	empty.p = NULL;

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

    visitati = (nodo*)calloc(nRighe*nColonne, sizeof(nodo));
    daVisitare = (nodo*)calloc((nRighe*nColonne + 1), sizeof(nodo));
    daVisitare[0] = empty;
    arraySize = nRighe * nColonne;
    heapSize = 0;

    for (i = 0; i < 9; i++) {
    arrayCoordinate[i] = -1;
    }

    k = 1;

    for(i = 0; i < nRighe; i++){
        for (j = 0; j < nColonne; j++){
            fscanf(file, "%d", &graph[i][j].h);
            graph[i][j].p = NULL;
            if(i == 0 && j == 0){
                graph[i][j].d = 0;
            }
            else {
                graph[i][j].d = INT_MAX;
            }
            graph[i][j].i = i;
            graph[i][j].j = j;
            daVisitare[k] = graph[i][j];
            k++;
        }
    }

    buildMinHeap(daVisitare);

    dijkstra(graph);

    end = graph[nRighe-1][nColonne-1];

    printPath(&end);
    printf("-1 -1\n");
    printf("%ld",end.d+costoFisso);

    for (i = 0; i < nRighe; i++) {
        free(graph[i]);
    }

    free(graph);
    free(daVisitare);
    free(visitati);
	fclose(file);
	return 0;
}
void dijkstra(nodo** graph){

    int index;
    nodo minVertex;
    while(heapSize > 1){
        int i = 0;
        minVertex = heapExtractMin(daVisitare);
        if (contaVisitati < nRighe * nColonne) {
            visitati[contaVisitati] = minVertex;
            contaVisitati ++;
        }
        vicini(minVertex.i, minVertex.j);
        do{
            if (i + 1 >= 9) break;
            if(isVisited(visitati, arrayCoordinate[i], arrayCoordinate[i+1]) == 0){
                relax(&minVertex, &graph[arrayCoordinate[i]][arrayCoordinate[i+1]], graph);
                index = findIndex(daVisitare,arrayCoordinate[i], arrayCoordinate[i+1]);
                if (index != -1){
                    daVisitare[index].d = graph[arrayCoordinate[i]][arrayCoordinate[i+1]].d;
                    heap_decrease(daVisitare, index, daVisitare[index].d);
                }
            }
            i += 2;
        }
        while((arrayCoordinate[i] != -1));
    }
}
void vicini(int i, int j){

    int direzioni[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    int d;
    int nj;
    int ni;
    int k;


    for(k=0; k<=8; k++){
        arrayCoordinate[k] = -1;
    }

    k = 0;
    for (d = 0; d < 4; d++) {
        ni = i + direzioni[d][0];
        nj = j + direzioni[d][1];


        if (ni >= 0 && ni < nRighe && nj >= 0 && nj < nColonne) {
            arrayCoordinate[k] = ni;
            arrayCoordinate[k + 1] = nj;
            k += 2;
        }
    }
}
nodo heapExtractMin(nodo* heap){
    nodo min;
    min = heap[1];
    heap[1] = heap[heapSize-1];
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
    if(l < heapSize && heap[l].d < heap[i].d){
        smallest = l;
    }
    else {
        smallest = i;
    }
    if(r < heapSize && heap[r].d < heap[smallest].d){
        smallest = r;
    }
    if (smallest != i){
        temp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = temp;

        minHeapify( heap, smallest);
    }
}

void buildMinHeap(nodo* array){
    int i;
    heapSize = arraySize + 1;
    for(i = arraySize/2; i> 1; i--){
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
    int dislivello;
    dislivello = (n1.h - n2.h);
    return dislivello * dislivello * moltiplicatoreCosto;
}

void relax(nodo* n1, nodo* n2, nodo** graph){
    long int newCost;
    newCost = n1->d + weight(*n1, *n2) + costoFisso;
    if (n2->d > newCost) {
        n2->d = newCost;
        n2->p = &graph[n1->i][n1->j];
    }
}

void heap_decrease(nodo* heap, int i, int d){
    nodo temp;
    if(d > heap[i].d){
        printf("Errore: nuovo valore maggiore di corrente");
    }
    while(i>1 && heap[parent(i)].d > heap[i].d){
        temp = heap[i];
        heap[i] = heap[parent(i)];
        heap[parent(i)] = temp;
        i = parent(i);
    }
}

int isVisited(nodo* visitati, int i, int j){
    int k;
    for (k=0; k<= contaVisitati; k++){
        if(visitati[k].i == i && visitati[k].j == j){
            return 1;
        }
    }
    return 0;
}

int findIndex(nodo* heap, int i, int j){
    int k;
    if (visitati == NULL || contaVisitati < 0) return 0;
    for(k=1; k<= heapSize; k++){
        if(heap[k].i == i && heap[k].j == j){
            return k;
        }
    }
    return -1;
}

void printPath(nodo* end) {
    if (end == NULL) {
        return;
    }
    printPath(end->p);
    printf("%d %d\n", end->i, end->j);
}
