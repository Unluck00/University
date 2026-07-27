CC = gcc
CFLAGS = -Wall -Wextra -Werror -D_GNU_SOURCE

SHARED = src/headers/*.h src/utils/*.h src/utils/*.c
OBJS = msg.o sem.o
DIRECTOR = src/director.c src/include/director.h src/parser.c src/include/parser.h
TICKET = src/ticket.c src/include/ticket.h
OPERATOR = src/operator.c src/include/operator.h
USER = src/user.c src/include/user.h

all: director ticket operator user

director: $(OBJS) $(DIRECTOR)
	$(CC) $(CFLAGS) -O2 src/director.c src/parser.c *.o -o director

ticket: $(OBJS) $(TICKET)
	$(CC) $(CFLAGS) -O2 src/ticket.c *.o -o ticket

operator: $(OBJS) $(OPERATOR)
	$(CC) $(CFLAGS) -O2 src/operator.c *.o -o operator

user: $(OBJS) $(USER)
	$(CC) $(CFLAGS) -O2 src/user.c *.o -o user

%.o: src/utils/%.c $(SHARED) 
	$(CC) -c $(CFLAGS) -O2 src/utils/*.c

clean:
	rm -f *.o director ticket operator user *~

run: all
	./director
