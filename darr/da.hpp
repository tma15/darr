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

template <typename val_t>
class DoubleArray {
    public:
        std::vector<int> base;
        std::vector<int> check;
        std::vector<char> tail;

//        std::vector<int> value;
        std::vector<val_t> value;

        std::map<char, int> keymap;
        std::map<int, char> itos;
        int POS;

        DoubleArray() {
            printf("INIT\n");
            POS = 1;
//            base = std::vector<int>{0};
            base.push_back(0);
//            check = std::vector<int>{1};
            check.push_back(1);
//            value = std::vector<int>{0};
//            value = std::vector<val_t>{ val_t{} };
            value.push_back(val_t{});

            printf("base:%d\n", base.size());
            printf("base[0]:%d\n", base[0]);

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

        }

//        void build(std::string filename);

//        bool insert(std::string key);

//        bool insert(std::string key, float val);
        bool insert(std::string key, val_t val);

        int has(std::string key);

//        void a_insert(int r, std::string a, float val);
        void a_insert(int r, std::string a, val_t val);
//        void b_insert(int r, std::string prefix, std::string a, std::string b, float val);
        void b_insert(int r, std::string prefix, std::string a, std::string b, val_t val);

        int x_check(std::vector<int> list);
//        void insert_str(int h, std::string k, int d_pos, float val);
        void insert_str(int h, std::string k, int d_pos, val_t val);
        int str_tail(int p, std::string y);

        std::string fetch_str(int p);

        int str_cmp(std::string x, std::string y);

        int x_check(std::string list);

        std::string set_list(int k);

        int modify(int current_s, int h, std::string add, std::string org);

};

} // namespace darr

#endif // DARR_DOUBLE_ARRAY_H_
