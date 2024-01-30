#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

typedef struct tipochave{
    int valor; //Valor do produto, chave da árvore
    int rotulo; //Local para armazenar a passagem do robô
    int produto; //Local para armazenar o produto
}tipochave;

typedef struct aux{
    tipochave chave; //Quadrante
    struct aux *esq, *dir;
}no;

typedef no * pont;

typedef struct robozinho{
    int energia; //Quantidade de energia do robô;
    int informa; //Informar se o robô achou o local do produto
}robozinho;

pont incializa(){ //Inicializar a árvore
    return NULL;
}

pont criarNovoNo(tipochave ch){ //Criar um novo nó para a árvore
    pont novoNo = (pont) malloc(sizeof(no));
    novoNo->esq = NULL;
    novoNo->dir = NULL;
    novoNo->chave = ch;
    return novoNo;
}

pont adiciona (pont raiz, pont no){ //Adicionar produtos na árvore
    if(raiz == NULL){
        return no;
    }
    if(no->chave.valor < raiz->chave.valor){ //Produtos armazenador por ordem de valor
        raiz->esq = adiciona(raiz->esq, no);
    }else{
        raiz->dir = adiciona(raiz->dir, no);
    }
    return raiz;
}

void exibirArvoreEmOrdem(pont raiz){ //Imprimir a árvore em ordem
    if(raiz !=NULL){
        exibirArvoreEmOrdem(raiz->esq);
        printf("\t%i : P(%i)R(%i)\n", raiz->chave.valor, raiz->chave.produto,
        raiz->chave.rotulo);
        exibirArvoreEmOrdem(raiz->dir);
    }
}

robozinho procura_adiciona(tipochave ch, pont raiz, robozinho r){ //2
    if(raiz == NULL){
        return r;
    }
    r.energia++; //Energia do robô
    if(raiz->chave.valor == ch.valor){
        raiz->chave.rotulo = 1; //O robô passou nessa posição
        raiz->chave.produto = 1; //Adicionando o produto
        r.informa = 1; //Informando que o robô achou o produto;
        return r;
    }
    if(raiz->chave.valor > ch.valor){
        raiz->chave.rotulo = 1; //O robô passou nessa posição
        return procura_adiciona(ch, raiz->esq, r);
    }else{
        raiz->chave.rotulo = 1; //O robô passou nessa posição
        return procura_adiciona(ch, raiz->dir, r);
    }
}

robozinho procura_produto(tipochave ch, pont raiz, robozinho r){ //4
    if(raiz == NULL){
        return r;
    }
    r.energia++; //Energia do robô
    if(raiz->chave.valor == ch.valor){
        raiz->chave.rotulo = 1; //O robô passou nessa posição
        if(raiz->chave.produto == 1){//Informando que o robô que tem produto no local;
            r.informa = 1;
        }
        return r;
    }
    if(raiz->chave.valor > ch.valor){
        raiz->chave.rotulo = 1; //O robô passou nessa posição
        return procura_produto(ch, raiz->esq, r);
    }else{
        raiz->chave.rotulo = 1; //O robô passou nessa posição
        return procura_produto(ch, raiz->dir, r);
    }
}

void imprime_produto(pont raiz, tipochave ch){ //4
    if(raiz !=NULL){
        imprime_produto(raiz->esq, ch);
        if(raiz->chave.valor == ch.valor){
            printf("\n\t%i : P(%i)R(%i)\n", raiz->chave.valor, raiz->chave.produto,
            raiz->chave.rotulo);
        }
        imprime_produto(raiz->dir, ch);
    }
}

void passou_robo(pont raiz){ //5 a) e 9 locais onde o robô passou
    if(raiz !=NULL){
        passou_robo(raiz->esq);
        if(raiz->chave.rotulo == 1){
            printf("\t%i : P(%i)R(%i)\n", raiz->chave.valor, raiz->chave.produto,
            raiz->chave.rotulo);
        }
        passou_robo(raiz->dir);
    }
}

int soma_valores(pont raiz){ //5 b) Somar os valores dos produtos //7 Somar todos os produtos do estoque no campo de atuação do robo
    if(!raiz){
        return 0;
    }
    else if(raiz->chave.produto == 0){
        return soma_valores(raiz->esq) + 0
        + soma_valores(raiz->dir);
    }else{
        return soma_valores(raiz->esq) + raiz->chave.valor
        + soma_valores(raiz->dir);
    }
}

