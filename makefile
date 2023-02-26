#######################################
#    Makefile for Exchange System	      #
#######################################

#   The general compiling commands    #

CP = cp
CC = gcc
SUDO = sudo
CCFLAGES = -c 

exec/gateway: TARGET = gateway
exec/mengine: TARGET = exec/mengine
exec/mbalance: TARGET = exec/mbalance

TGT = tgt/*
EXEC = exec/*

GATEWAY_OBJS = tgt/gateway.o tgt/hashMap.o tgt/dataType.o tgt/arrayList.o tgt/network.o tgt/event.o tgt/stringType.o tgt/logger.o

BALANCE_OBJS = tgt/mbalance.o tgt/hashMap.o tgt/dataType.o tgt/arrayList.o tgt/network.o tgt/event.o tgt/stringType.o tgt/logger.o

MENGINE_OBJS = tgt/mengine.o tgt/hashMap.o tgt/dataType.o tgt/arrayList.o tgt/network.o tgt/event.o tgt/stringType.o tgt/logger.o

clean :
	 rm -f $(EXEC) $(TGT)

.PHONY : build clean

build : clean exec/gateway exec/mengine exec/mbalance

all: clean exec/gateway exec/mengine exec/mbalance

tgt/event.o	: src/network/event.c src/network/event.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/network.o	: src/network/network.c src/network/network.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/dataType.o : src/structure/dataType.c src/structure/dataType.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/hashMap.o : src/structure/hashMap.c src/structure/hashMap.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/arrayList.o : src/structure/arrayList.c src/structure/arrayList.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/stringType.o : src/structure/stringType.c src/structure/stringType.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/logger.o : src/logger/logger.c src/logger/logger.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/gateway.o : src/matching/gateway/gateway.c
	$(CC) $(CCFLAGES) $< -o $@

tgt/mbalance.o : src/matching/balance/mbalance.c
	$(CC) $(CCFLAGES) $< -o $@

tgt/mengine.o : src/matching/engine/mengine.c
	$(CC) $(CCFLAGES) $< -o $@

exec/gateway : tgt/gateway.o tgt/hashMap.o tgt/dataType.o tgt/arrayList.o tgt/network.o tgt/event.o tgt/stringType.o tgt/logger.o
	$(CC) $(GATEWAY_OBJS) -o $@

exec/mbalance : tgt/mbalance.o tgt/hashMap.o tgt/dataType.o tgt/arrayList.o tgt/network.o tgt/event.o tgt/stringType.o tgt/logger.o
	$(CC) $(BALANCE_OBJS) -o $@

exec/mengine : tgt/mengine.o tgt/hashMap.o tgt/dataType.o tgt/arrayList.o tgt/network.o tgt/event.o tgt/stringType.o tgt/logger.o
	$(CC) $(MENGINE_OBJS) -o $@
