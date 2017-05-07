#ifndef DARR_DOUBLE_ARRAY_H_
#define DARR_DOUBLE_ARRAY_H_

#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace darr {

void printvec(const std::vector<int>& v); 
void printvec_char(const std::vector<char>& v); 
std::string _or(std::string x, std::string y);
std::vector<uint8_t> _or2(std::vector<uint8_t> x, std::vector<uint8_t> y);

std::vector<uint8_t> get_subchars(const std::vector<uint8_t>& chars, int start, int end); 

template <typename val_t>
class DoubleArray {
    private:
        int POS;
        std::vector<val_t> value;

        std::vector<int> base;
        std::vector<int> check;

        std::vector<char> tail;
        std::vector<uint8_t> tail2;

//        void a_insert(int r, const std::string& a, const val_t& val);
        void a_insert(int r, const std::vector<uint8_t>& a, const val_t& val);

//        void b_insert(int r, const std::string& prefix,
//                const std::string& a, const std::string& b, const val_t& val);
        void b_insert(int r, const std::vector<uint8_t>& prefix,
                const std::vector<uint8_t>& a, const std::vector<uint8_t>& b, const val_t& val);

//        void insert_str(int h, const std::string& k, int d_pos, const val_t& val);
        void insert_str2(int h, const std::vector<uint8_t>& k, int d_pos, const val_t& val);

        int str_tail(int p, const std::string& y);
        int str_tail2(int p, const std::vector<uint8_t>& y);

        std::string fetch_str(int p);
        std::vector<uint8_t> fetch_str2(int p);

        int str_cmp(const std::string& x, const std::string& y);
        int mem_cmp(uint8_t* x, uint8_t* y, int len);

        int x_check(const std::string& list);
        int x_check2(const std::vector<uint8_t>& list);

        std::string set_list(int k);
        std::vector<uint8_t> set_list2(int k);

        int modify(int current_s, int h, const std::string& add, const std::string& org);
        int modify2(int current_s, int h, const std::vector<uint8_t>& add, const std::vector<uint8_t>& org);

    public:

#ifdef DEBUG
        std::map<char, int> keymap;
        std::map<int, char> itos;
#endif


        DoubleArray() {
            POS = 1;
            base.push_back(0);
            check.push_back(1);
            value.push_back(val_t{});

#ifdef DEBUG
            keymap.insert(std::make_pair('a', 1));
            keymap.insert(std::make_pair('b', 2));
            keymap.insert(std::make_pair('c', 3));
            keymap.insert(std::make_pair('d', 4));
            keymap.insert(std::make_pair('e', 5));
            keymap.insert(std::make_pair('f', 6));
            keymap.insert(std::make_pair('g', 7));
            keymap.insert(std::make_pair('h', 8));
            keymap.insert(std::make_pair('i', 9));
            keymap.insert(std::make_pair('j', 10));
            keymap.insert(std::make_pair('k', 11));
            keymap.insert(std::make_pair('l', 12));
            keymap.insert(std::make_pair('m', 13));
            keymap.insert(std::make_pair('n', 14));
            keymap.insert(std::make_pair('o', 15));
            keymap.insert(std::make_pair('p', 16));
            keymap.insert(std::make_pair('q', 17));
            keymap.insert(std::make_pair('r', 18));
            keymap.insert(std::make_pair('#', 19));

            itos.insert(std::make_pair(1, 'a'));
            itos.insert(std::make_pair(2, 'b'));
            itos.insert(std::make_pair(3, 'c'));
            itos.insert(std::make_pair(4, 'd'));
            itos.insert(std::make_pair(5, 'e'));
            itos.insert(std::make_pair(6, 'f'));
            itos.insert(std::make_pair(7, 'g'));
            itos.insert(std::make_pair(8, 'h'));
            itos.insert(std::make_pair(9, 'i'));
            itos.insert(std::make_pair(10, 'j'));
            itos.insert(std::make_pair(11, 'k'));
            itos.insert(std::make_pair(12, 'l'));
            itos.insert(std::make_pair(13, 'm'));
            itos.insert(std::make_pair(14, 'n'));
            itos.insert(std::make_pair(15, 'o'));
            itos.insert(std::make_pair(16, 'p'));
            itos.insert(std::make_pair(17, 'q'));
            itos.insert(std::make_pair(18, 'r'));
            itos.insert(std::make_pair(19, '#'));
#endif

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
