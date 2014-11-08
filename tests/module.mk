LOCAL_DIR  := tests
LOCAL_PROG := $(BINDIR)/unittests
LOCAL_SRC  := $(wildcard $(LOCAL_DIR)/*.c)
LOCAL_OBJ  := $(subst .c,.o,$(LOCAL_SRC))

SOURCES   += $(LOCAL_SRC)
TESTS     += $(LOCAL_PROG)

$(LOCAL_PROG): $(LOCAL_OBJ)
	$(CC) -o $@ $^ -L$(LIBDIR) -lpggds
