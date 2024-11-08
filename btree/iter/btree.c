/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h
 * a připravených koster funkcí implementujte binární vyhledávací
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci.
 */
void bst_init(bst_node_t **tree)
{
  *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * ukazatel na obsah daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 *
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, bst_node_content_t **value)
{
  while (tree != NULL) { // Goes through the tree
    if (tree->key == key) { // If the key is found
      *value = &tree->content;
      return true;
    }
    // Go to left/right according to the key value
    if (tree->key > key) {
      tree = tree->left;
    } else {
      tree = tree->right;
    }
  }
  return false; // If the key is not found
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, bst_node_content_t value)
{
  bst_node_t *current = *tree;
  bst_node_t *parent = NULL;

  while (current != NULL) { // Goes thrue the tree
      parent = current; // Saves the parent node if the key is not found
      if (current->key == key) { // If the key is found replace the value
          if (current->content.value != NULL) {
              free(current->content.value);
          }
          current->content = value;
          return;
      } else if (current->key > key) { // Go to the left subtree
          current = current->left;
      } else { // Go to the right subtree
          current = current->right;
      }
  }
  // Creare a new node
  bst_node_t *new_node = (bst_node_t*)malloc(sizeof(bst_node_t));
  if (new_node == NULL) {
      exit(1);
  }
  new_node->key = key;
  new_node->content = value;
  new_node->left = NULL;
  new_node->right = NULL;

  // If the tree is empty create a new node and set it as the root
  if (*tree == NULL) {
      *tree = new_node;
      return;
  }

  // The node with the key is not in the tree so using the parent node from the while loop set the new node as its child
  if (key < parent->key) {
      parent->left = new_node;
  } else {
      parent->right = new_node;
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
  bst_node_t **rightmost = tree;
  while ((*rightmost)->right != NULL) { // Go to the rightmost node
      rightmost = &(*rightmost)->right;
  }
  
  if (target->content.value != NULL) {
      free(target->content.value);
  }
  // Set the target values to the rightmost node values
  target->key = (*rightmost)->key;
  target->content = (*rightmost)->content;
  
  bst_node_t *temp = *rightmost;
  *rightmost = (*rightmost)->left; // Sets the rightmost node as its left child
  free(temp);
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 *
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key)
{
 if (*tree == NULL) {
        return;  // Tree is empty
    }
    bst_node_t **current = tree;
    while (*current != NULL && (*current)->key != key) { // Find the node with the key
      current = (key < (*current)->key) ? &(*current)->left : &(*current)->right; // Set the current node to the left/right child depending on the key value
    }
    if (*current == NULL) { // Node doesn't exist
        return;
    }
    // Key is found
    if ((*current)->left == NULL && (*current)->right == NULL) { // Node doesn't have children
        if ((*current)->content.value != NULL) {
            free((*current)->content.value);
        }
        free(*current);
        *current = NULL;
    }
    else if ((*current)->left == NULL) { // Node has only right child
        bst_node_t *temp = *current;
        *current = (*current)->right;
        if (temp->content.value != NULL) {
            free(temp->content.value);
        }
        free(temp);
    }
    else if ((*current)->right == NULL) { // Node has only left child
        bst_node_t *temp = *current;
        *current = (*current)->left;
        if (temp->content.value != NULL) {
            free(temp->content.value);
        }
        free(temp);
    }
    else {
        // Node has two children
        bst_replace_by_rightmost(*current, &(*current)->left);
    }
}

/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree)
{
  if (*tree == NULL) { // Tree is empty
    return; 
  }

  stack_bst_t stack;
  stack_bst_init(&stack);
  stack_bst_push(&stack, *tree); // Adds the root to the stack

  while (!stack_bst_empty(&stack)) { // While the stack is not empty
    bst_node_t *current = stack_bst_pop(&stack); // Gets the top node
    if (current->left != NULL) { // If the left child exists
      stack_bst_push(&stack, current->left); // Adds it to the stack
    }
    if (current->right != NULL) { // If the right child exists
      stack_bst_push(&stack, current->right); // Adds it to the stack
    }
    if (current->content.value != NULL) {
      free(current->content.value);
    }
    // Free the node
    free(current); 
  }
  // The tree is empty, set the root to NULL
  *tree = NULL;
}


/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items)
{
  while(tree != NULL){ // Goes thrue left side of the tree
    bst_add_node_to_items(tree, items); // Adds the node to the items
    stack_bst_push(to_visit, tree); // Adds the node to the stack
    tree = tree->left; // Goes to the left child
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items)
{
  if(tree == NULL){ // Tree is empty do nothing
    return;
  }
  stack_bst_t stack;
  stack_bst_init(&stack);
  bst_leftmost_preorder(tree, &stack, items); // Saves the leftmost nodes to the stack
  while(!stack_bst_empty(&stack)){ // While the stack is not empty
    bst_node_t *current = stack_bst_pop(&stack); // Gets the top node
    bst_leftmost_preorder(current->right, &stack, items); // Saves the leftmost nodes of the right child to the stack
  }
}

/*
 * Pomocná funkce pro iterativní inorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit)
{
  while(tree != NULL){
    stack_bst_push(to_visit, tree); // Adds the ndoe to the stack
    tree = tree->left; // Goes to the left child
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items)
{
  if(tree == NULL){ // Tree is empty do nothing
    return;
  }
  stack_bst_t stack;
  stack_bst_init(&stack);
  bst_leftmost_inorder(tree, &stack); // Saves leftmost nodes to the stack
  while(!stack_bst_empty(&stack)){ // Goes thrue the loop while the stack is not empty
    bst_node_t *current = stack_bst_pop(&stack); 
    bst_add_node_to_items(current, items); // Adds the top node to the items
    bst_leftmost_inorder(current->right, &stack); // Saves the leftmost nodes of the right child
  }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit)
{
  while(tree != NULL) {
    stack_bst_push(to_visit, tree); // Saves current node to the stack
    stack_bool_push(first_visit, true); // Saves value to the stack
    tree = tree->left; // Goes to the left child
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items)
{
  if (tree == NULL) { // Tree is empty do nothing
    return;
  }

  // Initializes the stacks
  stack_bst_t stack;
  stack_bool_t first_visit;
  stack_bst_init(&stack);
  stack_bool_init(&first_visit);

  bst_leftmost_postorder(tree, &stack, &first_visit); // Saves the leftmost nodes to the stack
  while (!stack_bst_empty(&stack)) { // While the stack is not empty
    // Sets current and first to the top of each stack
    bst_node_t *current = stack_bst_pop(&stack);
    bool first = stack_bool_pop(&first_visit);
    if (first) { // If the node is visited for the first time
      stack_bst_push(&stack, current); // Adds the node to the stack
      stack_bool_push(&first_visit, false); // Adds the value to the stack
      if (current->right != NULL) { // If the node has right child
        bst_leftmost_postorder(current->right, &stack, &first_visit); // Does postorder on the right child
      }
    } else { // The node was alredy visited
      bst_add_node_to_items(current, items); // Adds the node to the items
    }
  }
}
