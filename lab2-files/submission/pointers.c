/*
 pointers.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/


#include <stdio.h>
#include <stdlib.h>

char* text1 = "This is a string.";
char* text2 = "Yet another thing.";
int list1[80/sizeof(int)];
int list2[80/sizeof(int)];
int counter = 0;

void printlist(const char* lst){
  printf("ASCII codes and corresponding characters.\n");
  while(*lst != 0){
    printf("0x%03X '%c' ", (int)*lst, *lst);
    lst++;
  }
  printf("\n");
}

void endian_proof(const char* c){
  printf("\nEndian experiment: 0x%02x,0x%02x,0x%02x,0x%02x\n", 
         (int)*c,(int)*(c+1), (int)*(c+2), (int)*(c+3));
  
}

void copycodes(char *a0, int *a1, int *a2) {
  while ((int) *a0 != 0){
    *a1 = (int) *a0;
   
    a0++;
    a1++;

    (*a2)++;
  }  
}

void work() {
  char *a0 = text1;
  int *a1 = list1;
  int *a2 = &counter;
  copycodes(a0, a1, a2);

  a0 = text2;
  a1 = list2;
  a2 = &counter;
  copycodes(a0, a1, a2);

}

int main(void){
  work();
  printf("\nlist1: ");
  printlist(text1);
  printf("\nlist2: ");
  printlist(text2);
  printf("\nCount = %d\n", counter);

  endian_proof((char*) &counter);
}
