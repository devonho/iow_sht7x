CC = g++
STD = -std=c++17
CFLAGS  = -Wall #-Wextra
LDFLAGS = -liowkit
APP = app
SOURCES = src

all: app 
.PHONY: all

app:
	$(CC) $(STD) $(CFLAGS) $(SOURCES)/sht7x.cpp $(SOURCES)/test_app.cpp $(LDFLAGS) -o ./bin/$(APP)
	chmod 755 ./bin/$(APP)

testapp:
	./bin/$(APP)

clean:
	rm -rf ./bin/$(APP)