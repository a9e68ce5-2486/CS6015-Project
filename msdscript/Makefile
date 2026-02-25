CXX = c++
CXXFLAGS = -std=c++14 -Wall -Wextra

TARGET = msdscript
TEST_TARGET = test_msdscript

SRCS = main.cpp cmdline.cpp expr.cpp val.cpp parse.cpp tests.cpp
OBJS = main.o cmdline.o expr.o val.o parse.o tests.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp cmdline.h expr.h val.h parse.h
	$(CXX) $(CXXFLAGS) -c main.cpp

cmdline.o: cmdline.cpp cmdline.h catch.h
	$(CXX) $(CXXFLAGS) -c cmdline.cpp

expr.o: expr.cpp expr.h val.h
	$(CXX) $(CXXFLAGS) -c expr.cpp

val.o: val.cpp val.h expr.h
	$(CXX) $(CXXFLAGS) -c val.cpp

parse.o: parse.cpp parse.h expr.h
	$(CXX) $(CXXFLAGS) -c parse.cpp

tests.o: tests.cpp expr.h val.h parse.h catch.h
	$(CXX) $(CXXFLAGS) -c tests.cpp

$(TEST_TARGET): test_msdscript.cpp exec.cpp exec.h
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) test_msdscript.cpp exec.cpp

doc:
	doxygen Doxyfile

clean:
	rm -f *.o $(TARGET) $(TEST_TARGET)
