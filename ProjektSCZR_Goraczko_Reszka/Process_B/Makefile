CXX = g++
CXXFLAGS = -std=c++11 -lsfml-audio -pthread -lrt
TARGET = processB

$(TARGET): main.o
	$(CXX) main.o -o $(TARGET) -lsfml-audio -pthread -lrt

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -I ../


.PHONY: clean
clean:
	rm -f *.o
	rm -f *.gch
	rm -f $(TARGET)

