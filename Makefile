CC ?= cc
C_FLAGS := -std=c99 $\
					 -O2 -march=native -pipe $\
					 -Wall -Wextra -Wpedantic $\
					 -Iinclude -Ideps/termbox2
OBJECT_FILES := $(patsubst src/%.c,$\
									build/%.o,$\
									$(shell find src -name '*.c' -type f))

# Uncomment to process header files
# PROCESS_HEADER_FILES := yes
PROCESSED_HEADER_FILES :=	$(if ${PROCESS_HEADER_FILES},$\
														$(subst .h,$\
															$(if $(findstring clang,${CC}),$\
																.h.pch,$\
																.h.gch),$\
															$(shell find include -name '*.h' -type f)))

# Uncomment to build tests
# BUILD_TEST := yes
TEST_OBJECT_FILES :=

LIBTB_MENU_A_REQUIREMENTS := ${PROCESSED_HEADER_FILES} ${OBJECT_FILES}

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

all: libtb_menu.a $(if ${BUILD_TEST},test/test)

include test.mk

libtb_menu.a: ${LIBTB_MENU_A_REQUIREMENTS}
	$(info Linking $@)
	@ar rcs $@ ${OBJECT_FILES}

build/%.o: src/%.c
	$(call COMPILE,$<,$@)

%.gch: %
	$(call COMPILE,$<,$@)
%.pch: %
	$(call COMPILE,$<,$@)

clean:
	$(call REMOVE,libtb_menu.a)
	$(call REMOVE_LIST,${LIBTB_MENU_A_REQUIREMENTS})
	$(call REMOVE,test/test)
	$(call REMOVE_LIST,${TEST_OBJECT_FILES})

.PHONY: all clean
