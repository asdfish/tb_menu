TEST_LD_FLAGS := ${C_FLAGS} $\
								 -L. -ltb_menu

TEST_OBJECT_FILES := $(patsubst test/src/%.c,$\
										   test/build/%.o,$\
											 $(shell find test/src -name '*.c' -type f))

test/test: libtb_menu.a ${TEST_OBJECT_FILES}
	$(info Linking $@)
	@${CC} ${TEST_OBJECT_FILES} ${TEST_LD_FLAGS} -o $@

test/build/%.o: test/src/%.c
	$(call COMPILE,$<,$@)
