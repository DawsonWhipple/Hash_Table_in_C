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

#define HT_PRIME_1 2423
#define HT_PRIME_2 2287
#define HT_INITIAL_BASE_SIZE 53

static ht_item HT_DELETED_ITEM = {NULL, NULL};

//initialization of a new ht_item, allocates memory space, and assigns key and value variables, returns a pointer to the new item
static ht_item* ht_new_item(const char* k, const char* v){
    ht_item *i = malloc(sizeof(ht_item)); //static allocation
    i->key = strdup(k); //strdup() returns a duplicate of the given string, necassary when working with pointers
    i->value = strdup(v);
    return i;
}

static ht_hash_table* ht_new_sized(const int base_size) {
    ht_hash_table* ht = xmalloc(sizeof(ht_hash_table));
    ht->base_size = base_size;

    ht->size = next_prime(ht->base_size);

    ht->count = 0;
    ht->items = xcalloc((size_t)ht->size, sizeof(ht_item*));
    /*
    The stdlib.h and stddef.h header files define a datatype called size_t which is used to represent the size of an object. 
    Library functions that take sizes expect them to be of type size_t, and the sizeof operator evaluates to size_t.

    The actual type of size_t is platform-dependent; a common mistake is to assume size_t is the same as unsigned int, 
    which can lead to programming errors, particularly as 64-bit architectures become more prevalent.
    */
    return ht;
}


ht_hash_table* ht_new() {
    return ht_new_sized(HT_INITIAL_BASE_SIZE);
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

//takes a string as input, returns a number between 0 and m (desired bucket length)
//The variable a should be a prime number larger than the size of the alphabet.
//We're hashing ASCII strings, which has an alphabet size of 128, so we should choose a prime larger than that.
static int ht_hash(const char* s, const int a, const int m){
    long hash = 0;
    const int len_s = strlen(s);
    for(int i = 0; i < len_s; i++){
        hash += (long)pow(a, len_s - (i+1)) * s[i]; //generic hashing algorithm
        hash = hash % m;
    }
    return (int) hash;
}

//handles collisions by running the hash value through multiple different hashing functions
static int ht_get_hash(const char* s, const int num_buckets, const int attempt){
    const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

//To insert a new key-value pair, we iterate through indexes until we find an empty bucket. 
//We then insert the item into that bucket and increment the hash table's count attribute, to indicate a new item has been added.
void ht_insert(ht_hash_table* ht, const char* key, const char* value){
    const int load = ht->count * 100 / ht->size;
    if (load > 70) {
        ht_resize_up(ht);
    }
    ht_item* item = ht_new_item(key, value); //create a blank new item
    int index = ht_get_hash(item->key, ht->size, 0); //create a starting index hash 
    ht_item* cur_item = ht->items[index]; //establish the starting item from the starting index
    int i = 1;
    //search through indexes until an empty one is found
    while (cur_item != NULL && cur_item != &HT_DELETED_ITEM) {
        if (strcmp(cur_item->key, key) == 0) {
            ht_del_item(cur_item);
            ht->items[index] = item;
            return;
        }
        index = ht_get_hash(item->key, ht->size, i);
        cur_item = ht->items[index];
        i++;
    } 
    //add new item to the hash table once an index has been found
    ht->items[index] = item;
    ht->count++; //increment counter for amount of entries in the hash table
}

//Searching is similar to inserting, but at each iteration of the while loop, we check whether the item's key matches the key we're searching for. 
//If it does, we return the item's value. If the while loop hits a NULL bucket, we return NULL, to indicate that no value was found.
char* ht_search(ht_hash_table* ht, const char* key){
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM) { 
            //check if the current key matches the search key
            if (strcmp(item->key, key) == 0) {
                return item->value;
            }
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    } 
    return NULL;
}

//The item we wish to delete may be part of a collision chain. Removing it from the table will break that chain, and will make finding items in the tail of the chain impossible. To solve this, instead of deleting the item, we simply mark it as deleted.
//We mark an item as deleted by replacing it with a pointer to a global sentinel item which represents that a bucket contains a deleted item.
void ht_delete(ht_hash_table* ht, const char* key){
    const int load = ht->count * 100 / ht->size;
    if (load < 10) {
        ht_resize_down(ht);
    }
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM) {
            if (strcmp(item->key, key) == 0) {
                ht_del_item(item);
                ht->items[index] = &HT_DELETED_ITEM;
            }
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    } 
    ht->count--; 
}

static void ht_resize(ht_hash_table* ht, const int base_size) {
    if (base_size < HT_INITIAL_BASE_SIZE) {
        return;
    }
    ht_hash_table* new_ht = ht_new_sized(base_size);
    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];
        if (item != NULL && item != &HT_DELETED_ITEM) {
            ht_insert(new_ht, item->key, item->value);
        }
    }

    ht->base_size = new_ht->base_size;
    ht->count = new_ht->count;

    // To delete new_ht, we give it ht's size and items 
    const int tmp_size = ht->size;
    ht->size = new_ht->size;
    new_ht->size = tmp_size;

    ht_item** tmp_items = ht->items;
    ht->items = new_ht->items;
    new_ht->items = tmp_items;

    ht_del_hash_table(new_ht);
}   

static void ht_resize_up(ht_hash_table* ht) {
    const int new_size = ht->base_size * 2;
    ht_resize(ht, new_size);
}


static void ht_resize_down(ht_hash_table* ht) {
    const int new_size = ht->base_size / 2;
    ht_resize(ht, new_size);
}