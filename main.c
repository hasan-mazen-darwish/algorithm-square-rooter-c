#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

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

void printIntegerArray(int* array, size_t length, char* text) {
  int *lastElement = array + length - 1;
  printf("%s: <", text);
  for(int* p = array; p <= lastElement; p++) {
    if(p == lastElement) printf("%d", *p);
    else printf("%d,", *p);
  }
  printf(">\n");
}

// The structure of the parts: it holds pointers because the size of the arrays is unknown until the runtime.
struct Parts {
  unsigned int *wholePart;
  size_t wholePartSize;
  unsigned int *decimalPart;
  size_t decimalPartSize;
};

int main() {
  printf("Hello and welcome to the square rooter algorithm!\n");
  printf("An algorithm completely built with native C (refactored from my C++ original one). No external dependencues.\n");
  printf("-----------------------------\n\n");

  printf("Please enter the number you want to square root (maximum limit of 200 digits, including the dot):\n");
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
  size_t decimalIndex   = 0;
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
      else {
        inputIncludesDot = true;
        decimalIndex = (size_t)(p - input);
      }
    }
  }

  // Now we will truncate the zeroes in the beginning of the input:
  char *newInput = trimZeroes(input, &len, &inputIncludesDot);
  input = newInput;

  printf("\nGot it. The number you provided is %s.\n", input);
  printf("Now, please provide the number of digits you want to approximate to:\n");

  // Now we will handle digit input and sanitization:
  char buffer[100];
  char *endptr;
  unsigned int num = 0;

  if(fgets(buffer, sizeof(buffer), stdin) != NULL) {
    num = strtol(buffer, &endptr, 10);
    if(buffer == endptr) {
      printf("Invalid digits input!");
      return 1;
    }
    else if(*endptr != '\0' && *endptr != '\n') {
      printf("Invalid end of input!");
      return 1;
    }
  }

  // Now we will separate the number into parts. We will use our struct:
  struct Parts *parts = malloc(sizeof(struct Parts));
  parts->wholePart = calloc(0, sizeof(unsigned int));
  parts->wholePartSize = 0;
  parts->decimalPart = calloc(0, sizeof(unsigned int));
  parts->decimalPartSize = 0;

  size_t wholeLength = strlen(input);
  size_t decimalLength = 0;

  // If the input has a decimal point, then we will have to take care of it:
  if(inputIncludesDot) {
    wholeLength = decimalIndex;
    decimalLength = len - wholeLength - 1; // Subtracting 1 for the decimal (cannot count the . as a number)
  }

  if(wholeLength & 1) { // The wholeLength is odd, so we add 0 to the first of the input to make it even.
    len++;
    char *new_temp = realloc(input, len*sizeof(char));
    if(new_temp == NULL) {
      printf("Failed reallocating memory for the new leading 0 to the input.\n");
      return 1;
    }
    input = new_temp;
    
    memmove(input+1, input, len);
    input[0] = '0';
    wholeLength++;
    decimalIndex++;
  }

  if(((decimalLength & 1) == 1) && inputIncludesDot) { // The decimalLength is odd AND the decimal exists
    len++;
    input[len - 1] = '0';
    input[len] = '\0';
    decimalLength++;
  }

  // Now, we have the both the decimal length and the whole length even numbers.
  // We can process the parts easily.
  char *start;
  for(start = input; start < input + wholeLength; start+=2) {
    if(*start == '\0') break;
    parts->wholePartSize++;
    unsigned int pair = (((*start)-'0') * 10) + ((*(start + 1)) - '0');
    unsigned int *temp_pair = realloc(parts->wholePart, (parts->wholePartSize)*sizeof(unsigned int));
    if(temp_pair == NULL) {
      printf("Failed to reallocate a new memory location when reallocating for a new pair in the whole part.\n");
      return 1;
    }

    parts->wholePart = temp_pair;
    parts->wholePart[parts->wholePartSize - 1] = pair;
  }

  char *decimalStart;
  // Adding a check in the for loop to not calculate this part if the input has no decimal point
  for(decimalStart = input + decimalIndex + 1; inputIncludesDot && (decimalStart < input + strlen(input)); decimalStart+=2) {
    if(*decimalStart == '\0') break;
    parts->decimalPartSize++;
    unsigned int pair = (((*decimalStart)-'0') * 10) + ((*(decimalStart + 1)) - '0');
    unsigned int *temp_pair = realloc(parts->decimalPart, ((parts->decimalPartSize)+1)*sizeof(unsigned int));
    if(temp_pair == NULL) {
      printf("Failed to reallocate a new memory location when reallocating for a new pair in the decimal part.\n");
      return 1;
    }

    parts->decimalPart = temp_pair;
    parts->decimalPart[parts->decimalPartSize - 1] = pair;
  }

  if(parts->wholePartSize < 1) {
    parts->wholePartSize = 1;
    parts->wholePart = calloc(1, sizeof(unsigned int));
  }

  if(parts->decimalPartSize < 1) {
    parts->decimalPartSize = 1;
    parts->decimalPart = calloc(1, sizeof(unsigned int));
  }

  // Freeing the input because we don't need it anymore after dissecting it.
  free(input);



  free(parts->wholePart);
  free(parts->decimalPart);
  free(parts);
  return 0;
}
