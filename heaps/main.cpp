#include <chrono>
#include <random>
#include <memory>
#include <iostream>
#include <algorithm>

using std::size_t;

template <typename T>
class Heap {
    std::vector<T> data;
    size_t heap_end;
    size_t left(size_t pos) {
        return pos * 2 + 1;
    }
    size_t right(size_t pos) {
        return pos * 2 + 2;
    }
    void heapify(size_t pos) {
        size_t l = left(pos), r = right(pos);
        if(r >= heap_end) {
            if(l < heap_end && data[l] > data[pos]) {
                std::swap(data[l], data[pos]);
                heapify(l);
            }
            return;
        }
        size_t max = pos;
        if(data[max] < data[l]) max = l;
        if(data[max] < data[r]) max = r;
        if(max != pos) {
            std::swap(data[max], data[pos]);
            heapify(max);
        }
    }
public:
    explicit Heap(const std::vector<T> &data) : data(data), heap_end(data.size()) {
        size_t heap_start = heap_end / 2;
        while(true) {
            heapify(heap_start);
            if(heap_start == 0) break;
            heap_start--;
        }
    }
    bool FindMaxAndSwap() {
        std::swap(data[0], data[--heap_end]);
        if(heap_end == 0) return false;
        heapify(0);
        return true;
    }
    friend std::vector<T> GetData(Heap<T> &&heap) {
        return std::move(heap.data);
    }
};

template <typename T>
void Sort(Heap<T> &data) {
    while (data.FindMaxAndSwap());
}

template <typename T>
void Sort(std::vector<T> &data) {
    size_t dest = data.size() - 1;
    while(true) {
        size_t max = 0;
        for(size_t i = 1;i<=dest;i++) {
            if(data[i] > data[max]) max = i;
        }
        std::swap(data[max], data[dest]);
        if(dest == 0) break;
        dest--;
    }
}

struct times {
    std::chrono::nanoseconds heap_sort = std::chrono::nanoseconds (0), selection_sort = heap_sort, standard_sort = heap_sort;
};

void time_sorts(const std::vector<unsigned int> &perm, times &times) {
    using namespace std::chrono;
    std::vector<unsigned int> heap_sort, selection_sort, standard_sort;
    {
        auto start = high_resolution_clock::now();
        Heap<unsigned int> heap(perm); // copy and heapify the data
        Sort(heap); // FindMaxAndSwap loop
        heap_sort = GetData(std::move(heap)); // Unwrap the vector (just a pointer move)
        auto end = high_resolution_clock::now();
        times.heap_sort += end - start;
    }

    {
        auto start = high_resolution_clock::now();
        selection_sort = perm; // copy the data
        Sort(selection_sort); // selection sort
        auto end = high_resolution_clock::now();
        times.selection_sort += end - start;
    }

    {
        auto start = high_resolution_clock::now();
        standard_sort = perm; // copy the data
        std::sort(standard_sort.begin(), standard_sort.end()); // standard sort
        auto end = high_resolution_clock::now();
        times.standard_sort += end - start;
    }

    if(!(heap_sort == selection_sort && selection_sort == standard_sort && standard_sort == heap_sort))
        exit(2);
}

int main(int argc, char **argv) {
    using namespace std::chrono;
    if(argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <permutation size> <iterations>\n";
        return 1;
    }
    const unsigned int perm_size = std::stoi(argv[1]), iterations = std::stoi(argv[2]);
    std::vector<unsigned int> perm(perm_size);
    for(size_t i = 0;i<perm.size();i++)
        perm[i] = i;
    std::mt19937 rng {std::random_device{}()};
    times times;
    for(unsigned int i = 0; i < iterations; i++) {
        std::shuffle(perm.begin(), perm.end(), rng);
        time_sorts(perm, times);
    }
    auto div = [iterations](nanoseconds ms) {
        return static_cast<double>(duration_cast<milliseconds>(ms).count()) / iterations;
    };
    std::cout << "Heap sort: " << div(times.heap_sort) << "ms\n";
    std::cout << "Selection sort: " << div(times.selection_sort) << "ms\n";
    std::cout << "Standard sort: " << div(times.standard_sort) << "ms\n";
    return 0;
}
