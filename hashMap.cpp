#include <iostream>
#include <functional>
#include <list>
#include <vector>
#include <stdexcept>

template<class KeyType, class ValueType, class Hash = std::hash<KeyType>>
class HashMap {
public:
    using typeOfPair = typename std::pair<const KeyType, ValueType>;
    using const_iterator = typename std::list<typeOfPair>::const_iterator;
    using iterator = typename std::list<typeOfPair>::iterator;
    
    HashMap(Hash hasher = Hash{}) : hasher(hasher) {
        defaultParametrs();
    }
    
    template<class Iterator>
    HashMap(Iterator first, Iterator last, Hash hasher = Hash{}) : hasher(hasher) {
        defaultParametrs();
        while (first != last)
            insert(*(first++));
    }
    
    HashMap(std::initializer_list<typeOfPair> copyList, Hash hasher = Hash{})
    : HashMap(copyList.begin(), copyList.end(), hasher) {}
    
    HashMap(const HashMap& other) {
        clear();
        for (const auto& x : other)
            insert(x);
    }
    
    HashMap& operator=(const HashMap& other) {
        if (this == std::addressof(other))
            return *this;
        clear();
        for (auto x : other)
            insert(x);
        return *this;
    }
    
    size_t size() const {
        return sizeOfList;
    }
    
    bool empty() const {
        return size() == 0;
    }
    
    Hash hash_function() const {
        return hasher;
    }
    
    void erase(const KeyType& element) {
        size_t i = hasher(element) % table.size();
        auto it = table[i];
        while (it != list.end() && hasher(it->first) % table.size() == i) {
            if (it->first == element) {
                if (it == table[i]) {
                    auto nextIt = it;
                    ++nextIt;
                    table[i] = nextIt;
                    if (hasher(nextIt->first) % table.size() != i)
                        table[i] = list.end();
                }
                --sizeOfList;
                list.erase(it);
                return;
            }
            ++it;
        }
        return;
    }
    
    iterator insert(const typeOfPair& element, bool isFromRebuild = true) {
        if (isFromRebuild && lenOfChain > maxLenOfChain)
            rebuild();
        
        size_t elHash = hasher(element.first), i = elHash % table.size();
        if (table[i] == list.end()) {
            list.push_front(element);
            table[i] = list.begin();
            ++sizeOfList;
            return table[i];
        }
        size_t len = 0;
        auto it = table[i];
        while (it != list.end() && hasher(it->first) % table.size() == i) {
            if (it->first == element.first)
                break;
            if (hasher(it->first) != elHash)
                ++len;
            ++it;
        }
        lenOfChain = len;
        ++sizeOfList;
        return list.insert(it, element);
    }
    
    iterator begin() {
        return list.begin();
    }
    
    const_iterator begin() const {
        return list.begin();
    }
    
    iterator end() {
        return list.end();
    }
    
    const_iterator end() const {
        return list.end();
    }
    
    iterator find(const KeyType& element) {
        size_t i = hasher(element) % table.size();
        auto it = table[i];
        while (it != list.end() && hasher(it->first) % table.size() == i) {
            if (it->first == element)
                return it;
            ++it;
        }
        return list.end();
    }
    
    const_iterator find(const KeyType& element) const {
        size_t i = hasher(element) % table.size();
        auto it = table[i];
        while (it != list.end() && hasher(it->first) % table.size() == i) {
            if (it->first == element)
                return it;
            ++it;
        }
        return list.end();
    }
    
    ValueType& operator[](const KeyType& key) {
        auto it = find(key);
        if (it == end())
            return insert({key, ValueType()})->second;
        else
            return it->second;
    }
    
    const ValueType& at(const KeyType& key) const {
        auto it = find(key);
        if (it == end())
            throw std::out_of_range("out_of_range");
        
        return it->second;
    }
    
    void clear() {
        table.clear();
        list.clear();
        defaultParametrs();
        table.resize(defaultSize, list.end());
    }
    
private:
    std::list<typeOfPair> list;
    std::vector<typename std::list<typeOfPair>::iterator> table;
    const size_t maxLenOfChain = 5;
    const size_t defaultSize = 8;
    size_t lenOfChain;
    size_t sizeOfList;
    Hash hasher;
    
    void rebuild() {
        auto copy = std::move(list);
        size_t newSize = table.size() * 2;
        clear();
        table.resize(newSize, list.end());
        for (const auto& x : copy)
            insert(x, false);
    }
    
    void defaultParametrs() {
        table.resize(defaultSize, list.end());
        lenOfChain = 0;
        sizeOfList = 0;
    }
};


