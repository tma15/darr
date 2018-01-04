#include <darr/da.hpp>
from cython.operator cimport dereference as deref, preincrement as inc
from libc.string cimport const_char
from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp cimport bool

cdef extern from "darr/da.hpp" namespace "darr":
    cdef cppclass CppDoubleArray "darr::DoubleArray"[T]:
        CppDoubleArray() except +

        bool insert(string, T)
        bool has(string)
        T get(string)
        int delete "del"(string)
        void save(const_char*)
        void load(const_char*)
        vector[string] common_prefix_search(string)


cdef class DoubleArray:
    cdef CppDoubleArray[float]* this_ptr

    def __cinit__(self):
        self.this_ptr = new CppDoubleArray[float]()

    def insert(self, key, value):
        cdef string cpp_string = key.encode('utf-8')
        self.this_ptr.insert(cpp_string, value)

    def has(self, key):
        cdef string cpp_string = key.encode('utf-8')
        return self.this_ptr.has(cpp_string)

    def get(self, key):
        cdef string cpp_string = key.encode('utf-8')
        ret = self.this_ptr.get(cpp_string)
        return ret

    def delete(self, key):
        cdef string cpp_string = key.encode('utf-8')
        self.this_ptr.delete(cpp_string)

    def save(self, outfile):
        cdef bytes py_string = outfile.encode('utf-8')
        cdef const_char* out = py_string
        self.this_ptr.save(out)

    def load(self, outfile):
        cdef bytes py_string = outfile.encode('utf-8')
        cdef const_char* out = py_string
        self.this_ptr.load(out)

    def common_prefix_search(self, key):
        cdef string cpp_string = key.encode('utf-8')
        cdef vector[string] ret = self.this_ptr.common_prefix_search(cpp_string)
        cdef vector[string].iterator it = ret.begin()

        common_prefix = []
        while it != ret.end():
            key = deref(it)
            common_prefix.append(key.decode('utf-8'))
            inc(it)
        return common_prefix
