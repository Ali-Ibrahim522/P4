/*
  created by Ali Ibrahim

  Description:
   - program to emulate the ways memory can be allocated with the best, worst, and first fit approaches; compression, freeing, and displaying memory contents

  Design:
   - Linked list approach that starts with one node repersenting the whole, 80-sized memory block

   - allocation: will look for nodes repersenting empty memory (i.e. '.') and check their sizes to see if they fit
     and/or are a better fit based on the approach used. Once a "empty memory" node is chosen, the node will split
     into a node with the allocated name and size and another node holding the leftover contents of the memory node.

   - freeing: nodes will find memory nodes matching the name passed in and will set their name to '.', meaning its now free memory.
     Neighboring nodes to those freed, if also "empty", must be fused together with the newly freed memory.

   - compacting: deletes all "empty" nodes from the linked list while keeping a total of their combined space. Finally
     creates a new node at the end of list holding "empty" memory, equal to the combined size found.

  How to run:
   - gcc memory.c
   - ./a.out
*/

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

//splits a empty node into the allocated node and a leftover empty mem node
void mitosis(char name, int size, node* prevFit, node* fit) {
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

// allocating mempry using the worst fit algo
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

// allocating memory using the bets fit algo
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

// allocating memory using the first fit algo
void firstFit(char name, int size) {
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

// allocates memory given an algo and the alloc details
void allocateMem(char name, int size, char algo) {
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

//fuses right neighboring empty nodes
void rightFusion(node* rmv) {
  if (rmv->next != NULL && rmv->next->name == '.') {
    rmv->size += rmv->next->size;
    node *dlt = rmv->next;
    rmv->next = dlt->next;
    dlt->next = NULL;
    free(dlt);
  }
}

// fuses left neighboring empty nodes
bool leftFusion(node *prevRmv, node **rmv) {
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

// finds all memeory nodes with the passed in name and
// sets them to be empty, fuses neighoring empty nodes.
void freeMem(char name) {
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

// compacts all the memory, taking all empty memory and placing at
// end of the list
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

// prints out all the memory content
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

// processes a request, redirects program to corrosponding request method
bool processRequest(char *request) {
  char *del = " \n";
  static char *save;
  char *args;
  char *arg;
  char *file;
  char req;
  char name;
  char algo;
  int size;
  args = strtok_r(request, del, &save); 
  if (args == NULL) return true;
  req = *args;
  switch(req) {
    // allocation
    case 'A' :
      arg = strtok_r(NULL, del, &save);
      name = *arg;
      size = atoi(strtok_r(NULL, del, &save));
      arg = strtok_r(NULL, del, &save);
      algo = *arg;
      allocateMem(name, size, algo);
      break;
    // free
    case 'F' : 
      arg = strtok_r(NULL, del, &save);
      name = *arg;
      freeMem(name);
      break;
    // show
    case 'S' : 
      showMem();
      break;
    // read file
    case 'R' : 
      file = strtok_r(NULL, del, &save);
      readFile(file);
      break;
    // compression
    case 'C' : 
      compactMem();
      break;
    // exit
    case 'E': 
      return false;
  }
  return true;
}

// reads each request from a file and processes it
void readFile(char *file) {
  FILE *fp;
  char *req = NULL;
  size_t reqLen = 0;
  fp = fopen(file, "r");
  
  if (fp == NULL) exit(EXIT_FAILURE);
  while (getline(&req, &reqLen, fp) != -1) {
      processRequest(req);
  }
  fclose(fp);
}

// prompts user for requests and processes each one
void processInput() {
  size_t inLen = 0;
  char *input = NULL;
  bool running = true;
  while(running) {
    getline(&input, &inLen, stdin);
    running = processRequest(input);
  }
}

// starts program and starts command prompt input process
int main(int argc, char **argv) {
  head = (node *) malloc(sizeof(node));
  head->name = '.';
  head->size = MEMSIZE;
  head->next = NULL;
  processInput();
}