/***********************************************
* Project Title: Write a Hash Table in C
* Author: Dawson Whipple
* Date: 07/10/24
* Description: A hash table is a data structure which offers a fast implementation of the associative array API.
*              Associative arrays are a collection of unordered key-value pairs. Duplicate keys are not permitted. The following operations are supported:

    search(a, k): return the value v associated with key k from the associative array a, or NULL if the key does not exist.
    insert(a, k, v): store the pair k:v in the associative array a.
    delete(a, k): delete the k:v pair associated with k, or do nothing if k does not exist.

*********************************************/
//https://github.com/jamesroutley/write-a-hash-table/tree/master/01-introduction
//Associative array: an abstract data structure which implements the API described above. Also called a map, symbol table or dictionary.
//Hash table: a fast implementation of the associative array API which makes use of a hash function. Also called a hash map, map, hash or dictionary.

int is_prime(const int x);
int next_prime(int x);