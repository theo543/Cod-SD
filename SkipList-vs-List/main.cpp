#include <iostream>
#include <random>
#include <vector>
#include <set>
#include <chrono>
#include "SkipList.hpp"
#include "SortedList.hpp"

struct data {
    int key, value;
    data(int key, int value) : key(key), value(value) {}
    data() : key(0), value(0) {}
    std::strong_ordering operator<=>(const data& other) const {
        return key <=> other.key;
    }
    bool operator==(const data& other) const {
        return key == other.key;
    }
    bool operator!=(const data& other) const {
        return key != other.key;
    }
    friend std::ostream& operator<<(std::ostream& os, const data& d) {
        return os << '{' << d.key << " " << d.value << '}';
    }
};

class TimeScope {
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::string name;
public:
    explicit TimeScope(std::string name) : start(std::chrono::high_resolution_clock ::now()), name(std::move(name)) {}
    ~TimeScope() {
        using namespace std::chrono;
        std::cout << name << ": " << duration_cast<milliseconds>(high_resolution_clock ::now() - start).count() << "ms" << std::endl;
    }
};

template <typename T>
std::vector<T> random_sample(size_t size, const std::vector<T> &data) {
    std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<size_t> dist(0, data.size() - 1);
    std::vector<bool> chosen(data.size(), false);
    std::vector<T> result;
    for (size_t x = 0; x < size; x++) {
        size_t choice = 0;
        while (chosen[choice]) {
            choice = dist(rng);
        }
        chosen[choice] = true;
        result.push_back(data[choice]);
    }
    return result;
}

template <typename ListType>
void benchmark_list(const std::string &listName, const int INSERT) {
    const int SEARCH = INSERT / 2, SEARCH_AFTER = INSERT / 2;
    std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> dist;
    std::vector<data> test_data;
    std::set<int> keys;
    for(int x = 0; x < INSERT; x++) {
        int key = 0;
        while(keys.contains( key)) {
            key = dist(rng);
        }
        test_data.emplace_back(key, dist(rng));
        keys.insert(key);
    }
    std::vector<data> search_data = random_sample(SEARCH, test_data), search_data_2 = random_sample(SEARCH_AFTER, test_data);
    ListType list;
    using ListIterator = decltype(list.begin());
    {
        TimeScope scope(listName + " insert " + std::to_string(INSERT));
        for(const auto& d : test_data) {
            list.insert(d);
        }
    }
    {
        auto insert_sorted = test_data;
        std::sort(insert_sorted.begin(), insert_sorted.end());
        std::cout << "Checking if all elements were inserted and are in order\n";
        size_t index = 0;
        for (auto iter = list.begin(); iter != list.end(); ++iter, ++index) {
            if (*iter != insert_sorted[index]) {
                throw std::runtime_error("Element not in order or wrong element");
            }
        }
    }
    {
        std::vector<ListIterator> delete_iters;
        delete_iters.reserve(SEARCH);
        {
            TimeScope scope(listName + " search " + std::to_string(SEARCH));
            for (const auto &d: search_data) {
                auto iter = list.find(d);
                if(iter != list.end() && *iter != d) {
                    throw std::runtime_error("Found wrong element");
                }
                delete_iters.push_back(iter);
            }
        }
        std::cout << "Checking if all elements were found\n";
        for (const auto &d: search_data) {
            auto iter = list.find(d);
            if (iter == list.end() || *iter != d) {
                throw std::runtime_error("Failed to find element");
            }
        }
        {
            TimeScope scope(listName + " delete searched data");
            for (const auto &d: delete_iters) {
                list.erase(d);
            }
        }
    }
    {
        std::set<data> deleted_lookup(search_data.begin(), search_data.end());
        std::vector<std::pair<data, ListIterator>> found_iters;
        found_iters.reserve(SEARCH);
        {
            TimeScope scope(listName + " search after delete " + std::to_string(SEARCH_AFTER));
            for (const auto &d: search_data_2) {
                auto iter = list.find(d);
                if(iter != list.end() && *iter != d) {
                    throw std::runtime_error("Found wrong element");
                }
                found_iters.emplace_back(d, iter);
            }
        }
        std::cout << "Checking if all non-deleted elements were found\n";
        for(const auto& d : found_iters) {
            if(d.second == list.end() && !deleted_lookup.contains(d.first)) {
                throw std::runtime_error("Element not found but shouldn't be deleted");
            } else if(d.second != list.end() && deleted_lookup.contains(d.first)) {
                throw std::runtime_error("Element found but should be deleted");
            }
        }
    }
}

int main(int argc, char** argv) {
    if(argc != 2) {
        std::cout << "Usage: " << argv[0] << " <test size>" << std::endl;
        return 1;
    }
    const int INSERT = std::stoi(argv[1]);
    benchmark_list<SkipList<data>>("SkipList", INSERT);
    std::cout << std::endl << std::endl;
    benchmark_list<SortedList<data>>("SortedList", INSERT);
    std::cout << std::endl << std::endl;
    benchmark_list<std::set<data>>("std::set", INSERT);
    std::cout << std::endl << std::endl;
}
