CC ?= cc
CFLAGS ?= -O2 -march=native -pipe
COMMONFLAGS := -Wall -Wextra -Wpedantic $\
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

# Uncomment/comment to enable/disable tests
# BUILD_TEST := yes

LIBTB_MENU_A_REQUIREMENTS := ${PROCESSED_HEADER_FILES} ${OBJECT_FILES}

define COMPILE
${CC} -c $(1) $(CFLAGS) ${COMMONFLAGS} -o $(2)

endef
define REMOVE
$(if $(wildcard $(1)),$\
	rm $(1))

endef
define REMOVE_LIST
$(foreach ITEM,$\
	$(1),$\
	$(call REMOVE,${ITEM}))
endef

ifndef (,${BUILD_TEST})
include test.mk
endif

all: libtb_menu.a $(if ${BUILD_TEST},test/test)

libtb_menu.a: ${LIBTB_MENU_A_REQUIREMENTS}
	ar rcs $@ ${OBJECT_FILES}

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
