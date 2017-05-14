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
    std::vector<uint8_t> chars;
    int keysize = key.size();
    for (int i=0; i < keysize; ++i) {
//        std::cout << "has i:" << key[i] << std::endl;
        chars.push_back(key[i]);
    }
//    std::cout << std::endl;
    int n_char = chars.size();
//    printf("n_char:%d\n", n_char);
    chars.push_back('\0');
//    chars.push_back('#');

    int r = 0; // current node id
    int h = 0; // current position of the given string
    int t = 0;

    while (1) {
        if (base[r] < 0) {
//            printf("break b[r=%d] = %d < 0\n", r, base[r]);
            break;
        }
//        if (h == n_char+1) {
//            break;
//        }

#ifdef DEBUG
        int a = keymap[key_[h]];
#else
        int a = chars[h];
//        int a = chars[h] + 1;
#endif

        t = base[r] + a;
        printf("has move base[%d] + %c = %d + %d = %d, base[t=%d] = %d basesize:%d\n", r, chars[h], base[r], a, t, t, base[t], base.size());
        if (t >= base.size()) {
            printf("OVER\n");
        }

#ifdef DEBUG
        printf("t=%d\n", t);
#endif
        
//        if (check[t] != r) {
        if (check[t] != r || t >= check.size()) {
            printf("!has1 check[t=%d]=%d != %d = r\n", t, check[t], r);
            return -1;
        } else {
            r = t;
        }

        h += 1;
    }

    if (h == n_char+1) {
//    if (h == n_char) {
#ifdef DEBUG
        printf("found1: t=%d\n", t);
        std::cout << "value=" << value[t] << std::endl;
#endif
//        printf("has1\n");
        return t;
    } else {

        std::vector<uint8_t> s_temp = fetch_str(-base[r]);
        std::vector<uint8_t> remain = get_subchars(chars, h, n_char+1);

        std::string remains;
        for (int i=0; i < remain.size(); ++i) {
            remains += remain[i];
        }
        std::cout << "remain:" << remains << std::endl;
        std::string stemps;
        for (int i=0; i < s_temp.size(); ++i) {
            stemps += s_temp[i];
        }
        std::cout << "s_temp:" << stemps << std::endl;

        uint8_t* remain_ = remain.data();
        uint8_t* s_temp_ = s_temp.data();
        int match = mem_cmp(s_temp_, remain_, remain.size());

        if (match == -1) {
            printf("has2\n");
            return t;
        } else {
            printf("!has2 t=%d\n", t);
            return -1;
        }

    }
};

template<typename val_t>
int darr::DoubleArray<val_t>::del(const std::string& key) {
//    std::string key_ = key + "#";
    std::vector<uint8_t> chars;
    int keysize = key.size();
    for (int i=0; i < keysize; ++i) {
        chars.push_back(key[i]);
    }
    int n_char = chars.size();
    chars.push_back('\0');
//    chars.push_back('#');

    int r = 0; // current node id
    int h = 0; // current position of the given string
    int t = 0;

    while (1) {
        if (base[r] < 0) {
            break;
        }


#ifdef DEBUG
        int a = keymap[key_[h]];
#else
//        int a = chars[h] + 1;
        int a = chars[h];
#endif

        t = base[r] + a;

#ifdef DEBUG
        printf("t=%d\n", t);
#endif
        
        if (check[t] != r) {
            return -1;
        } else {
            r = t;
        }

        h += 1;
    }

    if (h == n_char+1) {
#ifdef DEBUG
        printf("found1: t=%d\n", t);
        std::cout << "value=" << value[t] << std::endl;
#endif
        printf("DEL base[t=%d]=0\n", t);
        base[t] = 0;
        check[t] = -1;
        value[t] = val_t{};
        return t;
    } else {
        std::vector<uint8_t> a_h12 = get_subchars(chars, h, n_char+1);
        uint8_t* a = a_h12.data();
        uint8_t* start_ptr = &tail[-base[r]-1];
        int match = mem_cmp(start_ptr, a, a_h12.size());
        if (match == -1) {
            printf("DEL base[t=%d]=0\n", t);
            base[t] = 0;
            check[t] = -1;
            value[t] = val_t{};

            return t;
        } else {
            return -1;
        }

    }
};



