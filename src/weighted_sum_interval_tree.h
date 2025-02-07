#ifndef WEIGHTED_SUM_INTERVAL_TREE_H
#define WEIGHTED_SUM_INTERVAL_TREE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#endif // WEIGHTED_SUM_INTERVAL_TREE_H

#ifndef WEIGHTED_SUM_INTERVAL_TREE_NAME
#error "Must define WEIGHTED_SUM_INTERVAL_TREE_NAME"
#endif

#ifndef WEIGHTED_SUM_INTERVAL_TREE_KEY_TYPE
#error "Must define WEIGHTED_SUM_INTERVAL_TREE_TYPE"
#endif

#ifndef WEIGHTED_SUM_INTERVAL_TREE_NEGATIVE_INFINITY
#error "Must define WEIGHTED_SUM_INTERVAL_TREE_NEGATIVE_INFINITY of the same type as key"
#endif

#ifndef WEIGHTED_SUM_INTERVAL_TREE_SUM_TYPE
#error "Must define WEIGHTED_TREE_SUM_INTERVAL_SUM_TYPE"
#endif

#ifndef WEIGHTED_SUM_INTERVAL_TREE_MAX_HEIGHT
#define WEIGHTED_SUM_INTERVAL_TREE_MAX_HEIGHT 128
#endif

#define WEIGHTED_SUM_INTERVAL_TREE_CONCAT_(a, b) a ## b
#define WEIGHTED_SUM_INTERVAL_TREE_CONCAT(a, b) WEIGHTED_SUM_INTERVAL_TREE_CONCAT_(a, b)
#define WEIGHTED_SUM_INTERVAL_TREE_TYPED(name) WEIGHTED_SUM_INTERVAL_TREE_CONCAT(WEIGHTED_SUM_INTERVAL_TREE_NAME, _##name)
#define WEIGHTED_SUM_INTERVAL_TREE_FUNC(func) WEIGHTED_SUM_INTERVAL_TREE_CONCAT(WEIGHTED_SUM_INTERVAL_TREE_NAME, _##func)

#ifndef WEIGHTED_SUM_INTERVAL_TREE_LESS_THAN
#define WEIGHTED_SUM_INTERVAL_TREE_LESS_THAN(a, b) ((a) < (b))
#endif

#ifndef WEIGHTED_SUM_INTERVAL_TREE_EQUALS
#define WEIGHTED_SUM_INTERVAL_TREE_EQUALS(a, b) ((a) == (b))
#endif


#define BST_NAME WEIGHTED_SUM_INTERVAL_TREE_NAME
#define BST_KEY_TYPE WEIGHTED_SUM_INTERVAL_TREE_KEY_TYPE
#define BST_NODE_EXTRA \
    WEIGHTED_SUM_INTERVAL_TREE_SUM_TYPE summand; \
    uint8_t height;

#ifdef WEIGHTED_SUM_INTERVAL_TREE_KEY_EQUALS
#define BST_KEY_EQUALS WEIGHTED_SUM_INTERVAL_TREE_KEY_EQUALS
#endif

#ifdef WEIGHTED_SUM_INTERVAL_TREE_KEY_LESS_THAN
#define BST_KEY_LESS_THAN WEIGHTED_SUM_INTERVAL_TREE_KEY_LESS_THAN
#endif

#include "binary_tree/binary_tree.h"

#undef BST_NAME
#undef BST_KEY_TYPE
#undef BST_NODE_EXTRA
#ifndef WEIGHTED_SUM_INTERVAL_TREE_KEY_EQUALS
#define WEIGHTED_SUM_INTERVAL_TREE_KEY_EQUALS WEIGHTED_SUM_INTERVAL_TREE_TYPED(key_equals)
#endif
#ifndef WEIGHTED_SUM_INTERVAL_TREE_KEY_LESS_THAN
#define WEIGHTED_SUM_INTERVAL_TREE_KEY_LESS_THAN WEIGHTED_SUM_INTERVAL_TREE_TYPED(key_less_than)
#endif

#define WEIGHTED_SUM_INTERVAL_TREE_NODE WEIGHTED_SUM_INTERVAL_TREE_TYPED(node_t)

#define WEIGHTED_SUM_INTERVAL_TREE_NODE_MEMORY_POOL_NAME WEIGHTED_SUM_INTERVAL_TREE_TYPED(node_memory_pool)

