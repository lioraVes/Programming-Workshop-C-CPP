#include "sort_bus_lines.h"

/**
 * This function swaps two BusLine elements pointed by a and b.
 * @param a- a BusLine pointer.
 * @param b- a BusLine pointer
 */
void swap (BusLine *a, BusLine *b)
{
  BusLine tmp = *a;
  *a = *b;
  *b = tmp;
}

void bubble_sort (BusLine *start, BusLine *end)
{
  long arr_length = end - start;
  for (int i = 0; i < arr_length - 1; i++)
  {
    int swapped = 1;
    for (int j = 0; j < arr_length - 1; j++)
    {
      if (((start + j)->distance) > ((start + j + 1)->distance))
      {
        swap ((start + j), (start + j + 1));
        swapped = 0;
      }
    }
    if (swapped == 1)
    {
      break;
    }
  }
}

BusLine *partition (BusLine *start, BusLine *end)
{
  BusLine *i = start - 1;
  for (int j = 0; j < end - start - 1; j++)
  {
    if (((start + j)->duration) <= ((end - 1)->duration))
    {
      i++;
      swap ((i), (start + j));
    }
  }
  swap (i + 1, end - 1);
  return i + 1;
}

void quick_sort (BusLine *start, BusLine *end)
{
  if (start < end)
  {
    BusLine *pivot = partition (start, end);
    quick_sort (start, pivot);
    quick_sort (pivot + 1, end);
  }
}
