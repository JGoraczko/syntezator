CXX = g++
CXXFLAGS = -std=c++11 -lrt -pthread 
TARGET = processD

$(TARGET): main.o menu.o
	$(CXX) main.o menu.o -o $(TARGET) -lrt -pthread 

main.o: main.cpp menu.h
	$(CXX) $(CXXFLAGS) -c main.cpp menu.h -I \..

menu.o: menu.h menu.cpp 
	$(CXX) $(CXXFLAGS) -c menu.h menu.cpp -I \..

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.gch
	rm -f $(TARGET)

