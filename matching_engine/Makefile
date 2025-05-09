CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra
TARGET=app.out

SRCS=main.cpp
INCLUDE=order.hpp ring_buffer.hpp port.hpp mapped_vector.hpp

$(TARGET) : $(SRCS) $(INCLUDE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm *.out





