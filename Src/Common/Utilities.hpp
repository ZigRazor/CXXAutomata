#ifndef CXXAUTOMATA_UTILITIES_HPP
#define CXXAUTOMATA_UTILITIES_HPP

#include <set>
#include <vector>

template<typename T>
static void set2Vector(const std::set<T>& set, std::vector<T>& vector) {
    for (auto& elem : set) {
        vector.push_back(elem);
    }
}


template<typename T>
static void vector2Set(const std::vector<T>& vector, std::set<T>& set) {
    for (auto& elem : vector) {
        set.insert(elem);
    }
}

#endif /* CXXAUTOMATA_UTILITIES_HPP */
