//
// Created by liora on 02/09/2022.
//

#include "RecommenderSystem.h"

#include <cmath>
#include <algorithm>
#define ZERO_DIVISION_MSG "Zero division error!"

/**
 * a comparator the movie_map use. compares between two sp_movies.
 * @param lhs
 * @param rhs
 * @return true if lhs < rhs.
 */
bool our_comp (const sp_movie &lhs, const sp_movie &rhs)
{
  return *lhs < *rhs;
}

RecommenderSystem::RecommenderSystem () : _movie_map (our_comp)
{}

sp_movie RecommenderSystem::add_movie (const std::string &name, int year,
                                       const std::vector<double> &features)
{
  sp_movie new_sp_movie = std::make_shared<Movie> (name, year);
  _movie_map[new_sp_movie] = features;
  return new_sp_movie;
}

sp_movie RecommenderSystem::get_movie (const std::string &name, int year) const
{
  Movie movie_to_find (name, year);
  auto i = std::make_shared<Movie> (movie_to_find);
  auto found = _movie_map.find (i);
  if (found == _movie_map.end ())
  {
    return sp_movie (nullptr);
  }
  return found->first;
}

std::ostream &operator<< (std::ostream &os,
                          const RecommenderSystem &recommender_system)
{
  for (const auto &rs: recommender_system._movie_map)
  {
    os << *(rs.first);
  }
  return os;
}

double RecommenderSystem::calc_avg (rank_map &user_rank_map)
{
  double sum = 0;
  int num_of_movies = 0;
  for (const auto &cur: user_rank_map)
  {
    sum += cur.second;
    num_of_movies++;
  }
  if (num_of_movies == 0)
  {
    throw std::logic_error (ZERO_DIVISION_MSG);
  }
  return sum / num_of_movies;
}

RecommenderSystem::vec_of_doubles RecommenderSystem::multiply_scalar_vector
    (double num, const vec_of_doubles &vec)
{
  vec_of_doubles new_vec = vec;
  for (double &i: new_vec)
  {
    i = i * num;
  }
  return new_vec;
}

RecommenderSystem::vec_of_doubles &RecommenderSystem::add_vectors
    (vec_of_doubles &first_vec, const vec_of_doubles &second_vec)
{
  if (first_vec.empty ())
  {
    first_vec = second_vec;
    return first_vec;
  }
  for (int i = 0; i < (int) first_vec.size (); i++)
  {
    first_vec[i] += second_vec[i];
  }
  return first_vec;
}

RecommenderSystem::vec_of_doubles RecommenderSystem::create_pref_vec
    (rank_map &user_rank_map)
{
  vec_of_doubles pref_vec;
  for (auto &cur_movie: user_rank_map)
  {
    vec_of_doubles multiply_by_scalar = multiply_scalar_vector
        (cur_movie.second, _movie_map[cur_movie.first]);
    add_vectors (pref_vec, multiply_by_scalar);
  }
  return pref_vec;
}

double
RecommenderSystem::dot_prod (const vec_of_doubles &first_vec,
                             const vec_of_doubles &second_vec)
{
  double sum = 0;
  for (int i = 0; i < (int) first_vec.size (); i++)
  {
    sum += first_vec[i] * second_vec[i];
  }
  return sum;
}

double RecommenderSystem::calc_norm (const vec_of_doubles &first_vec)
{

  return std::sqrt (dot_prod (first_vec, first_vec));

}
double RecommenderSystem::calc_angle (const vec_of_doubles &first_vec,
                                      const vec_of_doubles &second_vec)
{

  return dot_prod (first_vec, second_vec) /
         (calc_norm (first_vec) * calc_norm (second_vec));
}

RecommenderSystem::vec_movies RecommenderSystem::
create_vec_didnt_watch (rank_map user_rank_map)
{
  vec_movies movies_didnt_watch;
  for (auto &cur_movie: _movie_map)
  {
    if (user_rank_map.find (cur_movie.first) == user_rank_map.end ())
    {
      movies_didnt_watch.push_back (cur_movie.first);
    }
  }
  return movies_didnt_watch;
}

RecommenderSystem::vec_of_pairs RecommenderSystem::
calc_similarity (const vec_of_doubles &pref_vec,
                 const vec_movies &vec_movies)
{
  vec_of_pairs final_vector;
  for (const auto &cur_movie: vec_movies)
  {
    double tmp_angle = calc_angle (pref_vec, _movie_map[cur_movie]);
    final_vector.emplace_back (cur_movie, tmp_angle);
  }
  std::sort (final_vector.begin (), final_vector.end (), comp_angles ());
  return final_vector;
}

sp_movie RecommenderSystem::recommend_by_content (const RSUser &user)
{
  rank_map user_rank_map = user.get_ranks ();
  double avg_ranks = calc_avg (user_rank_map);
  //subtract avg
  for (auto &cur_movie: user_rank_map)
  {
    cur_movie.second = cur_movie.second - avg_ranks;
  }
  vec_of_doubles pref_vec = create_pref_vec (user_rank_map);

  vec_movies movies_didnt_watch = create_vec_didnt_watch (user_rank_map);

  vec_of_pairs similarity_vec = calc_similarity (pref_vec, movies_didnt_watch);

  return similarity_vec.at (0).first;
}

double RecommenderSystem::predict_movie_score (const RSUser &user,
                                               const sp_movie &movie,
                                               int k)
{
  rank_map user_rank_map = user.get_ranks ();

  auto movie_features = _movie_map.find (movie)->second;
  vec_movies vec_user;
  for (const auto &cur_rank: user_rank_map)
  {
    vec_user.push_back (cur_rank.first);
  }

  vec_of_pairs similarity_vec = calc_similarity (movie_features, vec_user);
  similarity_vec.resize (k);
  double sum_s = 0;
  double sum_s_r = 0;
  for (auto &pair: similarity_vec)
  {
    sum_s += pair.second;
    double pair_rank = user_rank_map[pair.first];
    sum_s_r += (pair.second * pair_rank);
  }
  if (sum_s == 0.0)
  {
    throw std::logic_error (ZERO_DIVISION_MSG);
  }
  return sum_s_r / sum_s;
}

sp_movie RecommenderSystem::recommend_by_cf (const RSUser &user, int k)
{
  const rank_map& user_rank_map = user.get_ranks ();
  vec_movies movies_didnt_watch = create_vec_didnt_watch (user_rank_map);
  double max_score = 0;
  sp_movie best_movie = nullptr;
  for (const auto &cur_movie: movies_didnt_watch)
  {
    double cur_score = predict_movie_score (user, cur_movie, k);
    if (cur_score > max_score)
    {
      max_score = cur_score;
      best_movie = cur_movie;
    }
  }
  return best_movie;
}