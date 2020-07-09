CXXFLAGS = --std=c++14 -g -Wall
LDFLAGS = -pthread -lpcap -lportaudio

main: cap.o main.o snd.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
