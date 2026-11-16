CC = clang
MODE ?= debug
BUILD := build/$(MODE)
.DEFAULT_GOAL := all
.PHONY: all clean release sanitize test
release:
	$(MAKE) MODE=release all
sanitize:
	$(MAKE) MODE=sanitize all
clean:
	rm -rf build
CFLAGS ?= -Wall -Wextra -Wpedantic -Werror
CFLAGS += -MMD -MP
ifeq ($(MODE),release)
CFLAGS += -O2
else
CFLAGS += -g -O0
endif
ifeq ($(MODE),sanitize)
CFLAGS += -fsanitize=address,undefined -fno-omit-frame-pointer
LDFLAGS += -fsanitize=address,undefined
endif
LDLIBS += -lm

all: $(BUILD)/calculator

$(BUILD)/calculator: $(BUILD)/calculator.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

test: all

-include $(wildcard $(BUILD)/*.d $(BUILD)/*/*.d)
