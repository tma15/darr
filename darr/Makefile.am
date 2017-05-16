pkginclude_HEADERS = da.hpp

lib_LTLIBRARIES = libdarr.la

if DEBUG
DBG = -DDEBUG
else
DBG = 
endif

libdarr_la_SOURCES = da.hpp da.cpp
libdarr_la_CXXFLAGS = -std=c++0x ${DBG}
