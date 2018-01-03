#include <stdlib.h>
#include <string.h>

#include <typeinfo>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

#include "da.hpp"

void darr::print_vec(const std::vector<int>& v) {
    std::cout << "[";
    for (int i=0; i < v.size(); ++i) {
        if (v[i] != 0) {
            std::cout << i << ":" << v[i] << ", ";
        }
    }
    std::cout << v.size()-1 << ":" << v[v.size()-1] << "]" << std::endl;;
}

std::vector<uint8_t> darr::_or(const std::vector<uint8_t>& x, const std::vector<uint8_t>& y) {
    std::vector<uint8_t> ret = y;
    int xsize = x.size();
    int ysize = y.size();
    for (int i=0; i < xsize; ++i) {
        bool in = false;
        for (int j=0; j < ysize; ++j) {
            if (x[i] == y[j]) {
                in = true;
                break;
            }
        }
        if (!in) {
            ret.push_back(x[i]);
        }
    }
    return ret;
};


std::vector<uint8_t> darr::get_subchars(const std::vector<uint8_t>& chars, int start, int end) {
    if (end < start) {
        std::vector<uint8_t> ret{};
        return ret;
    }
    std::vector<uint8_t>::const_iterator first = chars.begin() + start;
    std::vector<uint8_t>::const_iterator last = chars.begin() + end;
    std::vector<uint8_t> subchars(first, last);
    return subchars;
}


template<typename val_t>
int darr::DoubleArray<val_t>::has(const std::string& key) {
    int keysize = key.size();
    std::vector<uint8_t> chars(keysize);
    for (int i=0; i < keysize; ++i) {
        chars[i] = key[i];
    }
    int n_char = chars.size();
    chars.push_back('\0');

    int r = 0; // current node id
    int h = 0; // current position of the given string
    int t = 0;

    while (1) {
        if (node[r].base < 0) {
            break;
        }
        int a = chars[h];
        t = node[r].base + a;
        if (node[t].check != r || t >= node.size()) {
            return -1;
        } else {
            r = t;
        }
        h += 1;
    }

    if (h == n_char+1) {
        return t;
    } else {
        std::vector<uint8_t> s_temp = fetch_str(-node[r].base);
        std::vector<uint8_t> remain = get_subchars(chars, h, n_char+1);

        uint8_t* remain_ = remain.data();
        uint8_t* s_temp_ = s_temp.data();
        int match = mem_cmp(s_temp_, remain_, remain.size());

        if (match == -1) {
            return t;
        } else {
            return -1;
        }
    }
};

template<typename val_t>
int darr::DoubleArray<val_t>::del(const std::string& key) {
    std::vector<uint8_t> chars;
    int keysize = key.size();
    for (int i=0; i < keysize; ++i) {
        chars.push_back(key[i]);
    }
    int n_char = chars.size();
    chars.push_back('\0');

    int r = 0; // current node id
    int h = 0; // current position of the given string
    int t = 0;

    while (1) {
        if (node[r].base < 0) {
            break;
        }

        int a = chars[h];

        t = node[r].base + a;

        if (node[t].check != r) {
            return -1;
        } else {
            r = t;
        }

        h += 1;
    }

    if (h == n_char+1) {
//        printf("DEL base[t=%d]=0\n", t);
        node[t].base = 0;
        node[t].check = -1;
        node[t].val = val_t{};

        return t;
    } else {
        std::vector<uint8_t> a_h12 = get_subchars(chars, h, n_char+1);
        uint8_t* a = a_h12.data();
        uint8_t* start_ptr = &tail[-node[r].base-1];
        int match = mem_cmp(start_ptr, a, a_h12.size());
        if (match == -1) {
            node[t].base = 0;
            node[t].check = -1;
            node[t].val = val_t{};
            return t;
        } else {
            return -1;
        }

    }
};



template<typename val_t>
val_t darr::DoubleArray<val_t>::get(const std::string& key) {
    int id = has(key);
    int valuesize = node.size();

    if (id != -1) {
        val_t val = node[id].val;
        return val;
    } else {
        std::cerr << "Key Error: " << key << std::endl;
//        return -1;
    }
};


