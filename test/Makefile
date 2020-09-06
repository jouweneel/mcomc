CC=g++
EXEC=test
INCLUDE=-I../include -I../protocol/include -I../transport/include
SOURCE = $(wildcard ../protocol/*.c) \
  $(wildcard ../transport/*.c) \
  $(wildcard ../*.c) \
	$(EXEC).c

CFLAGS=-g $(INCLUDE)

obj = $(SOURCE:.c=.o)
dep = $(obj:.o=.d)

$(EXEC): $(obj)
	$(CC) -o $@ $^ $(CFLAGS)
	rm -f $(obj) && rm -f $(dep)

-include $(dep)

%.d: %.c
	@$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: clean
clean:
	rm -f $(obj) $(EXEC) && rm -f $(dep)
