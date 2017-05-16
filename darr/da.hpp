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

};

} // namespace darr

#endif // DARR_DOUBLE_ARRAY_H_
