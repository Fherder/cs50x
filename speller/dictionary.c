// Implements a dictionary's functionality

#include "dictionary.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 207;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int key = hash(word);

    node *pointer = table[key];

    while (pointer != NULL)
    {
        if (strcasecmp(word, pointer->word) == 0)
        {
            return true;
        }

        pointer = pointer->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int sum = 0;

    for (int i = 0; word[i] != '\0'; i++)
    {
        sum += toupper(word[i]);
    }

    int key = sum % N;

    return (sum % N);
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");

    int key;

    if (file == NULL)
    {

        return false;
    }

    char buffer[LENGTH + 2];

    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        // Delete \n
        buffer[strcspn(buffer, "\n")] = '\0';
        key = hash(buffer);

        addToHashFunction(buffer, key);
    }

    fclose(file);
    return true;
}

void addToHashFunction(const char *word, int k)
{
    node *new_node = malloc(sizeof(node));

    // Copy the word
    strcpy(new_node->word, word);

    new_node->next = NULL;

    if (table[k] == NULL)
    {
        // Add the new node to the Hash table
        table[k] = new_node;
    }
    else
    {
        new_node->next = table[k];
        table[k] = new_node;
    }
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    unsigned int size = 0;
    for (int i = 0; i < N; i++)
    {
        node *pt = table[i];
        while (pt != NULL)
        {
            size++;
            pt = pt->next;
        }
    }
    return size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *pt = table[i];

        while (pt != NULL)
        {
            node *temp = pt;
            pt = pt->next;
            free(temp);
        }
    }
    return true;
}
