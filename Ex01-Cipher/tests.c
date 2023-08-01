#include "tests.h"
#include <string.h>

#define K_1 3
#define K_2 2
#define K_3 -1
#define K_4 -3
#define K_5 29

int test_encode_non_cyclic_lower_case_positive_k ()
{
  char in[] = "abc";
  char out[] = "def";
  encode (in, K_1);
  return strcmp (in, out) != 0;
}

int test_encode_cyclic_lower_case_special_char_positive_k ()
{
  char in[] = "y$za! k";
  char out[] = "a$bc! m";
  encode (in, K_2);
  return strcmp (in, out) != 0;
}

int test_encode_non_cyclic_lower_case_special_char_negative_k ()
{
  char in[] = "s@he!ll,y";
  char out[] = "r@gd!kk,x";
  encode (in, K_3);
  return strcmp (in, out) != 0;
}

int test_encode_cyclic_lower_case_negative_k ()
{
  char in[] = "catod";
  char out[] = "zxqla";
  encode (in, K_4);
  return strcmp (in, out) != 0;
}

int test_encode_cyclic_upper_case_positive_k ()
{
  char in[] = "YWNA";
  char out[] = "BZQD";
  encode (in, K_5);
  return strcmp (in, out) != 0;
}

int test_decode_non_cyclic_lower_case_positive_k ()
{
  char in[] = "def";
  char out[] = "abc";
  decode (in, K_1);
  return strcmp (in, out) != 0;
}

int test_decode_cyclic_lower_case_special_char_positive_k ()
{
  char in[] = "a$bc! m";
  char out[] = "y$za! k";
  decode (in, K_2);
  return strcmp (in, out) != 0;
}

int test_decode_non_cyclic_lower_case_special_char_negative_k ()
{
  char in[] = "r@gd!kk,x";
  char out[] = "s@he!ll,y";
  decode (in, K_3);
  return strcmp (in, out) != 0;
}

int test_decode_cyclic_lower_case_negative_k ()
{
  char in[] = "zxqla";
  char out[] = "catod";
  decode (in, K_4);
  return strcmp (in, out) != 0;
}

int test_decode_cyclic_upper_case_positive_k ()
{
  char in[] = "BZQD";
  char out[] = "YWNA";
  decode (in, K_5);
  return strcmp (in, out) != 0;
}