#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

typedef struct NO {
    void *item;
    int frequencia;
    struct NO *prox;
    struct NO *esq;
    struct NO *dir;
} NO;

NO* criar_no(void *item, int frequencia)
{
    NO *novo_no = (NO*) malloc(sizeof(NO));
    novo_no->item = item;
    novo_no->frequencia = frequencia;
    novo_no->prox = NULL;
    novo_no->esq = NULL;
    novo_no->dir = NULL;
    return novo_no;
}

void test_criar_no(void)
{
    // Test creating a node with a character item and a frequency of 1
    char item_char = 'a';
    int freq = 1;
    NO *node_char = criar_no(&item_char, freq);
    CU_ASSERT_PTR_NOT_NULL(node_char);
    CU_ASSERT_EQUAL(*(char*)node_char->item, 'a');
    CU_ASSERT_EQUAL(node_char->frequencia, 1);
    CU_ASSERT_PTR_NULL(node_char->prox);
    CU_ASSERT_PTR_NULL(node_char->esq);
    CU_ASSERT_PTR_NULL(node_char->dir);

    // Test creating a node with an integer item and a frequency of 2
    int item_int = 42;
    freq = 2;
    NO *node_int = criar_no(&item_int, freq);
    CU_ASSERT_PTR_NOT_NULL(node_int);
    CU_ASSERT_EQUAL(*(int*)node_int->item, 42);
    CU_ASSERT_EQUAL(node_int->frequencia, 2);
    CU_ASSERT_PTR_NULL(node_int->prox);
    CU_ASSERT_PTR_NULL(node_int->esq);
    CU_ASSERT_PTR_NULL(node_int->dir);

    // Free the memory allocated for the nodes
    free(node_char);
    free(node_int);
}

int main()
{
    // Initialize the CUnit test registry
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    // Add the test suite to the registry
    CU_pSuite test_suite = CU_add_suite("Node test suite", NULL, NULL);
    if (NULL == test_suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add the tests to the test suite
    if ((NULL == CU_add_test(test_suite, "Test criar_no()", test_criar_no))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Run all tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
