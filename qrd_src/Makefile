CC ?= gcc
PREFIX ?= /usr/local
CFLAGS ?= -O3 -Wall

LIB_VERSION = 1.0

LIB_OBJ = \
    qrd-src/shared/quirc/lib/decode.o \
    qrd-src/shared/quirc/lib/identify.o \
    qrd-src/shared/quirc/lib/quirc.o \
    qrd-src/shared/quirc/lib/version_db.o

DEMO_OBJ = \
	qrd-src/demo/qrd-demo.o

all: libquirc.so qrd-demo

qrd-demo: $(DEMO_OBJ) qrd-demo/qrd-demo.o libquirc.a
	$(CC) -o $@ $(DEMO_OBJ) demo/demo.o libquirc.a $(LDFLAGS) -lm -lpng

libquirc.so.$(LIB_VERSION): $(LIB_OBJ)
	$(CC) -shared -o $@ $(LIB_OBJ) $(LDFLAGS) -lm

clean:
	rm -f */*.o
	rm -f */*.lo
	rm -f libquirc.so.$(LIB_VERSION)
	rm -f qrtest
	rm -f inspect
	rm -f quirc-demo
	rm -f quirc-scanner
