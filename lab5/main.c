#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct bst_node *bst;

struct bst_node {
    int value;
    bst left, right;
};

void bst_print_elements(bst tree) {
    if (tree != NULL) {
        bst_print_elements(tree->left);
        printf("%d, ", tree->value);
        bst_print_elements(tree->right);
    }
}

void bst_print(bst tree) {
    printf("{");
    bst_print_elements(tree);
    printf("}\n");
}

// Helper function to create a new node
bst create_node(int value) {
    bst new_node = (bst)malloc(sizeof(struct bst_node));
    if (new_node) {
        new_node->value = value;
        new_node->left = new_node->right = NULL;
    }
    return new_node;
}

// Task 1: bst_insert
bool bst_insert(bst *tree, int value) {
    //creat an new node in the tree of it has an empty spot
    if (*tree == NULL) {
        *tree = create_node(value);
        return *tree != NULL;
    }
    //if the value is less then current value, then inert left
    if (value < (*tree)->value) {
        return bst_insert(&((*tree)->left), value);
    //if the value is greater then current value, then inert right
    } else if (value > (*tree)->value) {
        return bst_insert(&((*tree)->right), value);
    }

    // Value already exists
    return false;
}

// Task 2: bst_remove
bool bst_remove(bst *tree, int value) {
    //retrun flase if tree is empty
    if (*tree == NULL) {
        return false;
    }
    //if the value is less then current value, then inert left
    if (value < (*tree)->value) {
        return bst_remove(&((*tree)->left), value);
    } 
    //if the value is greater then current value, then inert right
    else if (value > (*tree)->value) {
        return bst_remove(&((*tree)->right), value);
    } 
    //Find the node that need to be remove uring an temp node, free memory afterwards
    else {
        if ((*tree)->left == NULL) {
            bst temp = (*tree)->right;
            free(*tree);
            *tree = temp;
        } else if ((*tree)->right == NULL) {
            bst temp = (*tree)->left;
            free(*tree);
            *tree = temp;
        } else {
            bst min_node = (*tree)->right;
            while (min_node->left != NULL) {
                min_node = min_node->left;
            }
            (*tree)->value = min_node->value;
            return bst_remove(&((*tree)->right), min_node->value);
        }
        return true;
    }
}

// Helper functions for Task 3 and Task 4
void bst_to_list(bst root, int **list, int *size) {
    //funtions that vompare element an retrun it
    if (root == NULL) return;
    bst_to_list(root->left, list, size);
    (*list)[(*size)++] = root->value;
    bst_to_list(root->right, list, size);
}

int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

bst sorted_array_to_bst(int *arr, int start, int end) {
    //empty if starte list is greater the end list
    if (start > end) return NULL;
    //find middle index with new node
    int mid = (start + end) / 2;
    bst node = create_node(arr[mid]);
    //build left and right of sub trees
    node->left = sorted_array_to_bst(arr, start, mid - 1);
    node->right = sorted_array_to_bst(arr, mid + 1, end);
    return node;
}

// Task 3: bst_union
bst bst_union(bst tree1, bst tree2) {
    //each value will be stored in array
    int *list1 = malloc(100 * sizeof(int)), size1 = 0;
    int *list2 = malloc(100 * sizeof(int)), size2 = 0;
    bst_to_list(tree1, &list1, &size1);
    bst_to_list(tree2, &list2, &size2);
    //creat array to sort and stored union of both rtree
    int *union_list = malloc((size1 + size2) * sizeof(int));
    int i = 0, j = 0, k = 0;
    //combined both array to union_list
    while (i < size1 && j < size2) {
        if (list1[i] < list2[j]) {
            union_list[k++] = list1[i++];
        } else if (list1[i] > list2[j]) {
            union_list[k++] = list2[j++];
        } else {
            union_list[k++] = list1[i++];
            j++;
        }
    }
    //intert remining values from list 1 and 2 into union_list
    while (i < size1) {
        union_list[k++] = list1[i++];
    }

    while (j < size2) {
        union_list[k++] = list2[j++];
    }

    bst union_tree = sorted_array_to_bst(union_list, 0, k - 1);
    //free memory from list
    free(list1);
    free(list2);
    free(union_list);

    return union_tree;
}

// Task 4: bst_intersection
bst bst_intersection(bst tree1, bst tree2) {
    //each value will be stored in array
    int *list1 = malloc(100 * sizeof(int)), size1 = 0;
    int *list2 = malloc(100 * sizeof(int)), size2 = 0;
    bst_to_list(tree1, &list1, &size1);
    bst_to_list(tree2, &list2, &size2);
    //creat array to sort and stored union of both rtree
    int *intersection_list = malloc((size1 + size2) * sizeof(int));
    int i = 0, j = 0, k = 0;
    // Find same values that exist in both list and stor into array using loop
    while (i < size1 && j < size2) {
        if (list1[i] < list2[j]) {
            i++;
        } else if (list1[i] > list2[j]) {
            j++;
        } else {
            intersection_list[k++] = list1[i++];
            j++;
        }
    }

    bst intersection_tree = sorted_array_to_bst(intersection_list, 0, k - 1);

    free(list1);
    free(list2);
    free(intersection_list);

    return intersection_tree;
}

int main() {
    // Initialize two trees.
    bst tree1 = NULL, tree2 = NULL;

    assert(bst_insert(&tree1, 1));
    assert(bst_insert(&tree1, 9));
    assert(bst_insert(&tree1, 2));
    assert(bst_insert(&tree1, 7));
    assert(bst_insert(&tree1, 0));
    assert(bst_insert(&tree1, 3));
    assert(bst_insert(&tree1, 13));
    assert(bst_insert(&tree1, 5));
    assert(!bst_insert(&tree1, 3));

    // Should print: {0, 1, 2, 3, 5, 7, 9, 13, }
    bst_print(tree1);

    assert(bst_insert(&tree2, 12));
    assert(bst_insert(&tree2, 13));
    assert(bst_insert(&tree2, 1));
    assert(bst_insert(&tree2, 0));
    assert(bst_insert(&tree2, 19));
    assert(bst_insert(&tree2, 7));
    assert(!bst_insert(&tree2, 13));
    assert(bst_insert(&tree2, 5));

    // Should print: {0, 1, 5, 7, 12, 13, 19, }
    bst_print(tree2);

    // Should print: {0, 1, 2, 3, 5, 7, 9, 12, 13, 19, }
    bst union_1_2 = bst_union(tree1, tree2);
    bst_print(union_1_2);

    // Should print: {0, 1, 5, 7, 13, }
    bst intersection_1_2 = bst_intersection(tree1, tree2);
    bst_print(intersection_1_2);

    return 0;
}