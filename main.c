#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <conio.h>
#define BLACK 0
#define RED 1

typedef struct no {
    int key;
    struct no * pai, * esquerda, * direita;
    int cor;
} t_no;

t_no * nil; //folha sentinela

t_no * cria_no(int x) {
    t_no * novo = (t_no*)malloc(sizeof(t_no));

    if(novo == nil) {
        printf("Erro de alocação");
        return nil;
    }

    novo->pai = nil;
    novo->esquerda = nil;
    novo->direita = nil;
    novo->key = x;
    novo->cor = BLACK;
    return novo;
}

t_no * left_rotate(t_no * raiz, t_no * x) {
    t_no * y = x->direita;
    x->direita = y->esquerda;
    if (y->esquerda != nil) {
        y->esquerda->pai = x;
    }
    y->pai = x->pai;
    if (x->pai == nil) {
        raiz = y;
    } else {
        if (x == x->pai->esquerda) {
            x->pai->esquerda = y;
        } else {
            x->pai->direita = y;
        }
    }
    y->esquerda = x;
    x->pai = y;
    return raiz;
}

t_no * right_rotate(t_no * raiz, t_no * x) {
    t_no * y = x->esquerda;
    x->esquerda = y->direita;
    if (y->direita != nil) {
        y->direita->pai = x;
    }
    y->pai = x->pai;
    if (x->pai == nil) {
        raiz = y; //raiz
    } else {
        if (x == x->pai->direita) {
            x->pai->direita = y;
        } else {
            x->pai->esquerda = y;
        }
    }
    y->direita = x;
    x->pai = y;
    return raiz;
}

t_no * insertFixUp(t_no * raiz, t_no * z) {
    while (z != raiz && z->pai->cor == RED) {
        if (z->pai == z->pai->pai->esquerda) {
            t_no * y = z->pai->pai->direita;
            if (y->cor == RED) { //CASO 1
                z->pai->cor = BLACK;
                y->cor = BLACK;
                z->pai->pai->cor = RED;
                z = z->pai->pai;
            } else {
                if (z == z->pai->direita) { //CASO 2
                    z = z->pai;
                    raiz = left_rotate(raiz, z);
                }
                //CASO 3
                z->pai->cor = BLACK;
                z->pai->pai->cor = RED;
                raiz = right_rotate(raiz, z->pai->pai);
            }
        } else { //MESMA COISA DO IF INVERTENDO ESQUERDA E DIREITA
            if (z->pai == z->pai->pai->direita) {
                t_no * y = z->pai->pai->esquerda;
                if (y->cor == RED) {

                    z->pai->cor = BLACK;
                    y->cor = BLACK;
                    z->pai->pai->cor = RED;
                    z = z->pai->pai;
                } else {
                    if (z == z->pai->esquerda) {
                        z = z->pai;
                        raiz = right_rotate(raiz, z);
                    }
                    z->pai->cor = BLACK;
                    z->pai->pai->cor = RED;
                    raiz = left_rotate(raiz, z->pai->pai);
                }
            }
        }
    }
    raiz->cor = BLACK;
    return raiz;
}


t_no * insert(t_no * raiz, int key) {
    t_no * z = cria_no(key);
    t_no * y = nil;
    t_no * x = raiz;

    //Caso não exista nenhum elemento
    if(raiz == nil) {
        raiz = cria_no(z->key);
        raiz->cor = BLACK;
        return raiz;
    }

    //faz a busca para inserir o nó
    while (x != nil) {
        y = x;
        if (z->key < x->key) {
            x = x->esquerda;
        } else {
            x = x->direita;
        }
    }
    z->pai = y;
    if (y == nil) {
        raiz = z;
    } else if (z->key < y->key) {
        y->esquerda = z;
    } else {
        y->direita = z;
    }

    z->esquerda = nil;
    z->direita = nil;
    z->cor = RED;
    //chama a função insertFixUp
    //para verificar e restaurar as propriedades
    //caso tenham sido quebradas
    raiz = insertFixUp(raiz, z);
    return raiz;
}

//imprime a arvore em ordem crescente
void in_order(t_no * raiz) {
    if (raiz == nil) {
        return;
    }
    in_order(raiz->esquerda);
    printf("%d ", raiz->key);
    in_order(raiz->direita);
}

//procura o sucessor de um no
t_no * sucessor(t_no * z) {
    t_no * aux = z->direita;
    while (aux->esquerda != nil) {
        aux = aux->esquerda;
    }
    return aux;
}

