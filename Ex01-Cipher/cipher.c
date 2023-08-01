#include "cipher.h"
#include <string.h>

#define LOWER_A_VAL 97
#define UPPER_A_VAL 65
#define ALPHABET_LEN 26
#define LOWER_Z_VAL 122
#define UPPER_Z_VAL 90

int calc_new_char_lower (int k, char l)
{
  /**
   * This function gets an int k and a char l and returns the new ascii
   * value of l according to the shift (of k). First we subtract 'a'
   * (lower case) ascii value and add k, we calculate the modulo by the
   * alphabet length,then we add the lower 'a' value again in order to get the
   * right new ascii value and return it.
   */
  return (l - LOWER_A_VAL + k) % ALPHABET_LEN + LOWER_A_VAL;
}

int calc_new_char_upper (int k, char l)
{
  /**
   * This function gets an int k and a char l and returns the new ascii
   * value of l according to the shift (of k). First we subtract 'A'
   * (upper case) ascii value and add k, we calculate the modulo by the
   * alphabet length, then we add the upper 'A' value again in order to get the
   * right new ascii value and return it.
   */
  return (l - UPPER_A_VAL + k) % ALPHABET_LEN + UPPER_A_VAL;
}

int calc_mod_neg (int num)
{
  /**
   * This function gets a negative number and calculates the right(positive)
   * modulo (remaining) of the alphabet length.
   */
  while (num < 0)
  {
    num += ALPHABET_LEN;
  }
  return num;
}

void encode (char s[], int k)
{
  /**
   * This function gets a string-s[] and an int- k and encodes the string by
   * shifting the letters by k. if k is negative it calls the calc_mod_neg
   * function that calculates a new, positive k. Then it iterates over the
   * characters of the string and by calling the calc_new_char_lower\upper
   * functions calculates the new encoded character.
   */
  int len_s = (int) strlen (s);
  if (k < 0)
  {
    k = calc_mod_neg (k);
  }
  for (int i = 0; i < len_s; i++)
  {
    if (s[i] >= LOWER_A_VAL && s[i] <= LOWER_Z_VAL)
    {
      //if the character is a lower case letter
      int new_char = calc_new_char_lower (k, s[i]);
      s[i] = (char) new_char;
    }
    else if (s[i] >= UPPER_A_VAL && s[i] <= UPPER_Z_VAL)
    {
      //if the character is an upper case letter
      int new_char = calc_new_char_upper (k, s[i]);
      s[i] = (char) new_char;
    }
  }
}

void decode (char s[], int k)
{
  /**
   * This function gets a string-s[] and an int- k and decodes the string by k.
   * It calls the encode function with the same s and -k, because the functions
   * are inverse to each other.
   */
  encode (s, -k);
}
