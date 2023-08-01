//
// Created by liora on 07/09/2022.
//

#ifndef _HASHMAP_HPP_
#define _HASHMAP_HPP_

#include <map>
#include <vector>
#include <algorithm>

#define  LENGTH_ERROR_MSG "VECTORS HAS DIFFERENT SIZE!"
#define INVALID_KEY "Invalid key- Key not found!"
#define INIT_CAPACITY 16
#define UPPER_FACTOR 0.75
#define LOWER_FACTOR 0.25

using std::vector;
using std::pair;

template<typename KeyT, typename ValueT>

class HashMap
{
 private:
  /**
   * this function rehashes the HashMap if needed.
   * @param key -if need to insert.
   * @param value - value of the key.
   * @param mode - insert or erase.
   * @return true if successfully rehashed
   */
  bool rehash (const KeyT &key, const ValueT &value, const std::string &mode);

 protected:
  vector<pair<KeyT, ValueT>> *_arr;
  int _size;
  int _capacity;

 public:
  /** default constructor. */
  HashMap ();
  /**
   * constructor that receives two vectors and saves them in the array.
   * @param vec_keys-vector of keys
   * @param vec_vals -vector of values
   */
  HashMap (const vector<KeyT> &vec_keys, const vector<ValueT> &vec_vals);
  /**
   * copy constructor.
   * @param hash
   */
  HashMap (const HashMap &other);
  /**
   * destructor.
   */
  virtual ~HashMap ();
  /**
   * @return the size of the HashMap.
   */
  int size () const;
  /**
   * @return the capacity of the HashMap.
   */
  int capacity () const;
  /**
   * @return true if the HashMap is empty.
   */
  bool empty () const;
  /**
   * insert the key and value to the HashMap, only if its not already in it.
   * @param key
   * @param value
   * @return true if successfully inserted.
   */
  bool insert (const KeyT &key, const ValueT &value);
  /**
   * @param key to search for.
   * @return true if the HashMap contains the key.
   */
  bool contains_key (const KeyT &key) const;
  /**
   * gets a key and returns the value of it. throws exception if key not found.
   * @param key- to search for.
   * @return the value if the key was found.
   */
  ValueT &at (const KeyT &key);
  /**
   * gets a key and returns the value of it with no option to change it.
   * throws exception if key not found.
   * @param key- to search for.
   * @return the value if the key was found.
   */
  const ValueT &at (const KeyT &key) const;
  /**
   * gets a key and removes it and its value from the HashMap.
   * @param key to remove
   * @return true if successfully removed the key.
   */
  virtual bool erase (const KeyT &key);
  /**
   * @return the load factor -number of elements divided by the capacity
   */
  double get_load_factor () const;
  /**
   * gets a key and returns its bucket size. throws exception if the key is
   * not found.
   * @param key
   * @return the key bucket size.
   */
  int bucket_size (const KeyT &key) const;
  /**
   * gets a key and returns the index of its bucket. throws exception if the
   * key is not found.
   * @param key
   * @return the index of the key bucket.
   */
  int bucket_index (const KeyT &key) const;
  /** clears the HashMap. */
  void clear ();
  /**
   * assignment operator.
   * @param other- other HashMap
   * @return reference to the HashMap with the new data.
   */
  HashMap<KeyT, ValueT> &operator= (const HashMap &other);
  /**
   * subscript operator. no bound checking.
   * @param key
   * @return the value of the key.
   */
  ValueT &operator[] (const KeyT &key);
  /**
 * subscript operator-with no option to change it. no bound checking.
 * @param key
 * @return the value of the key.
 */
  const ValueT &operator[] (const KeyT &key) const;
  /**
   * comparison operator
   * @param other HashMap to compare
   * @return true if the HashMaps contain the same elements.
   */
  bool operator== (const HashMap &other) const;
  /**
   * comparison operator.
   * @param other HashMap to compare
   * @return false if the HashMaps contain the same elements
   */
  bool operator!= (const HashMap &other) const;

