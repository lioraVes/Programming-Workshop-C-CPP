#include "sort_bus_lines.h"
#include "test_bus_lines.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 63
#define MIN_LINE_NUMBER 1
#define MAX_LINE_NUMBER 999
#define MIN_DISTANCE 0
#define MAX_DISTANCE 1000
#define MIN_DURATION 10
#define MAX_DURATION 100
#define DEC_BASE 10

/**
 * This function prints the BusLine array.
 * @param start - a pointer to the beginning of the BusLine array.
 * @param end - a pointer to the end of the array (one block after the array).
 */
void print_array (BusLine *start, BusLine *end)
{
  for (int i = 0; i < end - start; i++)
  {
    fprintf (stdout, "%d,%d,%d\n", (start + i)->line_number,
             (start + i)->distance, (start + i)->duration);
  }
}

/**
 * This function checks if the quick sort function works correctly and prints
 * if the test passed or not.
 * @param start - a pointer to the beginning of the BusLine array.
 * @param end - a pointer to the end of the array (one block after the array).
 * @return EXIT_SUCCESS upon success, EXIT_FAILURE for failure.
 */
void test3_quick (BusLine *start, BusLine *end)
{
  quick_sort (start, end);
  if (is_sorted_by_duration (start, end) == 0)
  {
    fprintf (stdout, "TEST 3 FAILED:The array isn't sorted by duration\n");
  }
  fprintf (stdout, "TEST 3 PASSED: The array is sorted by duration\n");
}

/**
 * This function checks if after the bubble or quick sort the sorted array
 * contains the same objects as the original array and prints
 * if the tests passed or not.
 * @param start_sorted- a pointer to the beginning of the sorted array.
 * @param end_sorted- a pointer to the end of the sorted array (one block
 * after the array).
 * @param start_original- a pointer to the beginning of the original array.
 * @param end_original- a pointer to the end of the original array (one block
 * after the array).
 * @param test_num - the number of test - 2 or 4.
 * @return EXIT_SUCCESS upon success, EXIT_FAILURE for failure.
 */
void test2_4 (BusLine *start_sorted, BusLine *end_sorted,
              BusLine *start_original, BusLine *end_original, int test_num)
{

  if (is_equal (start_sorted, end_sorted, start_original, end_original) == 0)
  {
    fprintf (stdout, "TEST %d FAILED: The array doesn't have the "
                     "same items after sorting\n", test_num);
  }
  fprintf (stdout, "TEST %d PASSED: The array has the same items "
                   "after sorting\n", test_num);
}

/**
 * This function checks if the bubble sort function works correctly and prints
 * it the test passed or not.
 * @param start - a pointer to the beginning of the BusLine array.
 * @param end - a pointer to the end of the array (one block after the array).
 * @return EXIT_SUCCESS upon success, EXIT_FAILURE for failure.
 */
void test1_bubble (BusLine *start, BusLine *end)
{
  bubble_sort (start, end);
  if (is_sorted_by_distance (start, end) == 0)
  {
    fprintf (stdout, "TEST 1 FAILED:The array isn't sorted by distance\n");
  }
  fprintf (stdout, "TEST 1 PASSED: The array is sorted by distance\n");
}

/**
 * The function gets a pointer to an BusLine array and returns a copy of it
 * using memcpy.
 * @param start - a pointer to the beginning of a BusLine array.
 * @param end - a pointer to the end of a BusLine array (one block after the
 * array).
 * @return address for the copy array if the allocation was successful and
 * NULL if not.
 */
BusLine *get_copy (BusLine *start, BusLine *end)
{
  BusLine *p_copy = calloc (end - start, sizeof (BusLine));
  if (p_copy == NULL)
  {
    return NULL;
  }
  for (int i = 0; i < end - start; i++)
  {
    memcpy ((p_copy + i), (start + i), sizeof (BusLine));
  }
  return p_copy;
}

/**
 * This function checks if the bubble_sort and quick_sort functions sorts the
 * array as needed, and that the sorted array contains the same objects as the
 * original array.
 * @param lines_arr - the pointer to the original array
 * @param num_of_lines -the length of the array
 * @return EXIT_SUCCESS upon success, EXIT_FAILURE for failure.
 */
int test_command (BusLine *lines_arr, int num_of_lines)
{
  //gets a copy of the original array
  BusLine *p_copy = get_copy (lines_arr, (lines_arr + num_of_lines));
  if (p_copy == NULL)
  {
    return EXIT_FAILURE;
  }
  test1_bubble (lines_arr, lines_arr + num_of_lines);
  test2_4 (lines_arr, lines_arr + num_of_lines,
           p_copy, p_copy + num_of_lines, 2);
  test3_quick (lines_arr, lines_arr + num_of_lines);
  test2_4 (lines_arr, lines_arr + num_of_lines,
           p_copy, p_copy + num_of_lines, 4);
  //freeing the original copy
  free (p_copy);
  p_copy = NULL;
  return EXIT_SUCCESS;
}

/**
 * This function adds elements to an array.
 * @param arr - a BusLine pointer to the array.
 * @param index - where in the array(which index) we want to add the
 * information.
 * @param line_number - the line_number value
 * @param distance - the distance value
 * @param duration - the duration value
 */
void add_to_arr (BusLine *arr, int index, int line_number, int distance,
                 int duration)
{
  arr[index].line_number = line_number;
  arr[index].distance = distance;
  arr[index].duration = duration;
}

/**
 * This function checks if the lines information input is in the valid range.
 * If not- prints ERROR message.
 * @param line_number
 * @param distance
 * @param duration
 * @return EXIT_SUCCESS upon success, EXIT_FAILURE for failure.
 */
