/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  if(tree == NULL)
  {
    return false;
  }
  else
  {
    while((tree->left != NULL) || (tree->right != NULL))
    {
      if(tree->key != key)
      {
        if(key > tree->key)
        {
          tree = tree->right;
        }
        else
        {
          tree = tree->left;
        }
      }
      else
      {
        *value = tree->value;
        return true;
      }
    }
  }
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  bst_node_t *tmp = *tree;
  bst_node_t *tmp2 = malloc(sizeof(bst_node_t));
  tmp2->key = key;
  tmp2->value = value;
  tmp2->left = NULL;
  tmp2->right = NULL;
  if(!tmp)
  {
    *tree = tmp2;
    return;
  }
  while (tmp)
  {
    if(tmp->key == key)
    {
      tmp->value = value;
      free(tmp2);
      break;
    }
    else
    {
      if(tmp->key < key)
      {
        if(tmp->right != NULL)
        {
          tmp = tmp->right;
        }
        else
        {
          tmp->right = tmp2;
          break;
        }
      }
      else
      {
        if(tmp->left != NULL)
        {
          tmp = tmp->left;
        }
        else
        {
          tmp->left = tmp2;
          break;
        }
      }
    }
  }
  
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  bst_node_t *tmp;
  while((*tree)->right != NULL)
  {
    tree = &((*tree)->right);
  }
  target->value = (*tree)->value;
  target->key = (*tree)->key;
  tmp = (*tree)->left;
  free(*tree);
  *tree = tmp;
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  bst_node_t *tmp;
  bst_node_t *back;
  tmp = *tree;
  back = NULL;
  while(tmp)
  {
    if(tmp->key == key)
    {
      if((tmp->left) && (tmp->right))
      {
        if((!tmp->left && tmp->right) || (tmp->right && !tmp->left))
        {
          if(back == NULL)
          {
            if(tmp->left != NULL)
            {
              *tree = tmp->left;
            }
            else
            {
              if(tmp->right != NULL)
              {
                *tree = tmp->right;
              }
            }
            free(tmp);
            break;
          }
          else
          {
            if(back->left == tmp)
            {
              if(tmp->left != NULL)
              {
                back->left = tmp->left;
              }
              else
              {
                if(tmp->right != NULL)
                {
                  back->left = tmp->right;
                }
              }
            }
            else
            {
              if(back->right == tmp)
              {
                if(tmp->left != NULL)
                {
                  back->right = tmp->left;
                }
                else
                {
                  if(tmp->right != NULL)
                  {
                    back->right = tmp->right;
                  }
                }
              }
              free(tmp);
              break;
            }
          }
        }
        else
        {
          if((tmp->right) && (tmp->left))
          {
            bst_replace_by_rightmost(tmp, &(tmp->left));
          }
        }
      }
      else
      {
        if(tmp == *tree)
        {
          free(tmp);
          return;
        }
        else
        {
          if(back->right == tmp)
          {
            back->right = NULL;
          }
          if(back->left == tmp)
          {
            back->left = NULL;
          }
          free(tmp);
          break;
        }
      }
    }
    if(tmp->key > key)
    {
      if(tmp->left != NULL)
      {
        back = tmp;
        tmp = tmp->left;
      }
      else
      {
        break;
      }
    }
    else
    {
      if(tmp->right != NULL)
      {
        back = tmp;
        tmp = tmp->right;
      }
      else
      {
        break;
      }
    }
  }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  stack_bst_t stack;
  stack_bst_init(&stack);
  stack_bst_push(&stack, *tree);
  while (!stack_bst_empty(&stack))
  {
    bst_node_t *tmp = stack_bst_pop(&stack);
    if(!tmp)
    {
      continue;
    }
    stack_bst_push(&stack, tmp->right);
    stack_bst_push(&stack, tmp->left);
    free(tmp);
  }
  *tree = NULL;
  

}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
  while (tree != NULL)
  {
    stack_bst_push(to_visit, tree);
    bst_print_node(tree);
    tree = tree->left;
  }
  
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  stack_bst_t stack;
  stack_bst_init(&stack);
  bst_node_t *tmp = tree;
  bst_leftmost_preorder(tmp, &stack);
  while(!stack_bst_empty(&stack))
  {
    tmp = stack_bst_top(&stack);
    stack_bst_pop(&stack);
    bst_leftmost_preorder(tmp->right, &stack);
  }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  while (tree != NULL)
  {
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
  
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  bst_node_t *tmp;
  tmp = tree;
  stack_bst_t stack;
  stack_bst_init(&stack);
  bst_leftmost_inorder(tmp, &stack);
  while(!stack_bst_empty(&stack))
  {
    tmp = stack_bst_top(&stack);
    stack_bst_pop(&stack);
    bst_print_node(tmp);
    bst_leftmost_inorder(tmp->right, &stack);
  }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
                              while(tree != NULL)
                              {
                                stack_bst_push(to_visit, tree);
                                stack_bool_push(first_visit, true);
                                tree = tree->left;
                              }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  bool tmp_boolean;
  stack_bst_t stack;
  stack_bst_init(&stack);
  stack_bool_t stack2;
  stack_bool_init(&stack2);
  bst_node_t *tmp;
  tmp = tree;
  bst_leftmost_postorder(tmp, &stack, &stack2);
  while(!stack_bst_empty(&stack))
  {
    tmp = stack_bst_top(&stack);
    stack_bst_pop(&stack);
    tmp_boolean = stack_bool_pop(&stack2);
    if(tmp_boolean)
    {
      stack_bool_push(&stack2, false);
      stack_bst_push(&stack, tmp);
      bst_leftmost_postorder(tmp->right, &stack, &stack2);
    }
    else
    {
      bst_print_node(tmp);
    }
  }

}
