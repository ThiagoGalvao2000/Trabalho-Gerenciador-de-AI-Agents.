#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_AGENTES 50

// CORES DO PROGRAMA
#define RESET "\x1b[0m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define CYAN "\x1b[36m"

// VETORES GLOBAIS
int ids[MAX_AGENTES];
char nomes[MAX_AGENTES][81];
char scripts[MAX_AGENTES][1001];
int notas[MAX_AGENTES];
char categorias[MAX_AGENTES][50];

int total_agentes = 0;

// FUNÇÕES
void carregarDados();
void salvarDados();
void confirmarSalvamento();
void cadastrarAgente();
void listarAgentes();
void buscarPorId();
void buscarPorNome();
void menuFiltrosAvancados();

int main()
{
    carregarDados();
    int opcao;

    do
    {
        printf(CYAN "\n========================================\n" RESET);
        printf(CYAN "          AI AGENT REGISTRY\n" RESET);
        printf(CYAN "========================================\n" RESET);
        printf("1. Cadastrar novo agente\n");
        printf("2. Listar todos os agentes\n");
        printf("3. Buscar agente por ID\n");
        printf("4. Buscar agente por Nome\n");
        printf("5. Filtros Avancados (Categoria)\n");
        printf(RED "0. Encerrar programa\n" RESET);
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
        case 0:
            printf(YELLOW "\nEncerrando o programa...\n" RESET);
            break;
        default:
            printf(RED "\nOpcao invalida!\n" RESET);
        }
    } while (opcao != 0);

    return 0;
}

void carregarDados()
{
    FILE *arquivo = fopen("agentes.txt", "r");
    if (arquivo == NULL)
    {
        printf(YELLOW "Arquivo agentes.txt nao encontrado. O banco comecara vazio.\n" RESET);
        return;
    }

    total_agentes = 0;
    while (fscanf(arquivo, "%d|%80[^|]|%1000[^|]|%d|%49[^\n]\n",
                  &ids[total_agentes],
                  nomes[total_agentes],
                  scripts[total_agentes],
                  &notas[total_agentes],
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
        printf(RED "Erro ao criar o arquivo de salvamento!\n" RESET);
        return;
    }

    for (int i = 0; i < total_agentes; i++)
    {
        fprintf(arquivo, "%d|%s|%s|%d|%s\n", ids[i], nomes[i], scripts[i], notas[i], categorias[i]);
    }
    fclose(arquivo);
}

void confirmarSalvamento()
{
    int resp;
    do
    {
        printf(YELLOW "\nDeseja salvar as alteracoes no arquivo? (1 - Sim / 2 - Nao): " RESET);
        scanf("%d", &resp);
        getchar();

        if (resp == 1)
        {
            salvarDados();
            printf(GREEN "Dados salvos com sucesso!\n" RESET);
        }
        else if (resp == 2)
        {
            printf(YELLOW "Alteracoes mantidas apenas na memoria (nao foram pro arquivo).\n" RESET);
        }
        else
        {
            printf(RED "Opcao invalida. Digite 1 para Sim ou 2 para Nao.\n" RESET);
        }
    } while (resp != 1 && resp != 2);
}

void cadastrarAgente()
{
    if (total_agentes >= MAX_AGENTES)
    {
        printf(RED "\nErro: Limite maximo atingido!\n" RESET);
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
    printf(CYAN "\n--- NOVO CADASTRO ---\n" RESET);
    printf("ID gerado: " GREEN "%d\n" RESET, novo_id);

    printf("Nome do agente (ate 80 letras): ");
    fgets(nomes[total_agentes], 81, stdin);
    nomes[total_agentes][strcspn(nomes[total_agentes], "\n")] = 0;

    printf("Script/instrucoes: ");
    fgets(scripts[total_agentes], 1001, stdin);
    scripts[total_agentes][strcspn(scripts[total_agentes], "\n")] = 0;

    do
    {
        printf("Nota do agente (0 a 5): ");
        scanf("%d", &notas[total_agentes]);
        getchar();
    } while (notas[total_agentes] < 0 || notas[total_agentes] > 5);

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
            printf(RED "Opcao invalida.\n" RESET);
            op_categoria = 0;
        }
    } while (op_categoria == 0);

    total_agentes++;
    printf(GREEN "\nCadastro concluido!\n" RESET);

    confirmarSalvamento();
}

