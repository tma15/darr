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

//    std::string key_ = key + "#";
    int keysize = key.size();
    std::vector<uint8_t> chars(keysize);
    for (int i=0; i < keysize; ++i) {
//        std::cout << "has i:" << key[i] << std::endl;
//        chars.push_back(key[i]);
        chars[i] = key[i];
    }
//    std::cout << std::endl;
    int n_char = chars.size();
//    printf("n_char:%d\n", n_char);
    chars.push_back('\0');
//    chars.push_back('#');

    int r = 0; // current node id
    int h = 0; // current position of the given string
    int t = 0;

//    for (int i=0; i < node.size(); ++i) {
//        printf("i:%d base:%d check:%d\n", i, node[i].base, node[i].check);
//    }

    while (1) {
//#ifdef DEBUG
        if (node[r].base < 0) {
//            printf("node[r=%d].base = %d < 0\n", r, node[r].base);
            break;
        }
//#else
//        if (base[r] < 0) {
//            break;
//        }
//#endif

        int a = chars[h];

//#ifdef DEBUG
        t = node[r].base + a;
//        printf("t=node[r].base+c=%d\n", t);
//        printf("has move base[%d] + %c = %d + %d = %d, base[t=%d] = %d nodesize:%d\n", r, chars[h], node[r].base, a, t, t, node[t].base, node.size());
//#else
//        t = base[r] + a;
//        printf("has move base[%d] + %c = %d + %d = %d, base[t=%d] = %d basesize:%d\n", r, chars[h], base[r], a, t, t, base[t], base.size());
//#endif

//        printf("has move base[%d] + %c = %d + %d = %d, base[t=%d] = %d basesize:%d\n", r, chars[h], base[r], a, t, t, base[t], base.size());
//        if (t >= base.size()) {
//            printf("OVER\n");
//        }

//#ifdef DEBUG
        if (node[t].check != r || t >= node.size()) {
//#else
//        if (check[t] != r || t >= check.size()) {
//            printf("!has1 check[t=%d]=%d != %d = r\n", t, check[t], r);
//#endif
            return -1;
        } else {
            r = t;
        }

        h += 1;
    }

    if (h == n_char+1) {
//        printf("has1\n");
        return t;
    } else {
//#ifdef DEBUG
        std::vector<uint8_t> s_temp = fetch_str(-node[r].base);
//#else
//        std::vector<uint8_t> s_temp = fetch_str(-base[r]);
//#endif
        std::vector<uint8_t> remain = get_subchars(chars, h, n_char+1);

//        std::string remains;
//        for (int i=0; i < remain.size(); ++i) {
//            remains += remain[i];
//        }
//        std::cout << "remain:" << remains << std::endl;
//        std::string stemps;
//        for (int i=0; i < s_temp.size(); ++i) {
//            stemps += s_temp[i];
//        }
//        std::cout << "s_temp:" << stemps << std::endl;

        uint8_t* remain_ = remain.data();
        uint8_t* s_temp_ = s_temp.data();
        int match = mem_cmp(s_temp_, remain_, remain.size());

        if (match == -1) {
//            printf("has2\n");
            return t;
        } else {
//            printf("!has2 t=%d\n", t);
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
//#ifdef DEBUG
        if (node[r].base < 0) {
//#else
//        if (base[r] < 0) {
//#endif
            break;
        }


        int a = chars[h];

//#ifdef DEBUG
        t = node[r].base + a;
//#else
//        t = base[r] + a;
//#endif

//#ifdef DEBUG
        if (node[t].check != r) {
//#else
//        if (check[t] != r) {
//#endif
            return -1;
        } else {
            r = t;
        }

        h += 1;
    }

    if (h == n_char+1) {
//        printf("DEL base[t=%d]=0\n", t);
//        base[t] = 0;
//        check[t] = -1;
//        value[t] = val_t{};

        node[t].base = 0;
        node[t].check = -1;
        node[t].val = val_t{};

        return t;
    } else {
        std::vector<uint8_t> a_h12 = get_subchars(chars, h, n_char+1);
        uint8_t* a = a_h12.data();
//#ifdef DEBUG
        uint8_t* start_ptr = &tail[-node[r].base-1];
//#else
//        uint8_t* start_ptr = &tail[-base[r]-1];
//#endif
        int match = mem_cmp(start_ptr, a, a_h12.size());
        if (match == -1) {
//            printf("DEL base[t=%d]=0\n", t);
//#ifdef DEBUG
            node[t].base = 0;
            node[t].check = -1;
            node[t].val = val_t{};
//#else
//            base[t] = 0;
//            check[t] = -1;
//            value[t] = val_t{};
//#endif
            return t;
        } else {
            return -1;
        }

    }
};



