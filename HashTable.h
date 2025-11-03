/**
 * Project: Hash Table
 * Amanda Milovich
*/

#pragma once
#include <string>
#include <optional>
#include <vector>
#include <ostream>


class HashTable {
public:
    HashTable(size_t initCapacity = 8);
    bool insert(const std::string &key, const size_t &value);
    bool remove(const std::string &key);
    bool contains(const std::string &key) const;
    std::optional<size_t> get(const std::string &key) const;
    size_t &operator[](const std::string &key);
    std::vector<std::string> keys() const;
    double alpha() const;
    size_t capacity() const;
    size_t size() const;

private:
    enum class BucketType {
        NORMAL,
        EMPTY_SINCE_START,
        EMPTY_SINCE_REMOVAL
    };

    class HashTableBucket {
    public:
        HashTableBucket();
        HashTableBucket(const std::string &key, const size_t &value);
        void load(const std::string &key, const size_t &value);
        bool isEmpty() const;
        friend std::ostream &operator<<(std::ostream &os, const HashTableBucket &bucket);
        std::string getKey() const;
        size_t getValue() const;

    private:
        std::string key;
        size_t value;
        BucketType type;
    };
};


