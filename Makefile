# Compiler
CC = gcc

# Include Flags
IFLAGS = -I/comp/40/build/include -I/usr/sup/cii40/include/cii

# Compiler Flags
CFLAGS = -g -std=gnu99 -Wall -Wextra -Werror -Wfatal-errors -pedantic $(IFLAGS)

# Linking Flags
LDFLAGS = -g -L/comp/40/build/lib -L/usr/sup/cii40/lib64

# Libraries
LDLIBS = -larith40 -l40locality -lnetpbm -lpnm -lcii40 -lm -lrt

# Collect all .h files in your directory
INCLUDES = $(wildcard *.h)

# Rules
all: 40image-6
# ppmdiff

# Compile step (.c files -> .o files)
%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@

# Linking step (.o -> executable program)
40image-6: 40image.o compress40.o bitpack.o pixel_conversion.o \
         uarray2b.o uarray2.o a2blocked.o dct.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)


# ppmdiff: ppmdiff.o uarray2.o a2plain.o
# 	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)


# Clean up build artifacts
clean:
	rm -f 40image-6 *.o
# 	ppmdiff 