template<typename val_t>
val_t darr::DoubleArray<val_t>::get(const std::string& key) {
    int id = has(key);
//    printf("get id:%d\n", id);

    int valuesize = value.size();
    if (id >= valuesize) {
        std::cout << "id:" << id << " > valuesize:" << valuesize << std::endl;
    }

    if (id != -1) {
        val_t val = value[id];
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
//    std::cout << "insert key:" << key << std::endl;
    for (int i=0; i < keysize; ++i) {
        chars[i] = key[i];
    }
//    std::cout << std::endl;
    int n_char = chars.size();
    chars.push_back('\0');
//    chars.push_back('#');

    int r = 0; // current node id
    int h = 0; // current position of the given string

#ifdef DEBUG
    printf("INSERT %s %f\n", key_.c_str(), val);
#endif

    while (1) {

#ifdef DEBUG
        int a = keymap[key_[h]];
#else
//        int a = chars[h] + 1;
        int a = chars[h];
#endif

        int t = base[r] + a;

        int basesize = base.size();
        if (t > basesize) {
            for (int k=basesize-1; k<=t; ++k) {
                base.push_back(0);
                value.push_back(val_t{});
            }
        }

        int checksize = check.size();
        if (t >= checksize) {
            for (int k=checksize-1; k <= t; ++k) {
                check.push_back(-1);
            }
        }

        printf("insert move base[%d] + %c[h=%d] = %d + %d = %d, base[t=%d] = %d basesize:%d\n", r, chars[h], h, base[r], a, t, t, base[t], base.size());
        if (t >= base.size()) {
            printf("OVER\n");
        }


#ifdef DEBUG
        printf("t = base[r=%d] + %c = %d\n", r, key_[h], t);
        printf("size:%d check[t=%d]=%d\n", check.size(), t, check[t]);
#endif

        if (check[t] != r || t > check.size()) {
//        if (check[t] != r ){
            std::vector<uint8_t> sub_key = get_subchars(chars, h, n_char+1);
            std::string a_;
            for (int i=0; i < sub_key.size(); ++i) {
                a_ += sub_key[i];
            }
//            printf("nchar:%d\n", n_char);
//            printf("sub_key size:%d\n", sub_key.size());
            printf("A_INSERT(r=%d, %s)\n", r, a_.c_str());
            int basesize = base.size();
            if (r > basesize) {
                for (int k=basesize-1; k<=r; ++k) {
                    base.push_back(0);
                    value.push_back(val_t{});
                }
            }

            a_insert(r, sub_key, val);

#ifdef DEBUG
            printf("after insert %s t=%d\n", key_.c_str(), t);
//            darr::printvec(base);
//            darr::printvec(check);
//            darr::printvec_char(tail);
            printf("POS=%d\n", POS);
#endif

            return false;
        } else {
            int basesize = base.size();
            if (r > basesize) {
                for (int k=basesize-1; k<=r; ++k) {
                    base.push_back(0);
                    value.push_back(val_t{});
                }
            }

            r = t;
        }

        if (base[r] < 0) {
            break;
        }

        h += 1;
    }

    if (h == n_char+1) {
        return true;
    } else {

        std::vector<uint8_t> s_temp = fetch_str(-base[r]);

#ifdef DEBUG
        printf("s_temp:%s\n", s_temp.c_str());
        printf("remain:%s\n", remain.c_str());
#endif

//        printf("TAIL\n");
//        for (int i=0; i < tail.size(); ++i) {
//            printf("%c, ", tail[i]);
//        }
//        printf("\n");
        printf("h:%d \n", h+1);

        std::vector<uint8_t> remain = get_subchars(chars, h+1, n_char+1);
//        std::vector<uint8_t> remain = get_subchars(chars, h, n_char+1);

        std::string remains;
        for (int i=0; i < remain.size(); ++i) {
            remains += remain[i];
        }
        std::cout << "remain:" << remains << std::endl;
        std::string stemps;
        for (int i=0; i < s_temp.size(); ++i) {
            stemps += s_temp[i];
        }
        std::cout << "s_temp:" << stemps << std::endl;

        uint8_t* remain_ = remain.data();
        uint8_t* s_temp_ = s_temp.data();
        int prefix_len = mem_cmp(s_temp_, remain_, remain.size());
        if (prefix_len==-1) {
            return true;
        } else {

            std::vector<uint8_t> prefix = get_subchars(chars, h+1, h+1+prefix_len);
            std::vector<uint8_t> remain_key = get_subchars(chars, h+1+prefix_len, n_char+1);
//            std::vector<uint8_t> prefix = get_subchars(chars, h, h+prefix_len);
//            std::vector<uint8_t> remain_key = get_subchars(chars, h+prefix_len, n_char+1);

            std::vector<uint8_t> remain_s_temp = get_subchars(s_temp, prefix_len, s_temp.size());

            std::string prefixs;
            for (int i=0; i < prefix.size(); ++i) {
                prefixs += prefix[i];
            }
            std::cout << "prefix:" << prefixs << std::endl;

            printf("B_INSERT\n");
            b_insert(r, prefix, remain_key, remain_s_temp, val);


#ifdef DEBUG
            printf("after insert %s t=%d\n", key_.c_str(), h);
//            darr::printvec(base);
//            darr::printvec(check);
//            darr::printvec_char(tail);
#endif

            return false;
        }
    }

};

template<typename val_t>
void darr::DoubleArray<val_t>::a_insert(int r, const std::vector<uint8_t>& key, const val_t& val) {

#ifdef DEBUG
//    printf("A_INSERT(%d %s)\n", r, key.c_str());
    int a = keymap[key[0]];
#else
//    int a = key[0] + 1;
    int a = key[0];
#endif

    int t = base[r] + a;
//    printf("t = base[r=%d] + %c = %d\n", r, key[0], a);

#ifdef DEBUG
    printf("t = base[r=%d] + %c = %d\n", r, key[0], t);
    printf("check[t=%d] = %d\n", t, check[t]);
#endif

    int checksize = check.size();
    if (t >= checksize) {
        printf("t = %d >= check.size() = %d\n", t, checksize);
        for (int i=checksize; i <= t; ++i) {
            check.push_back(-1);
        }
    }

    if (check[t] <= -1) {
//    if (check[t] > -1) {
        insert_str(r, key, POS, val);
    } else {
        printf("MODIFY\n");
        std::vector<uint8_t> rlist2 = set_list(r);
        std::vector<uint8_t> tlist2 = set_list(check[t]);

        int s;
        if (rlist2.size() + 1 < tlist2.size()) {
            std::vector<uint8_t> b = {key[0]};
            printf("MODIFY1\n");
            s = modify(r, r, b, rlist2);
        } else {
            printf("MODIFY2\n");
            std::vector<uint8_t> empty;
            s = modify(r, check[t], empty, tlist2);
        }

//#ifdef DEBUG
//        printf("A_INSERT INS_STR(%d, %s, %d)\n", s, key.c_str(), POS);
//#endif
        insert_str(s, key, POS, val);

    }
//    insert_str(r, key, POS, val);
};

template<typename val_t>
void darr::DoubleArray<val_t>::insert_str(int h, const std::vector<uint8_t>& key, int d_pos, const val_t& val) {
#ifdef DEBUG
    int e1 = keymap[key[0]];
#else
    int e1 = key[0];
//    int e1 = key[0] + 1;
#endif

//    printf("INSERTSTR\n");
    int t = base[h] + e1;

    int basesize = base.size();
    if (t >= basesize) {
        for (int k=basesize; k <= t; ++k) {
            base.push_back(0);
        }
    }
    int checksize = check.size();
    if (t >= checksize) {
        for (int k=checksize; k <= t; ++k) {
            check.push_back(-1);
        }
    }
    int valuesize = value.size();
    if (t >= valuesize) {
        for (int k=valuesize; k <= t; ++k) {
            value.push_back(val_t{});
        }
    }

#ifdef DEBUG
    printf("insert! %s t=%d val=%f\n", key.c_str(), t, val);
    printf("t = base[h=%d] + %c = %d + %d = %d\n", h, key[0], base[h], e1, t);
#endif

//    printf("basesize=%d\n", basesize);
//    printf("checksize=%d\n", checksize);
//    printf("valuesize=%d\n", valuesize);
//    printf("-d_pos=%d\n", -d_pos);

    base[t] = -d_pos;
    check[t] = h;
    value[t] = val;
    printf("t = base[h=%d] + %c = %d + %d = %d\n", h, key[0], base[h], e1, t);
    printf("INSERT check[t=%d]=%d\n", t, check[t]);

#ifdef DEBUG
    printf("INSERT base[t=%d]=%d\n", t, base[t]);
    printf("INSERT check[t=%d]=%d\n", t, check[t]);
#endif

    check[0] = check.size();

#ifdef DEBUG
    printf("key=%s\n", key.c_str());
#endif
    std::vector<uint8_t> e = get_subchars(key, 1, key.size());
    e.push_back('$');

    int pos = str_tail(d_pos, e);

    std::string a;
    for (int i=0; i < e.size(); ++i) {
        a += e[i];
    }
    printf("POS = STR_TAIL(%d, %s) = %d\n", d_pos, a.c_str(), POS + pos);
//#ifdef DEBUG
//    printf("POS = STR_TAIL(%d, %s) = %d\n", d_pos, e.c_str(), POS + pos);
//#endif
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

//    printf("TAIL\n");
//    for (int i=0; i < tail.size(); ++i) {
//        printf("%c, ", tail[i]);
//    }
//    printf("\n");

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
//        printf("compare %c %c\n", x[i], y[i]);
        if (x[i] == y[i]) {
            k += 1;
//            printf("k+=1=%d\n", k);
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

    std::string a_;
    for (int i=0; i < a.size(); ++i) {
        a_ += a[i];
    }
    std::string b_;
    for (int i=0; i < b.size(); ++i) {
        b_ += b[i];
    }
    std::string prefix_;
    for (int i=0; i < prefix.size(); ++i) {
        prefix_ += prefix[i];
    }
    printf("B_INSERT(%d, %s, %s, %s)\n", r, prefix_.c_str(), a_.c_str(), b_.c_str());

#ifdef DEBUG
    printf("B_INSERT(%d, %s, %s, %s)\n", r, prefix.c_str(), a.c_str(), b.c_str());
#endif

    int old_pos = -base[r];

#ifdef DEBUG
    printf("old_pos = -base[r=%d] = %d\n", r, old_pos);
#endif

    int prefixsize = prefix.size();
    for (int i=0; i < prefixsize; ++i) {
        std::vector<uint8_t> pi = {prefix[i]};

#ifdef DEBUG
        int b = keymap[prefix[i]];
#else
        int b = prefix[i];
//        int b = prefix[i] + 1;
#endif

        int basesize = base.size();
        if (r >= basesize) {
            for (int k=basesize; k <= r; ++k) {
                base.push_back(0);
            }
        }

        int t = x_check(pi);
        printf("B_INSERT x_check(%c) = %d\n", prefix[i], t);
        base[r] = t;

        int checksize = check.size();
        if (base[r] + b >= checksize) {
            for (int k=checksize; k <= base[r]+b; ++k) {
                check.push_back(-1);
            }
        }
        int valuesize = value.size();
        if (base[r]+b >= valuesize) {
            for (int k=valuesize; k <= base[r]+b; ++k) {
                value.push_back(val_t{});
            }
        }

        check[base[r] + b] = r;
        value[base[r] + b] = value[r];

        printf("B_INSERT base[r=%d] = %d\n", r, t);
        printf("B_INSERT check[base[r]+b] = check[%d+%d] = check[%d] = %d\n",
                base[r], b, base[r]+b, check[base[r]+b]);
//        printf("value[base[r]+b] = value[%d+%d]=", base[r], b);
//        std::cout << value[base[r]+b] << std::endl;
//        printf("checksize:%d index:%d\n", check.size(), base[r]+b);
        r = base[r] + b;
//        printf("r = base[r=%d] + %c = %d + %d = %d\n", r, prefix[i], base[r], b,r);
    }

    int basesize = base.size();
    if (r >= basesize) {
        for (int k=basesize; k <= r; ++k) {
            base.push_back(0);
        }
    }

    std::vector<uint8_t> list = {a[0], b[0]};
    int q = x_check(list);
    base[r] = q;
    printf("B_INSERT base[r=%d]=%d\n", r, q);

#ifdef DEBUG
    printf("r=%d value[r=%d]=%d\n", r, r, value[r]);
    printf("B_INSERT1 INS_STR(%d, %s, %d)\n", r, b.c_str(), old_pos);
#endif
    printf("B_INSERT1 INS_STR(%d, %s, %d)\n", r, b_.c_str(), old_pos);
//    std::cout << "value:" << value[r] << std::endl;
    insert_str(r, b, old_pos, value[r]);

#ifdef DEBUG
    printf("r=%d value[r=%d]=%d\n", r, r, value[r]);
    printf("B_INSERT2 INS_STR(%d, %s, %d)\n", r, a.c_str(), POS);
#endif
    printf("B_INSERT2 INS_STR(%d, %s, %d)\n", r, a_.c_str(), POS);
//    std::cout << "value:" << val << std::endl;
    insert_str(r, a, POS, val);
};


template<typename val_t>
int darr::DoubleArray<val_t>::x_check(const std::vector<uint8_t>& list) {
    int minq = -1;
//    int minc = -1;
//    int minc = list[0];

//    int minc = 1;
//    if (list.size() > 0) {
//        minc = list[0] + 1;
//        minc = list[0];
//    }
    bool found = false;

#ifdef DEBUG
    printf("list '%s'\n", list.c_str());
#endif

    int listsize = list.size();
//    for (int q=0; q < check.size(); ++q) {
    for (int q=1; q < check.size(); ++q) {
        bool ok = true;
        for (int i=0; i < listsize; ++i) {
            int c = list[i];
            if (q + c >= check.size()) {
                continue;
            }
            if (check[q + c] > -1) {
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

//    int listsize = list.size();
//    for (int i=0; i < listsize; ++i) {
//#ifdef DEBUG
//        int c = keymap[list[i]];
//#else
//        int c = list[i];
//        int c = list[i] + 1;
//#endif
//        if (c < minc) {
//            minc = c;
//        }
//        for (int q=0; q < check.size(); ++q) {
//            if (q + c >= check.size()) {
//                continue;
//            }
//            if (check[q + c] <= -1) {
//                if (q < minq) {
//                    minq = q;
//                    found = true;
//                }
//            }
//        }
//    }
    if (!found) {
//        minq = check.size() - minc;
//#ifdef DEBUG
//        printf("minq update: check.size() - minc = %d - %d = %d\n", check.size(), minc, minq);
//#endif
//        printf("expand in x_check\n");
        base.push_back(0);
        check.push_back(-1);
        value.push_back(val_t{});
        minq = base.size();
    }

//#ifdef DEBUG
//    std::cout << "found:" << found << std::endl;
//    printf("minq:%d\n", minq);
//#endif
    printf("minq:%d\n", minq);
    if (minq == 0) {
        printf("MINQ == 0!\n");
    }
    return minq;
};

template<typename val_t>
std::vector<uint8_t> darr::DoubleArray<val_t>::set_list(int k) {
    std::vector<uint8_t> ret;
//std::vector<int> darr::DoubleArray<val_t>::set_list(int k) {
//    std::vector<int> ret;

//    fprintf(stderr, "set-list k=%d base.size()=%d\n", k, base.size());
    if (k >= base.size()) {
        return ret;
    }

//    for (int i=0; i < check.size(); ++i) {
    int checksize = check.size();
    for (int i=0; i < checksize; ++i) {
//        uint8_t c = i + 1;
        uint8_t c = i;
//        if (base[k] + c >= checksize) {
//            break;
//        }
//        if (base[k] + c >= checksize || base[k] + c < 0) {
        if (base[k] + i >= checksize || base[k] + i < 0) {
            continue;
        }

//        fprintf(stderr, "base[k=%d]=%d\n", k, base[k]);
//        fprintf(stderr, "base[k] + c = %d checksize=%d\n", base[k] + c, checksize);
//        fprintf(stderr, "check[base[k]=%d + c=%d]=%d\n", base[k], c, check[base[k]+c]);

//        if (check[base[k] + c] == k) {
        if (check[base[k] + i] == k) {
#ifdef DEBUG
            ret.push_back(itos[i]);
#else
            printf("set_list: add[i=%d] %c %d\n", ret.size(), c, i);
//            ret.push_back(c);
            ret.push_back(i);
#endif
        }
    }
    return ret;
};


template<typename val_t>
int darr::DoubleArray<val_t>::modify(int current_s, int h, const std::vector<uint8_t>& add, const std::vector<uint8_t>& org) {

//    if (h >= base.size()) {
//        printf("h = %d >= base.size() = %d\n", h, base.size());
//    }

    int old_base = base[h];

//#ifdef DEBUG
    printf("old_base = BASE[h=%d] = %d\n", h, old_base);
//#endif

    int t = x_check(_or(add, org));
    base[h] = t;
    printf("MODIFY base[h=%d] = x_check() = %d\n", h, t);
//    print_vec(base);

//#ifdef DEBUG
    printf("BASE[h=%d] = %d\n", h, base[h]);
//#endif

    int orgsize = org.size();
    for (int i=0; i < orgsize; ++i) {
//#ifdef DEBUG
//        int c = keymap[org[i]];
//#else
        int c = org[i];
        printf("MODIFY char[i=%d]:%c %d\n", i, org[i], c);
//        int c = org[i] + 1;
//#endif

        int t = old_base + c;

//#ifdef DEBUG
        printf("t = old_base(%d) + %c(%d) = %d\n", old_base, org[i], c, t);
//#endif

        int t2 = base[h] + c;
//#ifdef DEBUG
        printf("t2 = base[h=%d] + %c(%d) = %d\n", h, org[i], c, t2);
//#endif

        int checksize = check.size();
        if (t2 >= checksize) {
            for (int i=checksize; i <= t2; ++i) {
                check.push_back(-1);
            }
        }

        int basesize = base.size();
        if (t2 >= basesize) {
            for (int i=basesize; i <= t2; ++i) {
                base.push_back(0);
            }
        }

        int valuesize = value.size();
        if (t2 >= valuesize) {
            for (int i=valuesize; i <= t2; ++i) {
                value.push_back(val_t{});
            }
        }

        printf("MODIFY base[t=%d]=%d\n", t, base[t]);
        check[t2] = h;
        base[t2] = base[t];
        value[t2] = value[t];

//#ifdef DEBUG
        printf("MODIFY check[t2=%d] = %d\n", t2, h);
//        printf("MODIFY base[t2=%d] = base[t] = %d\n", t2, h, base[t2]);
        printf("MODIFY base[t2=%d] = base[t=%d] = %d\n", t2, t, base[t2]);
//        print_vec(base);
//#endif

        if (base[t] > 0) {
            for (int q=0; q < check.size(); ++q) {
                if (check[q] == t) {
                    check[q] = t2;
                    printf("MODIFY check[q=%d] = t = %d => t2 = %d\n", q, t, t2);
                }
                if (t == current_s) {
                    current_s = t2;
                }
            }

        }

//#ifdef DEBUG
//        printf("base[t=%d] = 0\n", t);
//        printf("check[t=%d] = 0\n", t);
//#endif
        printf("MODIFY clear base[t=%d] = 0\n", t);
        printf("MODIFY clear check[t=%d] = 0\n", t);

        base[t] = 0;
        check[t] = -1;
        value[t] = val_t{};
    }
    return current_s;
};

template<typename val_t>
void darr::DoubleArray<val_t>::save(const char* filename) {
    FILE* fp = fopen(filename, "wb");

    fwrite(&POS, sizeof(int), 1, fp);

    int basesize = base.size();
    fwrite(&basesize, sizeof(int), 1, fp);
    fwrite(base.data(), sizeof(int), basesize, fp);

    int checksize = check.size();
    fwrite(&checksize, sizeof(int), 1, fp);
    fwrite(check.data(), sizeof(int), checksize, fp);

    int tailsize = tail.size();
    fwrite(&tailsize, sizeof(int), 1, fp);
    fwrite(tail.data(), sizeof(uint8_t), tailsize, fp);

    int valuesize = value.size();
    fwrite(&valuesize, sizeof(int), 1, fp);
    fwrite(value.data(), sizeof(val_t), valuesize, fp);

//    for (int i=0; i < basesize; ++i) {
//        if (base[i] != 0) {
//            printf("i=%d write base %d\n", i, base[i]);
//        }
//    }

//    for (int i=0; i < checksize; ++i) {
//        if (check[i] != -1) {
//            printf("i=%d write check %d\n", i, check[i]);
//        }
//    }

//#if DEBUG
//    printf("write basesize:%d\n", basesize);
//    printf("write checksize:%d\n", checksize);
//    printf("write tailsize:%d\n", tailsize);
//    printf("write valuesize:%d\n", valuesize);
//#endif

    fclose(fp);
};

template<typename val_t>
void darr::DoubleArray<val_t>::load(const char* filename) {
    FILE* fp = fopen(filename, "rb");

    fread(&POS, sizeof(int), 1, fp);

    int basesize = 0;
    fread(&basesize, sizeof(int), 1, fp);
    base = std::vector<int>(basesize, 0);
    for (int i=0; i < basesize; ++i) {
        fread(&base[i], sizeof(int), 1, fp);
//        if (base[i] != 0) {
//        printf("i=%d read base %d\n", i, base[i]);
//        }

    }

    int checksize = 0;
    fread(&checksize, sizeof(int), 1, fp);
    check = std::vector<int>(checksize, -1);
    for (int i=0; i < checksize; ++i) {
        fread(&check[i], sizeof(int), 1, fp);
//        if (check[i] != -1) {
//            printf("i=%d read check %d\n", i, check[i]);
//        }
    }

    int tailsize = 0;
    fread(&tailsize, sizeof(int), 1, fp);
    tail = std::vector<uint8_t>(tailsize);
    for (int i=0; i < tailsize; ++i) {
        fread(&tail[i], sizeof(uint8_t), 1, fp);
    }

    int valuesize = 0;
    fread(&valuesize, sizeof(int), 1, fp);
    value = std::vector<val_t>(valuesize);
    for (int i=0; i < valuesize; ++i) {
        fread(&value[i], sizeof(val_t), 1, fp);
    }

//#if DEBUG
//    printf("read basesize:%d\n", basesize);
//    printf("read checksize:%d\n", checksize);
//    printf("read tailsize:%d\n", tailsize);
//    printf("read valuesize:%d\n", valuesize);
//#endif

    fclose(fp);
};



template class darr::DoubleArray<int>;
template class darr::DoubleArray<float>;
template class darr::DoubleArray<double>;
