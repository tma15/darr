#include <stdlib.h>
#include <string.h>

#include <typeinfo>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

#include "da.hpp"

void printvec(const std::vector<int>& v) {
    printf("[");
    for (int i=0; i < v.size()-1; ++i) {
        std::cout << i << ":" << v[i] << ",";
    }
};

void printvec_char(const std::vector<char>& v) {
    printf("[");
    for (int i=0; i < v.size()-1; ++i) {
        printf("%c, ", v[i]);
    }
    printf("%c]\n", v[v.size()-1]);
};

std::string darr::_or(std::string x, std::string y) {
    std::string ret = y;
    for (int i=0; i < x.size(); ++i) {
        bool in = false;
        for (int j=0; j < y.size(); ++j) {
            if (x[i] == y[j]) {
                in = true;
                break;
            }
        }
        if (!in) {
            ret += x[i];
        }
    }
    return ret;
};

template<typename val_t>
int darr::DoubleArray<val_t>::has(const std::string& key) {
    std::string key_ = key + "#";
    const char* s = key_.c_str();
    int n_char = strlen(s);

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
        int a = key_[h];
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
        std::string s_temp = fetch_str(-base[r]);

        std::string a_h1 = key_.substr(h, key_.size());
        if (str_cmp(a_h1, s_temp)==-1) {
#ifdef DEBUG
            printf("found2: t=%d\n", t);
            std::cout << "value=" << value[t] << std::endl;
#endif
            return t;
        } else {
            return -1;
        }
    }
};

