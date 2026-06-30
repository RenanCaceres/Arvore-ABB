/*
 * ============================================================
 * UTFPR - Universidade Tecnológica Federal do Paraná
 * Curso: Tecnologia em Análise e Desenvolvimento de Sistemas
 * Disciplina: Estrutura de Dados 2026-01
 * Professor: Clayton Kossoski
 *
 * Atividade Avaliativa 3
 * Tema: Sistema de Cadastro de Alunos com Árvore Binária de Busca (ABB)
 *
 * Aluno 1: Renan Cáceres Anselmo     | RA: 2525526
 * Aluno 2: Pedro Lucas Sales Larini  | RA: 2767147
 */

#include <iostream>
#include <string>
#include <algorithm>   // usado em minusculo() para comparar nomes sem case-sensitive

using namespace std;

// ============================================================
// 1. ESTRUTURAS DE DADOS
// ============================================================

/*
 * Struct Aluno: dados de cada aluno cadastrado.
 * A MATRÍCULA é usada como chave de ordenação/busca na árvore,
 * pois é um valor numérico único — exatamente como o professor
 * usava o "info" (int) no código base.
 */
struct Aluno {
    int    matricula;
    string nome;
    string curso;
};

// Definição do nó da árvore (mesmo nome usado pelo professor: NO)
struct NO {
    Aluno aluno;
    NO*   esq;
    NO*   dir;
};

// ArvBin é um ponteiro para NO (igual ao código do professor)
typedef NO* ArvBin;

// ============================================================
// 2. CRIAÇÃO E LIBERAÇÃO DA ÁRVORE (iguais ao código do professor)
// ============================================================

// Cria uma árvore vazia
ArvBin* cria_ArvBin() {
    ArvBin* raiz = new ArvBin;
    if (raiz != nullptr)
        *raiz = nullptr;
    return raiz;
}

// Libera todos os nós da árvore (percurso pós-ordem)
void libera_NO(NO* no) {
    if (no == nullptr)
        return;
    libera_NO(no->esq);
    libera_NO(no->dir);
    delete no;
}

// Libera a estrutura da árvore (nós + ponteiro da raiz)
void libera_ArvBin(ArvBin* raiz) {
    if (raiz == nullptr)
        return;
    libera_NO(*raiz);  // libera todos os nós
    delete raiz;       // libera o ponteiro da raiz
}

// ============================================================
// 3. FUNÇÃO AUXILIAR DE COMPARAÇÃO DE NOMES
// ============================================================

/*
 * Converte string para minúsculas, usada apenas na BUSCA POR NOME
 * (funcionalidade extra pedida no enunciado: "localizar um aluno
 * a partir do seu nome"). A árvore continua ordenada por matrícula,
 * então a busca por nome percorre a árvore inteira comparando nomes.
 */
