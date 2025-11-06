#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para um cômodo da mansão (Árvore Binária)
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura para uma pista na BST
typedef struct Pista {
    char descricao[100];
    struct Pista *esquerda;
    struct Pista *direita;
} Pista;

// Estrutura para um item na tabela hash
typedef struct HashItem {
    char pista[100];
    char suspeito[50];
    struct HashItem *proximo;
} HashItem;

// Estrutura para contagem de suspeitos
typedef struct SuspeitoContagem {
    char nome[50];
    int count;
} SuspeitoContagem;

// Tamanho da tabela hash
#define TAMANHO_HASH 10

// Variáveis globais
Pista *arvorePistas = NULL;
HashItem *tabelaHash[TAMANHO_HASH];
SuspeitoContagem suspeitos[5];
int numSuspeitos = 0;

/**
 * cria dinamicamente um cômodo da mansão
 * @param nome Nome do cômodo
 * @param pista Pista encontrada no cômodo
 * @param esq Ponteiro para cômodo à esquerda
 * @param dir Ponteiro para cômodo à direita
 * @return Ponteiro para o novo cômodo criado
 */
Sala* criarSala(const char *nome, const char *pista, Sala *esq, Sala *dir) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = esq;
    novaSala->direita = dir;
    return novaSala;
}

/**
 * Insere uma pista na árvore BST de forma ordenada
 * @param raiz Raiz da árvore de pistas
 * @param pista Descrição da pista a ser inserida
 * @return Ponteiro para a raiz da árvore atualizada
 */
