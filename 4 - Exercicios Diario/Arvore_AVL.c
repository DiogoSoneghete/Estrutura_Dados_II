// Diogo Soneghete de Almeida
#include <stdio.h>
#include <stdlib.h>

// Estrutura de nó da árvore AVL
// Define a estrutura de um nó de uma árvore AVL, que contém um valor inteiro, 
// ponteiros para os filhos à esquerda e à direita, e a altura do nó.
struct NoAVL {
    int valor;            // Valor armazenado no nó
    struct NoAVL *esq;    // Ponteiro para o nó filho à esquerda
    struct NoAVL *dir;    // Ponteiro para o nó filho à direita
    int altura;           // Altura do nó, usada para balanceamento
};

// Função para criar um novo nó
// Aloca memória e inicializa um novo nó com o valor fornecido. 
// A altura do nó é inicializada como 1, já que é um nó folha.
struct NoAVL* criarNovoNo(int valor) {
    struct NoAVL* no = (struct NoAVL*) malloc(sizeof(struct NoAVL)); // Aloca memória para o novo nó
    no->valor = valor;    // Define o valor do nó
    no->esq = NULL;       // Inicializa o filho esquerdo como NULL
    no->dir = NULL;       // Inicializa o filho direito como NULL
    no->altura = 1;       // Altura inicial do nó é 1
    return no;            // Retorna o novo nó criado
}

// Função para calcular a altura de um nó
// Retorna a altura do nó. Se o nó for NULL, a altura é 0.
int obterAltura(struct NoAVL* no) {
    if (no == NULL)       // Se o nó é NULL, altura é 0
        return 0;
    return no->altura;     // Caso contrário, retorna a altura armazenada
}

// Função para obter o maior valor entre dois números
// Retorna o maior valor entre 'a' e 'b'.
int max(int a, int b) {
    return (a > b) ? a : b; // Retorna 'a' se for maior que 'b', caso contrário, retorna 'b'
}

// Função para realizar uma rotação à direita
// Corrige o balanceamento da árvore realizando uma rotação à direita em um nó desbalanceado.
struct NoAVL* rotacaoDireita(struct NoAVL* y) {
    struct NoAVL* x = y->esq;    // Nó 'x' será o filho à esquerda de 'y'
    struct NoAVL* T2 = x->dir;   // 'T2' será o filho à direita de 'x'

    // Rotaciona os nós: 'x' torna-se a nova raiz, e 'y' é movido para a direita
    x->dir = y;
    y->esq = T2;                // 'T2' torna-se o filho à esquerda de 'y'

    // Atualiza as alturas dos nós após a rotação
    y->altura = max(obterAltura(y->esq), obterAltura(y->dir)) + 1;
    x->altura = max(obterAltura(x->esq), obterAltura(x->dir)) + 1;

    return x;                   // Retorna a nova raiz após a rotação
}

// Função para realizar uma rotação à esquerda
// Corrige o balanceamento da árvore realizando uma rotação à esquerda em um nó desbalanceado.
struct NoAVL* rotacaoEsquerda(struct NoAVL* x) {
    struct NoAVL* y = x->dir;    // Nó 'y' será o filho à direita de 'x'
    struct NoAVL* T2 = y->esq;   // 'T2' será o filho à esquerda de 'y'

    // Rotaciona os nós: 'y' torna-se a nova raiz, e 'x' é movido para a esquerda
    y->esq = x;
    x->dir = T2;                // 'T2' torna-se o filho à direita de 'x'

    // Atualiza as alturas dos nós após a rotação
    x->altura = max(obterAltura(x->esq), obterAltura(x->dir)) + 1;
    y->altura = max(obterAltura(y->esq), obterAltura(y->dir)) + 1;

    return y;                   // Retorna a nova raiz após a rotação
}

// Função para obter o fator de balanceamento de um nó
// Calcula o fator de balanceamento de um nó subtraindo a altura da subárvore direita da subárvore esquerda.
int obterFatorBalanceamento(struct NoAVL* no) {
    if (no == NULL)
        return 0;   // Se o nó for NULL, o fator de balanceamento é 0
    return obterAltura(no->esq) - obterAltura(no->dir);  // Subtrai a altura do filho direito da altura do filho esquerdo
}

// Função para inserir um valor na árvore AVL
// Insere um novo valor na árvore e ajusta o balanceamento da árvore, se necessário.
struct NoAVL* inserirNo(struct NoAVL* no, int valor) {
    if (no == NULL)              // Se o nó é NULL, cria um novo nó
        return criarNovoNo(valor);

    // Decide para onde inserir o novo valor: à esquerda ou à direita
    if (valor < no->valor)
        no->esq = inserirNo(no->esq, valor);  // Insere à esquerda se o valor for menor
    else if (valor > no->valor)
        no->dir = inserirNo(no->dir, valor);  // Insere à direita se o valor for maior
    else
        return no;  // Se o valor já existe, não faz nada e retorna o nó

    // Atualiza a altura do nó atual
    no->altura = 1 + max(obterAltura(no->esq), obterAltura(no->dir));

    // Obtém o fator de balanceamento para verificar se a árvore está desbalanceada
    int balance = obterFatorBalanceamento(no);