template<typename val_t>
val_t darr::DoubleArray<val_t>::get(const std::string& key) {
    int id = has(key);
//#ifdef DEBUG
    int valuesize = node.size();
//#else
//    int valuesize = value.size();
//#endif
//    if (id >= valuesize) {
//        std::cout << "id:" << id << " > valuesize:" << valuesize << std::endl;
//    }

    if (id != -1) {
//#ifdef DEBUG
        val_t val = node[id].val;
//#else
//        val_t val = value[id];
//#endif
//        std::cout << "GET:" << id << " " << val << std::endl;
        return val;
    } else {
        std::cout << "Key Error: " << key << std::endl;
        return -1;
//        exit(1);
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

//#ifdef DEBUG
        int t = node[r].base + a;
//        printf("DEBUG t=%d\n", t);
//#else
//        int t = base[r] + a;
//        printf("NOT DEBUG t=%d\n", t);
//#endif

        int nodesize = node.size();
        if (t >= nodesize) {
            for (int k=nodesize-1; k<=t; ++k) {
                darr::node_t<val_t> n;
                node.push_back(n);
//                value.push_back(val_t{});
            }
        }

//#ifndef DEBUG
//        int basesize = base.size();
//        if (t > basesize) {
//            for (int k=basesize-1; k<=t; ++k) {
//                base.push_back(0);
//                value.push_back(val_t{});
//            }
//        }

//        int checksize = check.size();
//        if (t >= checksize) {
//            for (int k=checksize-1; k <= t; ++k) {
//                check.push_back(-1);
//            }
//        }
//#endif

//        printf("insert move base[%d] + %c[h=%d] = %d + %d = %d, base[t=%d] = %d basesize:%d\n", r, chars[h], h, base[r], a, t, t, base[t], base.size());
//        if (t >= base.size()) {
//            printf("OVER\n");
//        }

//#ifdef DEBUG
        if (node[t].check != r || t > node.size()) {
//#else
//        if (check[t] != r || t > check.size()) {
//#endif
            std::vector<uint8_t> sub_key = get_subchars(chars, h, n_char+1);
//            std::string a_;
//            for (int i=0; i < sub_key.size(); ++i) {
//                a_ += sub_key[i];
//            }
//            printf("A_INSERT(r=%d, %s)\n", r, a_.c_str());
//            std::cout << val << std::endl;


//#ifdef DEBUG
            int nodesize1 = node.size();
            if (r > nodesize) {
                for (int k=nodesize-1; k<=r; ++k) {
                    node.push_back(darr::node_t<val_t>{});
//                    value.push_back(val_t{});
                }
            }
//#else
//            int basesize = base.size();
//            if (r > basesize) {
//                for (int k=basesize-1; k<=r; ++k) {
//                    base.push_back(0);
//                    value.push_back(val_t{});
//                }
//            }
//#endif

            a_insert(r, sub_key, val);

            return false;
        } else {
//#ifndef DEBUG
//            int basesize = base.size();
//            if (r > basesize) {
//                for (int k=basesize-1; k<=r; ++k) {
//                    base.push_back(0);
//                    value.push_back(val_t{});
//                }
//            }
//#else
            int nodesize1 = node.size();
            if (r > nodesize) {
                for (int k=nodesize-1; k<=r; ++k) {
                    node.push_back(darr::node_t<val_t>{});
//                    value.push_back(val_t{});
                }
            }
//#endif

            r = t;
        }

//#ifdef DEBUG
        if (node[r].base < 0) {
//#else
//        if (base[r] < 0) {
//#endif
            break;
        }

        h += 1;
    }

    if (h == n_char+1) {
        return true;
    } else {

//#ifdef DEBUG
        std::vector<uint8_t> s_temp = fetch_str(-node[r].base);
//#else
//        std::vector<uint8_t> s_temp = fetch_str(-base[r]);
//#endif

//        printf("h:%d \n", h+1);

        std::vector<uint8_t> remain = get_subchars(chars, h+1, n_char+1);

//        std::string remains;
//        for (int i=0; i < remain.size(); ++i) {
//            remains += remain[i];
//        }
//        std::cout << "remain:" << remains << std::endl;
//        std::string stemps;
//        for (int i=0; i < s_temp.size(); ++i) {
//            stemps += s_temp[i];
//        }
//        std::cout << "s_temp:" << stemps << std::endl;

        uint8_t* remain_ = remain.data();
        uint8_t* s_temp_ = s_temp.data();
        int prefix_len = mem_cmp(s_temp_, remain_, remain.size());
        if (prefix_len==-1) {
            return true;
        } else {

            std::vector<uint8_t> prefix = get_subchars(chars, h+1, h+1+prefix_len);
            std::vector<uint8_t> remain_key = get_subchars(chars, h+1+prefix_len, n_char+1);
            std::vector<uint8_t> remain_s_temp = get_subchars(s_temp, prefix_len, s_temp.size());

//            std::string prefixs;
//            for (int i=0; i < prefix.size(); ++i) {
//                prefixs += prefix[i];
//            }
//            std::cout << "prefix:" << prefixs << std::endl;

//            printf("B_INSERT\n");
            b_insert(r, prefix, remain_key, remain_s_temp, val);

            return false;
        }
    }

};

