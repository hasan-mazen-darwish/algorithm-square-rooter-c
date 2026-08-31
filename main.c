#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

char* trimZeroes(char *input, size_t *length, bool *hasDot) {
  // This code will check all the digits starting from the zero. Since the *start is zero, then the loop
  // will continue. And it will shift the pointer of the input to the right (trim the first digit). If
  // the next digit is not 0, the loop will stop.
  char *start = input;
  size_t len = *length;
  while (len > 1 && *start == '0') {
    start++;
    len--;
  }

  // Doing the exact same for the zeroes on the right: if we have a zero on the very right and there is a
  // decimal, we will be shifting the end pointer to the left.
  // Also, we will remove the last decimal too if it was in the end.
  char *end = input + *length - 1;
  if(*hasDot) {
    while(len > 1 && (*end == '0' || *end == '.') && *hasDot) {
      if(*end == '.') *hasDot = false;
      *end = '\0';
      end--;
      len--;
    }
  }

  *length = len;
  memmove(input, start, *length);
  input[*length] = '\0';
  return input;
}

int main() {
  printf("Hello and welcome to the square rooter algorithm!\n");
  printf("An algorithm completely built with native C (refactored from my C++ original one). No external dependencues.\n");
  printf("-----------------------------\n\n");

  printf("Please enter the number you want to square root (maximum limit of 200 digits, including the dot): ");
  char *input = malloc(200 * sizeof(char));
  if(input == NULL) {
    printf("An error during allocating memory for the input!\n");
    return 1;
  }
  fgets(input, 200 * sizeof(char), stdin);
  
  // Now we will be checking if the input is valid or not. Also, we will be reallocating the memory as well to fit the exact
  // size of the input buffer
  size_t len = strlen(input);
  if(input[len-1] == '\n') {
    input[len-1] = '\0';
    --len;
  }
  char *temp = realloc(input, len * sizeof(char));
  if(temp == NULL) {
    printf("An error during reallocating memory for the input!\n");
    return 1;
  }
  input = temp;

  char *p;
  bool inputIncludesDot = false;
  for(p = input; p<input+len; p++) {
    if(
        *p != '0' &&
        *p != '1' &&
        *p != '2' &&
        *p != '3' &&
        *p != '4' &&
        *p != '5' &&
        *p != '6' &&
        *p != '7' &&
        *p != '8' &&
        *p != '9' &&
        *p != '.'
      ) {
      printf("Error! The provided number is not a number. Please provide a real number!\n");
      return 1;
    }

    if(*p == '.') {
      if(inputIncludesDot) {
        printf("Error! The provided number has many dots. Please provide a real number!\n");
        return 1;
      }
      else inputIncludesDot = true;
    }
  }

  // Now we will truncate the zeroes in the beginning of the input:
  char *newInput = trimZeroes(input, &len, &inputIncludesDot);
  input = newInput;

  printf("Here is the input: %s\n", input);

  free(input);
  return 0;
}
