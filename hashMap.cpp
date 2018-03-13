#include <iostream>
#include <functional>
#include <list>
#include <vector>
#include <stdexcept>

template<class KeyType, class ValueType, class Hash = std::hash<KeyType>>
class HashMap {
public:
    using const_iterator = typename std::list<std::pair<const KeyType, ValueType>>::const_iterator;
    using iterator = typename std::list<std::pair<const KeyType, ValueType>>::iterator;
    
    HashMap(Hash hasher = Hash()) : hasher(hasher) {
        table.resize(defaultSize, lst.end());
        normalize();
    }
    
    template<class Iterator>
    HashMap(Iterator left, Iterator right, Hash hasher = Hash()) : hasher(hasher) {
        table.resize(defaultSize, lst.end());
        normalize();
        while (left != right)
            insert(*(left++));
    }
    
    HashMap(const std::initializer_list<std::pair<KeyType, ValueType>>& list,
            Hash hasher = Hash()) : hasher(hasher) {
        table.resize(defaultSize, lst.end());
        normalize();
        for (auto x : list)
            insert(x);
    }
    
    HashMap(const HashMap& other) {
        clear();
        table.resize(defaultSize, lst.end());
        normalize();
        for (auto x : other)
            insert(x);
    }
    
    HashMap& operator=(const HashMap& other) {
        if (this == &other)
            return *this;
        clear();
        table.resize(defaultSize, lst.end());
        normalize();
        for (auto x : other)
            insert(x);
        return *this;
    }
    
    size_t size() const {
        return curSize;
    }
    
    bool empty() const {
        return size() == 0;
    }
    
    Hash hash_function() const {
        return hasher;
    }
    
    void erase(const KeyType& el) {
        size_t i = hasher(el) % table.size();
        if (table[i] == lst.end()) {
            return;
        } else {
            auto it = table[i];
            while (it != lst.end() && hasher(it->first) % table.size() == i) {
                if (it->first == el) {
                    if (it == table[i]) {
                        auto nextIt = it;
                        ++nextIt;
                        if (nextIt == lst.end()) {
                            table[i] = lst.end();
                        } else {
                            if (hasher(nextIt->first) % table.size() == i)
                                table[i] = nextIt;
                            else
                                table[i] = lst.end();
                        }
                    }
                    --curSize;
                    lst.erase(it);
                    return;
                }
                ++it;
            }
        }
        return;
    }
    
    void insert(const std::pair<const KeyType, ValueType>& el, bool fl = true) {
        if (fl && (way > MAX_WAY))
            recalc();
        
        size_t elHash = hasher(el.first);
        size_t i = elHash % table.size();
        if (table[i] == lst.end()) {
            lst.push_front(el);
            table[i] = lst.begin();
            ++curSize;
        } else {
            size_t curWay = 0;
            auto it = table[i];
            while (it != lst.end() && hasher(it->first) % table.size() == i) {
                if (it->first == el.first)
                    break;
                if (hasher(it->first) != elHash)
                    ++curWay;
                ++it;
            }
            way = curWay;
            lst.insert(it, el);
            ++curSize;
        }
    }
    
    iterator begin() {
        return lst.begin();
    }
    
    const_iterator begin() const {
        return lst.begin();
    }
    
    iterator end() {
        return lst.end();
    }
    
    const_iterator end() const {
        return lst.end();
    }
    
    iterator find(const KeyType& el) {
        size_t i = hasher(el) % table.size();
        if (table[i] == lst.end()) {
            return lst.end();
        } else {
            auto it = table[i];
            while (it != lst.end() && hasher(it->first) % table.size() == i) {
                if (it->first == el)
                    return it;
                ++it;
            }
        }
        return lst.end();
    }
    
    const_iterator find(const KeyType& el) const {
        size_t i = hasher(el) % table.size();
        if (table[i] == lst.end()) {
            return lst.end();
        } else {
            auto it = table[i];
            while (it != lst.end() && hasher(it->first) % table.size() == i) {
                if (it->first == el)
                    return it;
                ++it;
            }
        }
        return lst.end();
    }
    
    ValueType& operator[](const KeyType& key) {
        auto it = find(key);
        if (it == end()) {
            insert(std::make_pair(key, ValueType()));
            return (find(key))->second;
        } else {
            return it->second;
        }
    }
    
    const ValueType& at(const KeyType& key) const {
        auto it = find(key);
        if (it == end())
            throw std::out_of_range("out_of_range");
        
        return (find(key))->second;
    }
    
    void clear() {
        table.clear();
        lst.clear();
        normalize();
        table.resize(defaultSize, lst.end());
    }
    
private:
    std::list<std::pair<const KeyType, ValueType>> lst;
    std::vector<typename std::list<std::pair<const KeyType, ValueType>>::iterator> table;
    const size_t MAX_WAY = 5, defaultSize = 8;
    size_t way, curSize;
    Hash hasher;
    
    void recalc() {
        std::vector<std::pair<const KeyType, ValueType>> tmp;
        for (auto x : lst)
            tmp.push_back(x);
        
        size_t newSize = table.size() * 2;
        clear();
        table.resize(newSize, lst.end());
        for (auto x : tmp)
            insert(x, false);
    }
    
    void normalize() {
        way = 0;
        curSize = 0;
    }
};
