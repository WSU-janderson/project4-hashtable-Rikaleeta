#include "HashTable.h"

#include <algorithm>
#include <random>

HashTable::HashTable(size_t initCapacity)
    : buckets(initCapacity),
      offsets(initCapacity),
      fullBuckets(0) {
    std::iota(offsets.begin(), offsets.end(), 0);
    std::shuffle(++offsets.begin(), offsets.end(), std::mt19937(std::random_device()()));
}

size_t HashTable::probe(std::string key, size_t i) const {
    size_t hashedKey = hasher(key);
    hashedKey = (hashedKey + offsets[i]) % buckets.size();
    return hashedKey;
}

bool HashTable::insert(const std::string &key, const size_t &value) {
    if (contains(key))
        return false;
    bool inserted = false;
    for (int i = 0; !inserted && i < buckets.size(); i++) {
        size_t insertBucket = probe(key, i);
        if (buckets[insertBucket].isEmpty()) {
            buckets[insertBucket].load(key, value);
            fullBuckets++;
            inserted = true;
        }
    }
    if (alpha() >= .5)
        resize(buckets.size() * 2);
    return inserted;
}

bool HashTable::remove(const std::string &key) {
    bool removed = false;
    for (int i = 0; !removed && i < buckets.size(); i++) {
        size_t removeBucket = probe(key, i);
        if (buckets[removeBucket].getKey() == key) {
            buckets[removeBucket].kill();
            fullBuckets--;
            removed = true;
        }
    }
    return removed;
}

bool HashTable::contains(const std::string &key) const {
    bool found = false;
    for (int i = 0; !found && i < buckets.size(); i++) {
        size_t keyHash = probe(key, i);
        if (buckets[keyHash].getKey() == key)
            found = true;
    }
    return found;
}

std::optional<size_t> HashTable::get(const std::string &key) const {
    if (!contains(key))
        return std::nullopt;
    for (int i = 0; i < buckets.size(); i++) {
        size_t keyHash = probe(key, i);
        if (buckets[keyHash].getKey() == key)
            return std::make_optional(buckets[keyHash].getValue());
    }
    return std::nullopt;
}

size_t &HashTable::operator[](const std::string &key) {
    if (!contains(key))
        insert(key, 0);
    for (int i = 0; i < buckets.size(); i++) {
        size_t keyHash = probe(key, i);
        if (buckets[keyHash].getKey() == key)
            return *buckets[keyHash];
    }
}

std::vector<std::string> HashTable::keys() const {
    std::vector<std::string> keys;
    for (int i = 0; i < buckets.size(); i++) {
        if (buckets[i].isEmpty())
            continue;
        keys.push_back(buckets[i].getKey());
    }
    return keys;
}

double HashTable::alpha() const {
    return static_cast<double>(fullBuckets) / static_cast<double>(buckets.size());
}

size_t HashTable::capacity() const {
    return buckets.size();
}

size_t HashTable::size() const {
    return fullBuckets;
}

size_t &HashTable::HashTableBucket::operator*() {
    return value;
}

HashTable::HashTableBucket::HashTableBucket()
    : key(""),
      value(0),
      type(BucketType::EMPTY_SINCE_START) {
}

HashTable::HashTableBucket::HashTableBucket(const std::string &key, const size_t &value)
    : key(key),
      value(value),
      type(BucketType::EMPTY_SINCE_START) {
}

void HashTable::HashTableBucket::load(const std::string &newKey, const size_t &newValue) {
    key = newKey;
    value = newValue;
    type = BucketType::NORMAL;
}

void HashTable::HashTableBucket::kill() {
    key = "";
    value = 0;
    type = BucketType::EMPTY_SINCE_REMOVAL;
}

bool HashTable::HashTableBucket::isEmpty() const {
    return type != BucketType::NORMAL;
}

std::string HashTable::HashTableBucket::getKey() const {
    return key;
}

size_t HashTable::HashTableBucket::getValue() const {
    return value;
}

std::ostream &operator<<(std::ostream &os, const HashTable &obj) {
    for (const auto & bucket : obj.buckets) {
        if (bucket.isEmpty())
            continue;
        os << "Key: " << bucket.getKey() << " -- Value: " << bucket.getValue() << std::endl;
    }
    return os;
}

void HashTable::resize(size_t newCapacity) {
    HashTable temp(newCapacity);
    for (const HashTableBucket &bucket : buckets) {
        if (bucket.isEmpty())
            continue;
        temp.insert(bucket.getKey(), bucket.getValue());
    }
    *this = std::move(temp);
}

