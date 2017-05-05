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

//void DoubleArray::build(std::string filename) {
//    std::ifstream ifs(filename.c_str());

//    if (ifs.fail()) {
//        std::cerr << "failed to open:" << filename << std::endl;
//    }

//    std::string line;

//    std::vector<std::string> keys;

//    while (getline(ifs, line)) {
//        keys.push_back(line);

//        insert(line);

//        bool ok = has(line);
//        std::cout << line << " has:" << ok << std::endl;
//    }
//    
//};

//bool DoubleArray::has(std::string key) {
template<typename val_t>
int darr::DoubleArray<val_t>::has(std::string key) {
    key += "#";
    const char* s = key.c_str();
    int n_char = strlen(s);

    int r = 0; // current node id
    int h = 0; // current position of the given string
    int t = 0;

    while (1) {
        if (base[r] < 0) {
            break;
        }

        int a = key[h];
//        int a = keymap[key[h]];
        t = base[r] + a; // 遷移先
        printf("t=%d\n", t);
        
        if (check[t] != r) {
            return -1;
//            return false;
        } else {
            r = t;
        }

        h += 1;
    }

    if (h == n_char+1) {
        printf("found1: t=%d\n", t);
        std::cout << "value=" << value[t] << std::endl;
        return t;
//        return true;
    } else {
        std::string s_temp = fetch_str(-base[r]);

        std::string a_h1 = key.substr(h, key.size());
        if (str_cmp(a_h1, s_temp)==-1) {
            printf("found2: t=%d\n", t);
            std::cout << "value=" << value[t] << std::endl;
            return t;
//            return true;
        } else {
            return -1;
//            return false;
        }
    }
};


template<typename val_t>
bool darr::DoubleArray<val_t>::insert(std::string key, val_t val) {
//bool darr::DoubleArray<val_t>::insert(std::string key, float val) {
    key += "#";
    const char* s = key.c_str();
    int n_char = strlen(s);

    int r = 0; // current node id
    int h = 0; // current position of the given string

    printf("INSERT %s %f\n", key.c_str(), val);

//    const char* tmp2 = key.c_str();
//    setlocale( LC_CTYPE, "jpn" );

//    while (*tmp2) {
//        char size = mblen(tmp2, MB_CUR_MAX);
//        printf("HOGE %.*s\n", size, tmp2);
//        printf("%d\n", size);
//        tmp2 += size;
//    };
    printf("basesize:%d\n", base.size());

    while (1) {
        int a = key[h];
//        int a = keymap[key[h]];
        int t = base[r] + a; // 遷移先

        if (t > check.size()) {
            int checksize = check.size();
            for (int k=checksize-1; k < t; ++k) {
                base.push_back(0);
                check.push_back(-1);
                value.push_back(0);
            }
        }

        printf("t = base[r=%d] + %c = %d\n", r, key[h], t);
        printf("size:%d check[t=%d]=%d\n", check.size(), t, check[t]);

        if (check[t] != r) {
            std::string sub_key = key.substr(h, key.size());
            a_insert(r, sub_key, val);

            printf("after insert %s t=%d\n", key.c_str(), t);
//            darr::printvec(base);
//            darr::printvec(check);
//            darr::printvec_char(tail);

            printf("POS=%d\n", POS);
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
        std::string remain = key.substr(h, key.size());

//        printf("s_temp:%s\n", s_temp.c_str());
//        printf("remain:%s\n", remain.c_str());

        int prefix_len = str_cmp(remain, s_temp);
//        printf("prefix_len:%d\n", prefix_len);
//        printf("h:%d\n", h);
        if (prefix_len==-1) {
            return true;
        } else {

            std::string prefix = key.substr(h, prefix_len);
            std::string remain_key = key.substr(h+prefix_len+1, key.size());
            std::string remain_s_temp = s_temp.substr(prefix_len, s_temp.size());

            b_insert(r, prefix, remain_key, remain_s_temp, val);

            printf("after insert %s t=%d\n", key.c_str(), h);
//            darr::printvec(base);
//            darr::printvec(check);
//            darr::printvec_char(tail);

            return false;
        }
    }

};

template<typename val_t>
void darr::DoubleArray<val_t>::a_insert(int r, std::string key, val_t val) {
//void darr::DoubleArray<val_t>::a_insert(int r, std::string key, float val) {
    printf("A_INSERT(%d %s)\n", r, key.c_str());

    int a = key[0];
//    int a = keymap[key[0]];
    int t = base[r] + a;

    printf("t = base[r=%d] + %c = %d\n", r, key[0], t);
    printf("check[t=%d] = %d\n", t, check[t]);

    if (check[t] == -1) {
        printf("A_INSERT INS_STR(%d, %s, %d)\n", r, key.c_str(), POS);
        insert_str(r, key, POS, val);
    } else {
        std::string rlist = set_list(r);
        std::string tlist = set_list(check[t]);

        printf("R-LIST = SET-LIST(%d) = %s\n", r, rlist.c_str());
        printf("T-LIST = SET-LIST(%d) = %s\n", check[t], tlist.c_str());

        int s;
        if (rlist.size() + 1 < tlist.size()) {
            printf("N(R-LIST) + 1 = %d < N(K-LIST) = %d\n", rlist.size()+1, tlist.size());
            std::string b = key.substr(0, 1);
            printf("MODIFY(%d, %d, %s, %s)\n", r, r, b.c_str(), rlist.c_str());
            s = modify(r, r, b, rlist);
        } else {
            printf("N(R-LIST) + 1 = %d >= N(K-LIST) = %d\n", rlist.size()+1, tlist.size());
            printf("MODIFY(%d, %d, , %s)\n", r, check[t], tlist.c_str());
            s = modify(r, check[t], "", tlist);
        }

        printf("A_INSERT INS_STR(%d, %s, %d)\n", s, key.c_str(), POS);
        insert_str(s, key, POS, val);

//        darr::printvec(base);
//        darr::printvec(check);
//        exit(1);
    }

};

template<typename val_t>
void darr::DoubleArray<val_t>::insert_str(int h, std::string key, int d_pos, val_t val) {
//void darr::DoubleArray<val_t>::insert_str(int h, std::string key, int d_pos, float val) {
    int e1 = key[0];
//    int e1 = keymap[key[0]];
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
//            value.push_back(0);
            value.push_back(int{});
        }
    }

    printf("insert! %s t=%d val=%f\n", key.c_str(), t, val);
    printf("t = base[h=%d] + %c = %d + %d = %d\n", h, key[0], base[h], e1, t);

    base[t] = -d_pos;
    check[t] = h;
