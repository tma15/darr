#include <fstream>
#include <string>
#include <vector>

#include "da.hpp"

void save_array(std::vector<std::string> keys, std::vector<float> values) {
    darr::DoubleArray<float> da = darr::DoubleArray<float>();
    for (int i=0; i < keys.size(); ++i) {
        da.insert(keys[i], values[i]);
    }
    da.save("da");
};

int main(int argc, char const* argv[]) {
#ifdef DEBUG
    printf("DEBUG\n");
#endif
    std::vector<std::string> keys;
    std::vector<float> values;

    std::string filename = std::string(argv[1]);
    std::ifstream ifs(filename.c_str());
    if (ifs.fail()) {
        std::cerr << "failed to open:" << filename << std::endl;
    }

    int i = 1;
    std::string line;
    while (getline(ifs, line)) {
        keys.push_back(line);
        float v = i * 2.;
        values.push_back(v);
        i += 1;
    }

    save_array(keys, values);

    darr::DoubleArray<float> da2;
    da2.load("da");

    int j = 1;
    for (int i=0; i < keys.size(); ++i) {
        float expect = j * 2.;
        float got = da2.get(keys[i]);
        std::cout << keys[i] << " expect:" << expect << " got:" << got  << std::endl;
        j += 1;
    }

    std::string k1 = "abcd";
    int id1 = da2.has(k1);
    printf("id1:%d\n", id1);
    if (id1 != -1) {
        float v1 = da2.value[id1];
        std::cout << k1  << " id:" << id1 << " expect: -1" << std::endl;
        std::cout << "v:" << v1 << std::endl;
    }

    std::string k2 = "bed";
    int id2 = da2.has(k2);
    printf("id2:%d\n", id2);
    if (id2 != -1) {
        float v2 = da2.value[id2];
        std::cout << k2  << " id:" << id2 << " expect: -1" << std::endl;
        std::cout << "v:" << v2 << std::endl;
    }

    
    return 0;
}
