# darr
A double array implementation

## Install
```
autoreconf -iv
./configure --prefix=/path/to/install
make
make install
```

## Example
```c++
#include <string>
#include <vector>

#include <darr/da.hpp>

int main(int argc, char const* argv[]) {
    // sample data
    std::vector<std::string> keys = {"apple", "banana"};
    std::vector<float> values = {1., 2.};

    // initialize double array
    darr::DoubleArray<float> da = darr::DoubleArray<float>();

    // construct double array
    for (int i=0; i < keys.size(); ++i) {
        da.insert(keys[i], values[i]);
    }

    // save Double Array into da_file
    da.save("da_file");

    // load Double Array
    darr::DoubleArray<float> da2;
    da2.load("da_file");

    for (int i=0; i < keys.size(); ++i) {
        float expect = values[i];
        float got = da2.get(keys[i]);
        std::cout << keys[i] << " expect:" << expect << " got:" << got  << std::endl;
    }

    std::string k1 = "abcd";
    int id1 = da2.has(k1);
    std::cout << k1 << " expect:-1" << " got:" << id1 << std::endl;
}
```
