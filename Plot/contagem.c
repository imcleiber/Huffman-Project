#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node {
    int data;
    struct node* left;
    struct node* right;
    int height;
} node;

void insertUnbalanced(node** root, int data) {
    if (*root == NULL) {
        *root = (node*) malloc(sizeof(node));
        (*root)->data = data;
        (*root)->left = NULL;
        (*root)->right = NULL;
    } else if (data < (*root)->data) {
        insertUnbalanced(&((*root)->left), data);
    } else if (data > (*root)->data) {
        insertUnbalanced(&((*root)->right), data);
    }
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(node *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

node *newNode(int data) {
    node *n = (node *)malloc(sizeof(node));
    n->data = data;
    n->left = NULL;
    n->right = NULL;
    n->height = 1;
    return n;
}

node *rightRotate(node *y) {
    node *x = y->left;
    node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

node *leftRotate(node *x) {
    node *y = x->right;
    node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(node *N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

node* insertAVL(node *node, int data) {
    if (node == NULL)
        return (newNode(data));

    if (data < node->data)
        node->left = insertAVL(node->left, data);
    else if (data > node->data)
        node->right = insertAVL(node->right, data);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && data < node->left->data)
        return rightRotate(node);

    if (balance < -1 && data > node->right->data)
        return leftRotate(node);

    if (balance > 1 && data > node->left->data) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && data < node->right->data) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

int search(struct node* root, int data) {
    int count = 1;
    while (root != NULL) {
        if (data == root->data) {
            return count;
        } else if (data < root->data) {
            root = root->left;
        } else {
            root = root->right;
        }
        count++;
    }
    return -1;
}

int main() {
    node *rootAVL = NULL;
    node *rootUnbalanced = NULL;
    int cost = 0;

    srand(time(NULL));

    FILE* file = fopen("contagem.csv", "w");
    fprintf(file, ",N,AD,AVL\n");

    // insere 1000 números aleatórios na árvore AVL
    for (int i = 0; i < 1000; i++) {
        int value = rand() % 10000;

        fprintf(file, "%d,", value);

        fprintf(file, "%d,", i + 1);

        rootAVL = insertAVL(rootAVL, value);
        cost = search(rootAVL, value); // imprime o custo de busca do último número inserido
        fprintf(file, "%d,", cost);
        cost = 0; // reseta o custo de busca

        // insere o mesmo número na árvore desbalanceada
        insertUnbalanced(&rootUnbalanced, value);
        cost = search(rootUnbalanced, value); // imprime o custo de busca do último número inserido
        fprintf(file, "%d\n", cost);

        // reseta o custo de busca
        cost = 0;
    }

    return 0;
}