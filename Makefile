ifdef PS5_PAYLOAD_SDK
include $(PS5_PAYLOAD_SDK)/toolchain/prospero.mk
else
$(error PS5_PAYLOAD_SDK is undefined)
endif

CFLAGS := -O3 `$(PS5_SYSROOT)/bin/sdl2-config --cflags`
LIBS := `$(PS5_SYSROOT)/bin/sdl2-config --libs`
LDLIBS := -lkernel_sys -lSDL2main -lm

SRCS := main.c
OBJS := $(SRCS:.c=.o)

cube.elf: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS) $(LDLIBS)

clean:
	rm -f cube.elf $(OBJS)