string minusculo(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

// ============================================================
// 4. INSERÇÃO — COM A CORREÇÃO DE DUPLICATAS
// ============================================================

/*
 * Insere um novo aluno na árvore, mantendo a ordenação pela matrícula.
 *
 * >>> CORREÇÃO PRINCIPAL <<<
 * Antes de criar o nó, percorremos a árvore procurando se já existe
 * um aluno com a MESMA MATRÍCULA. Se existir, a função:
 *   - exibe um aviso ao usuário,
 *   - NÃO cria o nó (evita alocar memória à toa),
 *   - retorna false (indicando que a inserção falhou).
 *
 * Isso resolve o problema relatado: matrícula e nome repetidos
 * não entram mais na árvore, e o terminal não fica mais "maluco"
 * com nós duplicados.
 */
bool insere_ArvBin(ArvBin* raiz, Aluno aluno) {
    if (raiz == nullptr)
        return false;

    // --- Verificação de duplicidade ANTES de criar o nó ---
    NO* atual = *raiz;
    while (atual != nullptr) {
        if (aluno.matricula == atual->aluno.matricula) {
            cout << "[ERRO] Matricula " << aluno.matricula
                 << " ja cadastrada para \"" << atual->aluno.nome << "\"!\n";
            return false; // cancela a inserção
        }
        if (minusculo(aluno.nome) == minusculo(atual->aluno.nome)) {
            cout << "[ERRO] Ja existe um aluno chamado \"" << aluno.nome << "\"!\n";
            return false; // cancela a inserção
        }

        // Continua descendo na árvore (mesma lógica do professor)
        if (aluno.matricula < atual->aluno.matricula) {
            if (atual->esq == nullptr) break; // achou o lugar, sai do while
            atual = atual->esq;
        } else {
            if (atual->dir == nullptr) break; // achou o lugar, sai do while
            atual = atual->dir;
        }
    }

    // --- Só chega aqui se NÃO houver duplicata: cria o novo nó ---
    NO* novo = new NO;
    novo->aluno = aluno;
    novo->esq   = nullptr;
    novo->dir   = nullptr;

    // Árvore vazia: o novo nó vira a raiz
    if (*raiz == nullptr) {
        *raiz = novo;
        return true;
    }

    // Desce novamente até o ponto de inserção correto
    atual = *raiz;
    while (true) {
        if (aluno.matricula < atual->aluno.matricula) {
            if (atual->esq == nullptr) {
                atual->esq = novo;
                return true;
            }
            atual = atual->esq;
        } else {
            if (atual->dir == nullptr) {
                atual->dir = novo;
                return true;
            }
            atual = atual->dir;
        }
    }
}

// ============================================================
// 5. BUSCA POR MATRÍCULA
// ============================================================

/*
 * Busca um aluno pela matrícula, percorrendo a árvore como uma
 * ABB tradicional (esquerda se menor, direita se maior).
 * Retorna o ponteiro do nó encontrado, ou nullptr se não existir.
 */
NO* busca_por_matricula(ArvBin* raiz, int matricula) {
    if (raiz == nullptr)
        return nullptr;

    NO* atual = *raiz;
    while (atual != nullptr) {
        if (matricula == atual->aluno.matricula)
            return atual; // encontrou!
        else if (matricula < atual->aluno.matricula)
            atual = atual->esq;
        else
            atual = atual->dir;
    }
    return nullptr; // não encontrado
}

// ============================================================
// 6. BUSCA POR NOME
// ============================================================

/*
 * Como a árvore está ordenada por MATRÍCULA (não por nome),
 * a busca por nome precisa visitar todos os nós (não há atalho).
 * Implementada recursivamente, percorrendo a árvore inteira.
 */
NO* busca_por_nome(NO* no, const string& nome) {
    if (no == nullptr)
        return nullptr;

    if (minusculo(no->aluno.nome) == minusculo(nome))
        return no; // encontrou!

    // Procura primeiro na esquerda; se não achar, procura na direita
    NO* achadoEsq = busca_por_nome(no->esq, nome);
    if (achadoEsq != nullptr)
        return achadoEsq;

    return busca_por_nome(no->dir, nome);
}

// ============================================================
// 7. PERCURSO EM ORDEM (listagem)
// ============================================================

/*
 * Percurso em ordem (esquerda -> raiz -> direita).
 * Como a árvore é ordenada por MATRÍCULA, este percurso lista
 * os alunos em ordem CRESCENTE DE MATRÍCULA.
 */
void emOrdem_porMatricula(NO* no) {
    if (no == nullptr)
        return;
    emOrdem_porMatricula(no->esq);
    cout << "Matricula: " << no->aluno.matricula
         << " - Nome: "   << no->aluno.nome
         << " - Curso: "  << no->aluno.curso << "\n";
    emOrdem_porMatricula(no->dir);
}

/*
 * O enunciado pede a listagem em ORDEM ALFABÉTICA DE NOME.
 * Como a árvore é ordenada por matrícula, não dá pra obter os
 * nomes em ordem só com o percurso em ordem da árvore.
 * Solução simples (sem usar vetor + sort pronto): construímos
 * uma pequena lista encadeada com os alunos e ordenamos essa
 * lista por nome usando inserção ordenada — sem usar nenhum
 * algoritmo de ordenação pronto da biblioteca padrão.
 */
struct ItemLista {
    Aluno aluno;
    ItemLista* prox;
};

// Insere um aluno na lista encadeada, já na posição correta (ordem alfabética)
void inserirOrdenado(ItemLista*& inicio, Aluno aluno) {
    ItemLista* novo = new ItemLista{aluno, nullptr};

    // Lista vazia ou nome menor que o primeiro: insere na frente
    if (inicio == nullptr || minusculo(aluno.nome) < minusculo(inicio->aluno.nome)) {
        novo->prox = inicio;
        inicio = novo;
        return;
    }

    // Percorre até achar a posição correta
    ItemLista* atual = inicio;
    while (atual->prox != nullptr && minusculo(atual->prox->aluno.nome) < minusculo(aluno.nome)) {
        atual = atual->prox;
    }
    novo->prox = atual->prox;
    atual->prox = novo;
}

// Percorre a árvore (qualquer ordem) e vai inserindo cada aluno na lista ordenada por nome
void coletarParaListaOrdenada(NO* no, ItemLista*& lista) {
    if (no == nullptr) return;
    inserirOrdenado(lista, no->aluno);
    coletarParaListaOrdenada(no->esq, lista);
    coletarParaListaOrdenada(no->dir, lista);
}

// Exibe a lista encadeada (já ordenada por nome) e libera a memória dela
void exibirEliberarLista(ItemLista* lista) {
    while (lista != nullptr) {
        cout << lista->aluno.nome
             << " - Matricula: " << lista->aluno.matricula
             << " - Curso: "     << lista->aluno.curso << "\n";
        ItemLista* temp = lista;
        lista = lista->prox;
        delete temp;
    }
}

// Função "wrapper" que junta os passos acima: monta a lista e exibe
void listarEmOrdemAlfabetica(ArvBin* raiz) {
    if (raiz == nullptr || *raiz == nullptr) {
        cout << "Nenhum aluno cadastrado.\n";
        return;
    }
    ItemLista* lista = nullptr;
    coletarParaListaOrdenada(*raiz, lista);
    exibirEliberarLista(lista);
}

// ============================================================
// 8. INFORMAÇÕES DA ÁRVORE
// ============================================================

// Conta quantos alunos (nós) existem na árvore
int contarAlunos(NO* no) {
    if (no == nullptr)
        return 0;
    return 1 + contarAlunos(no->esq) + contarAlunos(no->dir);
}

// Calcula a altura da árvore (maior caminho raiz -> folha)
int altura(NO* no) {
    if (no == nullptr)
        return 0;
    int altEsq = altura(no->esq);
    int altDir = altura(no->dir);
    return 1 + (altEsq > altDir ? altEsq : altDir);
}

// ============================================================
// 9. INTERFACE DE MENU
// ============================================================

void exibirMenu() {
    cout << "\n============================================\n";
    cout << "    CADASTRO DE ALUNOS - Arvore Binaria\n";
    cout << "============================================\n";
    cout << "  1. Inserir aluno\n";
    cout << "  2. Buscar aluno por matricula\n";
    cout << "  3. Buscar aluno por nome\n";
    cout << "  4. Listar alunos em ordem alfabetica (por nome)\n";
    cout << "  5. Listar alunos em ordem de matricula\n";
    cout << "  6. Informacoes da arvore\n";
    cout << "  0. Sair\n";
    cout << "============================================\n";
    cout << "  Opcao: ";
}

// Lê uma linha completa (aceita espaços). 'limparBuffer' descarta
// o Enter deixado no buffer depois de um cin >> anterior.
string lerLinha(const string& mensagem, bool limparBuffer = false) {
    string valor;
    cout << mensagem;
    if (limparBuffer) cin.ignore(1000, '\n');
    getline(cin, valor);
    return valor;
}

// ============================================================
// 10. FUNÇÃO PRINCIPAL
// ============================================================

int main() {
    // Cria a árvore (igual ao código do professor)
    ArvBin* raiz = cria_ArvBin();
    int opcao;

    // --- Carrega os alunos de exemplo do enunciado ---
    cout << "\n[Carregando alunos de exemplo...]\n";
    insere_ArvBin(raiz, {101, "John Smith",    "Engenharia de Software"});
    insere_ArvBin(raiz, {102, "Alice Johnson", "Sistemas de Informacao"});
    insere_ArvBin(raiz, {103, "Michael Brown", "Ciencia da Computacao"});
    insere_ArvBin(raiz, {104, "Emily Davis",   "Engenharia de Software"});
    cout << "[4 alunos carregados!]\n";

    // Teste de duplicata (demonstra a correção em ação)
    cout << "\n[Testando insercao de matricula duplicada (101)...]\n";
    insere_ArvBin(raiz, {101, "Pedro Duplicado", "Teste"});

    do {
        exibirMenu();
        cin >> opcao;

        switch (opcao) {

            // ---- Inserir ----
            case 1: {
                Aluno novo;
                novo.matricula = 0;
                cout << "  Matricula : ";
                cin  >> novo.matricula;
                novo.nome  = lerLinha("  Nome     : ", true);
                novo.curso = lerLinha("  Curso    : ");

                if (insere_ArvBin(raiz, novo))
                    cout << "  [OK] \"" << novo.nome << "\" inserido!\n";
                // Se falhar, a própria função insere_ArvBin já mostrou o motivo
                break;
            }

            // ---- Buscar por matrícula ----
            case 2: {
                int mat;
                cout << "  Matricula a buscar: ";
                cin >> mat;
                NO* resultado = busca_por_matricula(raiz, mat);
                if (resultado != nullptr) {
                    cout << "\n  [ENCONTRADO]\n";
                    cout << "  Matricula : " << resultado->aluno.matricula << "\n";
                    cout << "  Nome      : " << resultado->aluno.nome      << "\n";
                    cout << "  Curso     : " << resultado->aluno.curso     << "\n";
                } else {
                    cout << "  [NAO ENCONTRADO] Nenhum aluno com essa matricula.\n";
                }
                break;
            }

            // ---- Buscar por nome ----
            case 3: {
                string nome = lerLinha("  Nome a buscar: ", true);
                NO* resultado = busca_por_nome(*raiz, nome);
                if (resultado != nullptr) {
                    cout << "\n  [ENCONTRADO]\n";
                    cout << "  Nome      : " << resultado->aluno.nome      << "\n";
                    cout << "  Matricula : " << resultado->aluno.matricula << "\n";
                    cout << "  Curso     : " << resultado->aluno.curso     << "\n";
                } else {
                    cout << "  [NAO ENCONTRADO] Nenhum aluno com esse nome.\n";
                }
                break;
            }

            // ---- Listar em ordem alfabética (nome) ----
            case 4: {
                cout << "\n  --- Alunos em Ordem Alfabetica (por nome) ---\n";
                listarEmOrdemAlfabetica(raiz);
                cout << "  ----------------------------------------------\n";
                break;
            }

            // ---- Listar em ordem de matrícula ----
            case 5: {
                cout << "\n  --- Alunos em Ordem de Matricula ---\n";
                if (raiz == nullptr || *raiz == nullptr)
                    cout << "  Nenhum aluno cadastrado.\n";
                else
                    emOrdem_porMatricula(*raiz);
                cout << "  -------------------------------------\n";
                break;
            }

            // ---- Informações da árvore ----
            case 6: {
                NO* topo = (raiz != nullptr) ? *raiz : nullptr;
                cout << "\n  --- Informacoes da Arvore ---\n";
                cout << "  Total de alunos : " << contarAlunos(topo) << "\n";
                cout << "  Altura da arvore: " << altura(topo)       << "\n";
                cout << "  -----------------------------\n";
                break;
            }

            case 0:
                cout << "\n  Encerrando. Ate logo!\n\n";
                break;

            default:
                cout << "  [ERRO] Opcao invalida.\n";
        }

    } while (opcao != 0);

    // Libera toda a memória alocada (igual ao código do professor)
    libera_ArvBin(raiz);

    return 0;
}
