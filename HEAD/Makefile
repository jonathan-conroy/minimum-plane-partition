MAKEFILE      = Makefile

####### Compiler, tools and options

CXX           = /usr/local/opt/llvm/bin/clang++ #g++
CXXFLAGS      = -pipe -fopenmp -O2 -std=gnu++11 -Wall -W -D_REENTRANT -fPIC $(DEFINES)
INCPATH 	  = -I/usr/local/include/
DEL_FILE      = rm -f
LINK          = /usr/local/opt/llvm/bin/clang++ #g++
LFLAGS        = -Wl#,-O1
LIBS          = $(SUBLIBS) -L/usr/local/opt/llvm/lib -lpthread -lomp #/usr/lib/x86_64-linux-gnu/libgomp.so.1 

####### Output directory

OBJECTS_DIR   = ./

####### Files

SRC           = ./src

SOURCES  = $(SRC)/util/gfile.cpp \
		$(SRC)/graphe.cpp \
		$(SRC)/head.cpp \
		$(SRC)/main.cpp \
		$(SRC)/solution.cpp \
		$(SRC)/tabouSearch.cpp

OBJECTS       = gfile.o \
		graphe.o \
		head.o \
		solution.o \
		tabouSearch.o

TARGET        = head


$(TARGET):  $(OBJECTS) main.o
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(SRC)/main.cpp $(OBJCOMP) $(LIBS)

run_all:  $(OBJECTS) run_all.o
	$(LINK) $(LFLAGS) -o run_all $(INCPATH) $(OBJECTS) $(SRC)/run_all.cpp $(OBJCOMP) $(LIBS)


all: Makefile $(TARGET)

clean:
	-$(DEL_FILE) $(OBJECTS) $(TARGET) run_all
	-$(DEL_FILE) *~ core *.core


####### Compile

gfile.o: $(SRC)/util/gfile.cpp $(SRC)/util/gfile.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o gfile.o $(SRC)/util/gfile.cpp

graphe.o: $(SRC)/graphe.cpp $(SRC)/graphe.h \
		$(SRC)/util/gfile.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o graphe.o $(SRC)/graphe.cpp

head.o: $(SRC)/head.cpp $(SRC)/util/gfile.h \
		$(SRC)/head.h \
		$(SRC)/tabouSearch.h \
		$(SRC)/graphe.h \
		$(SRC)/solution.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o head.o $(SRC)/head.cpp

main.o: $(SRC)/main.cpp $(SRC)/graphe.h \
		$(SRC)/util/gfile.h \
		$(SRC)/head.h \
		$(SRC)/tabouSearch.h \
		$(SRC)/solution.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o $(SRC)/main.cpp

run_all.o: $(SRC)/run_all.cpp $(SRC)/graphe.h \
		$(SRC)/util/gfile.h \
		$(SRC)/head.h \
		$(SRC)/tabouSearch.h \
		$(SRC)/solution.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o $(SRC)/main.cpp

solution.o: $(SRC)/solution.cpp $(SRC)/solution.h \
		$(SRC)/graphe.h \
		$(SRC)/util/gfile.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o solution.o $(SRC)/solution.cpp

tabouSearch.o: $(SRC)/tabouSearch.cpp $(SRC)/tabouSearch.h \
		$(SRC)/graphe.h \
		$(SRC)/util/gfile.h \
		$(SRC)/solution.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tabouSearch.o $(SRC)/tabouSearch.cpp
