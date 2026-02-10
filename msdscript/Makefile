CXX = c++
CXXFLAGS = -std=c++14 -Wall -Wextra

TARGET = msdscript
# Added parse.cpp
SRCS = main.cpp cmdline.cpp expr.cpp parse.cpp tests.cpp
# Added parse.o
OBJS = main.o cmdline.o expr.o parse.o tests.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp cmdline.h expr.h parse.h
	$(CXX) $(CXXFLAGS) -c main.cpp

cmdline.o: cmdline.cpp cmdline.h catch.h
	$(CXX) $(CXXFLAGS) -c cmdline.cpp

expr.o: expr.cpp expr.h
	$(CXX) $(CXXFLAGS) -c expr.cpp

# New rule for parse
parse.o: parse.cpp parse.h expr.h
	$(CXX) $(CXXFLAGS) -c parse.cpp

tests.o: tests.cpp expr.h parse.h catch.h
	$(CXX) $(CXXFLAGS) -c tests.cpp

doc:
	doxygen Doxyfile

clean:
	rm -f *.o $(TARGET)