#define MEMORY_POOL_NAME WEIGHTED_SUM_INTERVAL_TREE_NODE_MEMORY_POOL_NAME
#define MEMORY_POOL_TYPE WEIGHTED_SUM_INTERVAL_TREE_NODE
#include "memory_pool/memory_pool.h"
#undef MEMORY_POOL_NAME
#undef MEMORY_POOL_TYPE

#define WEIGHTED_SUM_INTERVAL_TREE_NODE_MEMORY_POOL_FUNC(name) WEIGHTED_SUM_INTERVAL_TREE_CONCAT(WEIGHTED_SUM_INTERVAL_TREE_NODE_MEMORY_POOL_NAME, _##name)


typedef struct WEIGHTED_SUM_INTERVAL_TREE_NAME {
    WEIGHTED_SUM_INTERVAL_TREE_TYPED(node_t) *root;
    WEIGHTED_SUM_INTERVAL_TREE_TYPED(node_memory_pool) *pool;
} WEIGHTED_SUM_INTERVAL_TREE_NAME;


void WEIGHTED_SUM_INTERVAL_TREE_FUNC(push_down_sum_rotate_left)(WEIGHTED_SUM_INTERVAL_TREE_NODE *node) {
    // push down summand from node
    node->left->summand += node->summand;
    node->right->summand += node->summand;
    node->summand = (WEIGHTED_SUM_INTERVAL_TREE_SUM_TYPE)0;

    // push down summand from node->right
    WEIGHTED_SUM_INTERVAL_TREE_NODE *tmp_node = node->right;
    tmp_node->left->summand += tmp_node->summand;
    tmp_node->right->summand += tmp_node->summand;
    tmp_node->summand = (WEIGHTED_SUM_INTERVAL_TREE_SUM_TYPE)0;

    // perform normal rotation
    WEIGHTED_SUM_INTERVAL_TREE_FUNC(rotate_left)(node);
}

void WEIGHTED_SUM_INTERVAL_TREE_FUNC(push_down_sum_rotate_right)(WEIGHTED_SUM_INTERVAL_TREE_NODE *node) {
    // push down summand from node
    node->left->summand += node->summand;
    node->right->summand += node->summand;
    node->summand = (WEIGHTED_SUM_INTERVAL_TREE_SUM_TYPE)0;

    // push down summand from node->left
    WEIGHTED_SUM_INTERVAL_TREE_NODE *tmp_node = node->left;
    tmp_node->left->summand += tmp_node->summand;
    tmp_node->right->summand += tmp_node->summand;
    tmp_node->summand = (WEIGHTED_SUM_INTERVAL_TREE_SUM_TYPE)0;

    // perform normal rotation
    WEIGHTED_SUM_INTERVAL_TREE_FUNC(rotate_right)(node);
}

