#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_AGENTES 50

#define PADRAO "\x1b[0m"
#define VERMELHO "\x1b[31m"
#define VERDE "\x1b[32m"
#define AMARELO "\x1b[33m"
#define AZUL "\x1b[36m"

int ids[MAX_AGENTES];
char nomes[MAX_AGENTES][81];
char scripts[MAX_AGENTES][1001];
int soma_notas[MAX_AGENTES];
int qtd_avaliacoes[MAX_AGENTES];
char categorias[MAX_AGENTES][50];

int total_agentes = 0;

void carregarDados();
void salvarDados();
void confirmarSalvamento();
void cadastrarAgente();
void listarAgentes();
void buscarPorId();
void buscarPorNome();
void menuFiltrosAvancados();
void avaliarAgente();

int main()
{
    carregarDados();
    int opcao;

    do
    {
        printf(AZUL "\n========================================\n" PADRAO);
        printf(AZUL "          AI AGENT REGISTRY\n" PADRAO);
        printf(AZUL "========================================\n" PADRAO);
        printf("1. Cadastrar novo agente\n");
        printf("2. Listar todos os agentes\n");
        printf("3. Buscar agente por ID\n");
        printf("4. Buscar agente por Nome\n");
        printf("5. Filtros Avancados (Categoria)\n");
        printf("6. Avaliar agente\n");
        printf(VERMELHO "0. Encerrar programa\n" PADRAO);
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao)
        {
        case 1:
            cadastrarAgente();
            break;
        case 2:
            listarAgentes();
            break;
        case 3:
            buscarPorId();
            break;
        case 4:
            buscarPorNome();
            break;
        case 5:
            menuFiltrosAvancados();
            break;
        case 6:
            avaliarAgente();
            break;
        case 0:
            printf(AMARELO "\nEncerrando o programa...\n" PADRAO);
            break;
        default:
            printf(VERMELHO "\nOpcao invalida!\n" PADRAO);
        }
    } while (opcao != 0);

    return 0;
}

void carregarDados()
{
    FILE *arquivo = fopen("agentes.txt", "r");
    if (arquivo == NULL)
    {
        printf(AMARELO "Arquivo agentes.txt nao encontrado. O banco comecara vazio.\n" PADRAO);
        return;
    }

    total_agentes = 0;
    while (fscanf(arquivo, "%d|%80[^|]|%1000[^|]|%d|%d|%49[^\n]\n",
                  &ids[total_agentes],
                  nomes[total_agentes],
                  scripts[total_agentes],
                  &soma_notas[total_agentes],
                  &qtd_avaliacoes[total_agentes],
                  categorias[total_agentes]) != EOF)
    {
        total_agentes++;
    }
    fclose(arquivo);
}

void salvarDados()
{
    FILE *arquivo = fopen("agentes.txt", "w");
    if (arquivo == NULL)
    {
        printf(VERMELHO "Erro ao criar o arquivo de salvamento!\n" PADRAO);
        return;
    }

    for (int i = 0; i < total_agentes; i++)
    {
        fprintf(arquivo, "%d|%s|%s|%d|%d|%s\n", ids[i], nomes[i], scripts[i], soma_notas[i], qtd_avaliacoes[i], categorias[i]);
    }
    fclose(arquivo);
}

void confirmarSalvamento()
{
    int resp;
    do
    {
        printf(AMARELO "\nDeseja salvar as alteracoes no arquivo? (1 - Sim / 2 - Nao): " PADRAO);
        scanf("%d", &resp);
        getchar();

        if (resp == 1)
        {
            salvarDados();
            printf(VERDE "Dados salvos com sucesso!\n" PADRAO);
        }
        else if (resp == 2)
        {
            printf(AMARELO "Alteracoes mantidas apenas na memoria.\n" PADRAO);
        }
        else
        {
            printf(VERMELHO "Opcao invalida. Digite 1 para Sim ou 2 para Nao.\n" PADRAO);
        }
    } while (resp != 1 && resp != 2);
}

