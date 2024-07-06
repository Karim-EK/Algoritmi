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

struct nodo {
        struct nodo * p;
        int d;
        int h;
};
typedef struct nodo nodo;


void initialization();
int weight(nodo * n1, nodo * n2);
void relax(nodo * n1, nodo * n2);

int main(int argc, char *argv[])
{
	int nRighe;
	int nColonne;
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

        for(i = 0; i < nRighe; i++){
            for (j = 0; j < nColonne; j++){
                graph[i][j].p = NULL;
                graph[i][j].d = INT_MAX;
            }
        }
        graph[0][0].d = 0;


	fclose(file);
	return 0;
}

int weight(nodo * n1, nodo * n2){
    int w;
    w = (n1->h - n2->h);
    return w*w*moltiplicatoreCosto;
}

void relax(nodo * n1, nodo * n2){
    if (n2->d > n1->d + weight(n1, n2)) {
        n2->d = n1->d + weight(n1, n2);
        n2->p = n1;
    }
}
