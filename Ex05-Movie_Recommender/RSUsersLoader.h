//
// Created on 2/21/2022.
//

#ifndef SCHOOL_SOLUTION_USERFACTORY_H
#define SCHOOL_SOLUTION_USERFACTORY_H

#include "RSUser.h"

class RSUsersLoader
{
 private:
  /**
   * a function that creates vector of sp_movies the user had to rank.
   * @param first_line
   * @return vector of movies.
   */
  static std::vector<sp_movie> create_vec_movies (const std::string &
  first_line);

 public:
  RSUsersLoader () = delete;
  /**
   * loads users by the given format with their movie's ranks
   * @param users_file_path a path to the file of the users and their movie
   * ranks
   * @param rs RecommendingSystem for the Users
   * @return vector of the users created according to the file
   */
  static std::vector<RSUser> create_users_from_file (const std::string &
  users_file_path, std::unique_ptr<RecommenderSystem> rs) noexcept (false);
};

#endif //SCHOOL_SOLUTION_USERFACTORY_H
