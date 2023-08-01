
#ifndef RECOMMENDERSYSTEMLOADER_H
#define RECOMMENDERSYSTEMLOADER_H

#include "RecommenderSystem.h"

class RecommenderSystemLoader
{

 private:

 public:
  /**
   * typedef for unique pointer to RecommenderSystem
   */
  typedef std::unique_ptr<RecommenderSystem> sp_rec_system;

  RecommenderSystemLoader () = delete;
  /**
   * loads movies by the given format for movies with their feature's score
   * @param movies_file_path a path to the file of the movies
   * @return smart pointer to a RecommenderSystem which was created with
   * those movies.
   */
  static sp_rec_system create_rs_from_movies_file
      (const std::string &movies_file_path) noexcept (false);
};

#endif //RECOMMENDERSYSTEMLOADER_H
