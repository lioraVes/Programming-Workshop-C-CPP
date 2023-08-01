#include "test_bus_lines.h"

int is_sorted_by_distance (BusLine *start, BusLine *end)
{
  for (int i = 0; i < (end - start) - 1; i++)
  {
    if ((start[i].distance) > (start[i + 1].distance))
    {
      return 0;
    }
  }
  return 1;
}

int is_equal (BusLine *start_sorted, BusLine *end_sorted,
              BusLine *start_original, BusLine *end_original)
{
  if ((end_sorted - start_sorted) != (end_original - start_original))
  {
    return 0;
  }
  int found = 0;
  for (int i = 0; i < (end_original - start_original); i++)
  {
    for (int j = 0; j < (end_sorted - start_sorted); j++)
    {
      if (start_original[i].line_number == start_sorted[j].line_number &&
          start_original[i].distance == start_sorted[j].distance &&
          start_original[i].duration == start_sorted[j].duration)
      {
        found++;
        break;
      }
    }
  }
  if (found != end_original - start_original)
  {
    return 0;
  }
  return 1;
}

int is_sorted_by_duration (BusLine *start, BusLine *end)
{
  for (int i = 0; i < (end - start) - 1; i++)
  {
    if ((start[i].duration) > (start[i + 1].duration))
    {
      return 0;
    }
  }
  return 1;
}

