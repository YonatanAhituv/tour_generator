#ifndef HASHMAP_H
#define HASHMAP_H
#include <functional>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "base_classes.h"
template <typename T>
class HashMap {
 public:
  HashMap(double max_load = 0.75);  // constructor
  ~HashMap();  // destructor; deletes all of the items in the hashmap
  // We cannot copy hashmaps
  HashMap(const HashMap&) = delete;
  HashMap& operator=(const HashMap&) = delete;
  int size() const {  // return the number of associations in the hashmap
    return m_size;
  };
  // The insert method associates one item (key) with another (value).
  // If no association currently exists with that key, this method inserts
  // a new association into the hashmap with that key/value pair. If there is
  // already an association with that key in the hashmap, then the item
  // associated with that key is replaced by the second parameter (value).
  // Thus, the hashmap must contain no duplicate keys.
  void insert(const std::string& key, const T& value);
  // Defines the bracket operator for HashMap, so you can use your map like
  // this: your_map["david"] = 2.99; If the key does not exist in the hashmap,
  // this will create a new entry in the hashmap and map it to the default value
  // of type T (0 for builtin types). It returns a reference to the newly
  // created value in the map.
  T& operator[](const std::string& key);
  // If no association exists with the given key, return nullptr; otherwise,
  // return a pointer to the value associated with that key. This pointer can be
  // used to examine that value within the map.
  const T* find(const std::string& key) const;
  // If no association exists with the given key, return nullptr; otherwise,
  // return a pointer to the value associated with that key. This pointer can be
  // used to examine that value or modify it directly within the map.
  T* find(const std::string& key) {
    const auto& hm = *this;
    return const_cast<T*>(hm.find(key));
  }

 private:
  unsigned int generateHash(
      const std::string key) const;  // Varies on number of buckets
  struct Node {
    Node(std::string keyGiven, T valueGiven)
        : key(keyGiven), value(valueGiven){};
    std::string key;
    T value;
  };
  std::list<Node*>* m_map;
  double m_load;
  int m_buckets = 10;  // We start with 10 buckets
  int m_size = 0;
};
template <typename T>
HashMap<T>::HashMap(double max_load) {
  m_load = max_load;
  m_map =
      new std::list<Node*>[m_buckets];  // Allocate new array of linked lists
}
template <typename T>
HashMap<T>::~HashMap() {
  for (int i = 0; i < m_buckets; i++) {
    for (typename std::list<Node*>::iterator p = m_map[i].begin();
         p != m_map[i].end(); p++) {
      delete (*p);  // Delete dynamically allocated nodes
    }
  }
  delete[] m_map;  // Delete dynamically allocated array
}
template <typename T>
unsigned int HashMap<T>::generateHash(const std::string key) const {
  return std::hash<std::string>()(key) %
         m_buckets;  // Ensure hash is btwn. 0-m_buckets-1
}
template <typename T>
void HashMap<T>::insert(const std::string& key, const T& value) {
  // Add new value
  typename std::list<Node*>* bucket = &m_map[generateHash(
      key)];  // Get matching bucket and save it as a reference
  typename std::list<Node*>::iterator p = bucket->begin();
  for (; p != bucket->end(); p++) {
    if ((*p)->key == key) {
      (*p)->value = value;  // Update key value in bucket
      return;
    }
  }
  if (p == bucket->end()) {  // Key is unique since the for loop ran through the
                             // entire bucket
    m_size++;
    if ((m_size / m_buckets) > m_load) {  // Reallocate if needed
      m_buckets *= 2;                     // Double buckets
      typename std::list<Node*>* new_map =
          new typename std::list<Node*>[m_buckets];  // Allocate new array
      for (int i = 0; i < m_buckets / 2; i++) {
        for (typename std::list<Node*>::iterator p = m_map[i].begin();
             p != m_map[i].end(); p++) {
          new_map[generateHash((*p)->key)].push_back((*p));
        }
      }
      delete[] m_map;
      m_map = new_map;  // Update pointer to new map
      bucket = &m_map[generateHash(key)];
    }
    Node* newNode = new Node(key, value);
    bucket->push_back(newNode);  // Add new node
  }
}
template <typename T>
const T* HashMap<T>::find(const std::string& key) const {
  typename std::list<Node*> bucket = m_map[generateHash(key)];
  for (typename std::list<Node*>::iterator p = bucket.begin();
       p != bucket.end(); p++) {
    if ((*p)->key == key) {
      return &((*p)->value);
    }
  }
  return nullptr;  // No match
}
template <typename T>
T& HashMap<T>::operator[](const std::string& key) {
  T* item = find(key);
  if (item == nullptr) {
    insert(key, T());
    item = find(key);
  }
  return *(item);
}
#endif