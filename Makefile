CC ?= cc
C_FLAGS := -std=c99 $\
					 -O2 -march=native -pipe $\
					 -Wall -Wextra -Wpedantic
OBJECT_FILES := $(patsubst src/%.c,$\
									build/%.o,$\
									$(shell find src -name '*.c' -type f))

define COMPILE
$(info Compiling $2)
@${CC} -c $(1) ${C_FLAGS} -o $(2)
endef
define REMOVE
$(if $(wildcard $(1)),$\
	$(info Removing $(1))
	$(shell rm $(1)))
endef
define REMOVE_LIST
$(foreach ITEM,$\
	$(1),$\
	$(call REMOVE,${ITEM}))
endef

all: libtb_menu.a

libtb_menu.a: ${OBJECT_FILES}
	$(info Linking $@)
	@ar rcs $@ $<

build/%.o: src/%.c
	$(call COMPILE,$<,$@)

clean:
	$(call REMOVE,libtb_menu.a)
	$(call REMOVE_LIST,${OBJECT_FILES})

.PHONY: all clean