void cadastrarAgente()
{
    if (total_agentes >= MAX_AGENTES)
    {
        printf(VERMELHO "\nErro: Limite maximo atingido!\n" PADRAO);
        return;
    }

    int novo_id = 1;
    int id_em_uso;

    do
    {
        id_em_uso = 0;
        for (int i = 0; i < total_agentes; i++)
        {
            if (ids[i] == novo_id)
            {
                id_em_uso = 1;
                novo_id++;
                break;
            }
        }
    } while (id_em_uso == 1);

    ids[total_agentes] = novo_id;
    printf(AZUL "\n--- NOVO CADASTRO ---\n" PADRAO);
    printf("ID gerado: " VERDE "%d\n" PADRAO, novo_id);

    printf("Nome do agente (ate 80 letras): ");
    fgets(nomes[total_agentes], 81, stdin);
    nomes[total_agentes][strcspn(nomes[total_agentes], "\n")] = 0;

    printf("Script/instrucoes: ");
    fgets(scripts[total_agentes], 1001, stdin);
    scripts[total_agentes][strcspn(scripts[total_agentes], "\n")] = 0;

    int op_categoria;
    do
    {
        printf("\nCategorias:\n1. Vendas\n2. Pos-Venda / Suporte\n3. Financeiro\n4. Recursos Humanos\nOpcao: ");
        scanf("%d", &op_categoria);
        getchar();

        switch (op_categoria)
        {
        case 1:
            strcpy(categorias[total_agentes], "Vendas");
            break;
        case 2:
            strcpy(categorias[total_agentes], "Pos-Venda / Suporte");
            break;
        case 3:
            strcpy(categorias[total_agentes], "Financeiro");
            break;
        case 4:
            strcpy(categorias[total_agentes], "Recursos Humanos");
            break;
        default:
            printf(VERMELHO "Opcao invalida.\n" PADRAO);
            op_categoria = 0;
        }
    } while (op_categoria == 0);

    soma_notas[total_agentes] = 0;
    qtd_avaliacoes[total_agentes] = 0;

    total_agentes++;
    printf(VERDE "\nCadastro concluido! O agente comeca com 0 avaliacoes.\n" PADRAO);

    confirmarSalvamento();
}

void listarAgentes()
{
    if (total_agentes == 0)
    {
        printf(AMARELO "\nNenhum agente cadastrado.\n" PADRAO);
        return;
    }

    for (int i = 0; i < total_agentes - 1; i++)
    {
        for (int j = i + 1; j < total_agentes; j++)
        {
            if (ids[i] > ids[j])
            {
                int temp_id = ids[i];
                ids[i] = ids[j];
                ids[j] = temp_id;

                char temp_nome[81];
                strcpy(temp_nome, nomes[i]);
                strcpy(nomes[i], nomes[j]);
                strcpy(nomes[j], temp_nome);

                char temp_script[1001];
                strcpy(temp_script, scripts[i]);
                strcpy(scripts[i], scripts[j]);
                strcpy(scripts[j], temp_script);

                int temp_soma = soma_notas[i];
                soma_notas[i] = soma_notas[j];
                soma_notas[j] = temp_soma;

                int temp_qtd = qtd_avaliacoes[i];
                qtd_avaliacoes[i] = qtd_avaliacoes[j];
                qtd_avaliacoes[j] = temp_qtd;

                char temp_categoria[50];
                strcpy(temp_categoria, categorias[i]);
                strcpy(categorias[i], categorias[j]);
                strcpy(categorias[j], temp_categoria);
            }
        }
    }

    printf(AZUL "\n--- LISTA GERAL (ORDENADA POR ID) ---\n" PADRAO);
    for (int i = 0; i < total_agentes; i++)
    {
        float media = 0.0;
        if (qtd_avaliacoes[i] > 0)
        {
            media = (float)soma_notas[i] / qtd_avaliacoes[i];
        }

        printf("ID: " VERDE "%d" PADRAO " | Nome: %s | Categoria: %s | Nota: %.1f (%d)\n",
               ids[i], nomes[i], categorias[i], media, qtd_avaliacoes[i]);
    }
}