template<typename val_t>
void darr::DoubleArray<val_t>::a_insert(int r, const std::vector<uint8_t>& key, const val_t& val) {

    int a = key[0];

//#ifdef DEBUG
    int t = node[r].base + a;
//#else
//    int t = base[r] + a;
//#endif
//    printf("t = base[r=%d] + %c = %d\n", r, key[0], a);

//    int checksize = check.size();
//    if (t >= checksize) {
//        printf("t = %d >= check.size() = %d\n", t, checksize);
//        for (int i=checksize; i <= t; ++i) {
//            check.push_back(-1);
//        }
//    }

    int nodesize = node.size();
    if (t >= nodesize) {
//        printf("t = %d >= check.size() = %d\n", t, checksize);
        for (int i=nodesize; i <= t; ++i) {
            node.push_back(darr::node_t<val_t>{});
        }
    }

//#ifdef DEBUG
    if (node[t].check <= -1) {
//#else
//    if (check[t] <= -1) {
//#endif
        insert_str(r, key, POS, val);
    } else {
//        printf("MODIFY\n");
        std::vector<uint8_t> rlist2 = set_list(r);
//#ifdef DEBUG
        std::vector<uint8_t> tlist2 = set_list(node[t].check);
//#else
//        std::vector<uint8_t> tlist2 = set_list(check[t]);
//#endif

        int s;
        if (rlist2.size() + 1 < tlist2.size()) {
            std::vector<uint8_t> b = {key[0]};
//            printf("MODIFY1\n");
            s = modify(r, r, b, rlist2);
        } else {
//            printf("MODIFY2\n");
            std::vector<uint8_t> empty;
//#ifdef DEBUG
            s = modify(r, node[t].check, empty, tlist2);
//#else
//            s = modify(r, check[t], empty, tlist2);
//#endif
        }

        insert_str(s, key, POS, val);

    }
};

