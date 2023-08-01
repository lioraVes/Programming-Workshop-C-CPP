#include <string.h> // For strlen(), strcmp(), strcpy()
#include "markov_chain.h"

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#define EMPTY -1
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60

#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20
#define NUM_ARGC 3
#define SEED_IND 1
#define WALKS_NUM_IND 2
#define DEC_BASE 10
#define USAGE_ERROR_MASSAGE " Usage: The program require the following "\
"parameters\n"\
"1) Seed number.\n"\
"2) Number of lines to generate."

/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
 */
const int transitions[][2] = {{13, 4},
                              {85, 17},
                              {95, 67},
                              {97, 58},
                              {66, 89},
                              {87, 31},
                              {57, 83},
                              {91, 25},
                              {28, 50},
                              {35, 11},
                              {8,  30},
                              {41, 62},
                              {81, 43},
                              {69, 32},
                              {20, 39},
                              {33, 70},
                              {79, 99},
                              {23, 76},
                              {15, 47},
                              {61, 14}};

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell
{
    int number; // Cell number 1-100
    int ladder_to;  // ladder_to represents the jump of the ladder in case
    // there is one from this square
    int snake_to;  // snake_to represents the jump of the snake in case there
    // is one from this square
    //both ladder_to and snake_to should be -1 if the Cell doesn't have them
} Cell;

/** Error handler **/
static int handle_error (char *error_msg, MarkovChain **database)
{
  printf ("%s", error_msg);
  if (database != NULL)
  {
    free_markov_chain (database);
  }
  return EXIT_FAILURE;
}

static int create_board (Cell *cells[BOARD_SIZE])
{
  for (int i = 0; i < BOARD_SIZE; i++)
  {
    cells[i] = malloc (sizeof (Cell));
    if (cells[i] == NULL)
    {
      for (int j = 0; j < i; j++)
      {
        free (cells[j]);
      }
      handle_error (ALLOCATION_ERROR_MASSAGE, NULL);
      return EXIT_FAILURE;
    }
    *(cells[i]) = (Cell) {i + 1, EMPTY, EMPTY};
  }

  for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
  {
    int from = transitions[i][0];
    int to = transitions[i][1];
    if (from < to)
    {
      cells[from - 1]->ladder_to = to;
    }
    else
    {
      cells[from - 1]->snake_to = to;
    }
  }
  return EXIT_SUCCESS;
}

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int fill_database (MarkovChain *markov_chain)
{
  Cell *cells[BOARD_SIZE];
  if (create_board (cells) == EXIT_FAILURE)
  {
    return EXIT_FAILURE;
  }
  MarkovNode *from_node = NULL, *to_node = NULL;
  size_t index_to;
  for (size_t i = 0; i < BOARD_SIZE; i++)
  {
    add_to_database (markov_chain, cells[i]);
  }

  for (size_t i = 0; i < BOARD_SIZE; i++)
  {
    from_node = get_node_from_database (markov_chain, cells[i])->data;

    if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
    {
      index_to = MAX(cells[i]->snake_to, cells[i]->ladder_to) - 1;
      to_node = get_node_from_database (markov_chain, cells[index_to])
          ->data;
      add_node_to_counter_list (from_node, to_node, markov_chain);
    }
    else
    {
      for (int j = 1; j <= DICE_MAX; j++)
      {
        index_to = ((Cell *) (from_node->data))->number + j - 1;
        if (index_to >= BOARD_SIZE)
        {
          break;
        }
        to_node = get_node_from_database (markov_chain, cells[index_to])
            ->data;
        add_node_to_counter_list (from_node, to_node, markov_chain);
      }
    }
  }
  // free temp arr
  for (size_t i = 0; i < BOARD_SIZE; i++)
  {
    free (cells[i]);
  }
  return EXIT_SUCCESS;
}