t_no * deleteFixUp(t_no * raiz, t_no * x) {
    t_no * w;

    while (x != raiz && x->cor == BLACK) {
        if (x == x->pai->esquerda) {
            w = x->pai->direita;
            if (w->cor == RED) { //CASO 1
                w->cor = BLACK;
                x->pai->cor = RED;
                raiz = left_rotate(raiz, x->pai);
                w = x->pai->direita;
            }
            //CASO 2
            if (w->esquerda->cor == BLACK && w->direita->cor == BLACK) {
                w->cor = RED;
                x = x->pai;
            } else {
                if (w->direita->cor == BLACK) { //CASO 3
                    w->esquerda->cor = BLACK;
                    w->cor = RED;
                    raiz = right_rotate(raiz, w);
                    w = x->pai->direita;
                }
                //CASO 4
                w->cor = x->pai->cor;
                x->pai->cor = BLACK;
                w->direita->cor = BLACK;
                raiz = left_rotate(raiz, x->pai);
                x = raiz;
            }
        } else {
            w = x->pai->esquerda;
            if (w->cor == RED) {
                w->cor = BLACK;
                x->pai->cor = RED;
                raiz = right_rotate(raiz, x->pai);
                w = x->pai->esquerda;
            }
            if (w->direita->cor == BLACK && w->esquerda->cor == BLACK) {
                w->cor = RED;
                x = x->pai;
            } else {
                if (w->esquerda->cor == BLACK) {
                    w->direita->cor = BLACK;
                    w->cor = RED;
                    raiz = left_rotate(raiz, w);
                    w = x->pai->esquerda;
                }
                w->cor = x->pai->cor;
                x->pai->cor = BLACK;
                w->esquerda->cor = BLACK;
                raiz = right_rotate(raiz, x->pai);
                x = raiz;
            }
        }
    }
    x->cor = BLACK;
    return raiz;
}

//Procura um no
t_no * search(t_no * raiz, int key) {
    t_no * aux = raiz;

    while (aux != nil) {
        if (key < aux->key) {
            aux = aux->esquerda;
        } else if (key > aux->key) {
            aux = aux->direita;
        } else if (key == aux->key) {
            return aux;
        }
    }

    return NULL;
}

t_no * remover(t_no * raiz, int key) {
    t_no * z = search(raiz, key);
    if(z == NULL) {
        printf("No nao encontrado\n");
        return raiz;
    }
    t_no * y;
    t_no * x;
    if (z->esquerda == nil || z->direita == nil) {
        y = z;
    } else {
        y = sucessor(z);
    }

    if (y->esquerda != nil) {
        x = y->esquerda;
    } else {
        x = y->direita;
    }
    x->pai = y->pai;
    if (y->pai == nil) {
        raiz = x;

    } else {
        if (y == y->pai->esquerda) {
            y->pai->esquerda = x;
        } else {
            y->pai->direita = x;
        }
    }
    if (y != z) {
        z->key = y->key;
    }
    if (y->cor == BLACK) {
        raiz = deleteFixUp(raiz, x);
    }
    return raiz;
}


/*
    Define uma forma de limpar a tela do terminal,
    tanto no Linux quanto no Windows.
 */
#ifdef __linux__
    #define CLEAR() (system("clear"))
#elif _WIN32
    #define CLEAR() (system("cls"))
#endif
#define MSG_LEN 100

/*
    Imprime uma mensagem de cabeçalho
    compartilhada por todas as screens
 */
void print_header();

/*
    Tela de Home
    Permite acessar a screen de criação de B-Tree e a screen Sobre.
    Permite sair da aplicação.
 */
int home_screen();
/*
    Informações básicas sobre o projeto B-Tree e essa DEMO
 */
int about_screen();
/*
    Tela para definir a ordem da B-Tree a ser criada
 */
int run_screen();
/*
    Tela onde as operações INSERÇÃO, REMOÇÃO, PESQUISA, IMPRESSÃO serão executadas
 */
int running_screen();
/*
    Função auxiliar para gerar uma mensagem de "despedida"
 */
void goodbye();
/*
    A ordem das 3 primeiras screens deve ser RUN, ABOUT, EXIT.
    Os valores atribuídos à elas são utilizados na lógica interna do menu.
 */
typedef enum {
	RUN, ABOUT, EXIT, HOME, RUNNING
} screen;

/*
    Indica uma mensagem que será exibida no topo da próxima screen.
    É uma forma de comunicar mensagens ao usuário.
    O valor default (quando não há mensagens para exibir) deve ser o caracter '-'.
 */
char msg[MSG_LEN];

	//ponteiro global
	t_no *a;

/*
    Contador de quantas keys foram inseridas na B-Tree atual.
    É reiniciado sempre que a B-Tree é deletada.
 */
