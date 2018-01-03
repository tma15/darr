#include <time.h>

#include <fstream>
#include <string>
#include <vector>

#include <darr/da.hpp>

void save_array(darr::DoubleArray<float>& da,
        std::string filename) {

    std::ifstream ifs(filename.c_str());
    if (ifs.fail()) {
        std::cerr << "failed to open:" << filename << std::endl;
    }

    std::string line;
    clock_t start_all = clock();
    float v = 1.;
    while (getline(ifs, line)) {
        da.insert(line, v);
        clock_t end = clock();
        int i = v;
        std::cout << "["<<i<<"] ";
        std::cout << "insert key:" << line << " value:" << v << " ";
        std::cout << "time = " << (double)(end - start_all) / CLOCKS_PER_SEC << "sec.\n";
        v += 1.;
    }
    clock_t end_all = clock();
    std::cout << "insertion " << (double)(end_all - start_all) / CLOCKS_PER_SEC << "sec.\n";
    da.save("da_file");
};

void check(darr::DoubleArray<float>& da,
        std::string filename) {

    int n_all = 0;
    int n_match = 0;
    float expect = 1.;
    std::ifstream ifs1(filename.c_str());
    if (ifs1.fail()) {
        std::cerr << "failed to open:" << filename << std::endl;
    }

    clock_t start_all = clock();
    std::string line;
    while (getline(ifs1, line)) {

        float got = da.get(line);
        std::cout << "key:" << line;
        std::cout << " value expect:" << expect << " got:" << got<< std::endl;
        if (expect == got) {
            n_match += 1;
//        } else {
//            exit(1);
        }
        n_all += 1;
        expect += 1.;
    }
    clock_t end_all = clock();
    std::cout << "search " << (double)(end_all - start_all) / CLOCKS_PER_SEC << "sec.\n";

    std::ifstream ifs2(filename.c_str());
    if (ifs2.fail()) {
        std::cerr << "failed to open:" << filename << std::endl;
    }

    while (getline(ifs2, line)) {
        da.del(line);
        int k = da.has(line);
        printf("delete key:%s expect:-1 got:%d\n", line.c_str(), k);
        if (k == -1) {
            n_match += 1;
        }
        n_all += 1;
    }
    printf("match %d/%d\n", n_match, n_all);
};

int main(int argc, char const* argv[]) {
#ifdef DEBUG
    printf("DEBUG\n");
#endif

    std::string filename = std::string(argv[1]);

    darr::DoubleArray<float> da = darr::DoubleArray<float>();
    save_array(da, filename);

    darr::DoubleArray<float> da2;
    da2.load("da_file");

    check(da2, filename);
    
    return 0;
}
