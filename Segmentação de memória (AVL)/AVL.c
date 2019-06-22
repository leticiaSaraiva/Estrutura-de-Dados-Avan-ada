#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct arvore{
    int n, fb;
    struct arvore *esquerda;
    struct arvore *direita;
    struct arvore *pai;
};

struct arvore *raiz = NULL;

void menu();
void insere(int n);
int remove_p(int n);
void remover(struct arvore*);
struct arvore* menor_direita(struct arvore*);
struct arvore* buscar(int);
int altura(struct arvore*);
struct arvore* balanceia(struct arvore*);
struct arvore* rot_direita(struct arvore*);
struct arvore* rot_esquerda(struct arvore*);


int main(){
	struct arvore *no = NULL;
	
	int i, j, k, a, rem, erro = 0;
	int controle = 0;
	char str1[30];
	char str2[30];
	char inst = 'a';
	char instrucoes[60][60];
	int r[60][2];
	
	//Leitura das instruções
	for (i = 0; inst != 'E' && i <= 60; i++){
		scanf(" %[^\n]s", instrucoes[i]);
		inst = instrucoes[i][0];	
	}
	int qtd = i;
	int aloca = 0;
	
	//Análise das instruções
	for(i = 0; i <= qtd; i++){
		switch(instrucoes[i][0]){
			case 'A':
				
				strcpy(str1,"\0");
				strcpy(str2,"\0");
				for(j = 2; instrucoes[i][j] != ' ' && instrucoes[i][j] != '\n'; j++){
					str1[j-2] = instrucoes[i][j];
					
					controle = 1;
				}
				
				str1[j-1-controle] = '\0';
				controle = 0;
				
				for(k = j+1; instrucoes[i][k] != '\0' && instrucoes[i][k] != '\n'; k++){
					str2[k-j-1] = instrucoes[i][k];
					
				}
				str2[k-j-1] = '\0';
			
				r[aloca][0] = atoi(str1);
				r[aloca][1] = atoi(str2);
				
				aloca += 1;
			
				int result = atoi(str1) + atoi(str2) - 1;
				for(a = atoi(str1); a <= result ; a++){
					insere(a);
				}
				
				break;
				
			case 'D': 
				strcpy(str1,"\0");
				
				for(j = 2; instrucoes[i][j] != '\0'&& instrucoes[i][j] != '\n'; j++){
					str1[j-2] = instrucoes[i][j];
					controle = 1;
				}
				
				str1[j-1 - controle] = '\0';
				controle = 0;
				
				
				for(k=0; k<aloca; k++){
					if(r[k][0] == atoi(str1)){
						for(int a = r[k][0]; a < r[k][0] + r[k][1]; a++){
							rem = remove_p(a);
							if(rem == -1){
								break;
							}
						}
						break;
					}
				}
				if(k == aloca || rem == -1){
					erro = 1;
					printf("%d ", i+1);
				}
			
				strcpy(str1,"\0");
				
				break;
				
			case 'G':
				strcpy(str1,"\0");
				for(j = 2; instrucoes[i][j] != '\0' && instrucoes[i][j] != '\n'; j++){
					str1[j-2] = instrucoes[i][j];
					controle = 1;
				}
				str1[j-1 - controle] = '\0';
				controle = 0;
	
				no = buscar(atoi(str1));
				
				if(no == NULL){
					
					erro = 1;
					printf("%d ", i+1);
				}
				strcpy(str1,"\0");
				break;
				
			default:
				break;
		}
	}
	if(erro == 0){
		printf("0\n");
	}
}

void insere(int n){
    struct arvore *aux1 = NULL, *aux2 = NULL;
    
    if(!raiz){
        raiz = (struct arvore*) malloc(sizeof(struct arvore));

        if(!raiz){
            printf("Erro ao alocar =(\n");
            exit(1);
        }

        raiz->n = n;
        raiz->esquerda = NULL;
        raiz->direita = NULL;
        raiz->pai = NULL;

        raiz->fb = 0;
    }

    else{
        aux1 = raiz;
        aux2 = aux1;

        while(aux2){
            if(n < aux2->n){
                aux2 = aux2->esquerda;

                if(!aux2){
                    aux1->esquerda = (struct arvore*) malloc(sizeof(struct arvore));

                    if(!aux1->esquerda){
                        printf("Erro ao alocar =(\n");
                        exit(1);
                    }

                    aux2 = aux1->esquerda;

                    aux2->esquerda = NULL;
                    aux2->direita = NULL;
                    aux2->pai = aux1;
                    aux2->n = n;
                    break;
                }
                else{
                    aux1 = aux2;
                }
            }

            else{
                aux2 = aux2->direita;

                if(!aux2){
                    aux1->direita = (struct arvore*) malloc(sizeof(struct arvore));

                    if(!aux1->direita){
                        printf("Erro ao alocar =(\n");
                        exit(1);
                    }

                    aux2 = aux1->direita;

                    aux2->esquerda = NULL;
                    aux2->direita = NULL;
                    aux2->pai = aux1;
                    aux2->n = n;
                    break;
                }

                else{
                    aux1 = aux2;
                }
            }
        }
    }

    if(aux2){
        while(aux2){
            aux2->fb = altura(aux2->direita) - altura(aux2->esquerda);

            if(aux2->fb > 1 || aux2->fb < -1){
                aux2 = balanceia(aux2);
            }

            aux2 = aux2->pai;
        }
    }
}

