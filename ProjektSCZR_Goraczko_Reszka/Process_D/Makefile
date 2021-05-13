CXX = g++
CXXFLAGS = -std=c++11 -lrt
TARGET = processD

$(TARGET): main.o menu.o
	$(CXX) main.o menu.o -o $(TARGET) -lrt

main.o: main.cpp menu.h
	$(CXX) $(CXXFLAGS) -c main.cpp menu.h

menu.o: menu.h menu.cpp 
	$(CXX) $(CXXFLAGS) -c menu.h menu.cpp 

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.gch
	rm -f $(TARGET)

