# darr
A double array implementation. Darr dynamically constructs a Trie based on a double array.

## Install
```sh
autoreconf -iv
./configure --prefix=/path/to/install
make
make install
```

You need set environmental variables when you specified the custom prefix:

```sh
export LD_LIBRARY_PATH=/path/to/install/lib:$LD_LIBRARY_PATH
export PKG_CONFIG_PATH=/path/to/install/lib/pkgconfig:$PKG_CONFIG_PATH
```

For installing python wrapper, you need install `cython`.

```sh
cd darr/python
python setup.py install
```


## Examples
### C++
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

### Python

```python
# -*- coding: utf-8 -*-
import darr

da = darr.DoubleArray()

words = ['くるま', 'く', 'くる', 'りんご', 'オレンジ', 'baseball', 'soccer']

v = 0
for word in words:
    v += 1.
    da.insert(word, v)

print('### common prefix search ###')
ret = da.common_prefix_search('くるまで')
for w in ret:
    print(w)

print('### get values ###')
for word in words:
    v = da.get(word)
    print(word, v)

da.save('dafile')

print('### load dumped double array ###')
da2 = darr.DoubleArray()
da2.load('dafile')

for word in words:
    v = da2.get(word)
    print(word, v)
```


## Benchmark

```sh
wget https://dumps.wikimedia.org/enwiki/latest/enwiki-latest-all-titles-in-ns0.gz
gunzip enwiki-latest-all-titles-in-ns0.gz
time ./example/test_darr ./enwiki-latest-all-titles-in-ns0 > res-wiki.txt
```

## TODO
- benchmarking

## Authors
Takuya Makino,takuyamakino15@gmail.com