  /** Const_Iterator class.- const forward iterator. */
  class ConstIterator
  {
   public:
    /** iterator traits */
    typedef std::pair<KeyT, ValueT> value_type;
    typedef const std::pair<KeyT, ValueT> &reference;
    typedef const std::pair<KeyT, ValueT> *pointer;
    typedef std::ptrdiff_t difference_type;
    typedef std::forward_iterator_tag iterator_category;
    /**
     * ConstIterator constructor.
     * @param hash_ptr - a pointer to the HashMap to iterate.
     * @param buck_ind - the index of the bucket we iterate.
     * @param pair_ind - the index of the current pair.
     * @param is_null - does the iterator points to empty bucket.
     */
    ConstIterator (const HashMap<KeyT, ValueT> &hash_ptr, int buck_ind,
                   int pair_ind, bool is_null)
        : _hash_ptr (hash_ptr), _buck_ind (buck_ind), _pair_ind (pair_ind),
          _is_null (is_null)
    {}
    /**
     * @return a reference to the current pair.
     */
    reference operator* () const
    {
      return (_hash_ptr._arr[_buck_ind][_pair_ind]);
    }
    /**
     * @return a pointer to the current pair.
     */
    pointer operator-> () const
    {
      return &(operator* ());
    }
    /**
     * moves the iterator forward. pre-increment.
     * @return Const iterator& to the next pair.
     */
    ConstIterator &operator++ ()
    {
      if (_buck_ind >= _hash_ptr._capacity)
      {
        _is_null = true;
        return *this;
      }
      auto cur_buck = _hash_ptr._arr[_buck_ind];
      if (_pair_ind >= (int) cur_buck.size () - 1)
      {//find the next pair
        _buck_ind++;
        _pair_ind = 0;
        if (_buck_ind == _hash_ptr._capacity)
        {
          _is_null = true;
          return *this;
        }
        cur_buck = _hash_ptr._arr[_buck_ind];
        while (cur_buck.empty () && _buck_ind != _hash_ptr._capacity)
        {
          _buck_ind++;
          _pair_ind = 0;
          if (_buck_ind == _hash_ptr._capacity)
          {
            _is_null = true;
            break;
          }
          cur_buck = _hash_ptr._arr[_buck_ind];
        }
      }
      else
      {
        _pair_ind++;
      }
      return *this;
    }
    /**
     * moves the iterator forward.-post incremented
     * @return ConstIterator.
     */
    ConstIterator operator++ (int)
    {
      ConstIterator tmp = *this;
      ++(*this);
      return tmp;
    }
    /**
     * comparison operator.
     * @param rhs- another iterator to compare
     * @return true if equal.
     */
    bool operator== (const ConstIterator &rhs) const
    {
      return &_hash_ptr == &rhs._hash_ptr && _buck_ind == rhs._buck_ind
             && _pair_ind == rhs._pair_ind;
    }
    /**
     * comparison operator.
     * @param rhs- another iterator to compare
     * @return false if equal.
     */
    bool operator!= (const ConstIterator &rhs) const
    {
      return !operator== (rhs);
    }

   private:
    const HashMap<KeyT, ValueT> &_hash_ptr;
    int _buck_ind;
    int _pair_ind;
    bool _is_null;

  };

/** typedef for the const iterator. */
  typedef ConstIterator const_iterator;
/**
 * @return const_iterator to the beginning of the HashMap.
 */
  const_iterator begin () const
  {
    if (empty ())
    {
      return ConstIterator (*this, 0, 0, true);
    }
    pair<KeyT, ValueT> start;
    int buc_ind = 0;
    for (int i = 0; i < _capacity; i++)
    {
      if (_arr[i].size () > 0)
      {
        buc_ind = i;
        break;
      }
    }
    return ConstIterator (*this, buc_ind, 0, false);
  }
/**
 * @return const_iterator to the end of the HashMap.
 */
  const_iterator end () const
  {
    if (empty ())
    {
      return ConstIterator (*this, 0, 0, true);
    }
    return ConstIterator (*this, _capacity, 0, true);
  }
/**
 * @return const_iterator to the beginning of the HashMap.
 */
  const_iterator cbegin () const
  {
    return begin ();
  }
/**
 * @return const_iterator to the end of the HashMap.
 */
  const_iterator cend () const
  {
    return end ();
  }
};

