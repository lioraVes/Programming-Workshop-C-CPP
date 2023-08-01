//
// Created by liora on 02/09/2022.
//

#include "RecommenderSystemLoader.h"
#include <fstream>
#include <iostream>

#define INVALID_FILE_MSG "Couldn't open file!"
#define SCORE_OUT_OF_RANGE_MSG "Score out of range!"
#define MIN_RATE 1
#define MAX_RATE 10

RecommenderSystemLoader::sp_rec_system RecommenderSystemLoader::
create_rs_from_movies_file (const std::string &movies_file_path)
noexcept (false)
{
  std::ifstream movies_file;
  movies_file.open (movies_file_path, std::ios::in);
  if (!(movies_file.is_open ()))
  {
    throw std::invalid_argument (INVALID_FILE_MSG);
  }
  RecommenderSystem new_rec_system;
  std::string line;
  while (std::getline (movies_file, line))
  {
    std::string first_word = line.substr (0, line.find (' '));
    std::string movie_name = first_word.substr (0, first_word.find ('-'));
    std::size_t pos = first_word.find ('-');
    int year = std::stoi (first_word.substr (pos + 1));
    pos = first_word.length () + 1;
    std::vector<double> feature_vec;
    while (pos < line.length ())
    {
      std::string last_part = line.substr (pos);
      std::string next_num_str = last_part.substr (0, last_part.find (' '));
      double next_num_double = std::stoi (next_num_str);
      if (next_num_double < MIN_RATE || next_num_double > MAX_RATE)
      {
        throw std::range_error (SCORE_OUT_OF_RANGE_MSG);
      }
      feature_vec.push_back (next_num_double);
      pos += next_num_str.length () + 1;
    }
    new_rec_system.add_movie (movie_name, year, feature_vec);
  }
  movies_file.close ();
  sp_rec_system rec_sys_ptr = std::make_unique<RecommenderSystem>
      (new_rec_system);
  return rec_sys_ptr;
}