template<typename val_t>
void darr::DoubleArray<val_t>::insert_str(int h, const std::vector<uint8_t>& key, int d_pos, const val_t& val) {

    int e1 = key[0];

//#ifdef DEBUG
    int t = node[h].base + e1;
//#else
//    int t = base[h] + e1;
//#endif

//#ifndef DEBUG
//    int basesize = base.size();
//    if (t >= basesize) {
//        for (int k=basesize; k <= t; ++k) {
//            base.push_back(0);
//        }
//    }
//    int checksize = check.size();
//    if (t >= checksize) {
//        for (int k=checksize; k <= t; ++k) {
//            check.push_back(-1);
//        }
//    }
//    int valuesize = value.size();
//    if (t >= valuesize) {
//        for (int k=valuesize; k <= t; ++k) {
//            value.push_back(val_t{});
//        }
//    }
//    base[t] = -d_pos;
//    check[t] = h;
//    value[t] = val;
//    printf("t = base[h=%d] + %c = %d + %d = %d\n", h, key[0], base[h], e1, t);
//    printf("INSERT check[t=%d]=%d\n", t, check[t]);
//#else
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

//#endif

//    check[0] = check.size();

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

//#ifdef DEBUG
    int old_pos = -node[r].base;
//#else
//    int old_pos = -base[r];
//#endif

    int prefixsize = prefix.size();
    for (int i=0; i < prefixsize; ++i) {
        std::vector<uint8_t> pi = {prefix[i]};

        int b = prefix[i];
//        int b = prefix[i] + 1;

//        int basesize = base.size();
//        if (r >= basesize) {
//            for (int k=basesize; k <= r; ++k) {
//                base.push_back(0);
//            }
//        }

        int nodesize = node.size();
        if (r >= nodesize) {
            for (int k=nodesize; k <= r; ++k) {
                node.push_back(darr::node_t<val_t>{});
            }
        }

        int t = x_check(pi);
//        printf("B_INSERT x_check(%c) = %d\n", prefix[i], t);
//#ifdef DEBUG
        node[r].base = t;
//#else
//        base[r] = t;
//#endif

//        int checksize = check.size();
//        if (base[r] + b >= checksize) {
//            for (int k=checksize; k <= base[r]+b; ++k) {
//                check.push_back(-1);
//            }
//        }

//        int valuesize = value.size();
//        if (base[r]+b >= valuesize) {
//            for (int k=valuesize; k <= base[r]+b; ++k) {
//                value.push_back(val_t{});
//            }
//        }

        int nodesize1 = node.size();
        if (node[r].base + b >= nodesize1) {
            for (int k=nodesize1; k <= node[r].base+b; ++k) {
                node.push_back(darr::node_t<val_t>{});
            }
        }

//#ifdef DEBUG
        int s = node[r].base + b;
        node[s].check = r;
        node[s].val = node[r].val;
//        value[node[r].base + b] = value[r];
//        printf("B_INSERT base[r=%d] = %d\n", r, t);
//        printf("B_INSERT check[base[r]+b] = check[%d+%d] = check[%d] = %d\n",
//                node[r].base, b, node[r].base+b, node[node[r].base+b].check);
//        printf("B_INSERT CHECK[%d]=%d\n", node[r].base+b, node[node[r].base+b].check);
//        printf("NODESIZE:%d\n", node.size());
//        for (int z=0; z < node.size(); ++z) {
//            if (node[z].check != -1) {
//                printf("CHECK[z=%d]=%d\n", z, node[z].check);
//            }
//        }
//        printf("CHECK[Z=%d]=%d\n", node[r].base+b, node[node[r].base+b].check);
//#else
//        check[base[r] + b] = r;
//        value[base[r] + b] = value[r];
//        printf("B_INSERT base[r=%d] = %d\n", r, t);
//        printf("B_INSERT check[base[r]+b] = check[%d+%d] = check[%d] = %d\n",
//                base[r], b, base[r]+b, check[base[r]+b]);

//#endif

//#ifdef DEBUG
        r = s;
//        r = node[r].base + b;
//#else
//        r = base[r] + b;
//#endif
    }

//    int basesize = base.size();
//    if (r >= basesize) {
//        for (int k=basesize; k <= r; ++k) {
//            base.push_back(0);
//        }
//    }
    int nodesize = node.size();
    if (r >= nodesize) {
        for (int k=nodesize; k <= r; ++k) {
            node.push_back(darr::node_t<val_t>{});
        }
    }

    std::vector<uint8_t> list = {a[0], b[0]};
    int q = x_check(list);
//#ifdef DEBUG
    node[r].base = q;
//#else
//    base[r] = q;
//#endif
//    printf("B_INSERT base[r=%d]=%d\n", r, q);

//    printf("B_INSERT1 INS_STR(%d, %s, %d)\n", r, b_.c_str(), old_pos);
//#ifdef DEBUG
//    std::cout << "val:" << node[r].val << std::endl;
    insert_str(r, b, old_pos, node[r].val);
//#else
//    std::cout << "val:" << value[r] << std::endl;
//    insert_str(r, b, old_pos, value[r]);
//#endif

//    printf("B_INSERT2 INS_STR(%d, %s, %d)\n", r, a_.c_str(), POS);
    insert_str(r, a, POS, val);
//    std::cout << "val:" << val << std::endl;
};


