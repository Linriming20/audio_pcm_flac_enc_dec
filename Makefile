all: pcm2flac flac2pcm

# our static lib is compile by x86_64 gcc
CC := gcc
CFLAG := -I./include
LDFLAGS := -lFLAC -L./libs -lm

pcm2flac: main_pcm2flac.c
	$(CC) $^ $(CFLAG) $(LDFLAGS) -o $@

flac2pcm: main_flac2pcm.c
	$(CC) $^ $(CFLAG) $(LDFLAGS) -o $@

clean :
	rm -rf pcm2flac flac2pcm out*
.PHONY := clean

