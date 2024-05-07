#!/bin/bash

gcc hashMap_test.c hashMap.c ../libft.a ../list/linked_list.a -g && valgrind --leak-check=full ./a.out
rm -rf a.out
