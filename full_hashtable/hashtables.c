#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/****
  Hash table key/value pair with linked list pointer
 ****/
typedef struct LinkedPair
{
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/****
  Hash table with linked pairs
 ****/
typedef struct HashTable
{
  int capacity;
  LinkedPair **storage;
} HashTable;

/****
  Create a key/value linked pair to be stored in the hash table.
 ****/
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL)
  {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/****
  djb2 hash function

  Do not modify this!
 ****/
unsigned int hash(char *str, int max)
{
  unsigned long hash = 5381;
  int c;
  unsigned char *u_str = (unsigned char *)str;

  while ((c = *u_str++))
  {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}

/****
  Fill this in.

  All values in storage should be initialized to NULL
 ****/
HashTable *create_hash_table(int capacity)
{
  HashTable *ht = malloc(sizeof(HashTable));

  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(char *));

  return ht;
}

/****
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 ****/
void hash_table_insert(HashTable *ht, char *key, char *value)
{
  unsigned int index = hash(key, ht->capacity);

  LinkedPair *new_pair = create_pair(key, value);

  if (ht->storage[index])
  {
    LinkedPair *pair = ht->storage[index];
    while (pair != NULL)
    {
      if (strcmp(pair->key, key) == 0)
      {
        pair->value = value;

        break;
      }
      else if (pair->next == NULL)
      {
        pair->next = new_pair;

        break;
      }

      pair = pair->next;
    }
  }
  else
  {
    ht->storage[index] = new_pair;
  }
}

/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(HashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);

  if (ht->storage[index])
  {
    LinkedPair *pair = ht->storage[index];

    if (strcmp(pair->key, key) == 0)
    {
      destroy_pair(pair);
      ht->storage[index] = NULL;
    }
    else
    {
      while (pair->next != NULL)
      {
        if (strcmp(pair->next->key, key) == 0)
        {
          LinkedPair *next_pair = pair->next->next;

          destroy_pair(pair->next);

          pair->next = next_pair;

          break;
        }
        else if (pair->next->next == NULL)
        {
          printf("no element with that key");

          break;
        }

        pair = pair->next;
      }
    }
  }
  else
  {
    printf("no element with that key");
  }
}

/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 ****/
char *hash_table_retrieve(HashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);

  if (ht->storage[index])
  {
    LinkedPair *pair = ht->storage[index];

    while (pair != NULL)
    {
      if (strcmp(pair->key, key) == 0)
      {
        return pair->value;
      }

      pair = pair->next;
    }
  }

  return NULL;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_pair_chain(LinkedPair *pair)
{
  if (pair != NULL)
  {
    LinkedPair *next_pair = pair->next;

    destroy_pair(pair);
    if (next_pair != NULL)
    {
      destroy_pair_chain(next_pair);
    }
  }
}

void destroy_hash_table(HashTable *ht)
{
  if (ht != NULL)
  {
    for (int i = 0; i < ht->capacity; i++)
    {
      destroy_pair_chain(ht->storage[i]);
    }

    free(ht->capacity);
    free(ht->storage);
    free(ht);
  }
}

/****
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 ****/
HashTable *hash_table_resize(HashTable *ht)
{
  if (ht != NULL)
  {
    HashTable *new_ht = create_hash_table(ht->capacity * 2);

    for (int i = 0; i < ht->capacity; i++)
    {
      LinkedPair *pair = ht->storage[i];

      while (pair != NULL)
      {
        hash_table_insert(new_ht, pair->key, pair->value);

        pair = pair->next;
      }
    }

    destroy_hash_table(ht);

    return new_ht;
  }

  return NULL;
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
