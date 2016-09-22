CC=gcc
CFLAG=-g -c
LDFLAGS=`pkg-config --cflags --libs gtk+-2.0`
SOURCES=$(foreach d,.,$(wildcard $(addprefix ./*,.c)))
HEADERS=$(foreach d,.,$(wildcard $(addprefix ./*,.h)))
OBJS=$(addsuffix .o, $(basename $(SOURCES)))
BIN=$(patsubst %.c,%,$(wildcard *.c))

.PHONY: all

all:$(OBJS)
#	$(CC) $(LDFLAGS) $< 
	for i in `echo $(BIN)|cut -d' ' -f 1-`;\
	do\
		echo $$i;\
		$(CC) $(LDFLAGS) $$i.o -o $$i;\
	done

%.o:%.c 
	$(CC) $(CFLAG) $(LDFLAGS) $< -o $@

clean:
	rm -rf *.o a.out $(BIN)
