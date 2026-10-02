#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

using Clock = std::chrono::steady_clock;
using ns = std::chrono::nanoseconds;

static volatile std::int64_t checksum_binary = 0;
static volatile std::int64_t checksum_merge = 0;

int binary_search_custom(const std::vector<int>& a, int target) {
    std::size_t left = 0;
    std::size_t right = a.size();
    while (left < right) {
        std::size_t mid = left + (right - left) / 2;
        int value = a[mid];
        if (value == target) return static_cast<int>(mid);
        if (value < target) left = mid + 1;
        else right = mid;
    }
    return -1;
}

void merge_sort_impl(std::vector<int>& a, std::vector<int>& tmp,
                     std::size_t left, std::size_t right) {
    if (right - left <= 1) return;
    std::size_t mid = left + (right - left) / 2;
    merge_sort_impl(a, tmp, left, mid);
    merge_sort_impl(a, tmp, mid, right);

    std::size_t i = left, j = mid, k = left;
    while (i < mid && j < right) {
        if (a[i] <= a[j]) tmp[k++] = a[i++];
        else tmp[k++] = a[j++];
    }
    while (i < mid) tmp[k++] = a[i++];
    while (j < right) tmp[k++] = a[j++];
    for (std::size_t p = left; p < right; ++p) a[p] = tmp[p];
}

void merge_sort_custom(std::vector<int>& a) {
    std::vector<int> tmp(a.size());
    merge_sort_impl(a, tmp, 0, a.size());
}

bool is_sorted_custom(const std::vector<int>& a) {
    for (std::size_t i = 1; i < a.size(); ++i) {
        if (a[i - 1] > a[i]) return false;
    }
    return true;
}

double mean(const std::vector<double>& v) {
    return std::accumulate(v.begin(), v.end(), 0.0) / static_cast<double>(v.size());
}

double median(std::vector<double> v) {
    std::sort(v.begin(), v.end());
    const std::size_t n = v.size();
    if (n % 2 == 1) return v[n / 2];
    return (v[n / 2 - 1] + v[n / 2]) / 2.0;
}

double stdev(const std::vector<double>& v) {
    const double m = mean(v);
    double s = 0.0;
    for (double x : v) s += (x - m) * (x - m);
    return std::sqrt(s / static_cast<double>(v.size() - 1));
}

int main(int argc, char** argv) {
    const std::vector<std::size_t> sizes = {
        1000, 2000, 5000, 10000, 20000, 50000, 100000,
        200000, 500000, 1000000
    };

    const int repetitions = 15;
    const int searches_per_rep = 10000;
    const std::uint64_t seed = 20261001ULL;

    std::string out = (argc > 1) ? argv[1] : "benchmark_resultados_cpp.csv";
    std::ofstream csv(out);
    if (!csv) {
        std::cerr << "No se pudo abrir el CSV de salida.\n";
        return 1;
    }

    csv << "n,binary_mean_ns_per_search,binary_median_ns_per_search,binary_stdev_ns_per_search,"
           "merge_mean_ms,merge_median_ms,merge_stdev_ms,log2_n,n_log2_n,"
           "binary_checksum,merge_checksum\n";

    std::mt19937_64 rng(seed);

    std::cout << "Benchmark C++\n";
    std::cout << "Repeticiones: " << repetitions << "\n";
    std::cout << "Busquedas por repeticion: " << searches_per_rep << "\n";
    std::cout << "Semilla: " << seed << "\n\n";

    for (std::size_t n : sizes) {
        std::cout << "n=" << n << std::flush;

        std::vector<int> data(n);
        std::iota(data.begin(), data.end(), 0);
        std::shuffle(data.begin(), data.end(), rng);

        // Preparación fuera de las mediciones.
        // La entrada contiene valores únicos, pero su orden es aleatorio.
        std::vector<int> sorted = data;
        std::sort(sorted.begin(), sorted.end());

        std::uniform_int_distribution<int> target_dist(static_cast<int>(n), static_cast<int>(2 * n));
        std::vector<int> targets(searches_per_rep);
        // Consultas aleatorias que no pertenecen al arreglo: caso desfavorable
        // (el algoritmo recorre la profundidad completa de la búsqueda binaria).
        for (auto& t : targets) t = target_dist(rng);

        // Warm-up mínimo.
        for (int i = 0; i < 100; ++i) {
            checksum_binary += binary_search_custom(sorted, targets[i % searches_per_rep]);
        }

        std::vector<double> binary_samples;
        std::vector<double> merge_samples;
        binary_samples.reserve(repetitions);
        merge_samples.reserve(repetitions);

        for (int r = 0; r < repetitions; ++r) {
            auto start_b = Clock::now();
            std::int64_t local_b = 0;
            for (int t : targets) local_b += binary_search_custom(sorted, t);
            auto end_b = Clock::now();
            checksum_binary += local_b;
            double elapsed_ns = std::chrono::duration<double, std::nano>(end_b - start_b).count();
            binary_samples.push_back(elapsed_ns / searches_per_rep);

            // Copiar antes de iniciar el cronometraje: el algoritmo debe recibir entrada no ordenada.
            std::vector<int> work = data;
            auto start_m = Clock::now();
            merge_sort_custom(work);
            auto end_m = Clock::now();
            checksum_merge += work[n / 2];
            if (!is_sorted_custom(work)) {
                std::cerr << "Error: Mergesort no ordeno correctamente n=" << n << "\n";
                return 2;
            }
            double elapsed_ms = std::chrono::duration<double, std::milli>(end_m - start_m).count();
            merge_samples.push_back(elapsed_ms);
        }

        const double bm = mean(binary_samples);
        const double bmed = median(binary_samples);
        const double bsd = stdev(binary_samples);
        const double mm = mean(merge_samples);
        const double mmed = median(merge_samples);
        const double msd = stdev(merge_samples);
        const double logn = std::log2(static_cast<double>(n));
        const double nlogn = static_cast<double>(n) * logn;

        csv << n << ','
            << std::setprecision(12) << bm << ',' << bmed << ',' << bsd << ','
            << mm << ',' << mmed << ',' << msd << ','
            << logn << ',' << nlogn << ','
            << checksum_binary << ',' << checksum_merge << '\n';

        std::cout << "  binary=" << std::fixed << std::setprecision(3) << bm << " ns/search"
                  << "  merge=" << mm << " ms\n";
    }

    std::cerr << "checksums: " << checksum_binary << ", " << checksum_merge << "\n";
    std::cout << "\nResultados: " << out << "\n";
    return 0;
}
