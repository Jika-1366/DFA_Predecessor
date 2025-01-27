CXX = g++
CXXFLAGS = -std=c++17 -O1 -fno-fast-math -fopenmp -mavx2 -Wall -Wextra
INCLUDES = -I..

TARGET = benchmark
SRCS = benchmark_main.cpp ../power_law_tool_7.cpp
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(CXXFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