template<typename val_t>
bool darr::DoubleArray<val_t>::insert(const std::string& key, const val_t& val) {

    int keysize = key.size();
    std::vector<uint8_t> chars(keysize);
    for (int i=0; i < keysize; ++i) {
        chars[i] = key[i];
    }
    int n_char = chars.size();
    chars.push_back('\0');

    int r = 0; // current node id
    int h = 0; // current position of the given string

    while (1) {
        int a = chars[h];
        int t = node[r].base + a;

        int nodesize = node.size();
        if (t >= nodesize) {
            for (int k=nodesize-1; k<=t; ++k) {
                darr::node_t<val_t> n;
                node.push_back(n);
            }
        }

        if (node[t].check != r || t > node.size()) {
            std::vector<uint8_t> sub_key = get_subchars(chars, h, n_char+1);
            int nodesize1 = node.size();
            if (r > nodesize) {
                for (int k=nodesize-1; k<=r; ++k) {
                    node.push_back(darr::node_t<val_t>{});
                }
            }

            a_insert(r, sub_key, val);
            return false;
        } else {
            int nodesize1 = node.size();
            if (r > nodesize) {
                for (int k=nodesize-1; k<=r; ++k) {
                    node.push_back(darr::node_t<val_t>{});
                }
            }
            r = t;
        }
        if (node[r].base < 0) {
            break;
        }
        h += 1;
    }

    if (h == n_char+1) {
        return true;
    } else {
        std::vector<uint8_t> s_temp = fetch_str(-node[r].base);
        std::vector<uint8_t> remain = get_subchars(chars, h+1, n_char+1);

        uint8_t* remain_ = remain.data();
        uint8_t* s_temp_ = s_temp.data();
        int prefix_len = mem_cmp(s_temp_, remain_, remain.size());
        if (prefix_len==-1) {
            return true;
        } else {
            std::vector<uint8_t> prefix = get_subchars(chars, h+1, h+1+prefix_len);
            std::vector<uint8_t> remain_key = get_subchars(chars, h+1+prefix_len, n_char+1);
            std::vector<uint8_t> remain_s_temp = get_subchars(s_temp, prefix_len, s_temp.size());
            b_insert(r, prefix, remain_key, remain_s_temp, val);
            return false;
        }
    }
};

template<typename val_t>
void darr::DoubleArray<val_t>::a_insert(int r, const std::vector<uint8_t>& key, const val_t& val) {

    int a = key[0];

    int t = node[r].base + a;

    int nodesize = node.size();
    if (t >= nodesize) {
        for (int i=nodesize; i <= t; ++i) {
            node.push_back(darr::node_t<val_t>{});
        }
    }

    if (node[t].check <= -1) {
        insert_str(r, key, POS, val);
    } else {
        std::vector<uint8_t> rlist2 = set_list(r);
        std::vector<uint8_t> tlist2 = set_list(node[t].check);
        int s;
        if (rlist2.size() + 1 < tlist2.size()) {
            std::vector<uint8_t> b = {key[0]};
//            printf("MODIFY1\n");
            s = modify(r, r, b, rlist2);
        } else {
//            printf("MODIFY2\n");
            std::vector<uint8_t> empty;
            s = modify(r, node[t].check, empty, tlist2);
        }

        insert_str(s, key, POS, val);

    }
};

template<typename val_t>
void darr::DoubleArray<val_t>::insert_str(int h, const std::vector<uint8_t>& key, int d_pos, const val_t& val) {

    int e1 = key[0];

    int t = node[h].base + e1;

    int nodesize = node.size();
    if (t >= nodesize) {
        for (int k=nodesize; k <= t; ++k) {
            node.push_back(darr::node_t<val_t>{});
        }
    }
    node[t].base = -d_pos;
    node[t].check = h;
    node[t].val = val;
//    printf("t = base[h=%d] + %c = %d + %d = %d\n", h, key[0], node[h].base, e1, t);
//    printf("INSERT check[t=%d]=%d\n", t, node[t].check);


    std::vector<uint8_t> e = get_subchars(key, 1, key.size());
    e.push_back('$');

    int pos = str_tail(d_pos, e);

    std::string a;
    for (int i=0; i < e.size(); ++i) {
        a += e[i];
    }
//    printf("POS = STR_TAIL(%d, %s) = %d\n", d_pos, a.c_str(), POS + pos);
    POS = POS + pos;
};

template<typename val_t>
int darr::DoubleArray<val_t>::str_tail(int p, const std::vector<uint8_t>& y) {
    int s_y = y.size();

    if (tail.size() <= p - 1 + s_y) {
        int start = tail.size();
        for (int i=start; i <= p - 1 + s_y; ++i) {
            tail.push_back(uint8_t{});
        }
    }
    for (int i=0; i < s_y; ++i) {
        tail[p-1+i] = y[i];
    }

    if (p == POS) {
        return s_y;
    } else {
        return 0;
    }
    

};

