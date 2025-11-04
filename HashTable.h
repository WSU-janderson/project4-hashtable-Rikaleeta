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
    explicit HashTable(size_t initCapacity = 8);
    bool insert(const std::string &key, const size_t &value);
    bool remove(const std::string &key);
    bool contains(const std::string &key) const;
    [[nodiscard]] std::optional<size_t> get(const std::string &key) const;
    size_t &operator[](const std::string &key);
    [[nodiscard]] std::vector<std::string> keys() const;
    [[nodiscard]] double alpha() const;
    [[nodiscard]] size_t capacity() const;
    [[nodiscard]] size_t size() const;

private:
    enum class BucketType {
        NORMAL,
        EMPTY_SINCE_START,
        EMPTY_SINCE_REMOVAL
    };

    size_t probe(std::string key, size_t i) const;

    class HashTableBucket {
    public:
        HashTableBucket();
        HashTableBucket(const std::string &key, const size_t &value);
        void load(const std::string &key, const size_t &value);
        void kill();
        bool isEmpty() const;
        friend std::ostream &operator<<(std::ostream &os, const HashTableBucket &bucket);
        std::string getKey() const;
        size_t getValue() const;
        size_t &operator*();

    private:
        std::string key;
        size_t value;
        BucketType type;
    };

    std::vector<HashTableBucket> buckets;
    std::vector<size_t> offsets;
    std::hash<std::string> hasher;
    size_t fullBuckets;

};


