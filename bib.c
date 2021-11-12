#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // srand.
#include <ctype.h> // toupper, tolower, strlwr.
#include <conio.h> // getch e getche.

#include "bib.h" // Biblioteca prÃ³pria.

void menuPrincipal();

void cadastro(){
	system("cls");
	FILE *pontArq; // Cria a variÃ¡vel ponteiro para arquivo.
	pontArq = fopen("dados.txt", "a"); // "a": grava novos dados sem sobrescrever os anteriores.
	
	char login[20], senha[20];
	printf("\nInforme o login: ");
	scanf(" %s", login);
	printf("Informe a senha: ");
	scanf(" %s", senha);
	
	while((strlen(login) > 20) || (strlen(senha) > 20)){
		printf("\nO login e/ou a senha excedem o tamanho permitido. Digite novamente.\n");
		printf("\nInforme o login: ");
		scanf("%s", login);
		printf("\nInforme o login: ");
		scanf("%s", senha);
	}
		
	fprintf(pontArq, "%s\n", strlwr(login)); // Armazena a string no arquivo. strlwr salva a string sempre como minÃºscula.
	fprintf(pontArq, "%s\n", senha); // Armazena a string no arquivo.
		
	fclose(pontArq); // Fecha o arquivo.
		
	printf("\nA conta foi criada.\n");
	printf("\n\nPressione qualquer tecla para continuar.");
	getch();
	menuPrincipal();
}

