#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Estrutura para representar um nó na TREAP
typedef struct Node {
    int key;        // Chave para a propriedade de árvore binária de busca
    int priority;   // Prioridade para a propriedade de heap
    struct Node* left;   // Ponteiro para o nó filho à esquerda
    struct Node* right;  // Ponteiro para o nó filho à direita
} Node;

// Função para criar um novo nó com uma chave e uma prioridade aleatória
Node* newNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->priority = rand() % 100; // Gera prioridade aleatória
    node->left = node->right = NULL;
    return node;
}

// Função para realizar uma rotação à direita
Node* rotateRight(Node* y) {
    Node* x = y->left;
    y->left = x->right;
    x->right = y;
    return x;
}

// Função para realizar uma rotação à esquerda
Node* rotateLeft(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

// Função para inserir um novo nó na TREAP
Node* insert(Node* root, int key) {
    if (root == NULL)
        return newNode(key);

    // Insere na subárvore esquerda ou direita com base na chave
    if (key < root->key) {
        root->left = insert(root->left, key);

        // Verifica a propriedade de heap e realiza rotação, se necessário
        if (root->left->priority > root->priority)
            root = rotateRight(root);
    } else if (key > root->key) {
        root->right = insert(root->right, key);

        // Verifica a propriedade de heap e realiza rotação, se necessário
        if (root->right->priority > root->priority)
            root = rotateLeft(root);
    }
    return root;
}

// Função para deletar um nó da TREAP
Node* deleteNode(Node* root, int key) {
    if (root == NULL)
        return root;

    // Encontra o nó a ser deletado
    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        // Se o nó tiver ambos filhos, realiza rotações até que o nó a ser removido se torne uma folha
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }

        // Rotaciona o nó com o filho com maior prioridade
        if (root->left->priority > root->right->priority) {
            root = rotateRight(root);
            root->right = deleteNode(root->right, key);
        } else {
            root = rotateLeft(root);
            root->left = deleteNode(root->left, key);
        }
    }
    return root;
}

// Função para imprimir a TREAP em um formato visual mais gráfico
void printTreap(Node* root, int space) {
    if (root == NULL) {
        return;
    }

    // Incrementa a distância entre os níveis
    space += 10;

    // Processa primeiro o nó da direita
    printTreap(root->right, space);

    // Imprime o nó atual após o espaço adequado
    printf("\n");
    for (int i = 10; i < space; i++) {
        printf(" ");
    }
    printf("(%d, %d)\n", root->key, root->priority);

    // Processa o nó da esquerda
    printTreap(root->left, space);
}

int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    Node* root = NULL;
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 70);
    root = insert(root, 60);
    root = insert(root, 80);

    printf("Visualização da TREAP:\n");
    printTreap(root, 0);

    printf("\nDeletando 20\n");
    root = deleteNode(root, 20);
    printTreap(root, 0);

    printf("\nDeletando 30\n");
    root = deleteNode(root, 30);
    printTreap(root, 0);

    printf("\nDeletando 50\n");
    root = deleteNode(root, 50);
    printTreap(root, 0);

    return 0;
}
