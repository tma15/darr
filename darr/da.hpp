#ifndef DARR_DOUBLE_ARRAY_H_
#define DARR_DOUBLE_ARRAY_H_

#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace darr {

void print_vec(const std::vector<int>& v);

std::vector<uint8_t> _or(const std::vector<uint8_t>& x, const std::vector<uint8_t>& y);

std::vector<uint8_t> get_subchars(const std::vector<uint8_t>& chars, int start, int end); 

template<typename val_t> 
class node_t {
    public:
        int base;
        int check;
        val_t val;

        node_t() {
            base = 0;
            check = -1;
            val = val_t{};
        };
};

template <typename val_t>
class DoubleArray {
    private:
        int POS;
        std::vector< node_t<val_t> > node;
        std::vector<uint8_t> tail;

        void a_insert(int r, const std::vector<uint8_t>& a, const val_t& val);

        void b_insert(int r, const std::vector<uint8_t>& prefix,
                const std::vector<uint8_t>& a, const std::vector<uint8_t>& b, const val_t& val);

        void insert_str(int h, const std::vector<uint8_t>& k, int d_pos, const val_t& val);

        int str_tail(int p, const std::vector<uint8_t>& y);

        std::vector<uint8_t> fetch_str(int p);

        int mem_cmp(uint8_t* x, uint8_t* y, int len);

        int x_check(const std::vector<uint8_t>& list);

        std::vector<uint8_t> set_list(int k);

        int modify(int current_s, int h, const std::vector<uint8_t>& add, const std::vector<uint8_t>& org);

    public:

        DoubleArray() {
            POS = 1;
            node.push_back(node_t<val_t>{});
        }

        bool insert(const std::string& key, const val_t& val);

        int has(const std::string& key);

        int del(const std::string& key);

        val_t get(const std::string& key);

        void save(const char* filename);

        void load(const char* filename);

        std::vector<std::string> common_prefix_search(const std::string& key);
};

int count_byte_of_utf8(unsigned char ch) {
    int byte;
    if ((ch >= 0x00) && (ch <= 0x7f)) {
        byte = 1;
    } else if ((ch >= 0xc2) && (ch <= 0xdf)) {
        byte = 2;
    } else if ((ch >= 0xe0) && (ch <= 0xef)) {
        byte = 3;
    } else if ((ch >= 0xf0) && (ch <= 0xf7)) {
        byte = 4;
    } else if ((ch >= 0xf8) && (ch <= 0xfb)) {
        byte = 5;
    } else if ((ch >= 0xfc) && (ch <= 0xfd)) {
        byte = 6;
    } else {
        byte = 0;
    }
    return byte;
}

char* substr_utf8(const char* str, int start, int len) {
    static char substr[1024];
    char* pRes = substr;
    int i = 0, pos = 0;
    int byte;

    while (str[i] != '\0') {
        byte = count_byte_of_utf8(str[i]);
        if (start <= pos && pos < start + len) {
            memcpy(pRes, (str + i), byte);
            pRes += byte;
        }
        i += byte;
        pos++;
    }
    *pRes = '\0';
    return substr;
}

int len_utf8(const char* str) {
    int i = 0, pos = 0;
    int byte;
    while (str[i] != '\0') {
        byte = count_byte_of_utf8(str[i]);
        i += byte;
        pos += 1;
    }
    return pos;
}

} // namespace darr

#endif // DARR_DOUBLE_ARRAY_H_
