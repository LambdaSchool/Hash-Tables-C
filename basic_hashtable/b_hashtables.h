#ifndef hashtables_h
#define hashtables_h

typedef struct Pair {
  char *key;
  char *value;
} Pair;

typedef struct BasicHashTable {
  int capacity;
  Pair **storage;
} BasicHashTable;


BasicHashTable *create_hash_table(int capacity);

void hash_table_insert(BasicHashTable *ht, char *key, char *value);

void hash_table_remove(BasicHashTable *ht, char *key);

char *hash_table_retrieve(BasicHashTable *ht, char *key);

void destroy_hash_table(BasicHashTable *ht);


#endif
