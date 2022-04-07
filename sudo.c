#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "sudo.h"
#include "shuffle.h"

int sudos[N][N];
int sudos_answer[N][N];
int number_list[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

bool check_row(int row);
bool check_col(int col);
bool check_rec(int rec);
bool if_number_in_row(int sudos[N][N], int row, int num);
bool if_number_in_col(int sudos[N][N], int col, int num);
bool if_number_in_rec(int sudos[N][N], int rec, int num);
bool generate_full_sudos(void);

void sudos_clear()
{
    for (int *p = &sudos[0][0]; p <= &sudos[N - 1][N - 1]; p++)
        *p = 0;
}

void sudos_add(int number, int i, int j)
{
    sudos[i][j] = number;
}

void sudos_delete(int i, int j)
{
    sudos[i][j] = 0;
}

bool sudos_check()
{
    int i, j, k;
    bool res = true;
    // check rows
    for (i = 0; i < N; i++)
        res = res && check_row(i);
    // check cols
    for (j = 0; j < N; j++)
        res = res && check_col(j);
    // check rectangles
    for (k = 0; k < N; k++)
        res = res && check_rec(k);

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

    bool vi[N] = {false};
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

    bool vi[N] = {false};
    int i, j, m, k;
    i = index / 3 * 3;
    j = index % 3 * 3;

    for (m = i; m < i + N / 3; m++)
    {
        for (k = j; k < j + N / 3; k++)
        {
            if (sudos[m][k])
                return false;
            vi[sudos[m][k]] = true;
        }
    }
    return true;
}

bool sudos_if_full(int sudos[N][N])
{
    for (int *p = &sudos[0][0]; p <= &sudos[N - 1][N - 1]; p++)
        if (*p == 0)
            return false;

    return true;
}

bool sudos_solve_sudos(int sudos[N][N], int *count, bool if_precise_number)
{
    int row = 0, col = 0, rec = 0, val;
    // find next empty cell
    for (int i = 0; i < N * N; i++)
    {
        row = i / N;
        col = i % N;
        rec = row / 3 * 3 + col / 3;

        if (sudos[row][col] == 0)
        {
            for (val = 1; val < 10; val++)
            {
                // check if val exists in row
                if (!if_number_in_row(sudos, row, val))
                {
                    // check if val exists in col
                    if (!if_number_in_col(sudos, col, val))
                    {
                        // check if val exists in this rectangle
                        if (!if_number_in_rec(sudos, rec, val))
                        {
                            sudos[row][col] = val;
                            if (sudos_if_full(sudos))
                            {
                                (*count) += 1;
                                if (if_precise_number)
                                    break;
                                else
                                    return true;
                            }
                            else
                            {
                                if (sudos_solve_sudos(sudos, count, if_precise_number))
                                {
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
            break;
        }
    }

    sudos[row][col] = 0;
    return false;
}

bool if_number_in_row(int sudos[N][N], int row, int num)
{
    if (row < 0 || row >= N)
        return false;
    for (int *p = &sudos[row][0]; p <= &sudos[row][N - 1]; p++)
        if (*p == num)
            return true;
    return false;
}

bool if_number_in_col(int sudos[N][N], int col, int num)
{
    if (col < 0 || col >= N)
        return false;
    for (int *p = &sudos[0][col]; p <= &sudos[N - 1][col]; p += N)
        if (*p == num)
            return true;

    return false;
}

bool if_number_in_rec(int sudos[N][N], int rec, int num)
{
    if (rec < 0 || rec > N)
        return false;

    int i, j, m, k;
    i = rec / (N / 3) * (N / 3);
    j = rec % (N / 3) * (N / 3);

    for (m = i; m < i + N / 3; m++)
        for (k = j; k < j + N / 3; k++)
            if (sudos[m][k] == num)
                return true;
    return false;
}

bool generate_full_sudos(void)
{
    int row = 0, col = 0, rec = 0, k, val;
    // find next empty cell
    for (int i = 0; i < N * N; i++)
    {
        row = i / N;
        col = i % N;
        rec = row / 3 * 3 + col / 3;

        if (sudos[row][col] == 0)
        {
            // shuffle
            shuffle(number_list, sizeof(number_list) / sizeof(number_list[0]));

            for (k = 0; k < 9; k++)
            {
                val = number_list[k];
                // check if val exists in row
                if (!if_number_in_row(sudos, row, val))
                {
                    // check if val exists in col
                    if (!if_number_in_col(sudos, col, val))
                    {
                        // check if val exists in this rectangle
                        if (!if_number_in_rec(sudos, rec, val))
                        {
                            sudos[row][col] = val;
                            if (sudos_if_full(sudos))
                            {
                                return true;
                            }
                            else
                            {
                                if (generate_full_sudos())
                                {
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
            break;
        }
    }

    sudos[row][col] = 0;
    return false;
}

void sudos_generate_real(void)
{
    srand(time(NULL));
    int row, col, temp, count;
    int attempts = 5;
    int sudos_temp[N][N] = {0};
    // clear
    sudos_clear();
    // generate sudos without empty
    generate_full_sudos();

    while (attempts > 0)
    {
        row = rand() % N;
        col = rand() % N;

        while (sudos[row][col] == 0)
        {
            row = rand() % N;
            col = rand() % N;
        }
        temp = sudos[row][col];
        sudos[row][col] = 0;
        // take a full copy of sudos
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                sudos_temp[i][j] = sudos[i][j];
        count = 0;
        sudos_solve_sudos(sudos_temp, &count, true);

        if (count != 1)
        {
            sudos[row][col] = temp;
            attempts--;
        }
        else if (count == 1)
        {
            for (int i = 0; i < N; i++)
                for (int j = 0; j < N; j++)
                    sudos_answer[i][j] = sudos[i][j];
            sudos_solve_sudos(sudos_answer, &count, false);
        }
    }
}