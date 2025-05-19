#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct{
   int sudo[9][9];
}Node;

Node* createNode(){
  Node* n=(Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n){
    Node* new=(Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file (char* file_name){
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  for(i=0;i<9;i++){
       for(j=0;j<9;j++){
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
       }
  }

  fclose (file);
  return n;
}

void print_node(Node* n){
    int i,j;
    for(i=0;i<9;i++){
       for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}

int is_valid(Node* n){

  // En las siguientes itreaciones confirmaremos las filas
  for(int i = 0; i < 9; i++){
    int numVisto[10];
    for(int cont = 0; cont < 10; cont++){
      numVisto[cont] = 0;
    }
    for(int j = 0; j < 9; j++){
      int num = n->sudo[i][j];
      if(num != 0){
        if(numVisto[num] == 1){
          return 0;
        }
        numVisto[num] = 1;
      }  
    }
  }

  // Las siguientes iteraciones confirmaremos las columnas

  for(int i = 0; i < 9; i++){
    int numVisto[10];
    for(int cont = 0; cont < 10; cont++){
      numVisto[cont] = 0;
    }
    for(int j = 0; j < 9; j++){
      int num = n->sudo[j][i];
      if(num != 0){
        if(numVisto[num] == 1){
          return 0;
        }
        numVisto[num] = 1;
      }  
    }
  }

  // En esta funcion vamos a validar las "submatrices" de 3x3
  // Marcaremos los valores que ya fueron vistos en esa submatriz 
  // obtenemos la celda luego ya nos saltamos esas celdas vacias 
  // y a ese valor le colocamos la marca de que ya fue visto
  for(int k = 0; k < 9; k++){
      int seen[10] = {0};
      for(int p = 0; p < 9; p++){
          int i = 3*(k/3) + p/3;
          int j = 3*(k%3) + p%3;
          int val = n->sudo[i][j];
          if(val == 0) continue;
          if(seen[val]) return 0;
          seen[val] = 1;
          
      }
  }  

  
  return 1;


}

// En esta funcion usamos 2 variables para guardar la poscion, recorremos filas
// y si encontramos un 0 vamos a guardar si la fila y columna

List* get_adj_nodes(Node* n){ 
  List* list = createList();
  int posX = -1;
  int posY = -1;
  for(int i = 0; i < 9; i++){
      for(int j = 0; j < 9; j++){
          if(n->sudo[i][j] == 0){
              posX = i;
              posY = j;
              break;
          }
      }
  }
  if(posX == -1){
      return list;
  }

  // Aqui vamos a llenar la casilla con valores del 1 al 9
  // Entonces creamos una copia del nodo y le insertamos un probable valor en esa casilla
  // Luego el nuevo nodo nos quedaria en la lista de los adyacentes
  for(int i = 1; i <= 9; i++){
      Node* newNode = copy(n);
      newNode->sudo[posX][posY] = i;
      if(is_valid(newNode)){
          pushBack(list, newNode);
      }else{
          free(newNode);
      }
  }

  return list;
}


int is_final(Node* n){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(n->sudo[i][j] == 0)
                return 0;
        }
    }
    return 1;
}

// Lo que haremois aqui es crear una pila para luego insertar el nodo inicial de la pila
// mientras que la pila no este vacia sacaremos el top de la pila, lo retiramos y seguimos con el siguiente
// Si el nodo es final entonces retornamos el nodo
// Si no es final entonces obtenemos los nodos adyacentes y los insertamos en la pila
Node* DFS(Node* initial, int* cont){
  Stack* stack = createStack();         
  push(stack, initial);                

  while(!is_empty(stack)){              
      Node* current = top(stack);       
      pop(stack);                       
      (*cont)++;                        

      if(is_final(current))            
          return current;              

      List* adj = get_adj_nodes(current); 
      Node* adjNode = first(adj);         

      while(adjNode != NULL){
          push(stack, adjNode);          
          adjNode = next(adj);           
        }

      free(adj);                          
      free(current);                      
    }

    return NULL; // Si no se encuentra solución
}

// int main( int argc, char *argv[] ){

//   Node* initial= read_file("s12a.txt");;

//   int cont=0;
//   Node* final = DFS(initial, &cont);
//   printf("iterations:%d\n",cont);
//   print_node(final);

//   return 0;
// }