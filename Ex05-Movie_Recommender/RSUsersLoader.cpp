//
// Created by liora on 02/09/2022.
//

#include <fstream>
#include <iostream>
#include <sstream>

#include "RSUsersLoader.h"
#include "RecommenderSystem.h"

#define INVALID_FILE_MSG "Couldn't open file!"
#define RATE_OUT_OF_RANGE_MSG "Rate out of range!"
#define MIN_RATE 1
#define MAX_RATE 10

std::vector<sp_movie> RSUsersLoader::create_vec_movies (const std::string &
first_line)
{
  std::vector<sp_movie> vec_movies;
  std::istringstream ss (first_line);
  std::string first_word;

  while (std::getline (ss, first_word, ' '))
  {
    std::string movie_name = first_word.substr (0, first_word.find ('-'));
    std::string year_str = first_word.substr (first_word.find ('-') + 1);
    int year = std::stoi (year_str);
    sp_movie new_sp_movie = std::make_shared<Movie> (movie_name, year);
    vec_movies.push_back (new_sp_movie);
  }

  return vec_movies;
}

std::vector<RSUser> RSUsersLoader::create_users_from_file (const std::string &
users_file_path, std::unique_ptr<RecommenderSystem> rs) noexcept (false)
{
  std::ifstream ranks_file;
  ranks_file.open (users_file_path, std::ios::in);
  if (!(ranks_file.is_open ()))
  {
    throw std::invalid_argument (INVALID_FILE_MSG);
  }
  sp_rec_system shared_rs = std::make_shared<RecommenderSystem> (*rs);
  std::vector<RSUser> rs_users_vec;
  std::string first_line, next_line;
  std::getline (ranks_file, first_line);
  std::vector<sp_movie> vec_movies = create_vec_movies (first_line);
  while (std::getline (ranks_file, next_line))
  {
    std::istringstream ss (next_line);
    std::string word, name;
    rank_map user_rank_map (0, sp_movie_hash, sp_movie_equal);
    bool found_name = false;
    int counter = 0;
    while (std::getline (ss, word, ' '))
    {
      if (!found_name)
      {
        name = word;
        found_name = true;
        continue;
      }
      if (word.find ("NA") != std::string::npos)
      {
        counter += 1;
        continue;
      }
      double rate_double = std::stod (word);
      if (rate_double < MIN_RATE || rate_double > MAX_RATE)
      {
        throw std::range_error (RATE_OUT_OF_RANGE_MSG);
      }
      sp_movie cur_movie = vec_movies[counter];
      user_rank_map[cur_movie] = rate_double;
      counter += 1;
    }
    RSUser new_user (name, user_rank_map, shared_rs);
    rs_users_vec.push_back (new_user);
  }
  ranks_file.close ();
  return rs_users_vec;
}