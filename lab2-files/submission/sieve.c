#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define COLUMNS 6
int counter = 0;

void print_number(int n) {
    if (counter >= COLUMNS) {
      printf("\n");
      counter = 0;
    }
    
    printf("%10d ", n);
    counter++;
}

void print_sieves(int n){
    // Should print out all prime numbers less than 'n'
    // with the following formatting. Note that
    // the number of columns is stated in the define
    // COLUMNS

    char arr[n-1]; 

    for (int i = 0; i < n - 1; i++) {
        arr[i] = 'U';
    }

    int p = 2;
    while (true) {
        int multiple = 2;
        while ((p * multiple) <= n) {
            arr[(p * multiple) - 2] = 'C';
            multiple++;
        }

        int current_p = p;
        for (int i = (p + 1); i < (n - 1); i++) {
            if(arr[i - 2] == 'C') 
                continue;
            p = i;
            break;
        }

        if (p == current_p) {
            break;
        }
    }

    for (int i = 0; i < (n - 1); i++) {
        int value = arr[i];
        if (value == 'C')
            continue;

        print_number(i + 2);
    }
}

// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]){
    if(argc == 2)
    {
        print_sieves(atoi(argv[1]));
    }
  else
    printf("Please state an integer number.\n");
  return 0;
}


 