bool WEIGHTED_SUM_INTERVAL_TREE_FUNC(rebalance)(WEIGHTED_SUM_INTERVAL_TREE_TYPED(stack_t) *stack) {
    WEIGHTED_SUM_INTERVAL_TREE_NODE *tmp_node;
    bool done = false;
    // rebalance
    while (!WEIGHTED_SUM_INTERVAL_TREE_FUNC(stack_empty)(stack) && !done) {
        uint8_t tmp_height, old_height;
        tmp_node = WEIGHTED_SUM_INTERVAL_TREE_FUNC(stack_pop)(stack);
        if (WEIGHTED_SUM_INTERVAL_TREE_FUNC(node_is_leaf)(tmp_node)) {
            // skip if there's a leaf node
            continue;
        }
        old_height = tmp_node->height;
        // left subtree is heavier
        if (tmp_node->left->height - tmp_node->right->height == 2) {
            if (tmp_node->left->left->height - tmp_node->right->height == 1) {
                // left subtree's left subtree is heavier, rotate right
                WEIGHTED_SUM_INTERVAL_TREE_FUNC(push_down_sum_rotate_right)(tmp_node);
                tmp_node->right->height = tmp_node->right->left->height + 1;
                tmp_node->height = tmp_node->right->height + 1;
            } else {
                // left subtree's right subtree is heavier, rotate left then right
                WEIGHTED_SUM_INTERVAL_TREE_FUNC(push_down_sum_rotate_left)(tmp_node->left);
                WEIGHTED_SUM_INTERVAL_TREE_FUNC(push_down_sum_rotate_right)(tmp_node);
                tmp_height = tmp_node->left->left->height;
                tmp_node->left->height = tmp_height + 1;
                tmp_node->right->height = tmp_height + 1;
                tmp_node->height = tmp_height + 2;
            }
        // right subtree is heavier
        } else if (tmp_node->right->height - tmp_node->left->height == 2) {
            if (tmp_node->right->right->height - tmp_node->left->height == 1) {
                // right subtree's right subtree is heavier, rotate left
                WEIGHTED_SUM_INTERVAL_TREE_FUNC(push_down_sum_rotate_left)(tmp_node);
                tmp_node->left->height = tmp_node->left->right->height + 1;
                tmp_node->height = tmp_node->left->height + 1;
            } else {
                // right subtree's left subtree is heavier, rotate right then left
                WEIGHTED_SUM_INTERVAL_TREE_FUNC(push_down_sum_rotate_right)(tmp_node->right);
                WEIGHTED_SUM_INTERVAL_TREE_FUNC(push_down_sum_rotate_left)(tmp_node);
                tmp_height = tmp_node->right->right->height;
                tmp_node->left->height = tmp_height + 1;
                tmp_node->right->height = tmp_height + 1;
                tmp_node->height = tmp_height + 2;
            }
        } else {
            // update height, no rotation needed
            if (tmp_node->left->height > tmp_node->right->height) {
                tmp_node->height = tmp_node->left->height + 1;
            } else {
                tmp_node->height = tmp_node->right->height + 1;
            }
        }
        if (tmp_node->height == old_height) {
            // terminate early if nothing changed heights
            done = true;
        }
    }
    return true;
}


// AVL tree insert with summand
bool WEIGHTED_SUM_INTERVAL_TREE_FUNC(insert)(WEIGHTED_SUM_INTERVAL_TREE_NAME *tree, WEIGHTED_SUM_INTERVAL_TREE_KEY_TYPE key, void *value) {
    if (tree == NULL) return false;
    WEIGHTED_SUM_INTERVAL_TREE_NODE *node = tree->root;
    WEIGHTED_SUM_INTERVAL_TREE_TYPED(node_memory_pool) *pool = tree->pool;

    WEIGHTED_SUM_INTERVAL_TREE_NODE *tmp_node;
    bool done = false;
    if (node->left == NULL) {
        // empty tree
        node->left = (WEIGHTED_SUM_INTERVAL_TREE_NODE *)value;
        node->key = key;
        node->height = 0;
        node->summand = (WEIGHTED_SUM_INTERVAL_TREE_SUM_TYPE)0;
        node->right = NULL;
    } else {
        // non-empty tree
        WEIGHTED_SUM_INTERVAL_TREE_TYPED(stack_t) stack = (WEIGHTED_SUM_INTERVAL_TREE_TYPED(stack_t)){
            .stack = {NULL},
            .stack_size = 0
        };
        tmp_node = WEIGHTED_SUM_INTERVAL_TREE_TYPED(candidate_leaf)(node, key, &stack);
        if (WEIGHTED_SUM_INTERVAL_TREE_KEY_EQUALS(key, tmp_node->key)) {
            // key already exists
            return false;
        }
        // tmp_node is the leaf that will become the parent of the new leaf
        WEIGHTED_SUM_INTERVAL_TREE_NODE *old_leaf = WEIGHTED_SUM_INTERVAL_TREE_NODE_MEMORY_POOL_FUNC(get)(pool);
        if (old_leaf == NULL) return false;
        old_leaf->key = tmp_node->key;
        old_leaf->left = tmp_node->left;
        old_leaf->right = NULL;
        old_leaf->height = 0;
        old_leaf->summand = (WEIGHTED_SUM_INTERVAL_TREE_SUM_TYPE)0;
        WEIGHTED_SUM_INTERVAL_TREE_NODE *new_leaf = WEIGHTED_SUM_INTERVAL_TREE_NODE_MEMORY_POOL_FUNC(get)(pool);
        if (new_leaf == NULL) {
            WEIGHTED_SUM_INTERVAL_TREE_NODE_MEMORY_POOL_FUNC(release)(pool, old_leaf);
            return false;
        }
        new_leaf->key = key;
        new_leaf->left = (WEIGHTED_SUM_INTERVAL_TREE_NODE *)value;
        new_leaf->right = NULL;
        new_leaf->height = 0;
        new_leaf->summand = (WEIGHTED_SUM_INTERVAL_TREE_SUM_TYPE)0;
        if (WEIGHTED_SUM_INTERVAL_TREE_KEY_LESS_THAN(tmp_node->key, key)) {
            tmp_node->left = old_leaf;
            tmp_node->right = new_leaf;
            tmp_node->key = key;
        } else {
            tmp_node->left = new_leaf;
            tmp_node->right = old_leaf;
        }
        tmp_node->height = 1;
        return WEIGHTED_SUM_INTERVAL_TREE_FUNC(rebalance)(&stack);
    }
    return true;
}

