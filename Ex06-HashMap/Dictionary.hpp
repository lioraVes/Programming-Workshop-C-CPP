//
// Created by liora on 10/09/2022.
//

#ifndef _DICTIONARY_HPP_
#define _DICTIONARY_HPP_
#include "HashMap.hpp"
#include <string>

/**
 * InvalidKey class. derived class from std::invalid_argument.
 */
class InvalidKey : public std::invalid_argument
{
 private:
  std::string _err_msg = INVALID_KEY;
 public:
  /**
   * default constructor.
   */
  InvalidKey () : std::invalid_argument (_err_msg)
  {}
  /**
   * constructor.
   * @param err_msg- the message to throw.
   */
  InvalidKey (const std::string &err_msg) : std::invalid_argument (err_msg)
  {};
};

class Dictionary : public HashMap<std::string, std::string>
{
 public:
  using HashMap<std::string, std::string>::HashMap;
  /**
   * gets a key and removes it and its value from the Dictionary.
   * @param key
   * @return true if successfully removed the key.
   */
  bool erase (const std::string &key) override;
  /**
   * inserts the keys,values that the iterator points on, to the Dictionary .
   * @param start - iterator to the beginning.
   * @param end - iterator to the end.
   */
  template<typename Iterator>
  void update (Iterator start, Iterator end);
};

bool Dictionary::erase (const std::string &key)
{
  if (!contains_key (key))
  {
    throw InvalidKey (INVALID_KEY);
  }
  return HashMap<std::string, std::string>::erase (key);
}

template<typename Iterator>
void Dictionary::update (Iterator start, Iterator end)
{
  while (start != end)
  {
    if (contains_key (start->first))
    {
      at (start->first) = start->second;
      start++;
      continue;
    }
    insert (start->first, start->second);
    start++;
  }
}

#endif //_DICTIONARY_HPP_
