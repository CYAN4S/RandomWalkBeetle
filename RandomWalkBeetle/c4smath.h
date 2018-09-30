// Made by CYAN4S.
#pragma once
#include <cmath>

template <typename T>
double getAverage(T* arr, int size)
{
    if (size == 0)
        return -1;

    double sum = 0;

    for (int i = 0; i < size; i++)
        sum += arr[i];

    return sum / size;
}

template <typename T>
// If you want to get 'standard deviation', square the return value.
double getVariance(T* arr, int size)
{
    if (size == 0)
        return -1;

    double sum = 0;
    double avg = getAverage(arr, size);
    double result;

    for (int i = 0; i < size; i++)
        sum += pow(arr[i] - avg, 2);

    return sum / size;
}