int remove_p(int n){
    struct arvore *aux;
   
    aux = buscar(n);

    if(aux != NULL){
        remover(aux);
        return 0;
    }else{
		return -1;
	}
}

void remover(struct arvore *aux){
    struct arvore *aux2, *salva = aux->pai;

    if(aux != raiz){
        if(!aux->esquerda && !aux->direita){
            if(aux->n < aux->pai->n){
                aux->pai->esquerda = NULL;
            }
            else{
                aux->pai->direita = NULL;
            }

            free(aux);
        }

        else if(!aux->direita){
            if(aux->n < aux->pai->n){
                aux->pai->esquerda = aux->esquerda;
            }
            else{
                aux->pai->direita = aux->esquerda;
            }
            aux->esquerda->pai = aux->pai;
            free(aux);
        }

        else if(!aux->esquerda){
            if(aux->n < aux->pai->n){
                aux->pai->esquerda = aux->direita;
            }
            else{
                aux->pai->direita = aux->direita;
            }
            aux->direita->pai = aux->pai;
            free(aux);
        }

        else{
            aux2 = menor_direita(aux->direita);
            aux->n = aux2->n;

            remover(aux2);
        }
    }

    else{
        if(!aux->esquerda && !aux->direita){
            free(aux);
            raiz = NULL;
        }

        else if(!aux->direita){
            aux->esquerda->pai = NULL;
            raiz = aux->esquerda;
            free(aux);
        }

        else if(!aux->esquerda){
            aux->direita->pai = NULL;
            raiz = aux->direita;
            free(aux);
        }

        else{
            aux2 = menor_direita(aux->direita);
            aux->n = aux2->n;

            remover(aux2);
        }
    }

    if(salva){
        while(salva){
            salva->fb = altura(salva->direita) - altura(salva->esquerda);

            if(salva->fb > 1 || salva->fb < -1){
                salva = balanceia(salva);
            }

            salva = salva->pai;
        }
    }
}

struct arvore* menor_direita(struct arvore *aux){
    while(aux->esquerda){
        aux = aux->esquerda;
    }

    return aux;
}


struct arvore* buscar(int n){
    struct arvore *aux;

    aux = raiz;
    

    while(aux && aux->n != n){		
        if(n < aux->n){
            aux = aux->esquerda;
        }

        else{
            aux = aux->direita;
        }
    }

    return aux;
}

int altura(struct arvore *aux){
    if(!aux){
        return 0;
    }

    int ae = altura(aux->esquerda);
    int ad = altura(aux->direita);

    return (ae > ad) ? ae + 1: ad + 1;
}

struct arvore* rot_direita(struct arvore *aux){
    struct arvore *esq = aux->esquerda;

    aux->esquerda = esq->direita;
    esq->direita = aux;

    if(aux->esquerda){
        aux->esquerda->pai = aux;
    }

    esq->pai = aux->pai;
    aux->pai = esq;

    if(aux == raiz){
        raiz = esq;
    }

    if(esq->pai){
        if(esq->n < esq->pai->n){
            esq->pai->esquerda = esq;
        }
        else{
            esq->pai->direita = esq;
        }
    }

    esq->fb = altura(esq->direita) - altura(esq->esquerda);
    aux->fb = altura(aux->direita) - altura(aux->esquerda);

    return esq;
}

struct arvore* rot_esquerda(struct arvore *aux){
    struct arvore *dir = aux->direita;

    aux->direita = dir->esquerda;
    dir->esquerda = aux;

    if(aux->direita){
        aux->direita->pai = aux;
    }

    dir->pai = aux->pai;
    aux->pai = dir;

    if(aux == raiz){
        raiz = dir;
    }

    if(dir->pai){
        if(dir->n < dir->pai->n){
            dir->pai->esquerda = dir;
        }
        else{
            dir->pai->direita = dir;
        }
    }

    dir->fb = altura(dir->direita) - altura(dir->esquerda);
    aux->fb = altura(aux->direita) - altura(aux->esquerda);
    return dir;
}

struct arvore* balanceia(struct arvore *aux){
    if(aux->fb < -1){
        if(aux->esquerda->fb > 0){
            aux->esquerda = rot_esquerda(aux->esquerda);
        }

        aux = rot_direita(aux);
    }

    else if(aux->fb > 1){
        if(aux->direita->fb < 0){
            aux->direita = rot_direita(aux->direita);
        }

        aux = rot_esquerda(aux);
    }

    return aux;
}
