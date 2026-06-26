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
 * Aluno: Renan Caceres  |  RA: 2525526
 * ============================================================
 *
 * COMO FUNCIONA UMA ÁRVORE BINÁRIA DE BUSCA (ABB)?
 *
 *   - Cada "nó" da árvore guarda os dados de um aluno.
 *   - Cada nó pode ter até dois filhos: esquerdo e direito.
 *   - Regra de ordenação (chave = nome do aluno):
 *       * Nomes menores (alfabeticamente) vão para a ESQUERDA
 *       * Nomes maiores vão para a DIREITA
 *
 *   Exemplo com os alunos do enunciado:
 *
 *              [John Smith]          <- raiz (primeiro inserido)
 *             /            \
 *      [Alice Johnson]   [Michael Brown]
 *             \
 *          [Emily Davis]
 *
 *   Percorrendo em ordem (esquerda → raiz → direita):
 *   Alice → Emily → John → Michael  ✓ (ordem alfabética!)
 * ============================================================
 */

#include <iostream>
#include <string>
#include <algorithm>   // usado apenas para transform() na comparação de nomes

using namespace std;

// ============================================================
// 1. ESTRUTURAS DE DADOS
// ============================================================

/*
 * Struct Aluno: guarda os dados de cada aluno.
 * Usamos 'string' para nome e curso, e 'int' para matrícula.
 */
struct Aluno {
    int    matricula;
    string nome;
    string curso;
};

/*
 * Struct No (nó): cada "caixa" da árvore.
 * Guarda um aluno + ponteiros para o filho esquerdo e direito.
 * Quando não há filho, o ponteiro aponta para nullptr (vazio).
 */
struct No {
    Aluno aluno;   // dados do aluno neste nó
    No*   esq;     // ponteiro para o filho à esquerda (nome menor)
    No*   dir;     // ponteiro para o filho à direita  (nome maior)
};

// ============================================================
// 2. FUNÇÕES AUXILIARES
// ============================================================

/*
 * Converte uma string para letras minúsculas.
 * Usamos isso para comparar nomes sem diferenciar maiúsculas de minúsculas.
 * Ex: "Alice" e "alice" são tratados como iguais.
 */