void *WEIGHTED_SUM_INTERVAL_TREE_FUNC(delete)(WEIGHTED_SUM_INTERVAL_TREE_NAME *tree, WEIGHTED_SUM_INTERVAL_TREE_KEY_TYPE key) {
    if (tree == NULL) return NULL;
    WEIGHTED_SUM_INTERVAL_TREE_NODE *node = tree->root;
    WEIGHTED_SUM_INTERVAL_TREE_TYPED(node_memory_pool) *pool = tree->pool;

    WEIGHTED_SUM_INTERVAL_TREE_NODE *tmp_node, *upper_node, *other_node;
    void *deleted_value;
    if (node->left == NULL) {
        return NULL;
    } else if (node->right == NULL) {
        if (WEIGHTED_SUM_INTERVAL_TREE_KEY_EQUALS(key, node->key)) {
            deleted_value = (void *)node->left;
            node->left = NULL;
            return deleted_value;
        } else {
            return NULL;
        }
    } else {
        tmp_node = node;
        WEIGHTED_SUM_INTERVAL_TREE_TYPED(stack_t) stack = (WEIGHTED_SUM_INTERVAL_TREE_TYPED(stack_t)){
            .stack = {NULL},
            .stack_size = 0
        };
        while (WEIGHTED_SUM_INTERVAL_TREE_FUNC(node_is_internal)(tmp_node)) {
            if (!WEIGHTED_SUM_INTERVAL_TREE_FUNC(stack_push(&stack, tmp_node))) return NULL;
            upper_node = tmp_node;
            if (WEIGHTED_SUM_INTERVAL_TREE_KEY_LESS_THAN(key, tmp_node->key)) {
                tmp_node = upper_node->left;
                other_node = upper_node->right;
            } else {
                tmp_node = upper_node->right;
                other_node = upper_node->left;
            }
        }
        if (!WEIGHTED_SUM_INTERVAL_TREE_KEY_EQUALS(key, tmp_node->key)) {
            return NULL;
        } else {
            upper_node->key = other_node->key;
            upper_node->left = other_node->left;
            upper_node->right = other_node->right;
            upper_node->height = other_node->height;
            deleted_value = (void *)tmp_node->left;
            // remove upper_node from the stack since it's now a leaf node
            WEIGHTED_SUM_INTERVAL_TREE_FUNC(stack_pop)(&stack);

            WEIGHTED_SUM_INTERVAL_TREE_NODE_MEMORY_POOL_FUNC(release)(pool, tmp_node);
            WEIGHTED_SUM_INTERVAL_TREE_NODE_MEMORY_POOL_FUNC(release)(pool, other_node);
            if (!WEIGHTED_SUM_INTERVAL_TREE_FUNC(rebalance)(&stack)) {
                return NULL;
            }
            return deleted_value;
        }
    }

}

WEIGHTED_SUM_INTERVAL_TREE_NAME *WEIGHTED_SUM_INTERVAL_TREE_FUNC(new)(void) {
    WEIGHTED_SUM_INTERVAL_TREE_NAME *tree = malloc(sizeof(WEIGHTED_SUM_INTERVAL_TREE_NAME));
    if (tree == NULL) return NULL;
    tree->pool = WEIGHTED_SUM_INTERVAL_TREE_NODE_MEMORY_POOL_FUNC(new)();
    if (tree->pool == NULL) {
        free(tree);
        return NULL;
    }
    tree->root = WEIGHTED_SUM_INTERVAL_TREE_NODE_MEMORY_POOL_FUNC(get)(tree->pool);
    if (tree->root == NULL) {
        WEIGHTED_SUM_INTERVAL_TREE_NODE_MEMORY_POOL_FUNC(destroy)(tree->pool);
        free(tree);
        return NULL;
    }
    tree->root->summand = (WEIGHTED_SUM_INTERVAL_TREE_SUM_TYPE)0;
    // need key for -inf, use root as non-NULL object pointer
    WEIGHTED_SUM_INTERVAL_TREE_FUNC(insert)(tree, WEIGHTED_SUM_INTERVAL_TREE_NEGATIVE_INFINITY, (void *)tree->root);
    return tree;
}

