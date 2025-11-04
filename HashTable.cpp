#include "HashTable.h"

#include <algorithm>
#include <random>

HashTable::HashTable(size_t initCapacity)
    : buckets(initCapacity), // sets the number of buckets in the HashMap
      offsets(initCapacity), // sets the number of numbers used for pseudo random probing
      fullBuckets(0) // sets the initial number of full buckets
{
    std::iota(offsets.begin(), offsets.end(), 0); // fills the offsets with numbers 0-initCapacity
    std::shuffle(++offsets.begin(), offsets.end(), std::mt19937(std::random_device()())); // shuffles offsets
}

size_t HashTable::probe(std::string key, size_t i) const {
    size_t hashedKey = hasher(key); // hashes key into size_t so it can go through pseudo-random probing
    hashedKey = (hashedKey + offsets[i]) % buckets.size();
    return hashedKey;
}

bool HashTable::insert(const std::string &key, const size_t &value) {
    if (contains(key)) // does not insert new key-value pair if key is already present
        return false;
    bool inserted = false;
    for (int i = 0; !inserted && i < buckets.size(); i++) {
        size_t insertBucket = probe(key, i);
        if (buckets[insertBucket].isEmpty()) { // checks if the bucket the probing found is full
            buckets[insertBucket].load(key, value);
            fullBuckets++;
            inserted = true;
        }
    }
    if (alpha() >= .5)
        resize(buckets.size() * 2);
    return inserted; // informs method call if the key-value pair was successfully inserted
}

bool HashTable::remove(const std::string &key) {
    bool removed = false;
    for (int i = 0; !removed && i < buckets.size(); i++) {
        size_t removeBucket = probe(key, i); // key-value pair to remove
        if (buckets[removeBucket].getKey() == key) {
            buckets[removeBucket].kill();
            fullBuckets--;
            removed = true;
        }
    }
    return removed; // informs method call if the key-value pair was successfully removed
}

bool HashTable::contains(const std::string &key) const {
    bool found = false;
    for (int i = 0; !found && i < buckets.size(); i++) {
        size_t keyHash = probe(key, i);
        if (buckets[keyHash].getKey() == key)
            found = true;
    }
    return found; // informs method call if the key was found
}

std::optional<size_t> HashTable::get(const std::string &key) const {
    if (!contains(key))
        return std::nullopt; // if the list does not contain the key, returns nothing
    for (int i = 0; i < buckets.size(); i++) {
        size_t keyHash = probe(key, i);
        if (buckets[keyHash].getKey() == key)
            return std::make_optional(buckets[keyHash].getValue()); // returns value for specified key
    }
    return std::nullopt; // code should not reach here, but if it does, returns nothing
}

size_t &HashTable::operator[](const std::string &key) {
    if (!contains(key))
        insert(key, 0);
    for (int i = 0; i < buckets.size(); i++) {
        size_t keyHash = probe(key, i);
        if (buckets[keyHash].getKey() == key)
            return *buckets[keyHash]; // returns reference to the value
    }
}

std::vector<std::string> HashTable::keys() const {
    std::vector<std::string> keys;
    for (int i = 0; i < buckets.size(); i++) {
        if (buckets[i].isEmpty())
            continue; // if the bucket is empty it will not add to the vector
        keys.push_back(buckets[i].getKey()); // adds non-empty buckets to the vector
    }
    return keys; // returns the vector of keys
}

double HashTable::alpha() const {
    return static_cast<double>(fullBuckets) / static_cast<double>(buckets.size()); // calculates fullness of Hashmap and returns it
}

size_t HashTable::capacity() const {
    return buckets.size(); // returns the amount of buckets in the HashMap
}

size_t HashTable::size() const {
    return fullBuckets; // returns the amount of currently filled buckets in the HashMap
}

size_t &HashTable::HashTableBucket::operator*() {
    return value; // returns a non-const l-value reference to the value
}

HashTable::HashTableBucket::HashTableBucket() // default bucket constructor
    : key(""),
      value(0),
      type(BucketType::EMPTY_SINCE_START) {
}

HashTable::HashTableBucket::HashTableBucket(const std::string &key, const size_t &value) // non-default bucket constructor
    : key(key),
      value(value),
      type(BucketType::EMPTY_SINCE_START) {
}

void HashTable::HashTableBucket::load(const std::string &newKey, const size_t &newValue) {
    key = newKey;
    value = newValue; // loads bucket with new key-value pair
    type = BucketType::NORMAL; // sets bucket flag to filled
}

void HashTable::HashTableBucket::kill() {
    key = "";
    value = 0; // returns the key-value pair to its default state
    type = BucketType::EMPTY_SINCE_REMOVAL; // places a tombstone on the bucket
}

bool HashTable::HashTableBucket::isEmpty() const {
    return type != BucketType::NORMAL; // checks if a bucket is currently filled
}

std::string HashTable::HashTableBucket::getKey() const {
    return key;
}

size_t HashTable::HashTableBucket::getValue() const {
    return value;
}

std::ostream &operator<<(std::ostream &os, const HashTable &obj) {
    for (const auto & bucket : obj.buckets) { // iterates over the whole vector to print it
        if (bucket.isEmpty()) // does not print empty buckets
            continue;
        os << "Key: " << bucket.getKey() << " -- Value: " << bucket.getValue() << std::endl; // print format for buckets
    }
    return os;
}

void HashTable::resize(size_t newCapacity) {
    HashTable temp(newCapacity); // creates a temporary vector to hold the values in the new size
    for (const HashTableBucket &bucket : buckets) {
        if (bucket.isEmpty())
            continue; // does not copy over empty buckets
        temp.insert(bucket.getKey(), bucket.getValue()); // copies fills buckets
    }
    *this = std::move(temp); // copies temp vector onto the permanent one
}

