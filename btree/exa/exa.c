/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte triviální funkci letter_count. Všimněte si, že výstupní strom může být značně degradovaný 
 * (až na úroveň lineárního seznamu). Jako typ hodnoty v uzlu stromu využijte 'INTEGER'.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
bst_node_content_t create_content(int count) {
    bst_node_content_t result = {
        .type = INTEGER,
        .value = malloc(sizeof(int))
    }; // Creates a new content
    if (result.value != NULL) {
        *((int*)(result.value)) = count; // Sets the value
    }
    return result;
}

/// @brief Increments the value of the key in the tree, creates a new node if the key is not found
/// @param tree source tree
/// @param key key that will be incremented or added
void value_count(bst_node_t **tree, char key) {
    bst_node_content_t *node = NULL;  
    bool found = bst_search(*tree, key, &node);  // Searches for the key, sets the found value and sets the node to the found node
    if (found && node != NULL) {  // If the key is found and the node is not NULL
        (*(int *)(node->value)) += 1; // Increments the value
    } else {  
        bst_node_content_t content = create_content(1); // Creates a new content
        bst_insert(tree, key, content); // Inserts the node to the tree
    }
}

/// @brief Converts the character to its lower case equivalent
/// @param c the character
/// @return Returns the lower case equivalent of the character
int to_lower(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

void letter_count(bst_node_t **tree, char *input) {
    bst_init(tree); // Initializes the tree
    while(*input) { // Goes through the input
        char c = *input;
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) { // If the input is a character
            value_count(tree, to_lower(c)); // Lower case the character and count it
        } else if (c == ' ') { // If the input is a space
            value_count(tree, c);
        } else { // If the input is not a character or a space
            value_count(tree, '_');
        }
        input++; // Moves to the next character
    }
}