void buscarPorId()
{
    if (total_agentes == 0)
        return;

    int id_busca, indice_encontrado = -1;
    printf("\nDigite o ID pra buscar: ");
    scanf("%d", &id_busca);
    getchar();

    for (int i = 0; i < total_agentes; i++)
    {
        if (ids[i] == id_busca)
        {
            indice_encontrado = i;
            break;
        }
    }

    if (indice_encontrado == -1)
    {
        printf(VERMELHO "\nAgente nao encontrado.\n" PADRAO);
        return;
    }

    float media = 0.0;
    if (qtd_avaliacoes[indice_encontrado] > 0)
    {
        media = (float)soma_notas[indice_encontrado] / qtd_avaliacoes[indice_encontrado];
    }

    printf(AZUL "\n--- DADOS ---\n" PADRAO);
    printf("ID: %d\nNome: %s\nCategoria: %s\nNota: %.1f (%d avaliacoes)\nScript: %s\n",
           ids[indice_encontrado], nomes[indice_encontrado], categorias[indice_encontrado], media, qtd_avaliacoes[indice_encontrado], scripts[indice_encontrado]);

    int acao;
    printf("\n1. Editar | 2. Excluir | 3. Voltar\nOpcao: ");
    scanf("%d", &acao);
    getchar();

    if (acao == 1)
    {
        printf("Novo Nome: ");
        fgets(nomes[indice_encontrado], 81, stdin);
        nomes[indice_encontrado][strcspn(nomes[indice_encontrado], "\n")] = 0;

        printf("Novo Script: ");
        fgets(scripts[indice_encontrado], 1001, stdin);
        scripts[indice_encontrado][strcspn(scripts[indice_encontrado], "\n")] = 0;

        int op_cat;
        do
        {
            printf("\nNova Categoria:\n1. Vendas\n2. Pos-Venda / Suporte\n3. Financeiro\n4. Recursos Humanos\nOpcao: ");
            scanf("%d", &op_cat);
            getchar();

            switch (op_cat)
            {
            case 1:
                strcpy(categorias[indice_encontrado], "Vendas");
                break;
            case 2:
                strcpy(categorias[indice_encontrado], "Pos-Venda / Suporte");
                break;
            case 3:
                strcpy(categorias[indice_encontrado], "Financeiro");
                break;
            case 4:
                strcpy(categorias[indice_encontrado], "Recursos Humanos");
                break;
            default:
                printf(VERMELHO "Invalido.\n" PADRAO);
                op_cat = 0;
            }
        } while (op_cat == 0);

        printf(VERDE "\nEditado com sucesso! (Avaliacoes foram mantidas)\n" PADRAO);
        confirmarSalvamento();
    }
    else if (acao == 2)
    {
        for (int j = indice_encontrado; j < total_agentes - 1; j++)
        {
            ids[j] = ids[j + 1];
            strcpy(nomes[j], nomes[j + 1]);
            strcpy(scripts[j], scripts[j + 1]);
            soma_notas[j] = soma_notas[j + 1];
            qtd_avaliacoes[j] = qtd_avaliacoes[j + 1];
            strcpy(categorias[j], categorias[j + 1]);
        }
        total_agentes--;
        printf(VERDE "\nAgente apagado com sucesso!\n" PADRAO);
        confirmarSalvamento();
    }
}

void buscarPorNome()
{
    if (total_agentes == 0)
        return;

    char nome_busca[81];
    int indice_encontrado = -1;

    printf("\nDigite o Nome certinho: ");
    fgets(nome_busca, 81, stdin);
    nome_busca[strcspn(nome_busca, "\n")] = 0;

    for (int i = 0; i < total_agentes; i++)
    {
        if (strcmp(nomes[i], nome_busca) == 0)
        {
            indice_encontrado = i;
            break;
        }
    }

    if (indice_encontrado == -1)
    {
        printf(VERMELHO "\nNao achou.\n" PADRAO);
        return;
    }

    float media = 0.0;
    if (qtd_avaliacoes[indice_encontrado] > 0)
    {
        media = (float)soma_notas[indice_encontrado] / qtd_avaliacoes[indice_encontrado];
    }

    printf(AZUL "\n--- DADOS ---\n" PADRAO);
    printf("ID: %d\nNome: %s\nCategoria: %s\nNota: %.1f (%d avaliacoes)\nScript: %s\n",
           ids[indice_encontrado], nomes[indice_encontrado], categorias[indice_encontrado], media, qtd_avaliacoes[indice_encontrado], scripts[indice_encontrado]);

    int acao;
    printf("\n1. Editar | 2. Excluir | 3. Voltar\nOpcao: ");
    scanf("%d", &acao);
    getchar();

    if (acao == 1)
    {
        printf("Novo Nome: ");
        fgets(nomes[indice_encontrado], 81, stdin);
        nomes[indice_encontrado][strcspn(nomes[indice_encontrado], "\n")] = 0;

        printf("Novo Script: ");
        fgets(scripts[indice_encontrado], 1001, stdin);
        scripts[indice_encontrado][strcspn(scripts[indice_encontrado], "\n")] = 0;

        int op_cat;
        do
        {
            printf("\nNova Categoria:\n1. Vendas\n2. Pos-Venda / Suporte\n3. Financeiro\n4. Recursos Humanos\nOpcao: ");
            scanf("%d", &op_cat);
            getchar();

            switch (op_cat)
            {
            case 1:
                strcpy(categorias[indice_encontrado], "Vendas");
                break;
            case 2:
                strcpy(categorias[indice_encontrado], "Pos-Venda / Suporte");
                break;
            case 3:
                strcpy(categorias[indice_encontrado], "Financeiro");
                break;
            case 4:
                strcpy(categorias[indice_encontrado], "Recursos Humanos");
                break;
            default:
                printf(VERMELHO "Invalido.\n" PADRAO);
                op_cat = 0;
            }
        } while (op_cat == 0);

        printf(VERDE "\nEditado com sucesso! (Avaliacoes foram mantidas)\n" PADRAO);
        confirmarSalvamento();
    }
    else if (acao == 2)
    {
        for (int j = indice_encontrado; j < total_agentes - 1; j++)
        {
            ids[j] = ids[j + 1];
            strcpy(nomes[j], nomes[j + 1]);
            strcpy(scripts[j], scripts[j + 1]);
            soma_notas[j] = soma_notas[j + 1];
            qtd_avaliacoes[j] = qtd_avaliacoes[j + 1];
            strcpy(categorias[j], categorias[j + 1]);
        }
        total_agentes--;
        printf(VERDE "\nAgente apagado com sucesso!\n" PADRAO);
        confirmarSalvamento();
    }
}

