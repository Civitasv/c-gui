#include <stdbool.h>
#include "sudo.h"

bool check_row(int row);
bool check_col(int col);
bool check_rec(int rec);

void add(int number, int i, int j)
{
    sudos[i][j] = number;
}

void delete (int i, int j)
{
    sudos[i][j] = 0;
}

bool check()
{
    int i, j, k;
    bool res = true;
    // check rows
    for (i = 0; i < N; i++)
        res &= check_row(i);
    // check cols
    for (j = 0; j < N; j++)
        res &= check_col(j);
    // check rectangles
    for (k = 0; k < N; k++)
        res &= check_rec(k);

    return res;
}

bool check_row(int row)
{
    if (row < 0 || row >= N)
        return false;
    bool vi[N] = {false};
    int *p;
    for (p = &sudos[row][0]; p <= &sudos[row][N - 1]; p++)
    {
        if (vi[*p])
            return false;
        vi[*p] = true;
    }
    return true;
}

bool check_col(int col)
{
    if (col < 0 || col >= N)
        return false;

    bool vi[N] = false;
    int *p;
    for (p = &sudos[0][col]; p <= &sudos[N - 1][col]; p += N)
    {
        if (vi[*p])
            return false;
        vi[*p] = true;
    }
    return true;
}

bool check_rec(int index)
{
    if (index < 0 || index >= N)
        return false;

    bool vi[N] = false;
    int i, j;
    i = index / 3 * 3;
    j = index % 3 * 3;
}