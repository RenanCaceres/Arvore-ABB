# 🌳 Sistema de Cadastro de Alunos — Árvore Binária de Busca (ABB)

> **Atividade Avaliativa 3** — Disciplina: Estrutura de Dados 2026-01  
> Universidade Tecnológica Federal do Paraná — UTFPR, Campus Cornélio Procópio  
> Curso: Tecnologia em Análise e Desenvolvimento de Sistemas (TADS)

---

## 👥 Integrantes

| Nome | RA |
|------|----|
| Renan Cáceres Anselmo | 2525526 |
| Pedro Lucas Sales Larini | 2767147 |

**Professor:** Clayton Kossoski  
**Período de entrega:** 22 a 30 de junho de 2026

---

## 📋 Sobre o Projeto

Sistema de cadastro de alunos implementado em **C++** utilizando uma **Árvore Binária de Busca (ABB)** como estrutura de dados principal. O sistema permite inserir, buscar e listar alunos em ordem alfabética, além de exibir informações sobre a árvore.

A chave de ordenação da árvore é o **nome do aluno**, garantindo que o percurso In-Ordem produza automaticamente a listagem em ordem alfabética.

---

## 🗂️ Estrutura da Árvore

Com os alunos do exemplo inseridos na ordem 101 → 102 → 103 → 104, a árvore fica organizada assim:

```
              [John Smith]          ← raiz (1º inserido)
             /            \
      [Alice Johnson]   [Michael Brown]
             \
          [Emily Davis]
```

Percurso In-Ordem (esq → raiz → dir): **Alice → Emily → John → Michael** ✓

---

## ⚙️ Funcionalidades

| # | Funcionalidade | Descrição |
|---|---------------|-----------|
| 1 | **Inserir aluno** | Cadastra matrícula, nome e curso na árvore |
| 2 | **Buscar aluno** | Localiza um aluno pelo nome (busca O(log n)) |
| 3 | **Listar em ordem alfabética** | Percurso In-Ordem na árvore, sem ordenação externa |
| 4 | **Informações da árvore** | Exibe total de alunos e altura da árvore |

---

## 🚀 Como compilar e executar

### Pré-requisitos
- Compilador C++ (g++ recomendado)

### Compilar
```bash
g++ -o cadastro_alunos cadastro_alunos.cpp
```

### Executar
```bash
./cadastro_alunos
```

---

## 🖥️ Exemplo de uso

**Entrada (alunos do enunciado):**

| Matrícula | Nome | Curso |
|-----------|------|-------|
| 101 | John Smith | Engenharia de Software |
| 102 | Alice Johnson | Sistemas de Informação |
| 103 | Michael Brown | Ciência da Computação |
| 104 | Emily Davis | Engenharia de Software |

**Saída (listagem em ordem alfabética):**
```
Alice Johnson  - Matricula: 102 - Curso: Sistemas de Informacao
Emily Davis    - Matricula: 104 - Curso: Engenharia de Software
John Smith     - Matricula: 101 - Curso: Engenharia de Software
Michael Brown  - Matricula: 103 - Curso: Ciencia da Computacao
```

**Informações da árvore:**
```
Total de alunos : 4
Altura da arvore: 3
```

---

## 📁 Arquivos do projeto

```
📦 cadastro-alunos-abb/
 ┣ 📄 cadastro_alunos.cpp   ← código-fonte principal
 ┣ 📄 relatorio_abb.docx    ← relatório técnico (1-2 páginas)
 ┗ 📄 README.md             ← este arquivo
```

---

## 🛠️ Tecnologias

![C++](https://img.shields.io/badge/C++-00599C?style=flat&logo=cplusplus&logoColor=white)
![UTFPR](https://img.shields.io/badge/UTFPR-TADS-yellow?style=flat)
