CXX = g++
CXXFLAGS = -std=c++11 -lsfml-audio
TARGET = processC

$(TARGET): main.o ProcessC.o
	$(CXX) main.o ProcessC.o -o $(TARGET) -lsfml-audio

main.o: main.cpp ProcessC.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp ProcessC.hpp

ProcessC.o: ProcessC.hpp ProcessC.cpp 
	$(CXX) $(CXXFLAGS) -c ProcessC.hpp ProcessC.cpp

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.gch
	rm -f $(TARGET)