/**
 * This function generates possible walks on the board.
 * @param walks_num -number of walks to generate.
 * @param markov_chain -the markov chain that contains the database.
 */
static void generate_walks (int walks_num, MarkovChain *markov_chain)
{
  for (int i = 0; i < walks_num; i++)
  {
    fprintf (stdout, "Random Walk %d:", i + 1);
    MarkovNode *first_node = markov_chain->database->first->data;
    generate_random_sequence (markov_chain, first_node,
                              MAX_GENERATION_LENGTH);
    fprintf (stdout, "\n");
  }
}

/**
 * This function checks if a cell is the the last cell (if its number is 100)
 * @param cell
 * @return true if it is, false if not.
 */
static bool is_last_cell (Cell *cell)
{
  if (cell->number == BOARD_SIZE)
  {
    return true;
  }
  return false;
}

/**
 * This function dynamically allocates place for a new cell and copies the
 * original cell info to the new one.
 * @param original_cell - to copy from.
 * @return a new dynamically allocated cell which is a copy of the original
 * one.
 */
static Cell *copy_cell (Cell *original_cell)
{
  Cell *copy_original_cell = malloc (sizeof (Cell));
  if (copy_original_cell == NULL)
  {
    return NULL;
  }
  copy_original_cell->number = original_cell->number;
  copy_original_cell->ladder_to = original_cell->ladder_to;
  copy_original_cell->snake_to = original_cell->snake_to;
  return copy_original_cell;
}

/**
 * This function compares between two cells numbers.
 * @param cell1
 * @param cell2
 * @return - a positive value if the first cell number is bigger
 *         - a negative value if the second cell number is bigger
 *         - 0 if the numbers are equal
 */
static int cmp_cells (Cell *cell1, Cell *cell2)
{
  if (cell1->number == cell2->number)
  {
    return 0;
  }
  return cell1->number - cell2->number;
}

/**
 * This function prints the cell data.
 * @param data- the cell we want to print.
 */
static void print_cell (Cell *data)
{
  if (data->ladder_to != -1)
  {
    fprintf (stdout, " [%d]-ladder to %d ->", data->number, data->ladder_to);
  }
  else if (data->snake_to != -1)
  {
    fprintf (stdout, " [%d]-snake to %d ->", data->number, data->snake_to);
  }
  else if (data->number == BOARD_SIZE)
  {
    fprintf (stdout, " [%d]", data->number);
  }
  else
  {
    fprintf (stdout, " [%d] ->", data->number);
  }
}

/**
 * This function checks if the input is valid, if not prints a usage error
 * message.
 * @param argc -number of arguments.
 * @return EXIT_SUCCESS upon success and EXIT_FAILURE for failure.
 */
static int check_valid_input (int argc)
{
  if (argc != NUM_ARGC)
  {
    fprintf (stdout, USAGE_ERROR_MASSAGE);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
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
    free (new_markov_chain);
    return EXIT_FAILURE;
  }
  *new_database = (LinkedList) {NULL, NULL, 0};

  new_markov_chain->database = new_database;
  new_markov_chain->print_func = (Print_fp) print_cell;
  new_markov_chain->comp_func = (Cmp_fp) cmp_cells;
  new_markov_chain->free_data = free;
  new_markov_chain->copy_func = (Copy_fp) copy_cell;
  new_markov_chain->is_last = (IsLast) is_last_cell;

  if (fill_database (new_markov_chain) != 0)
  {
    free_markov_chain (&new_markov_chain);
    return EXIT_FAILURE;
  }
  //generate walks
  unsigned int seed_num = (unsigned int) strtol (argv[SEED_IND], NULL,
                                                 DEC_BASE);
  srand (seed_num);
  int walks_num = (int) strtol (argv[WALKS_NUM_IND], NULL, DEC_BASE);
  generate_walks (walks_num, new_markov_chain);
  free_markov_chain (&new_markov_chain);
  return EXIT_SUCCESS;
}
