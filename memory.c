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

node *head;

void mitosis(char name, int size, node* prevFit, node* fit) {
  // printf("---mitosis---\n");
  // fflush(stdout);
  if (fit == NULL) return;
  if (size == fit->size) {
    //size is equal, no mitosis, just replace contents
    fit->name = name;
  } else if (prevFit == NULL) {
    //size is less than spot, mitosis and reduce mem remaining
    head->size -= size;
    node *alloced = (node *) malloc(sizeof(node));
    alloced->name = name;
    alloced->size = size;
    alloced->next = head;
    head = alloced;
  } else {
    fit->size -= size;
    node *alloced = (node *) malloc(sizeof(node));
    alloced->name = name;
    alloced->size = size;
    alloced->next = fit;
    prevFit->next = alloced;
  }
}

void worstFit(char name, int size) {
  node *prevFit = NULL;
  node *fit = NULL;
  node *prev = NULL;
  node *curr = head;
  //find worst fit
  while (curr != NULL) {
    if (curr->name == '.') {
      //if first fit we're finding or worser fit
      if ((fit == NULL && curr->size >= size) || (fit != NULL && curr->size > fit->size)) {
        prevFit = prev;
        fit = curr;
      }
    }
    prev = curr;
    curr = curr->next;
  }
  mitosis(name, size, prevFit, fit);
}

void bestFit(char name, int size) {
  node *prevFit = NULL;
  node *fit = NULL;
  node *prev = NULL;
  node *curr = head;
  //find best fit
  while (curr != NULL) {
    if (curr->name == '.') {
      //if first fit we're finding or smaller fit
      if ((fit == NULL || curr->size < fit->size) && curr->size >= size) {
        prevFit = prev;
        fit = curr;
      }
    }
    prev = curr;
    curr = curr->next;
  }
  mitosis(name, size, prevFit, fit);
}

void firstFit(char name, int size) {
  // printf("---firstFit---\n");
  // fflush(stdout);
  node *prevFit = NULL;
  node *fit = NULL;
  node *prev = NULL;
  node *curr = head;
  //find best fit
  while (curr != NULL) {
    if (curr->name == '.') {
      //if first fit we're finding or smaller fit
      if (curr->size >= size) {
        prevFit = prev;
        fit = curr;
        break;
      }
    }
    prev = curr;
    curr = curr->next;
  }
  mitosis(name, size, prevFit, fit);
}

void allocateMem(char name, int size, char algo) {
  // printf("---allocateMem---\n");
  // fflush(stdout);
  switch (algo) {
    case 'F' :
      firstFit(name, size);
      break;
    case 'B' :
      bestFit(name, size);
      break;
    case 'W' :
      worstFit(name, size);
      break;
  }
}

void rightFusion(node* rmv) {
  // printf("---rightFusion---\n");
  // fflush(stdout);
  if (rmv->next != NULL && rmv->next->name == '.') {
    rmv->size += rmv->next->size;
    node *dlt = rmv->next;
    rmv->next = dlt->next;
    dlt->next = NULL;
    free(dlt);
  }
}

bool leftFusion(node *prevRmv, node **rmv) {
  // printf("---leftFusion---\n");
  // fflush(stdout);
  if (prevRmv->name == '.') {
    prevRmv->size += (*rmv)->size;
    prevRmv->next = (*rmv)->next;
    (*rmv)->next = NULL;
    free((*rmv));
    *rmv = prevRmv;
    return true;
  }
  return false;
}

void freeMem(char name) {
  // printf("---freeMem---\n");
  // fflush(stdout);
  //remove pointers
  node *prevPrevRmv = NULL;
  node *prevRmv = NULL;
  node *rmv = NULL;
  //traverse pointers
  node *prevPrev = NULL;
  node *prev = NULL;
  node *curr = head;
  //remove all occurences
  bool removing = true;
  while (removing) {
    removing = false;
    //find to remove
    while (curr != NULL) {
        if (curr->name == name) {
        prevPrevRmv = prevRmv;
        prevRmv = prev;
        rmv = curr;
        removing = true;
        break;
      }
      prevPrev = prev;
      prev = curr;
      curr = curr->next;
    }
    //removing
    if (rmv != NULL) {
      bool leftRemoved = false;
      if (prevRmv != NULL) leftRemoved = leftFusion(prevRmv, &rmv);
      if (rmv->next != NULL) rightFusion(rmv);
      rmv->name = '.';
      if (!leftRemoved) {
        prevPrev = prev;
        prev = curr;
      }
      curr = rmv->next;
    }
  }
}

void compactMem() {
  int empty = 0;
  node *prev = NULL;
  node *curr = head;
  while (curr != NULL) {
    if (curr->name == '.') {
      if (curr->next == NULL) {
        curr->size += empty;
        return;
      }
      empty += curr->size;
      prev->next = curr->next;
      curr->next = NULL;
      free(curr);
      curr = prev->next;
    } else {
      prev = curr;
      curr = curr->next;
    }
  }
  if (empty > 0) {
    node* compacted = (node *) malloc(sizeof(node));
    compacted->name = '.';
    compacted->size = empty;
    compacted->next = NULL;
    prev->next = compacted;
  }
}

void showMem() {
  node *curr = head;
  while (curr != NULL) {
    for (int i = 0; i < curr->size; i++) {
      printf("%c", curr->name);
    }
    curr = curr->next;
  }
  printf("\n");
}

void readFile(char *file);

bool processRequest(char *request) {
  // printf("---processRequest---\n");
  // fflush(stdout);
  static char *save;
  char *args;
  char *arg;
  char *file;
  char req;
  char name;
  char algo;
  int size;
  args = strtok_r(request, " ", &save); 
  req = *args;
  switch(req) {
    case 'A' :
      arg = strtok_r(NULL, " ", &save);
      name = *arg;
      size = atoi(strtok_r(NULL, " ", &save));
      arg = strtok_r(NULL, " ", &save);
      algo = *arg;
      allocateMem(name, size, algo);
      break;
    case 'F' : 
      arg = strtok_r(NULL, " ", &save);
      name = *arg;
      freeMem(name);
      break;
    case 'S' : 
      showMem();
      break;
    case 'R' : 
      file = strtok_r(NULL, " ", &save);
      readFile(file);
      break;
    case 'C' : 
      compactMem();
      break;
    default : 
      return false;
  }
  return true;
}

void readFile(char *file) {

}

void processInput() {
  // printf("---processInput---\n");
  // fflush(stdout);
  size_t inLen = 0;
  char *input = NULL;
  bool running = true;
  while(running) {
    printf(">");
    fflush(stdout);
    getline(&input, &inLen, stdin);
    running = processRequest(input);
  }
}


int main(int argc, char **argv) {
  head = (node *) malloc(sizeof(node));
  head->name = '.';
  head->size = MEMSIZE;
  head->next = NULL;
  // printf("---main---\n");
  // fflush(stdout);
  processInput();
}