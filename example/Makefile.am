bin_PROGRAMS = test_darr
test_darr_SOURCES = test.cpp
test_darr_LDADD = ../darr/libdarr.la

if DEBUG
DBG = -DDEBUG
else
DBG = 
endif

test_darr_CXXFLAGS = -std=c++0x ${DBG}
