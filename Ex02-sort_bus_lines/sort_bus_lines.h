#ifndef EX2_REPO_SORTBUSLINES_H
#define EX2_REPO_SORTBUSLINES_H

/**
 * Struct BusLine has three (int) fields: line_number, distance, duration.
 */
typedef struct BusLine
{
    int line_number, distance, duration;
} BusLine;

/**
 * This function sorting the BusLine array it gets using bubble-sort. It sorts
 * the objects in the array by the distance value.
 * @param start- a pointer to the beginning of the array.
 * @param end - a pointer to the end of the array (one block after the array).
 */
void bubble_sort (BusLine *start, BusLine *end);

/**
 * This recursive function sorting the BusLine array it gets using quick-sort.
 * It sorts the objects in the array by the duration value.
 * @param start- a pointer to the beginning of the array.
 * @param end - a pointer to the end of the array (one block after the array).
 */
void quick_sort (BusLine *start, BusLine *end);

/**
 * This function, helps the quick_sort function to sort the BusLine array.
 * It keeps the array organized- so the objects that their duration is smaller
 * than the pivot duration (the pivot is the last element in the array)
 * will be on its left and the bigger will be to its right. The function
 * returns the new pivot.
 * @param start- a pointer to the beginning of the array.
 * @param end - a pointer to the end of the array (one block after the array).
 * @return a pointer to the new pivot.
 */
BusLine *partition (BusLine *start, BusLine *end);

#endif //EX2_REPO_SORTBUSLINES_H