template<typename val_t>
std::vector<uint8_t> darr::DoubleArray<val_t>::fetch_str(int p) {
    std::vector<uint8_t> y;
    int tailsize = tail.size();
    for (int i=p-1; i < tailsize; ++i) {
        y.push_back(tail[i]);
        if (tail[i] == '\0') {
//        if (tail[i] == '#') {
            break;
        }
    }
    return y;
};

template<typename val_t>
int darr::DoubleArray<val_t>::mem_cmp(uint8_t* x, uint8_t* y, int len) {
    int k = 0;
    for (int i=0; i < len; ++i) {
        if (x[i] == y[i]) {
            k += 1;
        } else {
            break;
        }
    }

    if (k == len) {
        return -1;
    } else {
        return k;
    }
};


template<typename val_t>
void darr::DoubleArray<val_t>::b_insert(int r, const std::vector<uint8_t>& prefix, const std::vector<uint8_t>& a, const std::vector<uint8_t>& b, const val_t& val) {

//    std::string a_;
//    for (int i=0; i < a.size(); ++i) {
//        a_ += a[i];
//    }
//    std::string b_;
//    for (int i=0; i < b.size(); ++i) {
//        b_ += b[i];
//    }
//    std::string prefix_;
//    for (int i=0; i < prefix.size(); ++i) {
//        prefix_ += prefix[i];
//    }
//    printf("B_INSERT(%d, %s, %s, %s)\n", r, prefix_.c_str(), a_.c_str(), b_.c_str());

    int old_pos = -node[r].base;

    int prefixsize = prefix.size();
    for (int i=0; i < prefixsize; ++i) {
        std::vector<uint8_t> pi = {prefix[i]};

        int b = prefix[i];

        int nodesize = node.size();
        if (r >= nodesize) {
            for (int k=nodesize; k <= r; ++k) {
                node.push_back(darr::node_t<val_t>{});
            }
        }

        int t = x_check(pi);
//        printf("B_INSERT x_check(%c) = %d\n", prefix[i], t);
        node[r].base = t;

        int nodesize1 = node.size();
        if (node[r].base + b >= nodesize1) {
            for (int k=nodesize1; k <= node[r].base+b; ++k) {
                node.push_back(darr::node_t<val_t>{});
            }
        }

        int s = node[r].base + b;
        node[s].check = r;
        node[s].val = node[r].val;
//        printf("B_INSERT base[r=%d] = %d\n", r, t);
//        printf("B_INSERT check[base[r]+b] = check[%d+%d] = check[%d] = %d\n",
//                node[r].base, b, node[r].base+b, node[node[r].base+b].check);
//        printf("B_INSERT CHECK[%d]=%d\n", node[r].base+b, node[node[r].base+b].check);

        r = s;
    }

    int nodesize = node.size();
    if (r >= nodesize) {
        for (int k=nodesize; k <= r; ++k) {
            node.push_back(darr::node_t<val_t>{});
        }
    }

    std::vector<uint8_t> list = {a[0], b[0]};
    int q = x_check(list);
    node[r].base = q;
//    printf("B_INSERT base[r=%d]=%d\n", r, q);

//    printf("B_INSERT1 INS_STR(%d, %s, %d)\n", r, b_.c_str(), old_pos);
    insert_str(r, b, old_pos, node[r].val);

//    printf("B_INSERT2 INS_STR(%d, %s, %d)\n", r, a_.c_str(), POS);
    insert_str(r, a, POS, val);
};


template<typename val_t>
int darr::DoubleArray<val_t>::x_check(const std::vector<uint8_t>& list) {
    int minq = -1;
    bool found = false;

    int listsize = list.size();
    int nodesize = node.size();
    for (int q=1; q < nodesize; ++q) {
        bool ok = true;
        for (int i=0; i < listsize; ++i) {
            int c = list[i];

            if (q + c >= nodesize) {
                continue;
            }
            if (node[q + c].check > -1) {
                ok = false;
                break;
            }
        }
        if (ok) {
            minq = q;
            found = true;
            break;
        }
    }

    if (!found) {
        node.push_back(darr::node_t<val_t>{});
        minq = node.size();
    }

//    printf("minq:%d\n", minq);
    return minq;
};

