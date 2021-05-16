CXX = g++
CXXFLAGS = -std=c++11 -lsfml-audio -pthread -lrt
TARGET = processA

$(TARGET): ProcessA.o libmidifile.a
	$(CXX) ProcessA.o libmidifile.a -o $(TARGET) -lsfml-audio -lrt -pthread 

ProcessA.o: ProcessA.cpp MidiEvent.h MidiMessage.h MidiFile.h
	$(CXX) $(CXXFLAGS) -c ProcessA.cpp MidiEvent.h MidiMessage.h MidiFile.h -I ../

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.gch
	rm -f $(TARGET)