template<typename val_t>
int darr::DoubleArray<val_t>::x_check(const std::vector<uint8_t>& list) {
    int minq = -1;
    bool found = false;

    int listsize = list.size();
//#ifdef DEBUG
    int nodesize = node.size();
    for (int q=1; q < nodesize; ++q) {
//#else
//    for (int q=1; q < check.size(); ++q) {
//#endif
        bool ok = true;
        for (int i=0; i < listsize; ++i) {
            int c = list[i];

//#ifdef DEBUG
//            if (q + c >= node.size()) {
            if (q + c >= nodesize) {
                continue;
//                break;
//#else
//            if (q + c >= check.size()) {
//#endif
//                continue;
            }
//#ifdef DEBUG
            if (node[q + c].check > -1) {
//#else
//            if (check[q + c] > -1) {
//#endif
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
//        base.push_back(0);
//        check.push_back(-1);
//        value.push_back(val_t{});
        node.push_back(darr::node_t<val_t>{});
//        minq = base.size();
//#ifdef DEBUG
        minq = node.size();
//#endif
    }

//    printf("minq:%d\n", minq);
    return minq;
};

template<typename val_t>
std::vector<uint8_t> darr::DoubleArray<val_t>::set_list(int k) {
    std::vector<uint8_t> ret;

//#ifdef DEBUG
    if (k >= node.size()) {
        return ret;
    }
//#else
//    if (k >= base.size()) {
//        return ret;
//    }
//#endif

//#ifdef DEBUG
    int nodesize = node.size();
    for (int i=0; i < nodesize; ++i) {
//#else
//    int checksize = check.size();
//    for (int i=0; i < checksize; ++i) {
//#endif

//#ifdef DEBUG
        if (node[k].base + i >= nodesize || node[k].base + i < 0) {
            continue;
        }
//#else
//        if (base[k] + i >= checksize || base[k] + i < 0) {
//            continue;
//        }
//#endif

//#ifdef DEBUG
        if (node[node[k].base + i].check == k) {
//#else
//        if (check[base[k] + i] == k) {
//#endif
//            printf("set_list: add[i=%d] %c %d\n", ret.size(), c, i);
//            ret.push_back(c);
            ret.push_back(i);
        }
    }
    return ret;
};


template<typename val_t>
int darr::DoubleArray<val_t>::modify(int current_s, int h, const std::vector<uint8_t>& add, const std::vector<uint8_t>& org) {

//#ifdef DEBUG
    int old_base = node[h].base;
//#else
//    int old_base = base[h];
//#endif

//    printf("old_base = BASE[h=%d] = %d\n", h, old_base);

    int t = x_check(_or(add, org));

//#ifdef DEBUG
    node[h].base = t;
//#else
//    base[h] = t;
//#endif
//    printf("MODIFY base[h=%d] = x_check() = %d\n", h, t);

    int orgsize = org.size();
    for (int i=0; i < orgsize; ++i) {
        int c = org[i];
//        printf("MODIFY char[i=%d]:%c %d\n", i, org[i], c);

        int t = old_base + c;

//        printf("t = old_base(%d) + %c(%d) = %d\n", old_base, org[i], c, t);

//#ifdef DEBUG
        int t2 = node[h].base + c;
//#else
//        int t2 = base[h] + c;
//#endif
//        printf("t2 = base[h=%d] + %c(%d) = %d\n", h, org[i], c, t2);

        int nodesize = node.size();
        if (t2 >= nodesize) {
            for (int i=nodesize; i <= t2; ++i) {
                node.push_back(darr::node_t<val_t>{});
            }
//            printf("%d %d\n", node[node.size()-1].base, node[node.size()-1].check);
        }

//        int checksize = check.size();
//        if (t2 >= checksize) {
//            for (int i=checksize; i <= t2; ++i) {
//                check.push_back(-1);
//            }
//        }

//        int basesize = base.size();
//        if (t2 >= basesize) {
//            for (int i=basesize; i <= t2; ++i) {
//                base.push_back(0);
//            }
//        }

//        int valuesize = value.size();
//        if (t2 >= valuesize) {
//            for (int i=valuesize; i <= t2; ++i) {
//                value.push_back(val_t{});
//            }
//        }

//        printf("MODIFY base[t=%d]=%d\n", t, base[t]);
//        check[t2] = h;
//        base[t2] = base[t];
//        value[t2] = value[t];

//#ifdef DEBUG
        node[t2].check = h;
        node[t2].base = node[t].base;
        node[t2].val = node[t].val;
//#endif

//        printf("MODIFY check[t2=%d] = %d\n", t2, h);
//        printf("MODIFY base[t2=%d] = base[t=%d] = %d\n", t2, t, base[t2]);
//        printf("MODIFY value[t2=%d] = value[t=%d] = %d\n", t2, t, value[t2]);

//#ifdef DEBUG
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
//#else
//        if (base[t] > 0) {
//            for (int q=0; q < check.size(); ++q) {
//                if (check[q] == t) {
//                    check[q] = t2;
//                    printf("MODIFY check[q=%d] = t = %d => t2 = %d\n", q, t, t2);
//                }
//                if (t == current_s) {
//                    current_s = t2;
//                }
//            }
//        }
//#endif

//        printf("MODIFY clear base[t=%d] = 0\n", t);
//        printf("MODIFY clear check[t=%d] = 0\n", t);

//        base[t] = 0;
//        check[t] = -1;
//        value[t] = val_t{};
//#ifdef DEBUG
        node[t].base = 0;
        node[t].check = -1;
        node[t].val = val_t{};
//#endif
    }
    return current_s;
};

template<typename val_t>
void darr::DoubleArray<val_t>::save(const char* filename) {
    FILE* fp = fopen(filename, "wb");

    fwrite(&POS, sizeof(int), 1, fp);

//    int basesize = base.size();
//    fwrite(&basesize, sizeof(int), 1, fp);
//    fwrite(base.data(), sizeof(int), basesize, fp);

//    int checksize = check.size();
//    fwrite(&checksize, sizeof(int), 1, fp);
//    fwrite(check.data(), sizeof(int), checksize, fp);

//    int valuesize = value.size();
//    fwrite(&valuesize, sizeof(int), 1, fp);
//    fwrite(value.data(), sizeof(val_t), valuesize, fp);

    int nodesize = node.size();
    fwrite(&nodesize, sizeof(int), 1, fp);
    fwrite(node.data(), sizeof(darr::node_t<val_t>), nodesize, fp);

    int tailsize = tail.size();
    fwrite(&tailsize, sizeof(int), 1, fp);
    fwrite(tail.data(), sizeof(uint8_t), tailsize, fp);


//    for (int i=0; i < basesize; ++i) {
//        if (base[i] != 0) {
//            printf("i=%d write base %d\n", i, base[i]);
//        }
//    }
    
//    for (int i=0; i < nodesize; ++i) {
//        fread(&node[i], sizeof(darr::node_t), 1, fp);
//        printf("read i:%d base:%d check:%d\n", i, node[i].base, node[i].check);
//    }


//    for (int i=0; i < checksize; ++i) {
//        if (check[i] != -1) {
//            printf("i=%d write check %d\n", i, check[i]);
//        }
//    }

//    printf("write basesize:%d\n", basesize);
//    printf("write checksize:%d\n", checksize);
//    printf("write tailsize:%d\n", tailsize);
//    printf("write valuesize:%d\n", valuesize);

    fclose(fp);
};

template<typename val_t>
void darr::DoubleArray<val_t>::load(const char* filename) {
    FILE* fp = fopen(filename, "rb");

    fread(&POS, sizeof(int), 1, fp);

//    int basesize = 0;
//    fread(&basesize, sizeof(int), 1, fp);
//    base = std::vector<int>(basesize, 0);
//    for (int i=0; i < basesize; ++i) {
//        fread(&base[i], sizeof(int), 1, fp);
//        if (base[i] != 0) {
//        printf("i=%d read base %d\n", i, base[i]);
//        }

//    }

//    int checksize = 0;
//    fread(&checksize, sizeof(int), 1, fp);
//    check = std::vector<int>(checksize, -1);
//    for (int i=0; i < checksize; ++i) {
//        fread(&check[i], sizeof(int), 1, fp);
//        if (check[i] != -1) {
//            printf("i=%d read check %d\n", i, check[i]);
//        }
//    }

//    int valuesize = 0;
//    fread(&valuesize, sizeof(int), 1, fp);
//    value = std::vector<val_t>(valuesize);
//    for (int i=0; i < valuesize; ++i) {
//        fread(&value[i], sizeof(val_t), 1, fp);
//    }

    int nodesize = 0;
    fread(&nodesize, sizeof(int), 1, fp);
    node = std::vector< darr::node_t<val_t> >(nodesize);
    for (int i=0; i < nodesize; ++i) {
        fread(&node[i], sizeof(darr::node_t<val_t>), 1, fp);
//        printf("write i:%d base:%d check:%d\n", i, node[i].base, node[i].check);
    }

    int tailsize = 0;
    fread(&tailsize, sizeof(int), 1, fp);
    tail = std::vector<uint8_t>(tailsize);
    for (int i=0; i < tailsize; ++i) {
        fread(&tail[i], sizeof(uint8_t), 1, fp);
    }


//    printf("read basesize:%d\n", basesize);
//    printf("read checksize:%d\n", checksize);
//    printf("read tailsize:%d\n", tailsize);
//    printf("read valuesize:%d\n", valuesize);

    fclose(fp);
};



template class darr::DoubleArray<int>;
template class darr::DoubleArray<float>;
template class darr::DoubleArray<double>;
