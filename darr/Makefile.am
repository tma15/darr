lib_LTLIBRARIES = libdarr.la
libdarr_la_SOURCES = da.hpp da.cpp
libdarr_la_CXXFLAGS = -std=c++0x ${DBG}

bin_PROGRAMS = test_darr
test_darr_SOURCES = test.cpp
test_darr_LDADD = libdarr.la

if DEBUG
DBG = -DDEBUG
else
DBG = 
endif

test_darr_CXXFLAGS = -std=c++0x ${DBG}