    // Rotação à direita se estiver desbalanceado para a esquerda
    if (balance > 1 && valor < no->esq->valor)
        return rotacaoDireita(no);

    // Rotação à esquerda se estiver desbalanceado para a direita
    if (balance < -1 && valor > no->dir->valor)
        return rotacaoEsquerda(no);

    // Rotação dupla: esquerda-direita
    if (balance > 1 && valor > no->esq->valor) {
        no->esq = rotacaoEsquerda(no->esq);
        return rotacaoDireita(no);
    }

    // Rotação dupla: direita-esquerda
    if (balance < -1 && valor < no->dir->valor) {
        no->dir = rotacaoDireita(no->dir);
        return rotacaoEsquerda(no);
    }

    return no;  // Retorna o nó (possivelmente ajustado)
}

// Função para encontrar o nó com o menor valor
// Navega pela árvore até encontrar o nó mais à esquerda, que contém o menor valor.
struct NoAVL* menorValorNo(struct NoAVL* no) {
    struct NoAVL* atual = no;

    // Percorre a árvore à esquerda até encontrar o nó com o menor valor
    while (atual->esq != NULL)
        atual = atual->esq;

    return atual;  // Retorna o nó com o menor valor
}

// Função para remover um nó da árvore AVL
// Remove um valor da árvore e ajusta o balanceamento após a remoção, se necessário.
struct NoAVL* removerNo(struct NoAVL* raiz, int valor) {
    if (raiz == NULL)
        return raiz;   // Se a raiz é NULL, não há o que remover

    // Procura o nó a ser removido
    if (valor < raiz->valor)
        raiz->esq = removerNo(raiz->esq, valor);   // Procura à esquerda se o valor for menor
    else if (valor > raiz->valor)
        raiz->dir = removerNo(raiz->dir, valor);   // Procura à direita se o valor for maior
    else {
        // Caso o nó tenha apenas um filho ou nenhum
        if ((raiz->esq == NULL) || (raiz->dir == NULL)) {
            struct NoAVL* temp = raiz->esq ? raiz->esq : raiz->dir;  // Escolhe o filho não nulo

            if (temp == NULL) {    // Caso não tenha filhos, apenas remove o nó
                temp = raiz;
                raiz = NULL;
            } else
                *raiz = *temp;    // Copia o conteúdo do filho para o nó atual

            free(temp);  // Libera a memória do nó removido
        } else {
            // Caso o nó tenha dois filhos, encontra o sucessor (menor valor da subárvore direita)
            struct NoAVL* temp = menorValorNo(raiz->dir);

            // Substitui o valor do nó a ser removido pelo valor do sucessor
            raiz->valor = temp->valor;

            // Remove o sucessor da subárvore direita
            raiz->dir = removerNo(raiz->dir, temp->valor);
        }
    }

    if (raiz == NULL)
        return raiz;  // Retorna NULL se a árvore ficou vazia após a remoção

    // Atualiza a altura do nó atual
    raiz->altura = 1 + max(obterAltura(raiz->esq), obterAltura(raiz->dir));

    // Obtém o fator de balanceamento para ajustar a árvore se necessário
    int balance = obterFatorBalanceamento(raiz);

    // Rotação à direita se estiver desbalanceado para a esquerda
    if (balance > 1 && obterFatorBalanceamento(raiz->esq) >= 0)
        return rotacaoDireita(raiz);

    // Rotação dupla: esquerda-direita
    if (balance > 1 && obterFatorBalanceamento(raiz->esq) < 0) {
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
    }

    // Rotação à esquerda se estiver desbalanceado para a direita
    if (balance < -1 && obterFatorBalanceamento(raiz->dir) <= 0)
        return rotacaoEsquerda(raiz);

    // Rotação dupla: direita-esquerda
    if (balance < -1 && obterFatorBalanceamento(raiz->dir) > 0) {
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
    }

    return raiz;  // Retorna o nó ajustado após a remoção
}

// Função para imprimir a árvore AVL em ordem
// Percorre e imprime a árvore AVL em ordem crescente.
void imprimirEmOrdem(struct NoAVL* raiz) {
    if (raiz != NULL) {
        imprimirEmOrdem(raiz->esq);    // Visita a subárvore esquerda
        printf("%d ", raiz->valor);    // Imprime o valor do nó
        imprimirEmOrdem(raiz->dir);    // Visita a subárvore direita
    }
}

// Função principal
int main() {
    struct NoAVL* raiz = NULL;    // Inicializa a árvore como vazia

    // Insere valores na árvore
    raiz = inserirNo(raiz, 20);
    raiz = inserirNo(raiz, 10);
    raiz = inserirNo(raiz, 30);
    raiz = inserirNo(raiz, 5);
    raiz = inserirNo(raiz, 15);
    raiz = inserirNo(raiz, 25);
    raiz = inserirNo(raiz, 35);
    raiz = removerNo(raiz, 10);   // Remove o valor 10 da árvore

    // Imprime a árvore em ordem crescente
    printf("Árvore AVL em ordem:\n");
    imprimirEmOrdem(raiz);
    printf("\n");

    return 0;
}
