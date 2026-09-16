/*
 print-primes.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/


#include <stdio.h>
#include <stdlib.h>

/*
  This assignment was authored by Anton Lennström
*/

#define COLUMNS 6
int counter = 0;

int is_prime(int n){

  if(n <= 1)
    return 0;
  
  int r = 2;

  while (r*r <= n) {
    if (n % r == 0)
      return 0; 
    r++;
  }

  return 1;
}

void print_number(int n) {
    if (counter == COLUMNS) {
      printf("\n");
      counter = 0;
    }
    
    printf("%10d ", n);
    counter++;
}

void print_primes(int n){
    // Should print out all prime numbers less than 'n'
    // with the following formatting. Note that
    // the number of columns is stated in the define
    // COLUMNS

    for (int i = 2; i < n; i++){
      if (is_prime(i))
        print_number(i);
    }
}

// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]){
    if(argc == 2)
    {
        print_primes(atoi(argv[1]));
    }
  else
    printf("Please state an integer number.\n");
  return 0;
}


 
