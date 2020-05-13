INCDIR=inc/
CC=gcc
CFLAGS=-I$(INCDIR) -DKXVER=3 -fPIC -shared

print-% : ; @echo $* = $($*)

CORE=blog.c
OUT=blog.so

eg1: $(CORE)
	$(CC) $(CORE) eg1.c $(CFLAGS) -o ${OUT}

eg2: $(CORE)
	$(CC) $(CORE) eg2.c $(CFLAGS) -o ${OUT}

.PHONY: clean

clean:
	rm -f ${OUT}