//    printf("base size:%d t=%d\n", base.size(), t);
//    printf("check size:%d t=%d\n", check.size(), t);
//    printf("value size:%d t=%d\n", value.size(), t);
//    darr::printvec(value);
//    printf("val[t=%d] before:%f\n", t, value[t]);
    value[t] = val;
//    printf("val[t=%d] after:%f\n", t, val);
//    darr::printvec(value);

    printf("INSERT base[t=%d]=%d\n", t, base[t]);
    printf("INSERT check[t=%d]=%d\n", t, check[t]);

//    for (int i= 0 ; i < base.size(); ++i) {
//        printf("%d base %d\n", i, base[i]);
//    }

    check[0] = check.size();

    printf("key=%s\n", key.c_str());
    std::string a = key.substr(1, key.size());
//    std::string e = a + '$';
    std::string e = key.substr(1, key.size()) + '$';
    const char* tmp = key.c_str();

//    const char* tmp2 = key.c_str();
//    while (*tmp2) {
//        char size = mblen(tmp2, MB_CUR_MAX);
//        printf("tmp2 %.*s\n", size, tmp2);
//        tmp2 += size;
//    };

//    printf("key %s\n", key.c_str());
//    printf("tmp %d %s\n", strlen(tmp), tmp);
//    printf("tmp %d %s\n", std::strlen(tmp), tmp);
//    for (int i=0; i < strlen(tmp); ++i) {
//        printf("tmp i:%d %s\n", i, tmp[i]);
//        std::cout << i << " " << tmp[i] << std::endl;
//    }
//    char* _key[strlen(key.c_str())-1];
//    char* e = new char[strlen(key.c_str())];
    int pos = str_tail(d_pos, e);
    printf("POS = STR_TAIL(%d, %s) = %d\n", d_pos, e.c_str(), POS + pos);
    POS = POS + pos;

};