void menuFiltrosAvancados()
{
    if (total_agentes == 0)
    {
        printf(AMARELO "\nNada pra filtrar.\n" PADRAO);
        return;
    }

    char categoria_busca[50];
    int encontrou = 0;
    int op_categoria;

    printf(AZUL "\n--- FILTRO ---\n" PADRAO);
    do
    {
        printf("Qual Categoria?\n");
        printf("1. Vendas\n2. Pos-Venda / Suporte\n3. Financeiro\n4. Recursos Humanos\nOpcao: ");
        scanf("%d", &op_categoria);
        getchar();

        switch (op_categoria)
        {
        case 1:
            strcpy(categoria_busca, "Vendas");
            break;
        case 2:
            strcpy(categoria_busca, "Pos-Venda / Suporte");
            break;
        case 3:
            strcpy(categoria_busca, "Financeiro");
            break;
        case 4:
            strcpy(categoria_busca, "Recursos Humanos");
            break;
        default:
            printf(VERMELHO "Opcao invalida.\n\n" PADRAO);
            op_categoria = 0;
        }
    } while (op_categoria == 0);

    printf("\nResultados:\n");

    for (int i = 0; i < total_agentes; i++)
    {
        if (strcmp(categorias[i], categoria_busca) == 0)
        {
            float media = 0.0;
            if (qtd_avaliacoes[i] > 0)
            {
                media = (float)soma_notas[i] / qtd_avaliacoes[i];
            }
            printf("ID: " VERDE "%d" PADRAO " | Nome: %s | Nota: %.1f (%d)\n", ids[i], nomes[i], media, qtd_avaliacoes[i]);
            encontrou = 1;
        }
    }

    if (!encontrou)
    {
        printf(AMARELO "Ninguem nessa categoria.\n" PADRAO);
    }
}

void avaliarAgente()
{
    if (total_agentes == 0)
    {
        printf(AMARELO "\nNenhum agente cadastrado para avaliar.\n" PADRAO);
        return;
    }

    int id_busca, indice_encontrado = -1;
    printf("\nDigite o ID do agente que deseja avaliar: ");
    scanf("%d", &id_busca);
    while (getchar() != '\n')
        ; // Limpa o buffer de forma segura

    for (int i = 0; i < total_agentes; i++)
    {
        if (ids[i] == id_busca)
        {
            indice_encontrado = i;
            break;
        }
    }

    if (indice_encontrado == -1)
    {
        printf(VERMELHO "\nAgente nao encontrado.\n" PADRAO);
        return;
    }

    int nova_nota;
    do
    {
        printf("Digite uma nota de 0 a 5 para o agente %s: ", nomes[indice_encontrado]);
        scanf("%d", &nova_nota);
        while (getchar() != '\n')
            ; // O SEGREDO: Limpa o ".2" ou qualquer letra que o usuario digitar sem querer

        if (nova_nota < 0 || nova_nota > 5)
        {
            printf(VERMELHO "Nota invalida! " PADRAO);
        }
    } while (nova_nota < 0 || nova_nota > 5);

    soma_notas[indice_encontrado] += nova_nota;
    qtd_avaliacoes[indice_encontrado]++;

    printf(VERDE "\nAvaliacao computada com sucesso!\n" PADRAO);
    confirmarSalvamento();
}