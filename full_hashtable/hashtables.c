#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Hash table key/value pair with linked list pointer.

  Note that an instance of `LinkedPair` is also a node in a linked list.
  More specifically, the `next` field is a pointer pointing to the the 
  next `LinkedPair` in the list of `LinkedPair` nodes. 
 */
typedef struct LinkedPair {
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/*
  Hash table with linked pairs.
 */
typedef struct HashTable {
  int capacity;
  LinkedPair **storage;
} HashTable;

/*
  Create a key/value linked pair to be stored in the hash table.
 */
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

/*
  Use this function to safely destroy a hashtable pair.
 */
void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL) {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/*
  djb2 hash function

  Do not modify this!
 */
unsigned int hash(char *str, int max)
{
  unsigned long hash = 5381;
  int c;
  unsigned char * u_str = (unsigned char *)str;

  while ((c = *u_str++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}

/*
  Fill this in.

  All values in storage should be initialized to NULL
 */
HashTable *create_hash_table(int capacity)
{
  HashTable *ht = malloc(sizeof(HashTable));
  //construct da struct! sotrage initialized to 0/NULL
  ht->capacity=capacity; // the input var
  ht->storage=calloc(capacity,sizeof(LinkedPair *)); //capacity is the number of blocks and the
  //LinkedPairs are the key values which each block needs to fit

  return ht;
}

/*
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 */
void hash_table_insert(HashTable *ht, char *key, char *value)
{
  int hashed_index = hash(key,ht->capacity);
  if(ht->storage[hashed_index] !=NULL){
    LinkedPair *current_index = ht->storage[hashed_index];
    while(current_index != NULL){
      if(current_index->key == key){
        current_index->value = value;
      }else if(current_index->next == NULL){
        current_index->next = create_pair(key,value);
      }else{
        // if the struct is empty
        current_index = current_index->next;
      }
    }
  }else{
    ht->storage[hashed_index] = create_pair(key,value);
  }
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 */
void hash_table_remove(HashTable *ht, char *key)
{
  // need to keep track of previous and current indices for removal
  int hash_key = hash(key,ht->capacity);
  if(ht->storage[hash_key] != NULL){
    LinkedPair *curri = ht->storage[hash_key];
    LinkedPair *prev = NULL;
    while(curri !=NULL){
      // if the keys match
      if(strcmp(curri->key, key) == 0){
        if(prev == NULL){
          ht->storage[hash_key]=curri->next;
        } else{
          prev->next=curri->next;
        }
        destroy_pair(curri);
        free(ht->storage[hash_key]);
      } else {
      prev = curri;
      curri = curri->next;
    }
   }
  }
 }

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 */
char *hash_table_retrieve(HashTable *ht, char *key)
{
  int hash_key = hash(key, ht->capacity);
  if(ht->storage[hash_key]!=NULL){
    LinkedPair *curr = ht->storage[hash_key];
    while(curr != NULL){
      if(strcmp(curr->key, key) == 0){
        return curr->value;
      } else{
        curr = curr->next;
      }
    }
  }
  return NULL;
}

/*
  Fill this in.

  Don't forget to free any malloc'ed memory!
 */
void destroy_hash_table(HashTable *ht)
{
  for(int i = 0; i<ht->capacity; i++){
    if(ht->storage[i] !=NULL){
      LinkedPair *next_one = ht->storage[i]->next;
      while(next_one!= NULL){
        free(ht->storage[i]);
        ht->storage[i] = next_one;
      }
    }
  }
  free(ht->storage);
  free(ht);
}

/*
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 */
HashTable *hash_table_resize(HashTable *ht)
{
  //make a new table to resize using linked list chaining twice the capacity
  HashTable *ht2 = create_hash_table(ht->capacity*2);
  for(int i=0; i<ht->capacity; i++){
    LinkedPair *curr = ht->storage[i];
    if(curr != NULL){
      //insert the values into the new table:
      hash_table_insert(ht2, curr->key, curr->value);
      curr = curr->next;
    }
  }
  //delete the old table
  destroy_hash_table(ht);
  return ht2;
}


#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);

  hash_table_insert(ht, "line_1", "Tiny hash table\n");
  hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  printf("%s", hash_table_retrieve(ht, "line_1"));
  printf("%s", hash_table_retrieve(ht, "line_2"));
  printf("%s", hash_table_retrieve(ht, "line_3"));

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif
