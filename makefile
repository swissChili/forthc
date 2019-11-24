PLATFORM_OBJS = \
	build/linux/_start.o\
	build/linux/puts.o\
	build/linux/writes.o\
	build/linux/strlen.o\
	build/linux/streq.o\
	build/linux/puti.o

GENERIC_OBJS = \
	build/generic/swap.o\
	build/generic/dup.o\
	build/generic/drop.o\
	build/generic/tuck.o\
	build/generic/over.o\
	build/generic/equal.o\
	build/generic/spaceship.o\
	build/generic/third.o\
	build/generic/setptr.o\
	build/generic/getptr.o

BOOTSTRAP_OBJS = \
	build/bootstrap/endl.o\
	build/bootstrap/booleans.o

all: setup build/libfstd.a

clean:
	rm **/*.o **/*.a

setup:
	mkdir -p build/linux build/bootstrap build/generic

build/libfstd.a: $(PLATFORM_OBJS) $(GENERIC_OBJS) $(BOOTSTRAP_OBJS)
	ar rcs $@ $^

build/linux/%.o: linux/%.s
	as -o $@ $^
build/generic/%.o: generic/%.s
	as -o $@ $^
build/bootstrap/%.o: build/bootstrap/%.s
	as -o $@ $^
build/bootstrap/%.s: bootstrap/%.forth build/forthc
	build/forthc $^ > $@

