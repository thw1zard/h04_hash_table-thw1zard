#include "hash_table.hpp"

#include <stdexcept>

namespace itis {

  int HashTable::hash(int key) const {
    return utils::hash(key, static_cast<int>(buckets_.size()));
  }

  HashTable::HashTable(int capacity, double load_factor) : load_factor_{load_factor} {
    if (capacity <= 0) {
      throw std::logic_error("hash table capacity must be greater than zero");
    }

    if (load_factor <= 0.0 || load_factor > 1.0) {
      throw std::logic_error("hash table load factor must be in range [0...1]");
    }
    buckets_.resize(capacity);
  }

  std::optional<std::string> HashTable::Search(int key) const {
    const int index = hash(key);
    for(auto iterator = buckets_[index].begin(); iterator != buckets_[index].end(); iterator++){
      if(iterator->first == key){
        return iterator->second;
      }
    }
    return std::nullopt;
  }

  void HashTable::Put(int key, const std::string &value) {
    const int index = hash(key);
    if(ContainsKey(key)) {
      for (auto iterator = buckets_[index].begin(); iterator != buckets_[index].end(); iterator++) {
        if (iterator->first == key) {
          iterator->second = value;
        }
      }
    }
    else{
      buckets_[index].push_back({key,value});
      num_keys_++;
    }
    // Tip 1: compute hash code (index) to determine which bucket to use
    // Tip 2: consider the case when the key exists (read the docs in the header file)

    if (static_cast<double>(num_keys_) / buckets_.size() >= load_factor_) {
      std::vector<Bucket> newBuckets(this->capacity() * kGrowthCoefficient);
      for(int i = 0; i < buckets_.size(); ++i){
        for(const auto &pair : buckets_[i]){
          auto newHash = utils::hash(pair.first, newBuckets.size());
          newBuckets[newHash].push_back(pair);
        }
      }
      this -> buckets_ = newBuckets;
    }
  }

  std::optional<std::string> HashTable::Remove(int key) {
    const int index = hash(key);
    std::string deletedElem = "";
    if(ContainsKey(key)){
      for(auto iterator = buckets_[index].begin(); iterator != buckets_[index].end(); iterator ++){
        if(iterator -> first == key){
          deletedElem = iterator -> second;
          buckets_[index].erase(iterator);
          num_keys_--;
          return deletedElem;
        }
      }
    }
    // Tip 1: compute hash code (index) to determine which bucket to use
    // TIp 2: find the key-value pair to remove and make a copy of value to return
    return std::nullopt;
  }

  bool HashTable::ContainsKey(int key) const {
    // Note: uses Search(key) which is not initially implemented
    return Search(key).has_value();
  }

  bool HashTable::empty() const {
    return size() == 0;
  }

  int HashTable::size() const {
    return num_keys_;
  }

  int HashTable::capacity() const {
    return static_cast<int>(buckets_.size());
  }

  double HashTable::load_factor() const {
    return load_factor_;
  }

  std::unordered_set<int> HashTable::keys() const {
    std::unordered_set<int> keys(num_keys_);
    for (const auto &bucket : buckets_) {
      for (const auto &[key, _] : bucket) {
        keys.insert(key);
      }
    }
    return keys;
  }

  std::vector<std::string> HashTable::values() const {
    std::vector<std::string> values;
    for (const auto &bucket : buckets_) {
      for (const auto &[_, value] : bucket) {
        values.push_back(value);
      }
    }
    return values;
  }

}  // namespace itis