template<typename val_t>
int darr::DoubleArray<val_t>::str_tail(int p, std::string y) {
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
int darr::DoubleArray<val_t>::str_cmp(std::string x, std::string y) {
    int len = x.size();
    if (x.size() > y.size()) {
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
//void darr::DoubleArray<val_t>::b_insert(int r, std::string prefix, std::string a, std::string b, float val) {
void darr::DoubleArray<val_t>::b_insert(int r, std::string prefix, std::string a, std::string b, val_t val) {

    printf("B_INSERT(%d, %s, %s, %s)\n", r, prefix.c_str(), a.c_str(), b.c_str());

    int old_pos = -base[r];

    printf("old_pos = -base[r=%d] = %d\n", r, old_pos);

    for (int i=0; i < prefix.size(); ++i) {
        std::string pi = prefix.substr(i, i+1);
//        int b = keymap[prefix[i]];
        int b = prefix[i];
        base[r] = x_check(pi);
        check[base[r] + b] = r;
        r = base[r] + b;
    }

    char tmp[2] = {a[0], b[0]};
    std::string list(tmp, 2);
    int q = x_check(list);
    base[r] = q;

//    float v = value[r];
//    fprintf(stderr, "r=%d\n", r);
//    fprintf(stderr, "size=%d\n", value.size());
//    fprintf(stderr, "value[i=0]=%d\n", value[0]);
//    fprintf(stderr, "value[i=1]=%d\n", value[1]);
//    value[r] = 1;
//    for (int i=0; i < value.size(); ++i) {
//        value[i] = 1;
//        fprintf(stderr, "value[i=%d]=%d\n", i, value[i]);
//        fprintf(stdout, "value[i=%d]=%d\n", i, value[i]);
//    }
//    darr::printvec(value);
    printf("r=%d value[r=%d]=%d\n", r, r, value[r]);
    printf("B_INSERT1 INS_STR(%d, %s, %d)\n", r, b.c_str(), old_pos);
//    insert_str(r, b, old_pos, val);
    insert_str(r, b, old_pos, value[r]);
    printf("r=%d value[r=%d]=%d\n", r, r, value[r]);

//    darr::printvec(base);
//    darr::printvec(check);
//    printvec_char(tail);

    printf("B_INSERT2 INS_STR(%d, %s, %d)\n", r, a.c_str(), POS);
    insert_str(r, a, POS, val);

    printf("%d\n", base.size());
//    darr::printvec(base);
    printf("%d\n", check.size());
//    darr::printvec(check);
    printf("%d\n", tail.size());
//    printvec_char(tail);
//    exit(1);
};


template<typename val_t>
int darr::DoubleArray<val_t>::x_check(std::string list) {
    int minq = -1;
    int minc = -1;
    bool found = false;
    printf("list '%s'\n", list.c_str());
    for (int i=0; i < list.size(); ++i) {
        int c = list[i];
//        int c = keymap[list[i]];
        if (c < minc) {
            minc = c;
        }
        for (int q=0; q < check[0]; ++q) {
            if (q + c >= check.size()) {
                continue;
            }
//        for (int q=0; q < check.size(); ++q) {
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
        printf("minq update %d - %d = %d\n", check.size(), minc, minq);
        base.push_back(0);
        check.push_back(-1);
        value.push_back(0);
    }
    std::cout << "found:" << found << std::endl;

    printf("minq:%d\n", minq);
    return minq;
};

template<typename val_t>
std::string darr::DoubleArray<val_t>::set_list(int k) {
    std::string ret;
    for (int i=0; i < check.size(); ++i) {
        if (check[base[k] + i] == k) {
            ret += itos[i];
        }
    }
    return ret;
};

//int darr::DoubleArray::modify(int current_s, int h, std::string add, std::string org) {
template<typename val_t>
int darr::DoubleArray<val_t>::modify(int current_s, int h, std::string add, std::string org) {
    int old_base = base[h];
    printf("old_base = BASE[h=%d] = %d\n", h, old_base);
    base[h] = x_check(_or(add, org));
    std::string o = _or(add, org);
    printf("BASE[h=%d] = X_CHECK(%s) = %d\n", h, o.c_str(), base[h]);

    for (int i=0; i < org.size(); ++i) {
//        int c = keymap[org[i]];
        int c = org[i];

        int t = old_base + c;
        printf("t = old_base(%d) + %c(%d) = %d\n", old_base, org[i], c, t);
        int t2 = base[h] + c;
        printf("t2 = base[h=%d] + %c(%d) = %d\n", h, org[i], c, t2);

        check[t2] = h;
        base[t2] = base[t];
        value[t2] = value[t];
        printf("check[t2=%d] = %d\n", t2, h);
        printf("base[t2=%d] = base[t] = %d\n", t2, h, base[t2]);

        if (base[t] > 0) {
            for (int q=0; q < check[0]; ++q) {
                if (check[q] == t) {
                    check[q] = t2;
                }
                if (t == current_s) {
                    printf("current_s = %d is changed by %d\n", current_s, t2);
                    current_s = t2;
                }
            }
        }
        printf("base[t=%d] = 0\n", t);
        printf("check[t=%d] = 0\n", t);
        base[t] = 0;
        check[t] = 0;
        value[t] = 0;
    }
    return current_s;
};

template class darr::DoubleArray<int>;
template class darr::DoubleArray<float>;
//template class darr::DoubleArray<double>;