template<typename val_t>
int darr::DoubleArray<val_t>::del(const std::string& key) {
    std::string key_ = key + "#";
    const char* s = key_.c_str();
    int n_char = strlen(s);

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
        int a = key_[h];
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
        std::string s_temp = fetch_str(-base[r]);

        std::string a_h1 = key_.substr(h, key_.size());
        if (str_cmp(a_h1, s_temp)==-1) {
#ifdef DEBUG
            printf("found2: t=%d\n", t);
            std::cout << "value=" << value[t] << std::endl;
#endif
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
    const char* s = key_.c_str();
    int n_char = strlen(s);

    int r = 0; // current node id
    int h = 0; // current position of the given string

#ifdef DEBUG
    printf("INSERT %s %f\n", key_.c_str(), val);
#endif

    while (1) {

#ifdef DEBUG
        int a = keymap[key_[h]];
#else
        int a = key_[h];
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
            std::string sub_key = key_.substr(h, key_.size());
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
        std::string s_temp = fetch_str(-base[r]);
        std::string remain = key_.substr(h, key_.size());

#ifdef DEBUG
        printf("s_temp:%s\n", s_temp.c_str());
        printf("remain:%s\n", remain.c_str());
#endif

        int prefix_len = str_cmp(remain, s_temp);
        if (prefix_len==-1) {
            return true;
        } else {

            std::string prefix = key_.substr(h, prefix_len);
            std::string remain_key = key_.substr(h+prefix_len+1, key_.size());
            std::string remain_s_temp = s_temp.substr(prefix_len, s_temp.size());

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
void darr::DoubleArray<val_t>::a_insert(int r, const std::string& key, const val_t& val) {

#ifdef DEBUG
    printf("A_INSERT(%d %s)\n", r, key.c_str());
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
#ifdef DEBUG
        printf("A_INSERT INS_STR(%d, %s, %d)\n", r, key.c_str(), POS);
#endif
        insert_str(r, key, POS, val);
    } else {
        std::string rlist = set_list(r);
        std::string tlist = set_list(check[t]);

#ifdef DEBUG
        printf("R-LIST = SET-LIST(%d) = %s\n", r, rlist.c_str());
        printf("T-LIST = SET-LIST(%d) = %s\n", check[t], tlist.c_str());
#endif

        int s;
        if (rlist.size() + 1 < tlist.size()) {
#ifdef DEBUG
            printf("N(R-LIST) + 1 = %d < N(K-LIST) = %d\n", rlist.size()+1, tlist.size());
#endif

            std::string b = key.substr(0, 1);

#ifdef DEBUG
            printf("MODIFY(%d, %d, %s, %s)\n", r, r, b.c_str(), rlist.c_str());
#endif

            s = modify(r, r, b, rlist);
        } else {
#ifdef DEBUG
            printf("N(R-LIST) + 1 = %d >= N(K-LIST) = %d\n", rlist.size()+1, tlist.size());
            printf("MODIFY(%d, %d, , %s)\n", r, check[t], tlist.c_str());
#endif

            s = modify(r, check[t], "", tlist);
        }

#ifdef DEBUG
        printf("A_INSERT INS_STR(%d, %s, %d)\n", s, key.c_str(), POS);
#endif
        insert_str(s, key, POS, val);

//        darr::printvec(base);
//        darr::printvec(check);
//        exit(1);
    }

};

template<typename val_t>
void darr::DoubleArray<val_t>::insert_str(int h, const std::string& key, int d_pos, const val_t& val) {
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
    std::string e = key.substr(1, key.size()) + '$';
    const char* tmp = key.c_str();

    int pos = str_tail(d_pos, e);
#ifdef DEBUG
    printf("POS = STR_TAIL(%d, %s) = %d\n", d_pos, e.c_str(), POS + pos);
#endif
    POS = POS + pos;

};

template<typename val_t>
int darr::DoubleArray<val_t>::str_tail(int p, const std::string& y) {
    const char* str = y.c_str();
    int s_y = strlen(str);

    if (tail.size() <= p - 1 + s_y) {
        int start = tail.size();
        for (int i=start; i < p - 1 + s_y; ++i) {
            tail.push_back('?');
        }
    }

    for (int i=0; i < s_y; ++i) {
        tail[p-1+i] = str[i];
    }

    if (p == POS) {
        return s_y;
    } else {
        return 0;
    }
};

template<typename val_t>
std::string darr::DoubleArray<val_t>::fetch_str(int p) {
    std::string y;
    for (int i=p-1; i < tail.size(); ++i) {
        y += tail[i];
        if (tail[i] == '#') {
            break;
        }
    }
    return y;
};

template<typename val_t>
int darr::DoubleArray<val_t>::str_cmp(const std::string& x, const std::string& y) {
    int len = x.size();
    if (len > y.size()) {
        len = y.size();
    };

    int k = 0;
    for (int i=0; i < len; ++i) {
        if (x[i] == y[i]) {
            k += 1;
        } else {
            break;
        }
    }

    if (x.size() == y.size() && k == len) {
        return -1;
    } else {
        return k;
    }
};

template<typename val_t>
void darr::DoubleArray<val_t>::b_insert(int r, const std::string& prefix, const std::string& a, const std::string& b, const val_t& val) {

#ifdef DEBUG
    printf("B_INSERT(%d, %s, %s, %s)\n", r, prefix.c_str(), a.c_str(), b.c_str());
#endif

    int old_pos = -base[r];

#ifdef DEBUG
    printf("old_pos = -base[r=%d] = %d\n", r, old_pos);
#endif

    for (int i=0; i < prefix.size(); ++i) {
        std::string pi = prefix.substr(i, i+1);

#ifdef DEBUG
        int b = keymap[prefix[i]];
#else
        int b = prefix[i];
#endif

        base[r] = x_check(pi);
        check[base[r] + b] = r;
        r = base[r] + b;
    }

    char tmp[2] = {a[0], b[0]};
    std::string list(tmp, 2);
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
int darr::DoubleArray<val_t>::x_check(const std::string& list) {
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
std::string darr::DoubleArray<val_t>::set_list(int k) {
    std::string ret;
    for (int i=0; i < check.size(); ++i) {
        if (check[base[k] + i] == k) {
#ifdef DEBUG
            ret += itos[i];
#else
            ret += i;
#endif
        }
    }
    return ret;
};

template<typename val_t>
int darr::DoubleArray<val_t>::modify(int current_s, int h, const std::string& add, const std::string& org) {
    int old_base = base[h];
#ifdef DEBUG
    printf("old_base = BASE[h=%d] = %d\n", h, old_base);
#endif
    base[h] = x_check(_or(add, org));
    std::string o = _or(add, org);
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
    fwrite(tail.data(), sizeof(char), tail.size(), fp);

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
    tail = std::vector<char>(tailsize, 0);
    for (int i=0; i < tailsize; ++i) {
        fread(&tail[i], sizeof(char), 1, fp);
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
