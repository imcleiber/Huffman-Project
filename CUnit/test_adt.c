#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <CUnit/CUnit.h> // Include the CUnit header file
#include <CUnit/Basic.h> // Include the basic CUnit header file

// Include the header file with the ADTs
#include "adts.h"

void test_criar_no() {
    // Test creating a new node with item and frequency
    char* item = "a";
    int frequency = 1;
    NO* node = criar_no(item, frequency);

    CU_ASSERT_PTR_NOT_NULL(node); // Check if the node was created
    CU_ASSERT_STRING_EQUAL(node->item, item); // Check if the item was set correctly
    CU_ASSERT_EQUAL(node->frequencia, frequency); // Check if the frequency was set correctly 
    CU_ASSERT_PTR_NULL(node->prox); // Check if the next node is NULL
    CU_ASSERT_PTR_NULL(node->esq); // Check if the left node is NULL
    CU_ASSERT_PTR_NULL(node->dir); // Check if the right node is NULL
}

void test_criar_fila() {
    // Test creating a new queue
    FILA* queue = criar_fila();
    CU_ASSERT_PTR_NOT_NULL(queue); //  Check if the queue was created
    CU_ASSERT_PTR_NULL(queue->cabeca); // Check if the queue's head is NULL
}

void test_enfileirar() {
    // Test enqueuing nodes in a queue
    FILA* queue = criar_fila(); // Create a new queue
    NO* node1 = criar_no("a", 1); // Create a new node
    NO* node2 = criar_no("b", 2); // Create a new node
    enfileirar(queue, node1); // Enqueue the first node
    CU_ASSERT_PTR_EQUAL(queue->cabeca, node1); // Check if the first node is the queue's head
    enfileirar(queue, node2); // Enqueue the second node
    CU_ASSERT_PTR_EQUAL(queue->cabeca, node1); // Check if the first node is still the queue's head
    CU_ASSERT_PTR_EQUAL(queue->cabeca->prox, node2); // Check if the second node is the first node's next node
}

void test_desenfileirar() {
    // Test dequeuing nodes from a queue
    FILA* queue = criar_fila();
    NO* node1 = criar_no("a", 1);
    NO* node2 = criar_no("b", 2);
    enfileirar(queue, node1);
    enfileirar(queue, node2);
    NO* dequeued_node = desenfileirar(queue);
    CU_ASSERT_PTR_EQUAL(dequeued_node, node1);
    CU_ASSERT_PTR_EQUAL(queue->cabeca, node2);
    dequeued_node = desenfileirar(queue);
    CU_ASSERT_PTR_EQUAL(dequeued_node, node2);
    CU_ASSERT_PTR_NULL(queue->cabeca);
}

void test_hash_table() {
    // Test creating a hash table
    HT* hash_table = (HT*) malloc(sizeof(HT));
    CU_ASSERT_PTR_NOT_NULL(hash_table);
    for (int i = 0; i < 256; i++) {
        CU_ASSERT_PTR_NULL(hash_table->tabela[i]);
    }
}

int main() {
    // Initialize CUnit test registry
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    // Add tests to suite
    CU_pSuite suite = CU_add_suite("ADT Tests", NULL, NULL);
    CU_add_test(suite, "Test criar_no", test_criar_no);
    CU_add_test(suite, "Test criar_fila", test_criar_fila);
    CU_add_test(suite, "Test enfileirar", test_enfileirar);
    CU_add_test(suite, "Test desenfileirar", test_desenfileirar);
    CU_add_test(suite, "Test hash_table", test_hash_table);

    // Set output mode to verbose
    CU_basic_set_mode(CU_BRM_VERBOSE);

    // Run all tests using the basic interface
    CU_basic_run_tests();

    // Cleanup
    CU_cleanup_registry();

    return CU_get_error();
}
