#include <fstream>
#include <string>
#include <vector>

#include "da.hpp"

int main(int argc, char const* argv[]) {
#ifdef DEBUG
    printf("DEBUG\n");
#endif

    darr::DoubleArray<float> da = darr::DoubleArray<float>();

    std::string filename = std::string(argv[1]);

    std::ifstream ifs(filename.c_str());

    if (ifs.fail()) {
        std::cerr << "failed to open:" << filename << std::endl;
    }

    std::string line;

    std::vector<std::string> keys;

    int i = 1;
    while (getline(ifs, line)) {
        keys.push_back(line);
        float v = i * 2.;
        da.insert(line, v);
        i += 1;
    }
    
    int j = 1;
    for (int i=0; i < keys.size(); ++i) {
        int id = da.has(keys[i]);
        float expect = j * 2.;
        float got = da.value[id];
        std::cout << keys[i] << " id:" << id << " expect:" << expect << " got:" << got  << std::endl;
        j += 1;
    }

    std::string k1 = "abcd";
    int id1 = da.has(k1);
    printf("id1:%d\n", id1);
    if (id1 < da.value.size()) {
        float v1 = da.value[id1];
        std::cout << k1  << " id:" << id1 << " expect: -1" << std::endl;
        std::cout << "v:" << v1 << std::endl;
    }

    std::string k2 = "bed";
    int id2 = da.has(k2);
    printf("id2:%d\n", id2);
    if (id2 < da.value.size()) {
        float v2 = da.value[id2];
        std::cout << k2  << " id:" << id2 << " expect: -1" << std::endl;
        std::cout << "v:" << v2 << std::endl;
    }
    
    return 0;
}
