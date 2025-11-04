/**
 * Project: Hash Table
 * Amanda Milovich
 * Program is a hash map written with a vector as the base.
 * Program takes in key-value pairs, checks if the key is already present, and adds the pair if it is not.
 * Program also takes in keys to remove. If the key is not present the program will not attempt to remove it.
 * Program will automatically resize the vector if it becomes half full or more.
*/

#pragma once
#include <string>
#include <optional>
#include <vector>
#include <ostream>


class HashTable {
public:
    explicit HashTable(size_t initCapacity = 8); // Constructor for the initial HashTable
    bool insert(const std::string &key, const size_t &value); // Inserts new key-value pair
    bool remove(const std::string &key); // Removes key-value pair if present
    [[nodiscard]] bool contains(const std::string &key) const; // Checks if key-value pair is present
    [[nodiscard]] std::optional<size_t> get(const std::string &key) const; // Gets key-value pair by key
    size_t &operator[](const std::string &key); // Overrides [] operator to get references to keys
    [[nodiscard]] std::vector<std::string> keys() const; // Returns a list of present keys
    [[nodiscard]] double alpha() const; // Returns how full the HashMap currently is
    [[nodiscard]] size_t capacity() const; // Returns how many buckets are currently filled
    [[nodiscard]] size_t size() const; // Returns how many buckets are in the HashMap
    friend std::ostream &operator<<(std::ostream &os, const HashTable &obj); // Overrides the << operator
    void resize(size_t newCapacity); // Doubles the size of the HashMap and copies over present key-value pairs

private:
    enum class BucketType { // If the bucket is full, has always been empty, or was filled and emptied
        NORMAL,
        EMPTY_SINCE_START,
        EMPTY_SINCE_REMOVAL
    };

    [[nodiscard]] size_t probe(std::string key, size_t i) const; // Locates possible indices for a key


    class HashTableBucket {
    public:
        HashTableBucket(); // default constructor for a bucket
        HashTableBucket(const std::string &key, const size_t &value); // constructor for a bucket
        void load(const std::string &key, const size_t &value);  // adds a key-value pair to a bucket and sets it to full
        void kill(); // removes a key-value pair from a bucket and sets it to ESR
        [[nodiscard]] bool isEmpty() const; // returns whether a bucket is empty
        friend std::ostream &operator<<(std::ostream &os, const HashTableBucket &bucket); // overrides the << operator
        [[nodiscard]] std::string getKey() const; // returns the key for a bucket
        [[nodiscard]] size_t getValue() const; // returns the value for a bucket
        size_t &operator*(); // overrides the * operator

    private:
        std::string key;
        size_t value;
        BucketType type;
    };

    // HashTableBucket parameters
    std::vector<HashTableBucket> buckets;
    std::vector<size_t> offsets;
    std::hash<std::string> hasher;
    size_t fullBuckets;

};


