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

//key value pairs associated with the hash table
typedef struct {
    char* key;
    char* value;
} ht_item;

//hash table stores: an array of pointers to items, details about size and how full it is
typedef struct {
    int base_size;
    int size;
    int count;
    ht_item** items;
}ht_hash_table;

void ht_insert(ht_hash_table* ht, const char* key, const char* value);
char* ht_search(ht_hash_table* ht, const char* key);
void ht_delete(ht_hash_table* ht, const char* key);