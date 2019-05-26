# To get structure sizes correct, we must compile with -m32
# otherwise longs become 8 bytes and so forth.
# for example for db_fix.c ...
# Size of BP is 136496 bytes (default 64 bit compile)
# Size of BP is 133548 bytes ( with -m32, this is correct )

CC = cc -m32

.c.o:
	-$(CC) -c $<

all:	shm db_fix

shm:	shm.c
	$(CC) -o shm shm.c

DB_OBJ = db_fix.o fix_oven.o

db_fix:	$(DB_OBJ)
	$(CC) -o db_fix $(DB_OBJ)

install: all
	cp shm /home/tom/bin
	cp db_fix /home/tom/bin

clean:
	rm -f shm db_fix