void atuacao_robo(robozinho robo, pont raiz){ //5
    int valor;
    //5 a)
    printf("\n\t|Locais onde o robo passou|\n");
    passou_robo(raiz);
    //5 b)
    valor = soma_valores(raiz);
    printf("\tSoma dos valores das mercadorias: %i\n", valor);
    //5 c)
    printf("\tEnergia gasta pelo robo: %i\n\n", robo.energia);
}

int quadrante_ocupado(pont raiz){ //6 a)
    if(!raiz){
        return 0;
    }
    else if(raiz->chave.produto == 1){ //Produto no campo de atuação do robô
        return quadrante_ocupado(raiz->esq) + 1
        + quadrante_ocupado(raiz->dir);
    }else{
        return quadrante_ocupado(raiz->esq) + 0
        + quadrante_ocupado(raiz->dir);
    }
}

int quadrante_vazio(pont raiz){ //6 b)
    if(!raiz){
        return 0;
    }
    else if(raiz->chave.produto == 0){ //Espaço vazio no campo de atuação do robô
        return quadrante_vazio(raiz->esq) + 1
        + quadrante_vazio(raiz->dir);
    }else{
        return quadrante_vazio(raiz->esq) + 0
        + quadrante_vazio(raiz->dir);
    }
}

void numero_quadrante(pont raiz){ //6
    int valor;
    printf("\n\t|Campo de atuacao do robo|");
    valor = quadrante_ocupado(raiz); //6 a)
    printf("\n\tNumero de quadrantes ocupados: %i\n", valor);
    valor = quadrante_vazio(raiz);
    printf("\tNumero de quadrantes vazios: %i\n\n", valor);
}

int soma_quadrante_vazio(pont raiz){ //8 Soma dos valores dos quadrantes vazios no
    campo de atuação do robô
    if(!raiz){
        return 0;
    }
    else if(raiz->chave.produto == 0){
        return soma_quadrante_vazio(raiz->esq) + raiz->chave.valor
        + soma_quadrante_vazio(raiz->dir);
    }else{
        return soma_quadrante_vazio(raiz->esq) + 0
        + soma_quadrante_vazio(raiz->dir);
    }
}

void somatorio(pont raiz){ //7 //8
    int valor;
    printf("\n\t|Somatorio de todos os quadrantes ocupados|");
    valor = soma_valores(raiz);
    printf("\n\tValor total: %i\n", valor);
    printf("\n\t|Somatorio de todos os quadrantes vazios|");
    valor = soma_quadrante_vazio(raiz);
    printf("\n\tValor total: %i\n", valor);
}

void zerar_caminho_robo(pont raiz){
    if(raiz != NULL){
        zerar_caminho_robo(raiz->esq);
        if(raiz->chave.rotulo == 1){
        raiz->chave.rotulo = 0;
        }
        zerar_caminho_robo(raiz->dir);
    }
}

int imprime_rota(tipochave ch, pont raiz){ //Imprimir a roto do robô até as folhas
    if(raiz == NULL){
        return 0;
    }
    if(raiz->chave.valor == ch.valor){
        printf("\t%i : P(%i)R(%i)\n", raiz->chave.valor, raiz->chave.produto,
        raiz->chave.rotulo);
        return 0;
    }
    else if(raiz->chave.valor > ch.valor){
        printf("\t%i : P(%i)R(%i)\n", raiz->chave.valor, raiz->chave.produto,
        raiz->chave.rotulo);
    }else{
        printf("\t%i : P(%i)R(%i)\n", raiz->chave.valor, raiz->chave.produto,
        raiz->chave.rotulo);
        return imprime_rota(ch, raiz->dir);
    }
}

int confere_rota(tipochave ch, pont raiz, int resul){ //4
    if(raiz == NULL){
        return resul;
    }
    if(raiz->chave.rotulo == 1){
    resul++;
    }
    if(raiz->chave.valor == ch.valor){
        return resul;
    }
    else if(raiz->chave.valor > ch.valor){
        return confere_rota(ch, raiz->esq, resul);
    }else{
        return confere_rota(ch, raiz->dir, resul);
    }
}

