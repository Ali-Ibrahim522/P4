//imports
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//globals
#define MEMSIZE 80
struct node {
  char name;
  int size;
  struct node *next;
};
typedef struct node node;

node* head;

void allocateMem(char name, int size, char algo) {

}

void freeMem(char name) {

}

void compactMem() {

}

void showMem() {

}

void readFile(char *file) {

}

void processInput() {
  size_t inLen = 0;
  char *input = NULL;
  char *args = NULL;
  static char *save;
  bool running = true;
  char req;
  while(running) {
    getline(&input, &inLen, stdin);
    args = strtok_r(input, " ", &save); 
    req = *args;
    switch(req) {
      case 'A' :

      case 'F' : 
      case 'S' : 
      case 'R' : 
      case 'C' : 
      default : 
    }
  }
}


int main(int argc, char **argv) {
  head = (node *) malloc(sizeof(node));
  head->name = '.';
  head->size = MEMSIZE;
  head->next = NULL;
  processInput();
}