Pista* inserirPista(Pista *raiz, const char *pista) {
    if (raiz == NULL) {
        Pista *novaPista = (Pista*)malloc(sizeof(Pista));
        strcpy(novaPista->descricao, pista);
        novaPista->esquerda = NULL;
        novaPista->direita = NULL;
        return novaPista;
    }
    
    int comparacao = strcmp(pista, raiz->descricao);
    if (comparacao < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (comparacao > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    
    return raiz;
}

/**
 * Função hash simples baseada na soma dos caracteres da pista
 * @param pista Pista para calcular o hash
 * @return Índice na tabela hash
 */
int funcaoHash(const char *pista) {
    int soma = 0;
    for (int i = 0; pista[i] != '\0'; i++) {
        soma += pista[i];
    }
    return soma % TAMANHO_HASH;
}

/**
 * Insere associação pista/suspeito na tabela hash
 * @param pista Pista encontrada
 * @param suspeito Suspeito associado à pista
 */
void inserirNaHash(const char *pista, const char *suspeito) {
    int indice = funcaoHash(pista);
    
    HashItem *novoItem = (HashItem*)malloc(sizeof(HashItem));
    strcpy(novoItem->pista, pista);
    strcpy(novoItem->suspeito, suspeito);
    novoItem->proximo = NULL;
    
    // Inserção no início da lista encadeada (para lidar com colisões)
    if (tabelaHash[indice] == NULL) {
        tabelaHash[indice] = novoItem;
    } else {
        novoItem->proximo = tabelaHash[indice];
        tabelaHash[indice] = novoItem;
    }
}

/**
 * Consulta o suspeito correspondente a uma pista na tabela hash
 * @param pista Pista a ser consultada
 * @return Nome do suspeito ou "Desconhecido" se não encontrado
 */
const char* encontrarSuspeito(const char *pista) {
    int indice = funcaoHash(pista);
    HashItem *atual = tabelaHash[indice];
    
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    
    return "Desconhecido";
}

/**
 * Adiciona ou atualiza a contagem de pistas para um suspeito
 * @param suspeito Nome do suspeito
 */
void adicionarContagemSuspeito(const char *suspeito) {
    for (int i = 0; i < numSuspeitos; i++) {
        if (strcmp(suspeitos[i].nome, suspeito) == 0) {
            suspeitos[i].count++;
            return;
        }
    }
    
    // Novo suspeito
    if (numSuspeitos < 5) {
        strcpy(suspeitos[numSuspeitos].nome, suspeito);
        suspeitos[numSuspeitos].count = 1;
        numSuspeitos++;
    }
}

/**
 * Navega pela árvore e ativa o sistema de pistas
 * @param raiz Raiz da árvore de cômodos
 */
void explorarSalas(Sala *raiz) {
    Sala *atual = raiz;
    char comando;
    
    printf("\n=== EXPLORANDO A MANSÃO ===\n");
    
    while (atual != NULL) {
        printf("\nVocê está no: %s\n", atual->nome);
        
        if (strlen(atual->pista) > 0) {
            printf("💡 Pista encontrada: %s\n", atual->pista);
            
            // Adiciona pista à BST
            arvorePistas = inserirPista(arvorePistas, atual->pista);
            
            // Encontra suspeito associado e atualiza contagem
            const char *suspeito = encontrarSuspeito(atual->pista);
            adicionarContagemSuspeito(suspeito);
            
            printf("🔍 Esta pista aponta para: %s\n", suspeito);
        }
        
        // Opções de navegação
        if (atual->esquerda != NULL && atual->direita != NULL) {
            printf("Opções: [e] Esquerda (%s) | [d] Direita (%s) | [s] Sair\n", 
                   atual->esquerda->nome, atual->direita->nome);
        } else if (atual->esquerda != NULL) {
            printf("Opções: [e] Esquerda (%s) | [s] Sair\n", atual->esquerda->nome);
        } else if (atual->direita != NULL) {
            printf("Opções: [d] Direita (%s) | [s] Sair\n", atual->direita->nome);
        } else {
            printf("🚪 Fim do caminho! [s] Sair\n");
        }
        
        printf("Comando: ");
        scanf(" %c", &comando);
        
        if (comando == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if (comando == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } else if (comando == 's') {
            printf("\nSaindo da exploração...\n");
            break;
        } else {
            printf("Comando inválido ou direção indisponível!\n");
        }
    }
}

/**
 * Exibe todas as pistas coletadas em ordem (percurso in-order na BST)
 * @param raiz Raiz da árvore de pistas
 */
void exibirPistasEmOrdem(Pista *raiz) {
    if (raiz != NULL) {
        exibirPistasEmOrdem(raiz->esquerda);
        printf("🔎 %s → Suspeito: %s\n", raiz->descricao, encontrarSuspeito(raiz->descricao));
        exibirPistasEmOrdem(raiz->direita);
    }
}

/**
 * Conduz à fase de julgamento final e verifica a acusação
 */
void verificarSuspeitoFinal() {
    printf("\n=== FASE FINAL - ACUSAÇÃO ===\n");
    
    if (arvorePistas == NULL) {
        printf("❌ Nenhuma pista foi coletada! Não é possível fazer uma acusação.\n");
        return;
    }
    
    // Exibir pistas coletadas
    printf("\n📋 PISTAS COLETADAS (em ordem alfabética):\n");
    exibirPistasEmOrdem(arvorePistas);
    
    // Exibir contagem de suspeitos
    printf("\n📊 SUSPEITOS E PISTAS ASSOCIADAS:\n");
    for (int i = 0; i < numSuspeitos; i++) {
        printf("👤 %s: %d pista(s)\n", suspeitos[i].nome, suspeitos[i].count);
    }
    
    // Solicitar acusação
    char acusado[50];
    printf("\n🔍 Quem é o culpado? Digite o nome do suspeito: ");
    scanf(" %[^\n]", acusado);
    
    // Verificar se há pelo menos 2 pistas para o acusado
    int pistasParaAcusado = 0;
    for (int i = 0; i < numSuspeitos; i++) {
        if (strcmp(suspeitos[i].nome, acusado) == 0) {
            pistasParaAcusado = suspeitos[i].count;
            break;
        }
    }
    
    printf("\n=== RESULTADO FINAL ===\n");
    if (pistasParaAcusado >= 2) {
        printf("🎯 PARABÉNS! A acusação contra %s está sustentada!\n", acusado);
        printf("🔍 Foram encontradas %d pistas ligadas a este suspeito.\n", pistasParaAcusado);
        printf("🏆 A investigação foi concluída com sucesso!\n");
    } else {
        printf("❌ ACUSAÇÃO INSUFICIENTE! Não há pistas suficientes contra %s.\n", acusado);
        printf("💡 São necessárias pelo menos 2 pistas para sustentar uma acusação.\n");
        printf("📌 Pistas encontradas para %s: %d\n", acusado, pistasParaAcusado);
    }
}

/**
 * Inicializa a tabela hash com valores NULL
 */
void inicializarTabelaHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        tabelaHash[i] = NULL;
    }
}

/**
 * Inicializa as associações pista-suspeito na tabela hash
 */
void inicializarAssociacoesPistas() {
    // Associações predefinidas entre pistas e suspeitos
    inserirNaHash("Carta de amor assinada com 'J'", "João");
    inserirNaHash("Manchas de tinta vermelha no tapete", "Maria");
    inserirNaHash("Relógio de ouro quebrado", "Carlos");
    inserirNaHash("Pêlo de cachorro preto", "Ana");
    inserirNaHash("Cheiro forte de perfume", "Pedro");
    inserirNaHash("Livro com páginas rasgadas", "João");
    inserirNaHash("Cigarro half-smoked", "Maria");
    inserirNaHash("Pegadas de barro", "Carlos");
    inserirNaHash("Fio de cabelo loiro", "Ana");
    inserirNaHash("Nota fiscal de joias", "Pedro");
}

/**
 * Libera a memória da árvore de pistas
 */
void liberarArvorePistas(Pista *raiz) {
    if (raiz != NULL) {
        liberarArvorePistas(raiz->esquerda);
        liberarArvorePistas(raiz->direita);
        free(raiz);
    }
}

/**
 * Libera a memória da tabela hash
 */
void liberarTabelaHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        HashItem *atual = tabelaHash[i];
        while (atual != NULL) {
            HashItem *temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
}

/**
 * Função principal - Monta a mansão e inicia o jogo
 */
int main() {
    printf("🕵️  DETECTIVE QUEST - O MISTÉRIO DA MANSÃO 🕵️\n");
    printf("=============================================\n");
    
    // Inicializações
    inicializarTabelaHash();
    inicializarAssociacoesPistas();
    
    // Montagem manual da mansão (árvore binária fixa)
    Sala *sala11 = criarSala("Jardim de Inverno", "Pêlo de cachorro preto", NULL, NULL);
    Sala *sala10 = criarSala("Varanda", "Cigarro half-smoked", NULL, NULL);
    Sala *sala9 = criarSala("Porão", "Pegadas de barro", NULL, NULL);
    Sala *sala8 = criarSala("Área de Serviço", "Fio de cabelo loiro", NULL, NULL);
    Sala *sala7 = criarSala("Quarto de Hóspedes", "Nota fiscal de joias", NULL, NULL);
    Sala *sala6 = criarSala("Biblioteca", "Livro com páginas rasgadas", NULL, NULL);
    Sala *sala5 = criarSala("Escritório", "Cheiro forte de perfume", sala10, sala11);
    Sala *sala4 = criarSala("Quarto Principal", "Relógio de ouro quebrado", sala8, sala9);
    Sala *sala3 = criarSala("Sala de Jantar", "Manchas de tinta vermelha no tapete", sala6, sala7);
    Sala *sala2 = criarSala("Sala de Estar", "Carta de amor assinada com 'J'", sala4, sala5);
    Sala *sala1 = criarSala("Hall de Entrada", "", sala2, sala3);
    
    // Exploração interativa
    explorarSalas(sala1);
    
    // Fase final - Acusação
    verificarSuspeitoFinal();
    
    // Liberação de memória
    liberarArvorePistas(arvorePistas);
    liberarTabelaHash();
    
    // Liberar árvore de salas (simplificado)
    free(sala1); free(sala2); free(sala3); free(sala4); free(sala5);
    free(sala6); free(sala7); free(sala8); free(sala9); free(sala10); free(sala11);
    
    printf("\n🎭 Obrigado por jogar Detective Quest! 🎭\n");
    
    return 0;

}
