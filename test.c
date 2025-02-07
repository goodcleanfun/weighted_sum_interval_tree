#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <float.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "greatest/greatest.h"

#define WEIGHTED_SUM_INTERVAL_TREE_NAME sum_interval_tree_int32
#define WEIGHTED_SUM_INTERVAL_TREE_KEY_TYPE int32_t
#define WEIGHTED_SUM_INTERVAL_TREE_SUM_TYPE float
#define WEIGHTED_SUM_INTERVAL_TREE_NEGATIVE_INFINITY INT32_MIN
#include "weighted_sum_interval_tree.h"
#undef WEIGHTED_SUM_INTERVAL_TREE_NAME
#undef WEIGHTED_SUM_INTERVAL_TREE_KEY_TYPE
#undef WEIGHTED_SUM_INTERVAL_TREE_SUM_TYPE
#undef WEIGHTED_SUM_INTERVAL_TREE_NEGATIVE_INFINITY

TEST test_sum_interval_tree(void) {
    sum_interval_tree_int32 *tree = sum_interval_tree_int32_new();

    sum_interval_tree_int32_insert_interval(tree, 3, 4, 1.9f);
    sum_interval_tree_int32_insert_interval(tree, 2, 6, -0.3f);
    sum_interval_tree_int32_insert_interval(tree, 1, 9, 0.3f);
    sum_interval_tree_int32_insert_interval(tree, 5, 7, 1.4f);
    sum_interval_tree_int32_insert_interval(tree, 8, 11, 0.4f);
    sum_interval_tree_int32_insert_interval(tree, 10, 13, 1.1f);
    sum_interval_tree_int32_insert_interval(tree, 12, 14, -1.2f);
    sum_interval_tree_int32_insert_interval(tree, 12, 15, 0.7f);

    float sum = sum_interval_tree_int32_sum_weights(tree, 3);
    ASSERT_IN_RANGE(sum, 1.9f, FLT_EPSILON);

    sum = sum_interval_tree_int32_sum_weights(tree, 4);
    ASSERT_IN_RANGE(sum, 0.0f, FLT_EPSILON);

    sum = sum_interval_tree_int32_sum_weights(tree, 5);
    ASSERT_IN_RANGE(sum, 1.4f, FLT_EPSILON);

    sum = sum_interval_tree_int32_sum_weights(tree, 12);
    ASSERT_IN_RANGE(sum, 0.6f, FLT_EPSILON);

    sum = sum_interval_tree_int32_sum_weights(tree, 13);
    ASSERT_IN_RANGE(sum, -0.5f, FLT_EPSILON);

    sum = sum_interval_tree_int32_sum_weights(tree, 14);
    ASSERT_IN_RANGE(sum, 0.7f, FLT_EPSILON);

    sum_interval_tree_int32_delete(tree, 3);

    sum_interval_tree_int32_destroy(tree);
    PASS();
}



/* Add definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* command-line options, initialization. */

    RUN_TEST(test_sum_interval_tree);

    GREATEST_MAIN_END();        /* display results */
}