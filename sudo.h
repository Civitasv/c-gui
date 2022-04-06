#ifndef SUDO_H
#define SUDO_H

#define N 9

int sudos[N][N] = {0};

void random_initialize(void);

void add(int number, int i, int j);

void delete (int i, int j);

bool check(void);

#endif