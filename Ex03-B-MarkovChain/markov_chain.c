//
// Created by liora on 17/08/2022.
//
#include "markov_chain.h"

/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number maximal number to return (not including).
 * @return Random number.
 */
int get_random_number (int max_number)
{
  return rand () % max_number;
}

MarkovNode *get_first_random_node (MarkovChain *markov_chain)
{
  MarkovNode *i_markov_node = NULL;
  while (i_markov_node == NULL
         || markov_chain->is_last (i_markov_node->data) == true)
  {
    int i = get_random_number (markov_chain->database->size);
    Node *i_node = markov_chain->database->first;
    for (int j = 0; j < i; j++)
    {
      i_node = i_node->next;
    }
    i_markov_node = i_node->data;
  }
  return i_markov_node;
}

MarkovNode *get_next_random_node (MarkovNode *state_struct_ptr)
{
  int sum_freq = 0;
  for (int j = 0; j < state_struct_ptr->list_size; j++)
  {
    sum_freq += state_struct_ptr->counter_list[j].frequency;
  }
  int i = get_random_number (sum_freq);
  int counter = 0;
  for (int j = 0; j < state_struct_ptr->list_size; j++)
  {
    counter += state_struct_ptr->counter_list[j].frequency;
    if (counter > i)
    {
      return state_struct_ptr->counter_list[j].markov_node;
    }
  }
  return NULL; //will never get here
}

void generate_random_sequence (MarkovChain *markov_chain, MarkovNode *
first_node, int max_length)
{
  if (first_node == NULL || markov_chain->is_last (first_node->data) == true)
  {
    first_node = get_first_random_node (markov_chain);
  }
  markov_chain->print_func (first_node->data);
  int count = 1;
  bool is_last = false;
  MarkovNode *cur_node = first_node;
  while (count != max_length && is_last == false)
  {
    MarkovNode *next_node = get_next_random_node (cur_node);
    markov_chain->print_func (next_node->data);
    if (markov_chain->is_last (next_node->data) == true)
    {
      is_last = true;
    }
    cur_node = next_node;
    count++;
  }
}

bool add_node_to_counter_list (MarkovNode *first_node, MarkovNode
*second_node, MarkovChain *markov_chain)
{
  if (first_node == NULL || second_node == NULL)
  {
    return false;
  }
  for (int j = 0; j < first_node->list_size; j++)
  {
    if (first_node->counter_list[j].markov_node == second_node)
    {
      //if the second node is already in the list
      first_node->counter_list[j].frequency += 1;
      return true;
    }
  }
  NextNodeCounter *tmp = realloc (first_node->counter_list,
                                  (first_node->list_size + 1) *
                                  sizeof (NextNodeCounter));
  if (tmp == NULL)
  {
    fprintf (stdout, ALLOCATION_ERROR_MASSAGE);
    markov_chain->free_data (first_node->counter_list);
    first_node->counter_list = NULL;
    return false;
  }
  first_node->counter_list = tmp;
  first_node->counter_list[first_node->list_size].markov_node = second_node;
  first_node->counter_list[first_node->list_size].frequency = 1;
  first_node->list_size += 1;
  return true;
}

Node *get_node_from_database (MarkovChain *markov_chain, void *data_ptr)
{
  if (markov_chain->database == NULL)
  {
    return NULL;
  }
  Node *p = markov_chain->database->first;
  while (p != NULL)
  {
    if (markov_chain->comp_func (p->data->data, data_ptr) == 0)
    {
      return p;
    }
    p = p->next;
  }
  return NULL;
}

Node *add_to_database (MarkovChain *markov_chain, void *data_ptr)
{
  Node *node_from_database = get_node_from_database (markov_chain,
                                                     data_ptr);
  if (node_from_database != NULL)
  {
    return node_from_database;
  }
  MarkovNode *new_data_p = malloc (sizeof (MarkovNode));
  if (new_data_p == NULL)
  {
    return NULL;
  }
  char *copy_data = markov_chain->copy_func (data_ptr);
  if (copy_data == NULL)
  {
    free (new_data_p);
    return NULL;
  }
  *new_data_p = (MarkovNode) {copy_data, NULL, 0};

  if (add (markov_chain->database, new_data_p) == 1)
  {
    free (copy_data);
    free (new_data_p);
    return NULL;
  }
  return markov_chain->database->last;
}

void free_markov_chain (MarkovChain **ptr_chain)
{
  Node *cur = (*ptr_chain)->database->first;
  while (cur != NULL)
  {
    (*ptr_chain)->free_data (cur->data->counter_list);
    (*ptr_chain)->free_data (cur->data->data);
    (*ptr_chain)->free_data (cur->data);
    cur->data = NULL;
    Node *prev = cur;
    cur = cur->next;
    (*ptr_chain)->free_data (prev);
  }
  (*ptr_chain)->free_data ((*ptr_chain)->database);
  (*ptr_chain)->free_data (*ptr_chain);
  *ptr_chain = NULL;
}
