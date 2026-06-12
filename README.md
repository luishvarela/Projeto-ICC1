# Sistema de Gerenciamento de Mercadinho

Projeto desenvolvido para a disciplina **SCC 0221 – Introdução à Ciência da Computação I (ICC1)** do Instituto de Ciências Matemáticas e de Computação (**ICMC - USP**), no 1º Semestre de 2026.

**Professor:** Rudinei Goularte  
**Monitor:** José Fausto Vital Barbosa  

---

## Integrantes (Grupo)
* **Luís Henrique Varela Medeiros Bezerra**
* **Gustavo Ribeiro Britto Rezende**
* **Guilherme Souza de Oliveira Santos**

---

## Sobre o Projeto
O objetivo do projeto é desenvolver um sistema robusto em linguagem C para automatizar o controle de estoque, fluxo de caixa e vendas de um minimercado. 

O programa foi construído seguindo o padrão **C99**, utilizando **alocação dinâmica de memória** para gerenciar os produtos e persistência de dados em arquivo para manter o estado do sistema entre diferentes execuções.

### Funcionalidades Implementadas
1. **`IP` (Insere Produto):** Cadastro inicial de novos itens informando nome, quantidade e preço (geração de ID sequencial automático).
2. **`AE` (Aumenta Estoque):** Entrada de novas remessas de produtos (gera custo e altera o saldo do caixa).
3. **`MP` (Modifica Preço):** Atualização dinâmica de valores de venda por código.
4. **`VE` (Venda):** Processamento de vendas de múltiplos itens por ID, finalizada com a flag `-1`. Exibe cupom e soma o total ao caixa.
5. **`CE` (Consulta Estoque):** Listagem de todos os códigos, nomes e quantidades disponíveis.
6. **`CS` (Consulta Saldo):** Exibição do saldo atual do caixa (saldo inicial de R$ 100,00).
7. **`FE` (Finalizar o Dia):** Exporta e salva todos os dados em arquivo para persistência no dia seguinte.

---

## Como Compilar e Executar

O projeto foi desenvolvido para ser compilado via **GCC** em ambiente Linux/macOS ou Cygwin (Windows).

### Compilação
Abra o terminal na pasta do projeto e execute:
```bash
gcc -std=c99 -Wall main.c -o mercadinho
```

### Execução Manual
```bash
./mercadinho
```

### Execução com Arquivos de Teste (Redirecionamento)
Caso queira testar o comportamento do programa usando lotes de comandos automáticos:
```bash
./mercadinho < entrada.txt > saida_gerada.txt
```

---

## Estrutura de Arquivos para Entrega
O arquivo `.zip` final contém:
* `main.c` (ou os arquivos `.c` e `.h` estruturados do seu código fonte)
* `readme.txt` (conforme exigido na especificação da disciplina contendo nomes e NUSPs)
