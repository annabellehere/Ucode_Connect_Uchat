SHELL := /bin/zsh

BIN := uchat 

BIN2 := uchat_server
BIN_TARGET := UCHAT
BIN2_TARGET := UCHAT_SERVER





  
.PHONY = all clean uninstall reinstall dep_brew_install \
		 CONFIGURE_SSL_SYMLINKS dep_ubuntu_install \
		 install_dependancies

CC = clang -std=c11  -Wall -Wextra -Werror -Wpedantic 

CLIENT_SOURCE := $(wildcard client/src/*.c)
SERVER_SOURCE := $(wildcard server/src/*.c)

SERVER_HEADERS := $(wildcard server/inc/*.h)
CLIENT_HEADERS := $(wildcard client/inc/*.h)

all: install_dependancies ./libmx/libmx.a $(BIN_TARGET) $(BIN2_TARGET) server_chat_db.db client_chat_db.db

#client
$(BIN_TARGET): $(CLIENT_SOURCE) $(CLIENT_HEADERS)
#	@$(CC) -o $(BIN) $(CLIENT_OBJECTS) $(LIBS)
	@printf "Building Client app\n" 
	@make -C ./client
	@cp -f ./client/$(BIN) $(BIN)

#server
$(BIN2_TARGET): $(SERVER_SOURCE) $(SERVER_HEADERS)
#	@$(CC) -o $(BIN2) $(SERVER_OBJECTS) $(LIBS)
	@printf "Building Server app\n"
	@make -C ./server
	@cp -f ./server/$(BIN2) $(BIN2)

./libmx/libmx.a:
	@printf "Building libmx\n"
	@make -C ./libmx

dep_brew_install:
	brew install gtk+3 sqlcipher json-c openssl@3 pkg-config 

dep_ubuntu_install:
	sudo apt install libgtk-3-dev \
	pkg-config \
	libssl-dev libssl-doc \
	libjson-c-dev \
	libsqlcipher-dev \
	sqlcipher 

install_dependancies:
	@make dep_brew_install

server_chat_db.db:
	printf "PRAGMA key = '`cat serverDBKey`';\n" > tmp.sql
	sqlcipher server_chat_db.db < tmp.sql  < createServerDB.sql
	rm -f tmp.sql

client_chat_db.db:
	printf "PRAGMA key = '`cat clientDBKey`';\n" > tmp.sql
	sqlcipher client_chat_db.db < tmp.sql < createServerDB.sql 
	rm -f tmp.sql

obj/%.o: src/%.c $(HEAD)
	@mkdir -p obj 
	@$(CC) -c $< -o $@

uninstall: clean
	@make -C ./libmx uninstall
	@make -C ./client uninstall
	@make -C ./server uninstall
	@rm -f $(BIN) $(BIN2)
	

clean:
	@make -C ./libmx clean
	@make -C ./client clean
	@make -C ./server clean
	@rm -f $(wildcard *.o) 
	@rm -f $(wildcard *.log)

reinstall: uninstall all


