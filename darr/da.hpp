#ifndef DARR_DOUBLE_ARRAY_H_
#define DARR_DOUBLE_ARRAY_H_

//#include <stdio.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>

void printvec(const std::vector<int>& v); 
void printvec_char(const std::vector<char>& v); 
std::string intersect(std::string x, std::string y);
std::string _or(std::string x, std::string y);

class DoubleArray {
    public:
        std::vector<int> base;
        std::vector<int> check;
        std::vector<char> tail;

        std::map<char, int> keymap;
        std::map<int, char> itos;
        int POS;

        DoubleArray() {
            POS = 1;
            base = std::vector<int>{0};
            check = std::vector<int>{1};

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

        void build(std::string filename);

        bool insert(std::string key);

        bool has(std::string key);

        void a_insert(int r, std::string a);
        void b_insert(int r, std::string prefix, std::string a, std::string b);

        int x_check(std::vector<int> list);
        void insert_str(int h, std::string k, int d_pos);
        int str_tail(int p, std::string y);

        std::string fetch_str(int p);

        int str_cmp(std::string x, std::string y);

        int x_check(std::string list);

        std::string set_list(int k);

        int modify(int current_s, int h, std::string add, std::string org);

};

#endif // DARR_DOUBLE_ARRAY_H_
