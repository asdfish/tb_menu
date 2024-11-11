TEST_OBJECT_FILES := $(patsubst test/src/%.c,$\
										   test/build/%.o,$\
											 $(shell find test/src -name '*.c' -type f))

test/test: ${TEST_OBJECT_FILES}
	${CC} $< ${C_FLAGS} -o $@
