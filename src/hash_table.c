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

#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

//initialization of a new ht_item, allocates memory space, and assigns key and value variables, returns a pointer to the new item
static ht_item* ht_new_item(const char* k, const char* v){
    ht_item *i = malloc(sizeof(ht_item)); //static allocation
    i->key = strdup(k); //strdup() returns a duplicate of the given string, necassary when working with pointers
    i->value = strdup(v);
    return i;
}

ht_hash_table* ht_new(){
    ht_hash_table *ht = malloc(sizeof(ht_hash_table));
    ht->size = 53; //default size
    ht->count = 0; //ht starts out empty
    ht->items = calloc((size_t)ht->size, sizeof(ht_item*)); //dynamic allocaiton
    /*
    The stdlib.h and stddef.h header files define a datatype called size_t which is used to represent the size of an object. 
    Library functions that take sizes expect them to be of type size_t, and the sizeof operator evaluates to size_t.

    The actual type of size_t is platform-dependent; a common mistake is to assume size_t is the same as unsigned int, 
    which can lead to programming errors, particularly as 64-bit architectures become more prevalent.
    */
   return ht;
}
//delete an item from memory and therefore from the hashtable
static void ht_delete_item(ht_item* i){
    free(i->key);
    free(i->value);
    free(i);
}
//deletes an entire hash table by individually deleting each item
void ht_delete_hash_table(ht_hash_table* ht){
    //individually delete all items in the hash table
    for(int i = 0; i < ht->size; i++){
        ht_item* item = ht->items[i]; //create a new pointer instance of the current item
        if(item != NULL)
            ht_delete_item(item); //call the item delete function above
    }
    free(ht->items);
    free(ht);
}

