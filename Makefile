CFLAGS = -g -Wall
LDFLAGS = -pthread -lpcap -lportaudio

main: cap.o main.o ring.o snd.o
