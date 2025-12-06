CC = gcc
CFLAGS = -Wall -Wextra -O2


OBJS = common.o avl.o parser.o factory.o leaks.o histogram.o main.o


wildwater: $(OBJS)
$(CC) $(CFLAGS) -o wildwater $(OBJS)


common.o: common.h
avl.o: avl.h common.h
parser.o: parser.h common.h
factory.o: factory.h parser.h avl.h common.h
leaks.o: leaks.h parser.h avl.h common.h
histogram.o: histogram.h factory.h avl.h common.h
main.o: common.h avl.h parser.h factory.h histogram.h leaks.h


clean:
rm -f *.o wildwater