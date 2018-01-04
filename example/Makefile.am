bin_PROGRAMS = test_darr
test_darr_SOURCES = test.cpp
test_darr_LDADD = ../darr/libdarr.la

if DEBUG
DBG = -DDEBUG -lprofiler -L$(DARR_GPERF_INSTALL_PREFIX)/lib
else
DBG = 
endif

test_darr_CXXFLAGS = -std=c++0x ${DBG}
