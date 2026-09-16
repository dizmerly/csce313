opt: lab2.opt
dbg: lab2.dbg
san: lab2.san

CCFLAGS = -std=c18 -Wall -Wextra -Wpedantic -Wconversion -fanalyzer

lab2.opt: CCFLAGS += -Ofast -flto=auto
lab2.opt: main.c
	$(CC) $(CCFLAGS) -o $@ $^

lab2.dbg: CCFLAGS += -Og -ggdb3
lab2.dbg: main.c
	$(CC) $(CCFLAGS) -o $@ $^

lab2.san: CCFLAGS += -Og -ggdb3 -fsanitize=address
lab2.san: main.c
	$(CC) $(CCFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f lab2.opt lab2.dbg lab2.san output_file
