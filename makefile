PLATFORM_OBJS := $(patsubst %.s,build/%.o,$(wildcard linux/*))

GENERIC_OBJS := $(patsubst %.s,build/%.o,$(wildcard generic/*))

BOOTSTRAP_OBJS := $(patsubst %.forth,build/%.o,$(wildcard bootstrap/*))

all: setup build/libfstd.a

clean:
	@echo $(BOOTSTRAP_OBJS)
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