int n_keys;


int home_screen() {
	int opt;
	int n_items = 3;
	// Itens no menu
	char list[3][12] = { "Nova Arvore", "Sobre", "Sair" };

	print_header();

	int i;
	for (i = 0; i < n_items; ++i) {
		printf(" %d) %s\n", i+1, list[i]);
	}
	printf("\n");
	printf(" Digite uma opcao: ");
	scanf("%d%*c", &opt);

	if (opt < 1 || opt > n_items) {
		snprintf(msg, MSG_LEN, " Opcao invalida.");
		return HOME;
	}

	return opt-1;
}

int about_screen() {
	print_header();
	printf( "---------------------------------------------------------\n" \
	        "--       Trabalho apresentado na disciplina AED2       --\n" \
	        "--            ministrada pelo Prof. Edson              --\n" \
	        "--                    UFAM                             --\n" \
	        "---------------------------------------------------------\n");
	printf("\n");

	printf(" Pressione ENTER para voltar ao menu. ");
	getchar();

	return HOME;
}

int run_screen() {
	print_header();

	int order;

	printf("...Iniciando ARVORE");
	//unica criaçao de uma rubro negra
	nil = (t_no*)malloc(sizeof(t_no));
	a = nil;
	printf("\n");
	printf(" Pressione ENTER para continuar. ");
	getchar();


	return RUNNING;
}

int running_screen() {
	int n_items;
	char xxx[4][15] = { "Inserir", "Buscar","Remover","Imprimir"};
	n_items=4;

	int opt;

	print_header();

	int i;
	for (i = 0; i < n_items; ++i) {
		printf(" %d) %s\n", i+1, xxx[i]);
	}
	printf(" 0) Sair");
	printf("\n");
	printf(" Digite uma opcao: ");
	scanf("%d%*c", &opt);
	printf("\n");
	int op;
	int num;
	t_no *aux;
	switch (opt) {
	case 1:                 // Inserir
			printf(" Digite um numero:");
			scanf("%d", &num);
			a = insert(a,num);
			snprintf(msg, MSG_LEN, " Insercao realizada com sucesso.");
			n_keys++;
			break;
	case 2:                 // Consultas
			 printf(" Digite um numero: ");
			scanf("%d",&num);
			aux = search(a,num);

			if(aux!=NULL)
						printf("O numero %d esta contido na arvore\n",num);
			else
						printf("O numero %d nao esta contido na arvore\n",num);

					printf(" Pressione ENTER para continuar. ");
					getchar();
					getchar();
		break;
	case 3:                 //Remover
			printf(" Digite um numero: ");
			scanf("%d",&num);
			a = remover(a,num);
			printf(" Pressione ENTER para continuar. ");
			getchar();
			getchar();
		break;
	case 4:
			in_order(a);
			printf("\n\n");
			printf(" Pressione ENTER para continuar. ");
			getchar();
		break;
	case 0:                 // Voltar/Sair
		snprintf(msg, MSG_LEN, " Voltando ao menu");
		goodbye();
		return HOME;
		break;
	default:
		snprintf(msg, MSG_LEN, " Opcao invalida");
		return RUNNING;
		break;
	}

	return RUNNING;
}

void print_header() {
	CLEAR();
	printf( "---------------------------------------------------------\n" \
	        "--                 Arvore-Rubro-Negra                  --\n" \
	        "--                  Desenvolvido por:                  --\n" \
	        "--      Jean Cleison Braga Guimaraes - 21601227        --\n" \
	        "---------------------------------------------------------\n");

	printf("\n");

	if (strcmp(msg, "-")) {
		printf("%s\n\n", msg);
		snprintf(msg, MSG_LEN, "-");
	}
}

void goodbye() {
	// Apenas uma animação simples de "loading"
	print_header();
	int i;
	printf(" ");
	for (i = 0; i < 4; ++i) {
		printf(".");
		fflush(stdout);
		usleep(500000);
	}

	CLEAR();
}

int main() {
	screen next_screen = HOME;
	snprintf(msg, MSG_LEN, "-");
	n_keys = 0;

	while (next_screen != EXIT) {
		switch (next_screen) {
		case HOME:
			next_screen = home_screen();
			break;
		case ABOUT:
			next_screen = about_screen();
			break;
		case RUN:
			next_screen = run_screen();
			break;
		case RUNNING:
			next_screen = running_screen();
			break;
		case EXIT:
			break;
		}
	}

	snprintf(msg, MSG_LEN, " Saindo da aplicacao");
	goodbye();

	return 0;
}