template<typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>::HashMap ():
    _arr (new vector<pair<KeyT, ValueT>>[INIT_CAPACITY]),
    _size (0), _capacity (INIT_CAPACITY)
{}

template<typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>::HashMap (const vector<KeyT> &vec_keys,
                                const vector<ValueT> &vec_vals)
{
  if (vec_keys.size () != vec_vals.size ())
  {
    throw std::length_error (LENGTH_ERROR_MSG);
  }
  vector<pair<KeyT, ValueT>> keys_vals_updated;
  for (int i = 0; i < (int) vec_keys.size (); i++)
  {
    pair<KeyT, ValueT> cur_pair = {vec_keys[i], vec_vals[i]};

    auto find_cur = std::find_if
        (keys_vals_updated.begin (), keys_vals_updated.end (), [&cur_pair]
            (const pair<KeyT, ValueT> &element)
        {
            return element.first ==
                   cur_pair.first;
        });

    if (find_cur == keys_vals_updated.end ())
    {//not found
      keys_vals_updated.emplace_back (cur_pair);
    }
    else
    {//found-then replace
      (*find_cur).second = vec_vals[i];
    }
  }
  _size = keys_vals_updated.size ();
  _capacity = INIT_CAPACITY;
  while (keys_vals_updated.size () > _capacity * (UPPER_FACTOR))
  {
    _capacity *= 2;
  }
  _arr = new vector<pair<KeyT, ValueT>>[_capacity];
  for (int i = 0; i < _size; i++)
  {
    //calc the index using hash
    std::hash<KeyT> key_t_hash;
    int ind = key_t_hash (keys_vals_updated.at (i).first) & (_capacity - 1);
    _arr[ind].emplace_back (keys_vals_updated.at (i));
  }
}

template<typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>::HashMap (const HashMap &other)
{
  _arr = new vector<pair<KeyT, ValueT>>[other._capacity];
  _size = other._size;
  _capacity = other._capacity;
  for (int i = 0; i < _capacity; i++)
  {
    _arr[i] = other._arr[i];
  }
}

template<typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>::~HashMap ()
{
  delete[] _arr;
}

template<typename KeyT, typename ValueT>
int HashMap<KeyT, ValueT>::size () const
{
  return _size;
}

template<typename KeyT, typename ValueT>
int HashMap<KeyT, ValueT>::capacity () const
{
  return _capacity;
}

template<typename KeyT, typename ValueT>
double HashMap<KeyT, ValueT>::get_load_factor () const
{
  return (double) size () / capacity ();
}

template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::empty () const
{
  return size () == 0;
}

template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::contains_key (const KeyT &key) const
{
  std::hash<KeyT> key_t_hash;
  int ind = key_t_hash (key) & (_capacity - 1);
  if (_arr[ind].size () <= 0)
  {
    return false;
  }
  for (const auto &element: _arr[ind])
  {
    if (element.first == key)
    {
      return true;
    }
  }
  return false;
}

template<typename KeyT, typename ValueT>
int HashMap<KeyT, ValueT>::bucket_size (const KeyT &key) const
{
  if (!contains_key (key))
  {
    throw std::out_of_range (INVALID_KEY);
  }
  int buc_ind = bucket_index (key);
  return (int) _arr[buc_ind].size ();
}

template<typename KeyT, typename ValueT>
int HashMap<KeyT, ValueT>::bucket_index (const KeyT &key) const
{
  if (!contains_key (key))
  {
    throw std::out_of_range (INVALID_KEY);
  }
  std::hash<KeyT> key_t_hash;
  int ind = key_t_hash (key) & (_capacity - 1);
  return ind;
}