string minusculo(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

/*
 * Cria um novo nó na memória e devolve o ponteiro para ele.
 * Filhos começam como nullptr (nó folha, sem filhos ainda).
 */
No* criarNo(Aluno aluno) {
    No* novo = new No();  // aloca memória para o nó
    novo->aluno = aluno;
    novo->esq   = nullptr;
    novo->dir   = nullptr;
    return novo;
}

// ============================================================
// 3. OPERAÇÕES DA ÁRVORE
// ============================================================

/*
 * INSERIR ALUNO
 * -------------
 * Insere um aluno na árvore mantendo a regra da ABB.
 * Função recursiva: começa na raiz e desce até encontrar
 * o lugar certo (um ponteiro nullptr).
 *
 * Parâmetros:
 *   raiz  - nó atual da recursão
 *   aluno - aluno a ser inserido
 *
 * Retorno: ponteiro para o nó atual (necessário para atualizar a árvore)
 */
No* inserir(No* raiz, Aluno aluno) {

    // Caso base: chegamos em um espaço vazio → cria o nó aqui
    if (raiz == nullptr) {
        return criarNo(aluno);
    }

    // Compara o nome do novo aluno com o nome do nó atual
    // (comparação em minúsculas para ignorar maiúsculas)
    if (minusculo(aluno.nome) < minusculo(raiz->aluno.nome)) {
        // Nome novo é menor → vai para a ESQUERDA
        raiz->esq = inserir(raiz->esq, aluno);

    } else if (minusculo(aluno.nome) > minusculo(raiz->aluno.nome)) {
        // Nome novo é maior → vai para a DIREITA
        raiz->dir = inserir(raiz->dir, aluno);

    } else {
        // Nomes iguais → não permite duplicatas
        cout << "[AVISO] Aluno \"" << aluno.nome << "\" ja cadastrado!\n";
    }

    return raiz;  // retorna o nó atual sem modificação
}

/*
 * BUSCAR ALUNO
 * ------------
 * Procura um aluno pelo nome na árvore.
 * Também é recursiva: compara com o nó atual e decide se
 * continua pela esquerda, direita, ou achou.
 *
 * Retorno: ponteiro para o nó encontrado, ou nullptr se não existir.
 */
No* buscar(No* raiz, const string& nome) {

    // Caso base 1: chegamos em nullptr → não encontrou
    if (raiz == nullptr) {
        return nullptr;
    }

    // Caso base 2: encontrou o aluno!
    if (minusculo(nome) == minusculo(raiz->aluno.nome)) {
        return raiz;
    }

    // Decide em qual lado continuar a busca
    if (minusculo(nome) < minusculo(raiz->aluno.nome)) {
        return buscar(raiz->esq, nome);  // busca na subárvore esquerda
    } else {
        return buscar(raiz->dir, nome);  // busca na subárvore direita
    }
}

/*
 * LISTAR EM ORDEM ALFABÉTICA (percurso In-Ordem)
 * -----------------------------------------------
 * Visita os nós na ordem: esquerda → atual → direita
 * Isso naturalmente produz os nomes em ordem alfabética crescente,
 * pois os menores estão sempre à esquerda na ABB.
 *
 * Não usa nenhum vetor ou método de ordenação externo!
 * A própria estrutura da árvore já garante a ordem.
 */
void listarEmOrdem(No* raiz) {

    // Se chegou em nullptr, não há nada para visitar → para aqui
    if (raiz == nullptr) {
        return;
    }

    listarEmOrdem(raiz->esq);   // 1º: visita todos da esquerda (menores)

    // 2º: exibe o aluno deste nó
    cout << raiz->aluno.nome
         << " - Matricula: " << raiz->aluno.matricula
         << " - Curso: "     << raiz->aluno.curso
         << "\n";

    listarEmOrdem(raiz->dir);   // 3º: visita todos da direita (maiores)
}

/*
 * CALCULAR ALTURA DA ÁRVORE
 * --------------------------
 * A altura é o número de níveis da árvore (do maior caminho raiz→folha).
 * Calculamos recursivamente: altura = 1 + max(altura_esq, altura_dir)
 * Uma árvore vazia tem altura 0.
 */
int altura(No* raiz) {
    if (raiz == nullptr) {
        return 0;
    }

    int altEsq = altura(raiz->esq);  // altura da subárvore esquerda
    int altDir = altura(raiz->dir);  // altura da subárvore direita

    // Retorna 1 (nó atual) + o maior dos dois lados
    return 1 + (altEsq > altDir ? altEsq : altDir);
}

/*
 * CONTAR ALUNOS
 * -------------
 * Conta quantos nós (alunos) existem na árvore.
 * Percorre todos os nós recursivamente e soma 1 por nó.
 */
int contarAlunos(No* raiz) {
    if (raiz == nullptr) {
        return 0;
    }
    // Conta: este nó (1) + todos da esquerda + todos da direita
    return 1 + contarAlunos(raiz->esq) + contarAlunos(raiz->dir);
}

/*
 * LIBERAR MEMÓRIA
 * ---------------
 * Deleta todos os nós da árvore ao final do programa.
 * Usa percurso pós-ordem: deleta os filhos antes do pai.
 */
void destruirArvore(No* raiz) {
    if (raiz == nullptr) return;
    destruirArvore(raiz->esq);
    destruirArvore(raiz->dir);
    delete raiz;
}

// ============================================================
// 4. INTERFACE DE MENU
// ============================================================

/* Exibe o menu de opções */
void exibirMenu() {
    cout << "\n============================================\n";
    cout << "    CADASTRO DE ALUNOS - Arvore Binaria\n";
    cout << "============================================\n";
    cout << "  1. Inserir aluno\n";
    cout << "  2. Buscar aluno por nome\n";
    cout << "  3. Listar alunos em ordem alfabetica\n";
    cout << "  4. Informacoes da arvore\n";
    cout << "  0. Sair\n";
    cout << "============================================\n";
    cout << "  Opcao: ";
}

/*
 * Lê uma linha completa do teclado (aceita espaços).
 * O parâmetro 'limparBuffer' serve para descartar o Enter
 * que sobra depois de ler um número com cin >>.
 */
string lerLinha(const string& mensagem, bool limparBuffer = false) {
    string valor;
    cout << mensagem;
    if (limparBuffer) cin.ignore(1000, '\n');
    getline(cin, valor);
    return valor;
}

// ============================================================
// 5. FUNÇÃO PRINCIPAL
// ============================================================

int main() {

    No* raiz = nullptr;  // árvore começa vazia
    int opcao;

    // --- Carrega os alunos do exemplo do enunciado ---
    cout << "\n[Carregando alunos de exemplo...]\n";
    raiz = inserir(raiz, {101, "John Smith",    "Engenharia de Software"});
    raiz = inserir(raiz, {102, "Alice Johnson", "Sistemas de Informacao"});
    raiz = inserir(raiz, {103, "Michael Brown", "Ciencia da Computacao"});
    raiz = inserir(raiz, {104, "Emily Davis",   "Engenharia de Software"});
    cout << "[4 alunos carregados!]\n";

    // --- Loop principal ---
    do {
        exibirMenu();
        cin >> opcao;

        switch (opcao) {

            // ---- OPÇÃO 1: Inserir ----
            case 1: {
                Aluno novo;
                novo.matricula = 0;
                cout << "  Matricula : ";
                cin  >> novo.matricula;
                novo.nome  = lerLinha("  Nome     : ", true);  // true = limpa buffer
                novo.curso = lerLinha("  Curso    : ");
                raiz = inserir(raiz, novo);
                cout << "  [OK] \"" << novo.nome << "\" inserido!\n";
                break;
            }

            // ---- OPÇÃO 2: Buscar ----
            case 2: {
                string nome = lerLinha("  Nome para buscar: ", true);
                No* resultado = buscar(raiz, nome);

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

            // ---- OPÇÃO 3: Listar em ordem ----
            case 3: {
                cout << "\n  --- Alunos em Ordem Alfabetica ---\n";
                if (raiz == nullptr) {
                    cout << "  Nenhum aluno cadastrado.\n";
                } else {
                    listarEmOrdem(raiz);
                }
                cout << "  ----------------------------------\n";
                break;
            }

            // ---- OPÇÃO 4: Informações da árvore ----
            case 4: {
                cout << "\n  --- Informacoes da Arvore ---\n";
                cout << "  Total de alunos : " << contarAlunos(raiz) << "\n";
                cout << "  Altura da arvore: " << altura(raiz)       << "\n";
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

    // Libera toda a memória alocada antes de encerrar
    destruirArvore(raiz);

    return 0;
}
