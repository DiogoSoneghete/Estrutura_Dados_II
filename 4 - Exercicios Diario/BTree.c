#include <stdio.h>
#include <stdlib.h>

#define T 2  // Grau mínimo da árvore B

// Estrutura que representa um nó da árvore B
typedef struct BTreeNode {
    int *keys;            // Array de chaves armazenadas no nó
    int n;                // Número atual de chaves no nó
    struct BTreeNode **C; // Array de ponteiros para filhos do nó
    int leaf;             // Indica se o nó é uma folha (1) ou não (0)
} BTreeNode;

// Função para criar um novo nó da árvore B
BTreeNode* createNode(int leaf) {
    BTreeNode* newNode = (BTreeNode*)malloc(sizeof(BTreeNode)); // Aloca memória para o novo nó
    newNode->leaf = leaf; // Define se é folha ou não
    newNode->keys = (int*)malloc((2 * T - 1) * sizeof(int)); // Aloca espaço para as chaves
    newNode->C = (BTreeNode**)malloc(2 * T * sizeof(BTreeNode*)); // Aloca espaço para ponteiros dos filhos
    newNode->n = 0; // Inicializa o número de chaves como 0
    return newNode; // Retorna o novo nó
}

// Função para percorrer a árvore B em ordem
void traverse(BTreeNode* root) {
    if (root != NULL) { // Verifica se o nó não é nulo
        int i;
        for (i = 0; i < root->n; i++) { // Percorre todas as chaves no nó
            if (!root->leaf) // Se não for uma folha
                traverse(root->C[i]); // Percorre o filho antes de imprimir a chave
            printf("%d ", root->keys[i]); // Imprime a chave
        }
        if (!root->leaf) // Se não for uma folha, percorre o último filho
            traverse(root->C[i]);
    }
}

// Função para buscar uma chave na árvore B
BTreeNode* search(BTreeNode* root, int k) {
    int i = 0; // Inicializa o índice
    // Encontra a posição da chave na lista de chaves
    while (i < root->n && k > root->keys[i])
        i++;
    // Se a chave for encontrada
    if (i < root->n && root->keys[i] == k)
        return root; // Retorna o nó que contém a chave
    // Se o nó é uma folha, a chave não está presente
    if (root->leaf)
        return NULL;
    // Chama recursivamente a função no filho apropriado
    return search(root->C[i], k);
}

// Função para dividir um nó cheio
void splitChild(BTreeNode* parent, int i, BTreeNode* child) {
    BTreeNode* newChild = createNode(child->leaf); // Cria um novo nó para a metade das chaves
    newChild->n = T - 1; // A nova criança terá T-1 chaves

    // Move as últimas T-1 chaves de child para newChild
    for (int j = 0; j < T - 1; j++)
        newChild->keys[j] = child->keys[j + T];

    // Se child não for uma folha, move os filhos também
    if (!child->leaf) {
        for (int j = 0; j < T; j++)
            newChild->C[j] = child->C[j + T];
    }

    child->n = T - 1; // Atualiza o número de chaves de child

    // Move os filhos e chaves de parent para dar espaço para newChild
    for (int j = parent->n; j >= i + 1; j--)
        parent->C[j + 1] = parent->C[j]; // Move os ponteiros dos filhos

    parent->C[i + 1] = newChild; // Insere newChild como filho de parent

    // Move as chaves de parent para dar espaço para a nova chave
    for (int j = parent->n - 1; j >= i; j--)
        parent->keys[j + 1] = parent->keys[j];

    parent->keys[i] = child->keys[T - 1]; // Move a chave do meio para parent
    parent->n += 1; // Incrementa o número de chaves em parent
}

// Função para inserir uma chave em um nó que não está cheio
void insertNonFull(BTreeNode* node, int k) {
    int i = node->n - 1; // Inicializa o índice

    if (node->leaf) { // Se o nó é uma folha
        // Encontra a posição correta para a nova chave
        while (i >= 0 && node->keys[i] > k) {
            node->keys[i + 1] = node->keys[i]; // Move as chaves maiores para a direita
            i--;
        }
        node->keys[i + 1] = k; // Insere a nova chave
        node->n += 1; // Incrementa o número de chaves
    } else { // Se o nó não é uma folha
        // Encontra o filho onde a chave deve ser inserida
        while (i >= 0 && node->keys[i] > k)
            i--;
        i++; // A posição correta do filho

        // Se o filho está cheio, divide-o
        if (node->C[i]->n == 2 * T - 1) {
            splitChild(node, i, node->C[i]); // Divide o filho
            // Após a divisão, decide qual filho usar
            if (node->keys[i] < k)
                i++;
        }
        insertNonFull(node->C[i], k); // Insere a chave no filho correto
    }
}

// Função para inserir uma chave na árvore B
void insert(BTreeNode** root, int k) {
    if (*root == NULL) { // Se a árvore está vazia
        *root = createNode(1); // Cria um novo nó raiz
        (*root)->keys[0] = k; // Insere a chave
        (*root)->n = 1; // Atualiza o número de chaves
    } else {
        // Se a raiz está cheia
        if ((*root)->n == 2 * T - 1) {
            BTreeNode* s = createNode(0); // Cria um novo nó que será a nova raiz
            s->C[0] = *root; // A antiga raiz se torna o primeiro filho da nova raiz
            splitChild(s, 0, *root); // Divide a antiga raiz

            int i = 0;
            // Decide qual filho inserir a nova chave
            if (s->keys[0] < k)
                i++;
            insertNonFull(s->C[i], k); // Insere a chave no filho correto

            *root = s; // Atualiza a raiz
        } else {
            insertNonFull(*root, k); // Insere normalmente na raiz
        }
    }
}

// Função para imprimir a árvore B
void printBTree(BTreeNode* root, int space) {
    if (root != NULL) { // Verifica se o nó não é nulo
        int i;
        space += 10; // Incrementa o espaço para o próximo nível (deslocamento)

        // Imprime as chaves do nó da direita para a esquerda
        for (i = root->n; i >= 0; i--) {
            if (i < root->n) // Se não for o primeiro nó
                printf("%*s%d\n", space, "", root->keys[i]); // Imprime a chave com espaço
            // Se não for uma folha, imprime o filho correspondente
            if (!root->leaf)
                printBTree(root->C[i], space); 
        }
    }
}

// Função principal
int main() {
    BTreeNode* root = NULL; // Inicializa a raiz como nula

    // Insere várias chaves na árvore
    insert(&root, 50);
    insert(&root, 30);
    insert(&root, 20);
    insert(&root, 40);
    insert(&root, 70);
    insert(&root, 60);
    insert(&root, 80);
    insert(&root, 90);
    insert(&root, 55);
    insert(&root, 85);
    insert(&root, 65);
    insert(&root, 75);

    // Exibe a árvore B
    printf("Exibição da árvore B:\n");
    printBTree(root, 0); // Chama a função para imprimir a árvore

    return 0; // Termina o programa
}
