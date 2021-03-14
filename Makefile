FILES=src/main.c
CFLAGS=-O3
OUT=build/remake\ of\ remake
LIBS=-lX11

windowbounce: $(FILES)
	$(CC) $(FILES) $(CFLAGS) $(LIBS) -o $(OUT)
