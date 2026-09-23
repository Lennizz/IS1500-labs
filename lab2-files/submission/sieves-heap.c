#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/*
    this assignment was authored by Anton Lennström
*/

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

    char *arr = malloc(sizeof(char) * (n-1)); 

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

    free(arr);
}

// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]){
    if(argc == 2)
    {
        //time_t start_time, end_time;
        //start_time = time(NULL); 

        print_sieves(atoi(argv[1]));

        //end_time = time(NULL); 
        //double elapsed = difftime(end_time, start_time);
        
        //printf("Elapsed time: %.0000f seconds.\n", elapsed);
    }
  else
    printf("Please state an integer number.\n");
  return 0;
}


 
