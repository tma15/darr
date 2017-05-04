#include <fstream>
#include <string>
#include <vector>

#include "da.hpp"

int main(int argc, char const* argv[]) {
    DoubleArray da = DoubleArray();

    std::string filename = "sample.txt";

    std::ifstream ifs(filename.c_str());

    if (ifs.fail()) {
        std::cerr << "failed to open:" << filename << std::endl;
    }

    std::string line;

    std::vector<std::string> keys;

    while (getline(ifs, line)) {
        keys.push_back(line);
        da.insert(line);
    }
    
    printf("\n");
    for (int i=0; i < keys.size(); ++i) {
        bool ok = da.has(keys[i]);
        std::cout << keys[i] << " has:" << ok << " expect: 1" << std::endl;
    }

    std::string k1 = "abcd";
    bool ok1 = da.has(k1);
    std::cout << k1  << " has:" << ok1 << " expect: 0" << std::endl;

    std::string k2 = "bed";
    bool ok2 = da.has(k2);
    std::cout << k2  << " has:" << ok2 << " expect: 0" << std::endl;

    
    return 0;
}