int folhas(pont raiz, pont raiz_2){
    int resul;
    tipochave ch;
    if(raiz == NULL){
    return 0;
    }
    else{
        if(raiz->esq == NULL && raiz->dir == NULL){
            ch = raiz->chave;
            resul = 0;
            resul = confere_rota(ch, raiz_2, resul);
            if(resul == 1){
                resul =imprime_rota(ch, raiz_2);
                printf("\n\n");
            }
        }else{
            return folhas(raiz->esq, raiz_2) + folhas(raiz->dir, raiz_2);
        }
    }
}
void main(){

    pont r = incializa(); //Iniciar a árvore
    tipochave ch;
    pont no;
    robozinho robo;
    robo.energia = 0;
    int i, j, tam, quant = 1, vet[50], repet, cont;
    char cls;

    printf("\n\tDigite o tamanho do campo de atuacao (x e y) do robo: ");
    scanf("%d", &tam);

    for(i=0; i < tam; i++){
        quant = quant * 2;
    }

    quant = quant - 1;
    printf("\n\tQuantidade de quadrantes: %i\n", quant);
    printf("\n\tAperte qualquer tecla para continuar: "); //Limpar a tela
    scanf("%s", &cls);
    system("cls");
    srand(time(NULL));

    printf("\n\t|Numeros gerados|\n\t");

    for(i=0; i < quant; i++){
        cont = 0;
        do{ //Conferir se não gerou um número repetido
            ch.valor = rand() % 100;
            repet = 0;
            for(j=0; j<50; j++){
                if(vet[j] == ch.valor){
                    repet = 1;
                }
            }
            if(repet == 0){
                vet[i] = ch.valor;
                cont = 1;
            }
        }while(cont != 1);
        ch.rotulo = 0;
        ch.produto = 0;
        no = criarNovoNo(ch);
        r = adiciona (r,no);
        printf("%i ", ch.valor);
    }

    printf("\n\n");
    exibirArvoreEmOrdem(r);

    //2
    i = 0;
    printf("\n\t|Questao 02|\n");

    while(i != 3){ //Usar 2 exemplos para a questão 2
        printf("\n\tDigite o valor do produto para adicionar: "); //Valor para procurar o local no
        estoque
        scanf("%d", &ch.valor);
        system("cls");
        robo.informa = 0;
        robo = procura_adiciona(ch, r, robo); //procurar o local do produto e adicionar se achar
        if(robo.informa == 1){ //Resultado do procedimento:
            printf("\n\tLocal encontrado e produto adicionado!");
            printf("\n\tEnergia gasta: %i\n\n", robo.energia);
        }else{
            printf("\n\tNAO existe local reservado!");
            printf("\n\tEnergia gasta: %i\n\n", robo.energia);
        }
        i++;
        exibirArvoreEmOrdem(r);
    }

    printf("\n\t|Questao 05|\n"); //Atuação do robô
    atuacao_robo(robo, r); //5
    printf("\n\tAperte qualquer tecla para continuar: "); //Limpar a tela
    scanf("%s", &cls);
    system("cls");
    zerar_caminho_robo(r); //Zerar o caminho do robô para utilizar outras funcionalidades
    robo.energia = 0; //Zerar energia do robô para utilizar outras funcionalidades
    exibirArvoreEmOrdem(r);
    i = 0;
    printf("\n\t|Questao 04|\n");

    while(i != 3){ //Usar 2 exemplos para a questão 4
        printf("\n\tDigite o valor do produto para procurar: "); //Valor para procurar o produto
        no estoque
        scanf("%d", &ch.valor);
        system("cls");
        robo.informa = 0;
        robo = procura_produto(ch, r, robo);
        if(robo.informa == 1){ //Resultado do procedimento:
            printf("\n\tProduto encontrado!\n");
            imprime_produto(r, ch); //Imprimir o produto
            printf("\tEnergia gasta: %i\n\n", robo.energia);
        }else{
            printf("\n\tProduto NAO encontrado!");
            printf("\n\tEnergia gasta: %i\n\n", robo.energia);
        }
        i++;
        exibirArvoreEmOrdem(r);
    }
    
    printf("\n\n\t|Questao 09|\n"); //Quadrantes onde robô passou na busca
    printf("\t|Quadrantes onde robo passou na busca|\n");
    passou_robo(r);
    printf("\n\t|Questao 10|\n"); //Rota até as folhas que o robô não passou na busca
    printf("\t|Rota da raiz ate as folhas que o robo ainda nao passou na busca|\n");
    i = folhas(r, r);
    printf("\n\tAperte qualquer tecla para continuar: "); //Limpar a tela
    scanf("%s", &cls);
    system("cls");
    exibirArvoreEmOrdem(r);
    printf("\n\t|Questao 06|\n"); //Quadrantes vagos e ocupados
    numero_quadrante(r); //6
    printf("\n\t|Questoes 07 e 08|\n"); //Soma dos produtos
    somatorio(r);
}