int verificaLogin(FILE *pontArq, char login[]) {
	char senha[20], string1[20], string2[20];
	int loginCorreto = 0;
	strlwr(login); // Converte o que foi digitado, se maiÃºsculo, para minÃºsculo.
	printf("Informe a senha: ");
	scanf(" %s", senha);    
				   
	while((fscanf(pontArq, "%s%s", string1, string2)) == 2){ // LÃª a cada duas linhas (login, senha).
		if((strcmp(login, string1) == 0) && (strcmp(senha, string2) == 0)){ // Se == 0, as strings sÃ£o iguais.
			loginCorreto = 1;
			return loginCorreto;
		} 
		else{
			loginCorreto = 0;
		}
	}								
	fclose(pontArq);
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

void mostrarMenu(){
	printf("\nSeja bem-vindo ao quiz!\n\n");
	printf("O que voce deseja?\n");
	printf("\t 1. Jogar\n");
	printf("\t 2. Ver pontuacoes\n");
	printf("\t 3. Ver creditos\n");
	printf("\t 4. Sair\n");
	printf("\nInforme a opcao desejada: ");
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

void menuTemas(char temas[3][15]){
	system("cls");
	
	printf("\nEm qual dos temas abaixo voce deseja ser desafiado?\n");
	
	for(int i=0; i<3; i++)
		printf("\t%d. %s\n", i+1, temas[i]);
	
	printf("\nPara voltar ao menu principal, digite 0.\n");
	printf("\nInforme a opcao desejada: ");
}

int lerTema(){ // LÃª o dado informado pelo usuÃ¡rio e retorna a opÃ§Ã£o lida.
	int opcao;
	scanf("%d", &opcao);
	
	while(opcao < 0 || opcao > 3){ // Repete a frase do printf até que o usuário informe uma opção válida (0, 1, 2 ou 3).
		printf("\nEscolha uma opcao valida.\n");
		scanf("%d", &opcao);	
	}
	return opcao;
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

typedef struct lista
{
	char pergunta[120];
	char alternativa[4][80];
	char correta;
	struct lista *prox;
} lista;

lista *nova;

void lerPerguntas(lista *p, char temas[3][15], int temaEscolhido);
void mostrarPergunta(lista *p, int pergunta);

int pontos, acertos, parcial, erros;
	
int jogar(char temas[3][15], int temaEscolhido){
	system("cls");
	lista *ini;
	ini=(lista*)malloc(sizeof(lista));
	ini->prox = NULL;
	lerPerguntas(ini, temas, temaEscolhido);

	int ordemPerguntas[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int n = sizeof(ordemPerguntas) / sizeof(ordemPerguntas[0]);

	srand (time(NULL)); 

	for(int i = n - 1; i > 0; i--){
		int j = rand() % (i + 1);  
		int temp = ordemPerguntas[i];
		ordemPerguntas[i] = ordemPerguntas[j];
		ordemPerguntas[j] = temp;
	}

	pontos = 0, acertos = 0, parcial = 0, erros = 0;
	for (int n=0; n<10;n++){
		printf("\nPergunta %d:\n", n+1);
		mostrarPergunta(ini, ordemPerguntas[n]);
	}
	printf("\nPontuacao final: %d\n", pontos);
	printf("Voce acertou %d perguntas com a primeira chance e %d com a segunda.\n", acertos, parcial);
	printf("Total de erros: %d.\n", erros);
	printf("\n\nPressione qualquer tecla para continuar. ");
	getch();
	return pontos; 
}

//lerPerguntas(ini, temas, temaEscolhido);
void lerPerguntas(lista *p, char temas[3][15], int temaEscolhido){
	char lixo;
	lista *busca;
	FILE *arquivo;
	
	// A funÃ§Ã£o abaixo recebe duas variÃ¡veis: o vetor TEMAS e o int TEMAESCOLHIDO.
	// Tema escolhido retorna um index de Temas para saber o que deverÃ¡ ser
	// aberto ao ler temas[temaEscolhido-1].
	// temas = {"Esportes", "Geografia", "InformÃ¡tica"}
	char abrir[15];
	strcpy(abrir, temas[temaEscolhido-1]);
	strcat(abrir, ".txt");
	// strcpy e strcat fazem => temas[temaEscolhido-1] + ".txt";
	
	if ((arquivo = fopen(abrir, "r")) == NULL){
		printf("\nErro. Tente novamente mais tarde.");
		exit(1);
	}

    while(!feof(arquivo)){
        for (busca = p; busca->prox != NULL; busca= busca->prox);
        nova = (lista*)malloc(sizeof(lista));

        fgets(nova->pergunta, 120, arquivo);
        fgets(nova->alternativa[0], 80, arquivo);
        fgets(nova->alternativa[1], 80, arquivo);
        fgets(nova->alternativa[2], 80, arquivo);
        fgets(nova->alternativa[3], 80, arquivo);
        fscanf(arquivo, "%s", &nova->correta);
        fscanf(arquivo, "%c", &lixo);
        nova->prox = NULL;
        busca->prox = nova;
    }		
    fclose(arquivo);		
}

void mostrarPergunta(lista *p, int pergunta){
    int num = 0;
    lista *i;

    i = p->prox;
    
    while (num != pergunta){
        if (i != NULL)
        {
            i = i->prox;
        }
        num++;
    }
    
    printf("%s\n", i->pergunta);
    printf("%s", i->alternativa[0]);
    printf("%s", i->alternativa[1]);
    printf("%s", i->alternativa[2]);
    printf("%s", i->alternativa[3]);

    printf("\nInforme a resposta: ");
    char alternativa;
	int segundaChance = 0;

	resposta:
        
	alternativa = getch();
	printf("%c", alternativa);
    if (tolower(alternativa) == i->correta) 
    {
        printf("\n\nResposta correta!\n");
        if (segundaChance == 0){
			pontos += 10;
			acertos++;
        } 
        else{
			pontos += 5;
			parcial++;
        }
    }
    else{
        if (segundaChance == 0){
            printf("\n\nResposta incorreta. Voce possui mais uma chance: ");
            segundaChance++;
            goto resposta;
        }
		erros++;
        printf("\n\nResposta incorreta.\n");
        printf("A resposta correta era %c.\n", toupper(i->correta));
    }
    printf("\n\nPressione qualquer tecla para continuar.");
	getch();
    system("cls");
}
	
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

void salvarPontos(int pontos){
	printf("\n\nAgora vamos salvar sua pontuacao...");
	printf("\n\nSe voce estiver no TOP 5, seus pontos aparecerao no item 2 do menu principal.");
	
	typedef struct{ 
		char nome[5];
		int placar;
	} jogador;
	
	FILE *pontArq;
	char info[50];
	int i;
	char aux[50];

	jogador p[5]; 

	pontArq = fopen("Placar.txt", "r"); 
	
	if (pontArq == NULL) 
		printf("\n\nErro. Tente novamente mais tarde.");
	
	else{  
		for(i=0; i<5 && !feof(pontArq); i++){   
			fscanf(pontArq, "%s %d", p[i].nome, &p[i].placar); 
		}       

		printf("\n\nInforme o nome a ser salvo com a pontuacao (quatro caracteres): "); 
		scanf("%s",aux);
		for(i = 0; i < 4; i++){
			p[5].nome[i] = aux[i];
		}
		p[5].nome[4] = '\0';
		p[5].placar = pontos;
		
		fclose(pontArq); 
		
		int j, tempPontos, numJogadores = 6; 
		char tempNome[20];
		pontArq = fopen("Placar.txt", "w"); 

		for (i = 0; i < numJogadores; i++){
			for (j=0; j < (numJogadores - i - 1); j++){
				if (p[j].placar > p[j+1].placar){
					tempPontos = p[j].placar;
					strcpy(tempNome,p[j].nome);

					p[j].placar = p[j+1].placar;
					strcpy(p[j].nome,p[j+1].nome);

					p[j+1].placar = tempPontos;
					strcpy(p[j+1].nome,tempNome);
				}
			}
		}

		for(i=numJogadores-1; i>0; i--){
			fprintf(pontArq, "%s %03d\n", p[i].nome, p[i].placar); 
		}
		fclose(pontArq); 
		
		printf("\nPontuacao salva!\n");
		printf("\nPressione qualquer tecla para continuar.");
		getch();
	}
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

void pontuacoes(){
	system("cls");
	
	FILE *pontArq; // Cria a variÃ¡vel ponteiro para arquivo.
	char info[10];
	int trava = 0; // VariÃ¡vel de "trava" para ler somente atÃ© a quinta pontuaÃ§Ã£o.
	pontArq = fopen("Placar.txt", "r"); // "r": lÃª o que estÃ¡ armazenado no arquivo.
		
	printf("TOP 5 ENTRE AS PONTUACOES\n\n");
	if (pontArq == NULL) // Testa se o arquivo existe.
		printf("\n\nErro. Tente novamente mais tarde.");
	else
		while(((fgets(info, sizeof(info), pontArq))!= NULL) && trava < 5){ // LÃª e imprime o arquivo atÃ© a linha 5.
			printf("\t %s", info);
			trava++;
		};
	fclose(pontArq); // Fecha o arquivo	
	
	printf("\n\nPressione qualquer tecla para voltar.");
	getch();
	system("cls");
	menuPrincipal();
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

void creditos() {
	system("cls");
	
	FILE *pontArq; // Cria a variÃ¡vel ponteiro para arquivo.
	char info[20];
	pontArq = fopen("Creditos.txt", "r"); // "r": lÃª o que estÃ¡ armazenado no arquivo.
 
	if(pontArq == NULL)
		printf("Erro, nao foi possivel abrir o arquivo\n"); // Testa se o arquivo existe.
	else
		while((fgets(info, sizeof(info), pontArq))!= NULL){ // LÃª e imprime o arquivo.
			printf("%s", info);
		};
	fclose(pontArq); // Fecha o arquivo
	 
	printf("\nPressione qualquer tecla para voltar.");
	getch();
	system("cls");
	menuPrincipal();
}
