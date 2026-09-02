#ifndef BIGNUMBER_H
#define BIGNUMBER_H

#define BIGNUMBER_BASE (1ULL << 62)
#define BIGNUMBER_MASK (1ULL << 62) - 1ULL

typedef struct BigNumber BigNumber;

// ====== Value attaching functions ======

void bn_zero(BigNumber* target);

// ====== Debuggers and printers ======

void bn_print_big_to_small(const BigNumber* number); // Prints the digits from the most obvious digit to the less obvious digit
void bn_print_small_to_big(const BigNumber* number); // Prints the digits from the less obvious digit to the most obvious digit

// ====== Additions ======

void bn_add(BigNumber* target, const BigNumber* a, const BigNumber* b);
void bn_add_integer(BigNumber* target, const BigNumber* bn, const unsigned int* integer);

// ====== Comparsions ======

bool bn_is_zero(BigNumber number);

#endif // BIGNUMBER_H
