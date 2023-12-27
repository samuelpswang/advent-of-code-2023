CXX = g++
CFLAGS = --std=c++20
SRC_DIR = $(shell find . -name 'day_*' | sort)

.PHONY: build
build:
	@for dir in $(SRC_DIR); do \
		if [ -f $$dir/part_1.cpp ]; then \
			echo "[build] compiling $$dir/part_1.cpp"; \
			$(CXX) $$dir/part_1.cpp $(CFLAGS) -o $$dir/part_1; \
		fi; \
		if [ -f $$dir/part_2.cpp ]; then \
			echo "[build] compiling $$dir/part_2.cpp"; \
			$(CXX) $$dir/part_2.cpp $(CFLAGS) -o $$dir/part_2; \
		fi; \
	done

.PHONY: clean
clean:
	@rm -f $(shell find $(SRC_DIR) -name 'main' -or -name 'part_1' -or -name 'part_2')