int check_valid_input (int line_number, int distance, int duration)
{
  if (line_number < MIN_LINE_NUMBER || line_number > MAX_LINE_NUMBER)
  {
    fprintf (stdout, "ERROR: Line number should be an integer "
                     "between 1 and 999 (includes)\n");
    return EXIT_FAILURE;
  }
  if (distance < MIN_DISTANCE || distance > MAX_DISTANCE)
  {
    fprintf (stdout, "ERROR: Distance should be an integer between"
                     " 0 and 1000 (includes)\n");
    return EXIT_FAILURE;
  }
  if (duration < MIN_DURATION || duration > MAX_DURATION)
  {
    fprintf (stdout, "ERROR: Duration should be an integer between "
                     "10 and 100 (includes)\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

/**
 * This function allocates place for tan array.
 * @param num_of_lines -the length of the wanted array.
 * @return the address for the array if the allocation was successful and NULL
 * if not
 */
BusLine *allocate_arr (int num_of_lines)
{
  BusLine *p = calloc (num_of_lines, sizeof (BusLine));
  if (p == NULL)
  {
    return NULL;
  }
  return p;
}

/**
 * This function gets the lines information. First it allocates place for
 * them. Then checks if the fields it got are valid, if not - it prints an
 * ERROR message and asks again for valid information. If the fields are valid-
 * it adds it to the allocated array. In the end it returns the address for
 * the array.
 * @param num_of_lines -number of lines(the length of the wanted array).
 * @return address for the BusLine array if the allocation was successful,NULL
 * if not.
 */
BusLine *get_lines_info (int num_of_lines)
{
  BusLine *p = allocate_arr (num_of_lines);
  if (p == NULL)
  {
    return NULL;
  }
  int is_valid;
  for (int i = 0; i < num_of_lines; i++)
  {
    is_valid = 1;
    while (is_valid == 1)
    {
      fprintf (stdout, "Enter line info. Then enter\n");
      char line_info[MAX_LINE_LENGTH];
      fgets (line_info, MAX_LINE_LENGTH, stdin);
      int line_number, distance, duration;
      if (sscanf (line_info, "%d,%d,%d", &line_number, &distance,
                  &duration) == EOF)
      {
        return NULL;
      }
      if (check_valid_input (line_number, distance, duration) == 0)
      {
        is_valid = 0;
        add_to_arr (p, i, line_number, distance, duration);
      }
    }
  }
  return p;
}

/**
 * This function gets the number of lines from the stdin and checks if valid.
 * If not-prints an ERROR message and continue to ask for the number of lines.
 * @param num - starts as 0, then if the number of lines that the function got
 * is valid it changes to it.
 * @return the wanted number of lines.
 */
int get_num_lines (int num)
{
  while (num <= 0)
  {
    fprintf (stdout, "Enter number of lines. Then enter\n");
    char n[MAX_LINE_LENGTH];
    fgets (n, MAX_LINE_LENGTH, stdin);
    int num_of_lines = (int) strtol (n, NULL, DEC_BASE);
    if (num_of_lines > 0)
    {
      return num_of_lines;
    }
    fprintf (stdout, "ERROR: Number of lines should be a positive "
                     "integer\n");
    num = num_of_lines;
  }
  return 0;
}

/**
 * This function checks if the arguments are valid- if not prints an USAGE
 * error message.
 * @param argc - the number of arguments.
 * @param argv - the arguments.
 * @return EXIT_SUCCESS upon success, EXIT_FAILURE for failure.
 */
int check_valid_args (int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf (stdout, "USAGE: Program arguments are 'bubble',"
                     "'test' or 'quick'. Please enter one argument");
    return EXIT_FAILURE;
  }
  if (strcmp (argv[1], "test") != 0 && strcmp (argv[1], "bubble") != 0 &&
      strcmp (argv[1], "quick") != 0)
  {
    fprintf (stdout, "USAGE: Program arguments are 'bubble',"
                     "'test' or 'quick'. Please enter one argument");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

/**
 * First, checks if the arguments it got are valid, then it gets the number of
 * bus lines and allocates place for the array. it gets info about the lines
 * and adds it to the array. Then according to the command it got- test, bubble
 * or quick it sorts the array or tests the functions.
 * @param argc -number of arguments it got.
 * @param argv - the arguments it got (if input is valid supposed to be 'test',
 * 'bubble' or 'quick'.
 * @return EXIT_SUCCESS upon success, EXIT_FAILURE for failure.
 */
int main (int argc, char *argv[])
{
  if (check_valid_args (argc, argv) != 0)
  {
    return EXIT_FAILURE;
  }
  int num_of_lines = get_num_lines (0);

  BusLine *lines_arr = (BusLine *) get_lines_info (num_of_lines);
  if (lines_arr == NULL)
  {
    return EXIT_FAILURE;
  }
  if (strcmp (argv[1], "test") == 0)
  {
    if (test_command (lines_arr, num_of_lines) != 0)
    {
      free (lines_arr);
      lines_arr = NULL;
      return EXIT_FAILURE;
    }
  }
  else if (strcmp (argv[1], "bubble") == 0)
  {
    bubble_sort (lines_arr, lines_arr + num_of_lines);
    print_array (lines_arr, lines_arr + num_of_lines);
  }
  else if (strcmp (argv[1], "quick") == 0)
  {
    quick_sort (lines_arr, lines_arr + num_of_lines);
    print_array (lines_arr, lines_arr + num_of_lines);
  }
  free (lines_arr);
  lines_arr = NULL;
  return EXIT_SUCCESS;
}