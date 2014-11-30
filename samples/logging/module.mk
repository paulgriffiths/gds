LOCAL_DIR  := samples/logging
LOCAL_PROG := $(BINDIR)/logging
LOCAL_SRC  := $(wildcard $(LOCAL_DIR)/*.c)
LOCAL_OBJ  := $(subst .c,.o,$(LOCAL_SRC))

SOURCES   += $(LOCAL_SRC)
SAMPLES   += $(LOCAL_PROG)

$(LOCAL_PROG): $(LOCAL_OBJ)
	$(CC) -o $@ $^ -L$(LIBDIR) -lpggds
