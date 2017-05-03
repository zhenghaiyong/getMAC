GCC = gcc
G++ = g++
RM = rm -f

BINS = getMAC

all: $(BINS) 

%:%.c
	$(GCC) -o $@ -g $<

clean:
	$(RM) $(BINS)