template<typename val_t>
std::vector<uint8_t> darr::DoubleArray<val_t>::set_list(int k) {
    std::vector<uint8_t> ret;

    if (k >= node.size()) {
        return ret;
    }
    int nodesize = node.size();
    for (int i=0; i < nodesize; ++i) {

        if (node[k].base + i >= nodesize || node[k].base + i < 0) {
            continue;
        }

        if (node[node[k].base + i].check == k) {
            ret.push_back(i);
        }
    }
    return ret;
};


template<typename val_t>
int darr::DoubleArray<val_t>::modify(int current_s, int h, const std::vector<uint8_t>& add, const std::vector<uint8_t>& org) {

    int old_base = node[h].base;

//    printf("old_base = BASE[h=%d] = %d\n", h, old_base);

    int t = x_check(_or(add, org));

    node[h].base = t;
//    printf("MODIFY base[h=%d] = x_check() = %d\n", h, t);

    int orgsize = org.size();
    for (int i=0; i < orgsize; ++i) {
        int c = org[i];
//        printf("MODIFY char[i=%d]:%c %d\n", i, org[i], c);

        int t = old_base + c;

//        printf("t = old_base(%d) + %c(%d) = %d\n", old_base, org[i], c, t);

        int t2 = node[h].base + c;
//        printf("t2 = base[h=%d] + %c(%d) = %d\n", h, org[i], c, t2);

        int nodesize = node.size();
        if (t2 >= nodesize) {
            for (int i=nodesize; i <= t2; ++i) {
                node.push_back(darr::node_t<val_t>{});
            }
        }

        node[t2].check = h;
        node[t2].base = node[t].base;
        node[t2].val = node[t].val;

//        printf("MODIFY check[t2=%d] = %d\n", t2, h);
//        printf("MODIFY base[t2=%d] = base[t=%d] = %d\n", t2, t, base[t2]);
//        printf("MODIFY value[t2=%d] = value[t=%d] = %d\n", t2, t, value[t2]);

        if (node[t].base > 0) {
            int nodesize = node.size();
            for (int q=0; q < nodesize; ++q) {
                if (node[q].check == t) {
                    node[q].check = t2;
                }
                if (t == current_s) {
                    current_s = t2;
                }
            }
        }

//        printf("MODIFY clear base[t=%d] = 0\n", t);
//        printf("MODIFY clear check[t=%d] = 0\n", t);

        node[t].base = 0;
        node[t].check = -1;
        node[t].val = val_t{};
    }
    return current_s;
};

template<typename val_t>
void darr::DoubleArray<val_t>::save(const char* filename) {
    FILE* fp = fopen(filename, "wb");

    fwrite(&POS, sizeof(int), 1, fp);

    int nodesize = node.size();
    fwrite(&nodesize, sizeof(int), 1, fp);
    fwrite(node.data(), sizeof(darr::node_t<val_t>), nodesize, fp);

    int tailsize = tail.size();
    fwrite(&tailsize, sizeof(int), 1, fp);
    fwrite(tail.data(), sizeof(uint8_t), tailsize, fp);

    fclose(fp);
};

template<typename val_t>
void darr::DoubleArray<val_t>::load(const char* filename) {
    FILE* fp = fopen(filename, "rb");

    fread(&POS, sizeof(int), 1, fp);

    int nodesize = 0;
    fread(&nodesize, sizeof(int), 1, fp);
    node = std::vector< darr::node_t<val_t> >(nodesize);
    for (int i=0; i < nodesize; ++i) {
        fread(&node[i], sizeof(darr::node_t<val_t>), 1, fp);
    }

    int tailsize = 0;
    fread(&tailsize, sizeof(int), 1, fp);
    tail = std::vector<uint8_t>(tailsize);
    for (int i=0; i < tailsize; ++i) {
        fread(&tail[i], sizeof(uint8_t), 1, fp);
    }

    fclose(fp);
};

template<typename val_t> 
std::vector<std::string>
darr::DoubleArray<val_t>::common_prefix_search(const std::string& key) {
    int keysize = len_utf8(key.c_str());
    const char* _key = key.c_str();
    std::vector<std::string> common_prefix;
    for (int i=0; i < keysize; ++i) {
        char* s = substr_utf8(_key, 0, i+1);
        if (has(s) != -1) {
            common_prefix.push_back(s);
        }
    }
    return common_prefix;
}


template class darr::DoubleArray<int>;
template class darr::DoubleArray<float>;
template class darr::DoubleArray<double>;
template class darr::DoubleArray<std::string>;
