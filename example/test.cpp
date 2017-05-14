#include <time.h>

#include <fstream>
#include <string>
#include <vector>

#include <darr/da.hpp>

void save_array(darr::DoubleArray<float>& da, std::vector<std::string>& keys, std::vector<float>& values) {
    clock_t start_all = clock();
    int n_key = keys.size();
    for (int i=0; i < keys.size(); ++i) {
        da.insert(keys[i], values[i]);
        clock_t end = clock();
        std::cout << "["<<i+1<<"/"<<n_key<<"] ";
        std::cout << "insert key:" << keys[i] << " value:" << values[i] << " ";
        std::cout << "time = " << (double)(end - start_all) / CLOCKS_PER_SEC << "sec.\n";

//        float got = da.get(keys[i]);
//        std::cout << "got:" << got << std::endl;
    }
    clock_t end_all = clock();
    std::cout << "insertion " << (double)(end_all - start_all) / CLOCKS_PER_SEC << "sec.\n";
    da.save("da_file");
};

int main(int argc, char const* argv[]) {
#ifdef DEBUG
    printf("DEBUG\n");
#endif

    std::string filename = std::string(argv[1]);
    std::ifstream ifs(filename.c_str());
    if (ifs.fail()) {
        std::cerr << "failed to open:" << filename << std::endl;
    }

    int i = 1;
    std::vector<std::string> keys;
    std::vector<float> values;

    std::string line;
    while (getline(ifs, line)) {
        keys.push_back(line);
        float v = i * 2.;
        values.push_back(v);
        i += 1;
    }

    darr::DoubleArray<float> da = darr::DoubleArray<float>();
    save_array(da, keys, values);

    darr::DoubleArray<float> da2;
    da2.load("da_file");

    int n_all = 0;
    int n_match = 0;
    for (int i=0; i < keys.size(); ++i) {
        float expect = values[i];
//        printf("[[da.get]]\n");
//        float got1 = da.get(keys[i]);
        printf("[[da2.get]]\n");
        float got = da2.get(keys[i]);
        std::cout << "key:" << keys[i] << " value expect:" << expect << " got:" << got<< std::endl;
        if (expect == got) {
            n_match += 1;
        }
        n_all += 1;
    }

    for (int i=0; i < keys.size(); ++i) {
        printf("delete key:%s\n", keys[i].c_str());
        da2.del(keys[i]);
        int k = da2.has(keys[i]);
        printf("key:%s expect:-1 got:%d\n\n", keys[i].c_str(), k);
        if (k == -1) {
            n_match += 1;
        }
        n_all += 1;
    }
    printf("match %d/%d\n", n_match, n_all);
    printf("\n");

//    std::string k1 = "abcd";
//    int id1 = da2.has(k1);
//    std::cout << k1 << " expect:-1" << " got:" << id1 << std::endl;

//    std::string k2 = "bed";
//    int id2 = da2.has(k2);
//    std::cout << k2 << " expect:-1" << " got:" << id1 << std::endl;
    
    return 0;
}