void listarAgentes()
{
    if (total_agentes == 0)
    {
        printf(YELLOW "\nNenhum agente cadastrado.\n" RESET);
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

                int temp_nota = notas[i];
                notas[i] = notas[j];
                notas[j] = temp_nota;

                char temp_categoria[50];
                strcpy(temp_categoria, categorias[i]);
                strcpy(categorias[i], categorias[j]);
                strcpy(categorias[j], temp_categoria);
            }
        }
    }

    printf(CYAN "\n--- LISTA GERAL (ORDENADA POR ID) ---\n" RESET);
    for (int i = 0; i < total_agentes; i++)
    {
        printf("ID: " GREEN "%d" RESET " | Nome: %s | Categoria: %s | Nota: %d\n", ids[i], nomes[i], categorias[i], notas[i]);
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
        printf(RED "\nAgente nao encontrado.\n" RESET);
        return;
    }

    printf(CYAN "\n--- DADOS ---\n" RESET);
    printf("ID: %d\nNome: %s\nCategoria: %s\nNota: %d\nScript: %s\n",
           ids[indice_encontrado], nomes[indice_encontrado], categorias[indice_encontrado], notas[indice_encontrado], scripts[indice_encontrado]);

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

        do
        {
            printf("Nova Nota (0 a 5): ");
            scanf("%d", &notas[indice_encontrado]);
            getchar();
        } while (notas[indice_encontrado] < 0 || notas[indice_encontrado] > 5);

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
                printf(RED "Invalido.\n" RESET);
                op_cat = 0;
            }
        } while (op_cat == 0);

        printf(GREEN "\nEditado com sucesso!\n" RESET);
        confirmarSalvamento();
    }
    else if (acao == 2)
    {
        for (int j = indice_encontrado; j < total_agentes - 1; j++)
        {
            ids[j] = ids[j + 1];
            strcpy(nomes[j], nomes[j + 1]);
            strcpy(scripts[j], scripts[j + 1]);
            notas[j] = notas[j + 1];
            strcpy(categorias[j], categorias[j + 1]);
        }
        total_agentes--;
        printf(GREEN "\nAgente apagado com sucesso!\n" RESET);
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
        printf(RED "\nNao achou.\n" RESET);
        return;
    }

    printf(CYAN "\n--- DADOS ---\n" RESET);
    printf("ID: %d\nNome: %s\nCategoria: %s\nNota: %d\nScript: %s\n",
           ids[indice_encontrado], nomes[indice_encontrado], categorias[indice_encontrado], notas[indice_encontrado], scripts[indice_encontrado]);

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

        do
        {
            printf("Nova Nota (0 a 5): ");
            scanf("%d", &notas[indice_encontrado]);
            getchar();
        } while (notas[indice_encontrado] < 0 || notas[indice_encontrado] > 5);

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
                printf(RED "Invalido.\n" RESET);
                op_cat = 0;
            }
        } while (op_cat == 0);

        printf(GREEN "\nEditado com sucesso!\n" RESET);
        confirmarSalvamento();
    }
    else if (acao == 2)
    {
        for (int j = indice_encontrado; j < total_agentes - 1; j++)
        {
            ids[j] = ids[j + 1];
            strcpy(nomes[j], nomes[j + 1]);
            strcpy(scripts[j], scripts[j + 1]);
            notas[j] = notas[j + 1];
            strcpy(categorias[j], categorias[j + 1]);
        }
        total_agentes--;
        printf(GREEN "\nAgente apagado com sucesso!\n" RESET);
        confirmarSalvamento();
    }
}

void menuFiltrosAvancados()
{
    if (total_agentes == 0)
    {
        printf(YELLOW "\nNada pra filtrar.\n" RESET);
        return;
    }

    char categoria_busca[50];
    int encontrou = 0;
    int op_categoria;

    printf(CYAN "\n--- FILTRO ---\n" RESET);
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
            printf(RED "Opcao invalida.\n\n" RESET);
            op_categoria = 0;
        }
    } while (op_categoria == 0);

    printf("\nResultados:\n");

    for (int i = 0; i < total_agentes; i++)
    {
        if (strcmp(categorias[i], categoria_busca) == 0)
        {
            printf("ID: " GREEN "%d" RESET " | Nome: %s | Nota: %d\n", ids[i], nomes[i], notas[i]);
            encontrou = 1;
        }
    }

    if (!encontrou)
    {
        printf(YELLOW "Ninguem nessa categoria.\n" RESET);
    }
}