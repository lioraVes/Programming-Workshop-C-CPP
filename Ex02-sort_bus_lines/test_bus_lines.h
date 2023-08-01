#ifndef EX2_REPO_TESTBUSLINES_H
#define EX2_REPO_TESTBUSLINES_H

#include "sort_bus_lines.h"

/**
 * Checks if the BusLine array pointed by start is sorted by the distance
 * value.
 * @param start- a pointer to the beginning of the array.
 * @param end - a pointer to the end of the array (one block after the array).
 * @return 1 upon success, 0 for failure.
 */
int is_sorted_by_distance (BusLine *start, BusLine *end);

/**
 * Checks if the BusLine array pointed by start is sorted by the duration
 * value.
 * @param start- a pointer to the beginning of the array.
 * @param end - a pointer to the end of the array (one block after the array).
 * @return 1 upon success, 0 for failure.
 */
int is_sorted_by_duration (BusLine *start, BusLine *end);

/**
 * Checks if the sorted BusLine array and the original BusLine array that
 * it gets contain the same BusLine objects (doesnt have to be in the same
 * order).
 * First the function checks if the length of the two arrays are the same, if
 * it is, it goes over the original array and search for the unique line
 * number in the sorted array.
 * @param start_sorted- a pointer to the beginning of the sorted array.
 * @param end_sorted- a pointer to the end of the sorted array (one block
 * after the array).
 * @param start_original- a pointer to the beginning of the original array.
 * @param end_original- a pointer to the end of the original array (one block
 * after the array).
 * @return 1 upon success, 0 for failure.
 */
int is_equal (BusLine *start_sorted,
              BusLine *end_sorted, BusLine *start_original,
              BusLine *end_original);

#endif //EX2_REPO_TESTBUSLINES_H