WEIGHTED_SUM_INTERVAL_TREE_SUM_TYPE WEIGHTED_SUM_INTERVAL_TREE_FUNC(sum_weights)(WEIGHTED_SUM_INTERVAL_TREE_NAME *tree, WEIGHTED_SUM_INTERVAL_TREE_KEY_TYPE key) {
    if (tree == NULL || tree->root == NULL || tree->root->left == NULL) return (WEIGHTED_SUM_INTERVAL_TREE_SUM_TYPE)0;
    WEIGHTED_SUM_INTERVAL_TREE_SUM_TYPE sum = (WEIGHTED_SUM_INTERVAL_TREE_SUM_TYPE)0;
    WEIGHTED_SUM_INTERVAL_TREE_NODE *current_node = tree->root;
    sum = current_node->summand;
    while (!WEIGHTED_SUM_INTERVAL_TREE_FUNC(node_is_leaf)(current_node)) {
        if (WEIGHTED_SUM_INTERVAL_TREE_KEY_LESS_THAN(key, current_node->key)) {
            current_node = current_node->left;
        } else {
            current_node = current_node->right;
        }
        sum += current_node->summand;
    }
    return sum;
}

bool WEIGHTED_SUM_INTERVAL_TREE_FUNC(insert_interval)(WEIGHTED_SUM_INTERVAL_TREE_NAME *tree,
                                                      WEIGHTED_SUM_INTERVAL_TREE_KEY_TYPE start,
                                                      WEIGHTED_SUM_INTERVAL_TREE_KEY_TYPE end,
                                                      WEIGHTED_SUM_INTERVAL_TREE_SUM_TYPE weight) {

    if (tree == NULL) return false;
    if (WEIGHTED_SUM_INTERVAL_TREE_FUNC(get)(tree->root, start) == NULL) {
        WEIGHTED_SUM_INTERVAL_TREE_FUNC(insert)(tree, start, (void *)tree->root);
    }
    if (WEIGHTED_SUM_INTERVAL_TREE_FUNC(get)(tree->root, end) == NULL) {
        WEIGHTED_SUM_INTERVAL_TREE_FUNC(insert)(tree, end, (void *)tree->root);
    }

    WEIGHTED_SUM_INTERVAL_TREE_NODE *tmp_node = tree->root;
    // follow search path for start
    while (!WEIGHTED_SUM_INTERVAL_TREE_FUNC(node_is_leaf)(tmp_node)) {
        // add weight to everything right of path
        if (WEIGHTED_SUM_INTERVAL_TREE_KEY_LESS_THAN(start, tmp_node->key)) {
            tmp_node->right->summand += weight;
            tmp_node = tmp_node->left;
        } else {
            tmp_node = tmp_node->right;
        }
    }

    // leaf with key start
    tmp_node->summand += weight;
    tmp_node = tree->root;
    // follow search path for end
    while (!WEIGHTED_SUM_INTERVAL_TREE_FUNC(node_is_leaf)(tmp_node)) {
        // subtract weight from everything right of path
        if (WEIGHTED_SUM_INTERVAL_TREE_KEY_LESS_THAN(end, tmp_node->key)) {
            tmp_node->right->summand -= weight;
            tmp_node = tmp_node->left;
        } else {
            tmp_node = tmp_node->right;
        }
    }
    tmp_node->summand -= weight;
    return true;
}


void WEIGHTED_SUM_INTERVAL_TREE_FUNC(destroy)(WEIGHTED_SUM_INTERVAL_TREE_NAME *tree) {
    if (tree == NULL) return;
    if (tree->pool != NULL) {
        WEIGHTED_SUM_INTERVAL_TREE_NODE_MEMORY_POOL_FUNC(destroy)(tree->pool);
    }
    free(tree);
}
