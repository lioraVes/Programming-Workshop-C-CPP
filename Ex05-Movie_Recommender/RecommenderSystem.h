//
// Created on 2/20/2022.
//

#ifndef SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
#define SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
#include "RSUser.h"
#include <map>

/**
 * comparator struct. used to sort a vector of pairs.
 */
struct comp_angles
{
    bool operator() (const std::pair<sp_movie, double> &l,
                     const std::pair<sp_movie, double> &r) const
    {
      if (l.second != r.second)
      {
        return l.second > r.second;
      }
      return l.first > r.first;
    }
};

class RecommenderSystem
{
 public:
  /**
   * typedefs for this class.
   */
  typedef std::map<sp_movie, std::vector<double>, equal_func> map_of_movies;
  typedef std::vector<double> vec_of_doubles;
  typedef std::vector<std::pair<sp_movie, double>> vec_of_pairs;
  typedef std::vector<sp_movie> vec_movies;

  explicit RecommenderSystem ();
  /**
   * adds a new movie to the system
   * @param name name of movie
   * @param year year it was made
   * @param features features for movie
   * @return shared pointer for movie in system
   */
  sp_movie add_movie (const std::string &name, int year,
                      const std::vector<double> &features);

  /**
   * a function that calculates the movie with highest score based on movie
   * features
   * @param ranks user ranking to use for algorithm
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_content (const RSUser &user);

  /**
   * a function that calculates the movie with highest predicted score based
   * on ranking of other movies
   * @param ranks user ranking to use for algorithm
   * @param k
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_cf (const RSUser &user, int k);

  /**
   * Predict a user rating for a movie given argument using item cf procedure
   * with k most similar movies.
   * @param user_rankings: ranking to use
   * @param movie: movie to predict
   *
   * @param k:
   * @return score based on algorithm as described in pdf
   */
  double predict_movie_score (const RSUser &user, const sp_movie &movie,
                              int k);

  /**
   * gets a shared pointer to movie in system
   * @param name name of movie
   * @param year year movie was made
   * @return shared pointer to movie in system
   */
  sp_movie get_movie (const std::string &name, int year) const;
  /**
   * operator<< for recommender system
   * @param os
   * @param recommender_system
   * @return os
   */
  friend std::ostream &operator<<
      (std::ostream &os, const RecommenderSystem &recommender_system);

 private:
  map_of_movies _movie_map;
  /**
   * this function calculates average of the ranks in the rank_map.
   * @param user_rank_map
   * @return the average of ranks.
   */
  double calc_avg (rank_map &user_rank_map);
  /**
   * this function creates the preference vector.
   * @param user_rank_map
   * @return vector of doubles- the pref vec.
   */
  vec_of_doubles create_pref_vec (rank_map &user_rank_map);
  /**
   * this function multiply vector by scalar and returning the result.
   * @param num
   * @param vec
   * @return vector of doubles.
   */
  vec_of_doubles multiply_scalar_vector (double num,
                                         const vec_of_doubles &vec);
  /**
   * this function add one vector to the other. changes the first vector.
   * @param first_vec
   * @param second_vec
   * @return the result- vector of doubles.
   */
  vec_of_doubles &add_vectors (vec_of_doubles &first_vec,
                               const vec_of_doubles &second_vec);
  /**
   * this function calculates the angle between two vectors.
   * @param first_vec
   * @param second_vec
   * @return double- the angle between the two vectors.
   */
  double calc_angle (const vec_of_doubles &first_vec,
                     const vec_of_doubles &second_vec);
  /**
   * this function calculates the dot product between two vectors.
   * @param first_vec
   * @param second_vec
   * @return double- the dot product.
   */
  double dot_prod (const vec_of_doubles &first_vec,
                   const vec_of_doubles &second_vec);
  /**
   * this function calculates the norm of a vector.
   * @param first_vec
   * @return double- the norm of a vector.
   */
  double calc_norm (const vec_of_doubles &first_vec);
  /**
   * this function creates vector of sp_movies, the vector contains movies
   * that the user didnt watch- movies that doesnt appear in the user rank_map.
   * @param user_rank_map
   * @return vector of movies the user didnt watch.
   */
  vec_movies create_vec_didnt_watch (rank_map user_rank_map);
  /**
   * this function calculates the similarity between the pref vec and the
   * movies in vec_movies.
   * @param pref_vec
   * @param vec_movies
   * @return returns a vec_of_pairs that is sorted by the similarity from max
   * to min.
   */
  vec_of_pairs calc_similarity (const vec_of_doubles &pref_vec,
                                const vec_movies &vec_movies);
};

#endif //SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
