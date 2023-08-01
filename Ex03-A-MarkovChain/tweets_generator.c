//
// Created by liora on 09/08/2022.
//
#include "markov_chain.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MIN_ARGC 4
#define MAX_ARGC 5
#define SEED_IND 1
#define TWEETS_NUM_IND 2
#define PATH_IND 3
#define NUM_TO_READ_IND 4
#define MAX_LENGTH_TWEET 1000
#define DEC_BASE 10
#define MAX_NUM_WORDS_GENERATE 20
#define USAGE_ERROR_MASSAGE "Usage: The program require the following "\
"parameters\n"\
"1) Seed number.\n"\
"2) Number of lines to generate.\n"\
"3) Absolute path to file.\n"\
"4) Optional - Number of lines to read from file. If no "\
"value is given read the entire file."
#define FILE_ERROR_MESSAGE "Error: Failed to open file, make sure you use "\
"absolute path."

/**
 * This function generate the tweets and prints them to stdout.
 * @param num_to_generate -number of tweets to generate.
 * @param markov_chain -the markov chain that contains the database.
 */
void generate_tweets (int num_to_generate, MarkovChain *markov_chain)
{
  for (int i = 0; i < num_to_generate; i++)
  {
    fprintf (stdout, "Tweet %d:", i + 1);
    MarkovNode *first_node = get_first_random_node (markov_chain);
    generate_random_sequence (markov_chain, first_node,
                              MAX_NUM_WORDS_GENERATE);
    fprintf (stdout, "\n");
  }
}

/**
 * This function checks if a word is a last word in a sentence (if it has a
 * dot in the end).
 * @param word- a pointer to the word .
 * @return True if last word, false otherwise.
 */
bool is_last_word (char *word)
{
  char last_char = word[strlen (word) - 1];
  if (last_char == '.')
  {
    return true;
  }
  return false;
}

/**
 * this function 'cleans' a word - if it contains \n or \r it removes it.
 * @param word -a pointer to the word
 */
void clean_word (char *word)
{
  if (word != NULL && word[strlen (word) - 1] == '\n')
  {
    word[strlen (word) - 1] = '\0';
  }
  if (word != NULL && word[strlen (word) - 1] == '\r')
  {
    word[strlen (word) - 1] = '\0';
  }
}

/**
 * this function reads from a file and insert the words to the markov chain
 * and updates the counter list in each node.
 * @param fp - a pointer to the input text file.
 * @param words_to_read - number of words to read from the file, (-1 if we want
 * to read all the file)
 * @param markov_chain- a pointer to the markov chain.
 */
int fill_database (FILE *fp, int words_to_read, MarkovChain *markov_chain)
{
  int counter = 0;
  char buf[MAX_LENGTH_TWEET];
  MarkovNode *prev_node = NULL;
  while (fgets (buf, sizeof (buf), fp) != NULL)
  {
    char *word;
    word = strtok (buf, " ");
    while ((word != NULL) && (counter != words_to_read))
    {
      clean_word (word);
      Node *word_node = add_to_database (markov_chain, word);
      if (word_node == NULL)
      {
        fprintf (stdout, ALLOCATION_ERROR_MASSAGE);
        return EXIT_FAILURE;
      }
      if (prev_node != NULL && is_last_word (prev_node->data) == false)
      {
        if (add_node_to_counter_list (prev_node, word_node->data) == false)
        {
          return EXIT_FAILURE;
        }
      }
      prev_node = word_node->data;
      counter++;
      word = strtok (NULL, " ");
    }
  }
  return EXIT_SUCCESS;
}

/**
 * This function opens the file and calls the fill_database function.
 * @param argc - number of arguments.
 * @param argv - the arguments.
 * @param markov_chain- a pointer to the markov chain.
 * @return EXIT_SUCCESS upon success and EXIT_FAILURE for failure.
 */
int files (int argc, char *argv[], MarkovChain *markov_chain)
{
  FILE *input = fopen (argv[PATH_IND], "r");
  if (input == NULL)
  {
    fprintf (stdout, FILE_ERROR_MESSAGE);
    return EXIT_FAILURE;
  }
  if (argc == MAX_ARGC)
  {
    int num_to_read = (int) strtol (argv[NUM_TO_READ_IND], NULL, DEC_BASE);
    if (fill_database (input, num_to_read, markov_chain) != 0)
    {
      fclose (input);
      return EXIT_FAILURE;
    }
  }
  if (argc == MIN_ARGC)
  {
    if (fill_database (input, -1, markov_chain) != 0)
    {
      fclose (input);
      return EXIT_FAILURE;
    }
  }
  fclose (input);
  return EXIT_SUCCESS;
}

/**
 * This function checks if the input is valid, if not prints a usage error
 * message.
 * @param argc - number of arguments.
 * @return EXIT_SUCCESS upon success and EXIT_FAILURE for failure.
 */
int check_valid_input (int argc)
{
  if (argc != MIN_ARGC && argc != MAX_ARGC)
  {
    fprintf (stdout, USAGE_ERROR_MASSAGE);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

/**
 * First checks if the input is valid and tries to open the file, then reads
 * the words from the file and insert them to the markov chain database.
 * In the end it generate tweets.
 * @param argc- number of arguments.
 * @param argv - the arguments.
 * @return EXIT_SUCCESS upon success and EXIT_FAILURE for failure.
 */
int main (int argc, char *argv[])
{
  if (check_valid_input (argc) != 0)
  {
    return EXIT_FAILURE;
  }
  //allocate new empty markov-chain and database.
  MarkovChain *new_markov_chain = malloc (sizeof (MarkovChain));
  if (new_markov_chain == NULL)
  {
    fprintf (stdout, ALLOCATION_ERROR_MASSAGE);
    return EXIT_FAILURE;
  }
  LinkedList *new_database = malloc (sizeof (LinkedList));
  if (new_database == NULL)
  {
    fprintf (stdout, ALLOCATION_ERROR_MASSAGE);
    return EXIT_FAILURE;
  }
  *new_database = (LinkedList) {NULL, NULL, 0};
  new_markov_chain->database = new_database;

  if (files (argc, argv, new_markov_chain) != 0)
  {
    free (new_database);
    new_database = NULL;
    free (new_markov_chain);
    new_markov_chain = NULL;
    return EXIT_FAILURE;
  }
  //generate tweets
  unsigned int seed_num = (unsigned int) strtol (argv[SEED_IND], NULL,
                                                 DEC_BASE);
  srand (seed_num);
  int tweets_num = (int) strtol (argv[TWEETS_NUM_IND], NULL, DEC_BASE);
  generate_tweets (tweets_num, new_markov_chain);
  free_markov_chain (&new_markov_chain);
  return EXIT_SUCCESS;
}

