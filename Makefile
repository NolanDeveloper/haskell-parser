CPPFLAGS = -pedantic -std=c99 -g -Wall -Wextra -DPCRE2_CODE_UNIT_WIDTH=8
LDLIBS = -lpcre2-8

CFILES = $(wildcard *.c)
OFILES = $(patsubst %.c,%.o,$(CFILES))

a.out: $(OFILES)
	$(CC) $(LDLIBS) $^ -o $@

clean:
	$(RM) $(OFILES) a.out
