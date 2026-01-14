CXX = c++
CXXFLAGS = -std=c++14 -Wall -Wextra
TARGET = msdscript
SRCS = main.cpp cmdline.cpp expr.cpp tests.cpp
OBJS = main.o cmdline.o expr.o tests.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp cmdline.h
	$(CXX) $(CXXFLAGS) -c main.cpp

cmdline.o: cmdline.cpp cmdline.h catch.h
	$(CXX) $(CXXFLAGS) -c cmdline.cpp
expr.o : expr.cpp expr.h
	$(CXX) $(CXXFLAGS) -c expr.cpp
test.o: tests.cpp expr.h catch.h
	$(CXX) $(CXXFLAGS) -c tests.cpp

clean:
	rm -f *.o $(TARGET)
