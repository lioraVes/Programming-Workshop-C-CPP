#include "cipher.h"
#include "tests.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ARGC 5
#define MIN_ARGC 2
#define DEC_BASE 10
#define MAX_LINE_LENGTH 1026

int check_valid_input (int argc, char *argv[])
{
  /**
   * This function checks that the input is valid, and if not it prints the
   * right error message and returns EXIT_FAILURE.
   */
  if (argc != MIN_ARGC && argc != MAX_ARGC)
  {
    fprintf (stderr, "The program receives 1 or 4 arguments only.\n");
    return EXIT_FAILURE;
  }
  if (argc == MIN_ARGC)
  {
    if (strcmp (argv[1], "test") != 0)
    {
      fprintf (stderr, "Usage: cipher test\n");
      return EXIT_FAILURE;
    }
  }
  if (argc == MAX_ARGC)
  {
    if ((strcmp (argv[1], "encode") != 0) && (strcmp (argv[1], "decode") != 0))
    {
      fprintf (stderr, "The given command is invalid.\n");
      return EXIT_FAILURE;
    }
    char *p_chars;
    long k_int = strtol (argv[2], &p_chars, DEC_BASE);
    if (strlen (p_chars) != 0 || k_int != (int) k_int
        || strcmp (argv[2], "") == 0)
    {
      fprintf (stderr, "The given shift value is invalid.\n");
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

int files (char *argv[])
{
  /**
   * This function first checks that the files' directory is valid and tries to
   * open them for reading or writing, if it fails it prints an error message.
   * If the files opened successfully, the function reads the input file line
   * by line, encodes or decodes the text and puts it in the output file.
   */
  FILE *input = fopen (argv[3], "r");
  if (input == NULL)
  {
    fprintf (stderr, "The given file is invalid.\n");
    return EXIT_FAILURE;
  }
  FILE *output = fopen (argv[4], "w");
  if (output == NULL)
  {
    fprintf (stderr, "The given file is invalid.\n");
    fclose (input);
    return EXIT_FAILURE;
  }
  char max_line[MAX_LINE_LENGTH];
  int k = (int) strtol (argv[2], NULL, DEC_BASE);
  if (strcmp (argv[1], "encode") == 0)
  {
    while (fgets (max_line, sizeof (max_line), input) != NULL)
    {
      encode (max_line, k);
      fputs (max_line, output);
    }
  }
  else if (strcmp (argv[1], "decode") == 0)
  {
    while (fgets (max_line, sizeof (max_line), input) != NULL)
    {
      decode (max_line, k);
      fputs (max_line, output);
    }
  }
  fclose (input);
  fclose (output);
  return EXIT_SUCCESS;
}

int test_command ()
{
  /**
   * This function checks if any of the tests in the test file fails.
   */
  if ((test_encode_non_cyclic_lower_case_positive_k () != 0) ||
      (test_encode_cyclic_lower_case_special_char_positive_k () != 0) ||
      (test_encode_non_cyclic_lower_case_special_char_negative_k () != 0) ||
      (test_encode_cyclic_lower_case_negative_k () != 0) ||
      (test_encode_cyclic_upper_case_positive_k () != 0) ||
      (test_decode_non_cyclic_lower_case_positive_k () != 0) ||
      (test_decode_cyclic_lower_case_special_char_positive_k () != 0) ||
      (test_decode_non_cyclic_lower_case_special_char_negative_k () != 0) ||
      (test_decode_cyclic_lower_case_negative_k () != 0) ||
      (test_decode_cyclic_upper_case_positive_k () != 0))
  {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int main (int argc, char *argv[])
{
  /**
   * The main function. First checks that the input is valid, then if it got
   * only 2 arguments- meaning it got test command (since the input is valid)
   * it calls the test_command function to check that all the tests are
   * working. If not, then it must have got 5 arguments and in that case it
   * calls the files function to encode or decode the files (if they are
   * valid). If something fails it returns EXIT_FAILURE, else- EXIT_SUCCESS.
   */

  if (check_valid_input (argc, argv) != 0)
  {
    return EXIT_FAILURE;
  }
  if (argc == MIN_ARGC)
  {
    if (test_command () != 0)
    {
      return EXIT_FAILURE;
    }
  }
  if (argc == MAX_ARGC)
  {
    if (files (argv) != 0)
    {
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}
