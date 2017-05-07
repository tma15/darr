#include <stdlib.h>
#include <string.h>

#include <typeinfo>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

#include "da.hpp"

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

    std::string key_ = key + "#";
    std::vector<uint8_t> chars;
    for (int i=0; i < key_.size(); ++i) {
        chars.push_back(key_[i]);
    }
    int n_char = chars.size();

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
        int a = chars[h];
#endif

        t = base[r] + a; // 遷移先

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
        return t;
    } else {

        std::vector<uint8_t> a_h12 = get_subchars(chars, h, chars.size());
        uint8_t* a = a_h12.data();
        uint8_t* start_ptr = &tail[-base[r]-1];
        int match = mem_cmp(start_ptr, a, a_h12.size());
        if (match == -1) {
            return t;
        } else {
            return -1;
        }

    }
};

template<typename val_t>
int darr::DoubleArray<val_t>::del(const std::string& key) {
    std::string key_ = key + "#";
    std::vector<uint8_t> chars;
    for (int i=0; i < key_.size(); ++i) {
        chars.push_back(key_[i]);
    }
    int n_char = chars.size();

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
        int a = chars[h];
#endif

        t = base[r] + a; // 遷移先

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
        base[t] = 0;
        check[t] = -1;
        value[t] = val_t{};
        return t;
    } else {
        std::vector<uint8_t> a_h12 = get_subchars(chars, h, chars.size());
        uint8_t* a = a_h12.data();
        uint8_t* start_ptr = &tail[-base[r]-1];
        int match = mem_cmp(start_ptr, a, a_h12.size());
        if (match == -1) {
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
    if (id != -1) {
        val_t val = value[id];
        return val;
    } else {
        std::cerr << "Key Error: " << key << std::endl;
        exit(1);
    }
};


template<typename val_t>
bool darr::DoubleArray<val_t>::insert(const std::string& key, const val_t& val) {
    std::string key_ = key + "#";

    std::vector<uint8_t> chars;
    for (int i=0; i < key_.size(); ++i) {
        chars.push_back(key_[i]);
    }
    int n_char = chars.size();

    int r = 0; // current node id
    int h = 0; // current position of the given string

#ifdef DEBUG
    printf("INSERT %s %f\n", key_.c_str(), val);
#endif

    while (1) {

#ifdef DEBUG
        int a = keymap[key_[h]];
#else
        int a = chars[h];
#endif

        int t = base[r] + a; // 遷移先

        if (t > check.size()) {
            int checksize = check.size();
            for (int k=checksize-1; k < t; ++k) {
                base.push_back(0);
                check.push_back(-1);
                value.push_back(0);
            }
        }

#ifdef DEBUG
        printf("t = base[r=%d] + %c = %d\n", r, key_[h], t);
        printf("size:%d check[t=%d]=%d\n", check.size(), t, check[t]);
#endif

        if (check[t] != r) {
            std::vector<uint8_t> sub_key = get_subchars(chars, h, n_char);
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

        std::vector<uint8_t> remain = get_subchars(chars, h, n_char);
        uint8_t* remain_ = remain.data();
        uint8_t* start_ptr = &tail[-base[r]-1];
        int prefix_len = mem_cmp(start_ptr, remain_, remain.size());
        if (prefix_len==-1) {
            return true;
        } else {

            std::vector<uint8_t> prefix = get_subchars(chars, h, prefix_len);
            std::vector<uint8_t> remain_key = get_subchars(chars, h+prefix_len+1, n_char);
            std::vector<uint8_t> remain_s_temp = get_subchars(s_temp, prefix_len, s_temp.size());
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
    int a = key[0];
#endif

    int t = base[r] + a;

#ifdef DEBUG
    printf("t = base[r=%d] + %c = %d\n", r, key[0], t);
    printf("check[t=%d] = %d\n", t, check[t]);
#endif

    if (check[t] == -1) {
        insert_str(r, key, POS, val);
    } else {
        std::vector<uint8_t> rlist2 = set_list(r);
        std::vector<uint8_t> tlist2 = set_list(check[t]);

        int s;
        if (rlist2.size() + 1 < tlist2.size()) {

            std::vector<uint8_t> b = get_subchars(key, 0, 1);

            s = modify(r, r, b, rlist2);
        } else {
            std::vector<uint8_t> e;
            s = modify(r, check[t], e, tlist2);
        }

#ifdef DEBUG
        printf("A_INSERT INS_STR(%d, %s, %d)\n", s, key.c_str(), POS);
#endif
        insert_str(s, key, POS, val);

    }
};

template<typename val_t>
void darr::DoubleArray<val_t>::insert_str(int h, const std::vector<uint8_t>& key, int d_pos, const val_t& val) {
#ifdef DEBUG
    int e1 = keymap[key[0]];
#else
    int e1 = key[0];
#endif

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
            value.push_back(int{});
        }
    }

#ifdef DEBUG
    printf("insert! %s t=%d val=%f\n", key.c_str(), t, val);
    printf("t = base[h=%d] + %c = %d + %d = %d\n", h, key[0], base[h], e1, t);
#endif

    base[t] = -d_pos;
    check[t] = h;
    value[t] = val;

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
#ifdef DEBUG
    printf("POS = STR_TAIL(%d, %s) = %d\n", d_pos, e.c_str(), POS + pos);
#endif
    POS = POS + pos;
};

template<typename val_t>
int darr::DoubleArray<val_t>::str_tail(int p, const std::vector<uint8_t>& y) {
    int s_y = y.size();

    if (tail.size() <= p - 1 + s_y) {
        int start = tail.size();
        for (int i=start; i < p - 1 + s_y; ++i) {
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
    for (int i=p-1; i < tail.size(); ++i) {
        y.push_back(tail[i]);
        if (tail[i] == '#') {
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

#ifdef DEBUG
    printf("B_INSERT(%d, %s, %s, %s)\n", r, prefix.c_str(), a.c_str(), b.c_str());
#endif

    int old_pos = -base[r];

#ifdef DEBUG
    printf("old_pos = -base[r=%d] = %d\n", r, old_pos);
#endif

    for (int i=0; i < prefix.size(); ++i) {
//        std::string pi = prefix.substr(i, i+1);
        std::vector<uint8_t> pi = get_subchars(prefix, i, i+1);

#ifdef DEBUG
        int b = keymap[prefix[i]];
#else
        int b = prefix[i];
#endif

        base[r] = x_check(pi);
        check[base[r] + b] = r;
        r = base[r] + b;
    }

    std::vector<uint8_t> list = {a[0], b[0]};
    int q = x_check(list);
    base[r] = q;

#ifdef DEBUG
    printf("r=%d value[r=%d]=%d\n", r, r, value[r]);
    printf("B_INSERT1 INS_STR(%d, %s, %d)\n", r, b.c_str(), old_pos);
#endif
    insert_str(r, b, old_pos, value[r]);

#ifdef DEBUG
    printf("r=%d value[r=%d]=%d\n", r, r, value[r]);
    printf("B_INSERT2 INS_STR(%d, %s, %d)\n", r, a.c_str(), POS);
#endif
    insert_str(r, a, POS, val);
};


template<typename val_t>
int darr::DoubleArray<val_t>::x_check(const std::vector<uint8_t>& list) {
    int minq = -1;
    int minc = -1;
    bool found = false;

#ifdef DEBUG
    printf("list '%s'\n", list.c_str());
#endif

    for (int i=0; i < list.size(); ++i) {
#ifdef DEBUG
        int c = keymap[list[i]];
#else
        int c = list[i];
#endif
        if (c < minc) {
            minc = c;
        }
        for (int q=0; q < check[0]; ++q) {
            if (q + c >= check.size()) {
                continue;
            }
            if (check[q + c] == -1) {
                if (q < minq) {
                    minq = q;
                    found = true;
                }
            }
        }
    }
    if (!found) {
        minq = check.size() - minc;
#ifdef DEBUG
        printf("minq update %d - %d = %d\n", check.size(), minc, minq);
#endif
        base.push_back(0);
        check.push_back(-1);
        value.push_back(0);
    }

#ifdef DEBUG
    std::cout << "found:" << found << std::endl;
    printf("minq:%d\n", minq);
#endif
    return minq;
};

template<typename val_t>
std::vector<uint8_t> darr::DoubleArray<val_t>::set_list(int k) {
    std::vector<uint8_t> ret;
    for (int i=0; i < check.size(); ++i) {
        if (check[base[k] + i] == k) {
#ifdef DEBUG
            ret.push_back(itos[i]);
#else
            ret.push_back(i);
#endif
        }
    }
    return ret;
};


template<typename val_t>
int darr::DoubleArray<val_t>::modify(int current_s, int h, const std::vector<uint8_t>& add, const std::vector<uint8_t>& org) {

    int old_base = base[h];

#ifdef DEBUG
    printf("old_base = BASE[h=%d] = %d\n", h, old_base);
#endif

    base[h] = x_check(_or(add, org));

#ifdef DEBUG
    printf("BASE[h=%d] = X_CHECK(%s) = %d\n", h, o.c_str(), base[h]);
#endif

    for (int i=0; i < org.size(); ++i) {
#ifdef DEBUG
        int c = keymap[org[i]];
#else
        int c = org[i];
#endif

        int t = old_base + c;

#ifdef DEBUG
        printf("t = old_base(%d) + %c(%d) = %d\n", old_base, org[i], c, t);
#endif

        int t2 = base[h] + c;
#ifdef DEBUG
        printf("t2 = base[h=%d] + %c(%d) = %d\n", h, org[i], c, t2);
#endif

        check[t2] = h;
        base[t2] = base[t];
        value[t2] = value[t];

#ifdef DEBUG
        printf("check[t2=%d] = %d\n", t2, h);
        printf("base[t2=%d] = base[t] = %d\n", t2, h, base[t2]);
#endif

        if (base[t] > 0) {
            for (int q=0; q < check[0]; ++q) {
                if (check[q] == t) {
                    check[q] = t2;
                }
                if (t == current_s) {
#ifdef DEBUG
                    printf("current_s = %d is changed by %d\n", current_s, t2);
#endif
                    current_s = t2;
                }
            }
        }

#ifdef DEBUG
        printf("base[t=%d] = 0\n", t);
        printf("check[t=%d] = 0\n", t);
#endif
        base[t] = 0;
        check[t] = 0;
        value[t] = 0;
    }
    return current_s;
};

template<typename val_t>
void darr::DoubleArray<val_t>::save(const char* filename) {
    FILE* fp = fopen(filename, "wb");

    int basesize = base.size();
    fwrite(&basesize, sizeof(int), 1, fp);
    fwrite(base.data(), sizeof(int), basesize, fp);

    int checksize = check.size();
    fwrite(&checksize, sizeof(int), 1, fp);
    fwrite(check.data(), sizeof(int), checksize, fp);

    int tailsize = tail.size();
    fwrite(&tailsize, sizeof(int), 1, fp);
    fwrite(tail.data(), sizeof(uint8_t), tail.size(), fp);

    int valuesize = value.size();
    fwrite(&valuesize, sizeof(int), 1, fp);
    fwrite(value.data(), sizeof(val_t), value.size(), fp);

#if DEBUG
    printf("write basesize:%d\n", basesize);
    printf("write checksize:%d\n", checksize);
    printf("write tailsize:%d\n", tailsize);
    printf("write valuesize:%d\n", valuesize);
#endif

    fclose(fp);
};

template<typename val_t>
void darr::DoubleArray<val_t>::load(const char* filename) {
    FILE* fp = fopen(filename, "rb");

    int basesize;
    fread(&basesize, sizeof(int), 1, fp);
    base = std::vector<int>(basesize, 0);
    for (int i=0; i < basesize; ++i) {
        fread(&base[i], sizeof(int), 1, fp);
    }

    int checksize;
    fread(&checksize, sizeof(int), 1, fp);
    check = std::vector<int>(checksize, 0);
    for (int i=0; i < checksize; ++i) {
        fread(&check[i], sizeof(int), 1, fp);
    }

    int tailsize;
    fread(&tailsize, sizeof(int), 1, fp);
    tail = std::vector<uint8_t>(tailsize, 0);
    for (int i=0; i < tailsize; ++i) {
        fread(&tail[i], sizeof(uint8_t), 1, fp);
    }

    int valuesize;
    fread(&valuesize, sizeof(int), 1, fp);
    value = std::vector<val_t>(valuesize, 0);
    for (int i=0; i < valuesize; ++i) {
        fread(&value[i], sizeof(val_t), 1, fp);
    }

#if DEBUG
    printf("read basesize:%d\n", basesize);
    printf("read checksize:%d\n", checksize);
    printf("read tailsize:%d\n", tailsize);
    printf("read valuesize:%d\n", valuesize);
#endif

    fclose(fp);
};



template class darr::DoubleArray<int>;
template class darr::DoubleArray<float>;
template class darr::DoubleArray<double>;
