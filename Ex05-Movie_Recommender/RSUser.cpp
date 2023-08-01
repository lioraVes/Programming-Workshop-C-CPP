//
// Created on 2/20/2022.
//

#include "RSUser.h"
#include "RecommenderSystem.h"
#include <cmath>

RSUser::RSUser (std::string username, rank_map user_ranks, sp_rec_system
sp_recommender_system) : _username (username), _user_ranks (user_ranks),
                         _sp_recommender_system (sp_recommender_system)
{
}

std::string RSUser::get_name () const
{
  return _username;
}

sp_movie RSUser::get_recommendation_by_content () const
{
  return _sp_recommender_system->recommend_by_content (*this);
}

sp_movie RSUser::get_recommendation_by_cf (int k) const
{
  return _sp_recommender_system->recommend_by_cf (*this, k);
}

double RSUser::get_prediction_score_for_movie (const std::string &name,
                                               int year, int k) const
{
  sp_movie cur_movie = _sp_recommender_system->get_movie (name, year);
  return _sp_recommender_system->predict_movie_score (*this, cur_movie, k);
}

std::ostream &operator<< (std::ostream &os, const RSUser &user)
{
  os << "name: " << user._username << std::endl;
  os << *(user._sp_recommender_system);
  os << std::endl;
  return os;
}

const rank_map& RSUser::get_ranks () const
{
  return _user_ranks;
}

void RSUser::add_movie_to_rs (const std::string &name, int year,
                              const std::vector<double> &features,
                              double rate)
{
  sp_movie new_movie = _sp_recommender_system->add_movie (name, year,
                                                          features);
  if(std::isnan (rate))
  {
    return;
  }
  _user_ranks[new_movie] = rate;
}