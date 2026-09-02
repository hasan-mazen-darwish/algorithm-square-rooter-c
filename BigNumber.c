#include "BigNumber.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define unlikely(x) __builtin_expect(!!(x), 0)

struct BigNumber {
  uint64_t *digits;
  size_t digitsNumber;
};

// ====== Value attaching functions ======

void bn_zero(BigNumber* target) {
  uint64_t* allocation = calloc(1, sizeof(uint64_t));
  if(allocation == NULL) {
    printf("Failed to allocate memory for a new zero valued big number.\n");
    return;
  }
  free(target->digits);
  target->digits = allocation;
  target->digitsNumber = 1;
}

// ====== Comparsions ======

bool bn_is_zero(const BigNumber* number) {
  if((number->digitsNumber == 1 && number->digits[0] == 0) || number->digitsNumber == 0) return true;
  for(uint64_t* p = number->digits; p < number->digits + number->digitsNumber; p++) {
    if(*p > 0) return false;
  }
  return true;
}

// ====== Additions ======

void bn_add(BigNumber* target, const BigNumber* a, const BigNumber* b) {
  if(unlikely(bn_is_zero(a))) {
    free(target->digits);
    target->digits = malloc(b->digitsNumber * sizeof(uint64_t));
    memcpy(target->digits, b->digits, b->digitsNumber * sizeof(uint64_t));
    target->digitsNumber = b->digitsNumber;
    return;
  }
  else if(unlikely(bn_is_zero(b))) {
    free(target->digits);
    target->digits = malloc(a->digitsNumber * sizeof(uint64_t));
    memcpy(target->digits, a->digits, a->digitsNumber * sizeof(uint64_t));
    target->digitsNumber = a->digitsNumber;
    return;
  }

  size_t length = a->digitsNumber > b->digitsNumber ? a->digitsNumber : b->digitsNumber;
  uint64_t* digits = malloc(length * sizeof(uint64_t));
  if(digits == NULL) {
    printf("Error allocating memory for a new BigNumber.\n");
    return;
  }
  unsigned int carry = 0;
  uint64_t sum = 0;

  for(size_t position = 0; position < length; position++) {
    // First, the numbers still share `position` numbers of digits.
    if(position < a->digitsNumber && position < b->digitsNumber) {
      sum = a->digits[position] + b->digits[position] + carry;
    }
    else if(position >= a->digitsNumber) {
      // That means the number of b digits is bigger. Therefore we just add the carry to the b.
      sum = b->digits[position] + carry;
    }
    else {
      // The a is the bigger. And that's the only case left.
      sum = a->digits[position] + carry;
    }

    carry = sum >> 62; // When using a base number of 2^62, we shift the number 62 bits to
                       // the right, and for uint64_t, it leaves the numbers that are
                       // multipliers of 2^63 and 2^64.
    sum = sum & BIGNUMBER_MASK;
    digits[position] = sum;
  }

  if(carry > 0) {
    uint64_t* new_allocation = realloc(digits, (length + 1) * sizeof(uint64_t));
    if(new_allocation == NULL) {
      printf("Error reallocating the new BigNumber digits memory location for an additional digit.\n");
      free(digits);
      return;
    }
    digits = new_allocation;

    digits[length] = carry;
    carry = 0;
    length++;
  }
  
  free(target->digits);
  target->digits = digits;
  target->digitsNumber = length;
  return;
}

void bn_add_integer(BigNumber* target, const BigNumber* bn, const uint64_t* integer) {
  if(unlikely(*integer == 0)) {
    // If the integer is zero, just return the given big number.
    free(target->digits);
    target->digits = malloc(bn->digitsNumber * sizeof(uint64_t));
    memcpy(target->digits, bn->digits, bn->digitsNumber * sizeof(uint64_t));
    target->digitsNumber = bn->digitsNumber;
    return;
  }
  else if(unlikely(bn_is_zero(bn))) {
    // And if the big number is zero, we have to take care of the carry too
    unsigned int carry = *integer >> 62;
    size_t length = 1;
    uint64_t sum = *integer & BIGNUMBER_MASK;
    if(unlikely(carry > 0)) {
      length = 2;
    }

    uint64_t* allocation = malloc(length * sizeof(uint64_t));
    if(allocation == NULL) {
      printf("Failed allocating memory to return an integer plus a zero valued big number.\n");
      return;
    }
    allocation[0] = sum;
    if(unlikely(length == 2 && carry > 0)) allocation[1] = carry;
    free(target->digits);
    target->digits = allocation;
    target->digitsNumber = length;
    return;
  }

  // Now, to the real logic.
  unsigned __int128 sum = *integer;
  unsigned int carry = 0;
  size_t length = bn->digitsNumber;
  uint64_t* allocation = malloc(length * sizeof(uint64_t));
  if(allocation == NULL) {
    printf("Failed allocating memory to return an integer plus a non zero big number.\n");
    return;
  }

  for(size_t p = 0; p < length; p++) {
    sum += carry + bn->digits[p];
    carry = sum >> 62;
    allocation[p] = sum & BIGNUMBER_MASK;
    sum = 0;
  }

  // Now, if there is a carry, we allocate a new memory position. But that is unlikely.
  if(unlikely(carry > 0)) {
    length++;
    uint64_t* reallocation = realloc(allocation, length*sizeof(uint64_t));
    if(reallocation == NULL) {
      printf("Failed reallocating new location for the new digit in big number and integer addition.\n");
      return;
    }
    allocation = reallocation;
    allocation[length - 1] = carry;
    carry = 0;
  }
  
  free(target->digits);
  target->digits = allocation;
  target->digitsNumber = length;
  return;
}
