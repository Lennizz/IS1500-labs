/*
 pointers.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/


#include <stdio.h>
#include <stdlib.h>

/*
  This assignemnt was authored by Axel Åhman
*/

char* text1 = "This is a string.";
char* text2 = "Yet another thing.";

int list1[20];
int list2[20];

int counter = 0; 

void printlist(const int* lst){
  printf("ASCII codes and corresponding characters.\n");
  while(*lst != 0){
    printf("0x%03X '%c' ", *lst, (char)*lst);
    lst++;
  }
  printf("\n");
}

void endian_proof(const char* c){
  printf("\nEndian experiment: 0x%02x,0x%02x,0x%02x,0x%02x\n", 
         (int)*c,(int)*(c+1), (int)*(c+2), (int)*(c+3));
  
}

void copycodes(char *a0, int *a1, int *a2){

  while(1){
    char t0 = *a0;
    if(t0 == 0)
      break;
    *a1 = (int) t0;

    a0++;
    a1++;

    int t1 = *a2;
    t1++;
    *a2 = t1;
  }
  return;
}

void work(){
  char *a0 = text1;
  int *a1 = list1;
  int *a2 = &counter;
  copycodes(a0, a1, a2);

  a0 = text2;
  a1 = list2;
  a2 = &counter;
  copycodes(a0, a1, a2);

  return;
}

int main(void){
 
    work();
    printf("\nlist1: ");
    printlist(list1);
    printf("\nlist2: ");
    printlist(list2);
    printf("\nCount = %d\n", counter);

    endian_proof((char*) &counter);
}
