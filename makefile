# This file acts as a sugar to automate running meson build commands

all: build

# Build the software, dep auto resolved
build:
	meson setup build-debug && \
	cd build-debug && \
	ninja

# Run it, will not try to build it first
run:
	cd build-debug && ./BurningGround

# Build it and then run it
test: build
	make run

# Clean up the compiled files
clean:
	cd build-debug && meson compile --clean

# Scrap the previous build system config as well as compiled files
purge:
	rm -rf build-debug build-release

.PHONY: build run test clean purge
