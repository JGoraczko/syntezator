CXX = g++
CXXFLAGS = -std=c++11 -lsfml-audio -pthread -lrt
TARGET = processC

$(TARGET): main.o ProcessC.o
	$(CXX) main.o ProcessC.o -o $(TARGET) -lsfml-audio -pthread -lrt

main.o: main.cpp ProcessC.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp ProcessC.hpp

ProcessC.o: ProcessC.hpp ProcessC.cpp 
	$(CXX) $(CXXFLAGS) -c ProcessC.hpp ProcessC.cpp

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.gch
	rm -f $(TARGET)
	rm test*.txt

