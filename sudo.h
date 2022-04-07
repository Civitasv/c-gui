#ifndef SUDO_H
#define SUDO_H

#include <stdbool.h>
#define N 9
#define MINIMAL_CLUES 17

extern int sudos[N][N];
extern int sudos_answer[N][N];

void sudos_clear(void);

void sudos_add(int number, int i, int j);

void sudos_delete(int i, int j);

bool sudos_check(void);

bool sudos_if_full(int sudos[][N]);

bool sudos_solve_sudos(int sudos[][N], int *count, bool if_precise_number);

void sudos_generate_real(void);

#endif