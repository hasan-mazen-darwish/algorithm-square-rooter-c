#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// This function removes the first element from an array. For example:
// {1,2,3,4} -> {2,3,4}
// and it also modifies the length of the original array. That's why the length parameter is used
void* popFirst(char *input, size_t *length) {
  if(*length <= 1) {
    char *empty = malloc(1);
    if(empty) empty[0] = '\0';
    input = empty;
    *length = 0;
    return empty;
  }

  size_t outputLength = *length;
  char *output = malloc(outputLength * sizeof(char));

  for(size_t i=1; i<outputLength; i++)
    output[i-1] = input[i];
  
  output[outputLength - 1] = '\0';
  (*length)--;
  return output;
}

// This function removes the last element from an array. For example:
// {1,2,3,4} -> {1,2,3}
// and it also modifies the length of the original array. That's why the length parameter is used
void* popLast(char *input, size_t *length) {
  if(*length <= 1) {
    char *empty = malloc(1);
    if(empty) empty[0] = '\0';
    input = empty;
    *length = 0;
    return empty;
  }

  size_t outputLength = *length;
  char *output = malloc(outputLength * sizeof(char));

  for(size_t i=0; i<outputLength; i++)
    output[i] = input[i];
  
  output[outputLength - 1] = '\0';
  (*length)--;
  return output;
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

  p = input;
  // Now we will truncate the zeroes in the beginning of the input:
  while(len > 1 && *p == '0') {
    char *newInput = popFirst(input, &len);
    free(input);
    input = newInput;
    p = newInput;
  }

  len = strlen(input);
  p = input + len - 1;
  // And then the zeroes in the last of the input:
  while(len > 1 && *p == '0' && inputIncludesDot) {
    char *newInput = popLast(input, &len);
    free(input);
    input = newInput;
    p = newInput + len - 1;
  }
  
  // Finally just trim the final dot from the input if any:
  if(input[strlen(input) - 1] == '.') input[strlen(input) - 1] = '\0';

  printf("Here is the input: %s\n", input);

  // DON'T FORGET TO FREE THE INPUT

  return 0;
}