template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::rehash (const KeyT &key, const ValueT &value,
                                    const std::string &mode)
{
  vector<pair<KeyT, ValueT>> vec_key_vals;
  for (int i = 0; i < _capacity; i++)
  {
    for (auto &cur_pair: _arr[i])
    {
      vec_key_vals.emplace_back (cur_pair.first, cur_pair.second);
    }
  }
  if (mode == "insert")
  {
    vec_key_vals.emplace_back (key, value);
    _capacity *= 2;
  }
  else
  {
    _capacity /= 2;
  }
  delete[] _arr;
  _arr = new vector<pair<KeyT, ValueT>>[_capacity];
  for (int i = 0; i < _size; i++)
  {
    std::hash<KeyT> key_t_hash;
    int ind = key_t_hash (vec_key_vals.at (i).first) & (_capacity - 1);
    _arr[ind].emplace_back (vec_key_vals.at (i));
  }
  return true;
}

template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::insert (const KeyT &key, const ValueT &value)
{
  if (contains_key (key))
  {
    return false;
  }
  _size++;
  while (get_load_factor () > UPPER_FACTOR)
  {
    return rehash (key, value, "insert");
  }
  std::hash<KeyT> key_t_hash;
  int ind = key_t_hash (key) & (_capacity - 1);
  _arr[ind].emplace_back (key, value);
  return true;
}

template<typename KeyT, typename ValueT>
ValueT &HashMap<KeyT, ValueT>::at (const KeyT &key)
{
  if (!contains_key (key))
  {
    throw std::out_of_range (INVALID_KEY);
  }
  int buc_ind = bucket_index (key);
  for (auto &element: _arr[buc_ind])
  {
    if (element.first == key)
    {
      return element.second;
    }
  }
  throw std::out_of_range (INVALID_KEY);
}

template<typename KeyT, typename ValueT>
const ValueT &HashMap<KeyT, ValueT>::at (const KeyT &key) const
{
  if (!contains_key (key))
  {
    throw std::out_of_range (INVALID_KEY);
  }
  int buc_ind = bucket_index (key);
  for (auto &element: _arr[buc_ind])
  {
    if (element.first == key)
    {
      return element.second;
    }
  }
  throw std::out_of_range (INVALID_KEY);
}

template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::erase (const KeyT &key)
{
  if (!contains_key (key))
  {
    return false;
  }
  int buc_ind = bucket_index (key);
  for (auto element = _arr[buc_ind].begin (); element < _arr[buc_ind].end ();
       element++)
  {
    if (element->first == key)
    {
      _arr[buc_ind].erase (element);
    }
  }
  _size--;
  while (get_load_factor () < LOWER_FACTOR && _capacity > 1)
  {
    if (!rehash (KeyT (), ValueT (), "erase"))
    {
      return false;
    }
  }
  return true;
}

template<typename KeyT, typename ValueT>
void HashMap<KeyT, ValueT>::clear ()
{
  _size = 0;
  for (int i = 0; i < _capacity; i++)
  {
    _arr[i].clear ();
  }
}

template<typename KeyT, typename ValueT>
HashMap<KeyT, ValueT> &HashMap<KeyT, ValueT>::operator= (const HashMap &other)
{
  if (this == &other)
  {
    return *this;
  }
  delete[] _arr;
  _capacity = other._capacity;
  _size = other._size;
  _arr = new vector<pair<KeyT, ValueT>>[_capacity];
  for (int i = 0; i < _capacity; i++)
  {
    _arr[i] = other._arr[i];
  }
  return *this;
}

template<typename KeyT, typename ValueT>
ValueT &HashMap<KeyT, ValueT>::operator[] (const KeyT &key)
{
  if (!contains_key (key))
  {
    insert (key, ValueT ());
  }
  return at (key);
}

template<typename KeyT, typename ValueT>
const ValueT &HashMap<KeyT, ValueT>::operator[] (const KeyT &key) const
{
  if (!contains_key (key))
  {
    throw std::out_of_range (INVALID_KEY);
  }
  return at (key);
}

template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::operator== (const HashMap &other) const
{
  if (_size != other._size)
  {
    return false;
  }
  for (int i = 0; i < _capacity; i++)
  {
    for (const auto &p: _arr[i])
    {
      if (!(other.contains_key (p.first)) || other.at (p.first) != p.second)
      {
        return false;
      }
    }
  }
  return true;
}

template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::operator!= (const HashMap &other) const
{
  return !operator== (other);
}

#endif //_HASHMAP_HPP_
