#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "bib.h"

void menuPrincipal();

FILE *pontArq; // Cria a variável ponteiro para arquivo.

int main(){
	//o programa tinha um bug referente a opção inserida pelo usuário
	//o bug foi resolvido posteriormente à gravação do vídeo
	//diferenças mínimas podem ser encontradas entre o código do vídeo e o atual
	char op;
	pontArq = fopen("Dados.txt", "a"); // "a": grava novos dados sem sobrescrever os anteriores.
	
	if (pontArq == NULL){
		printf("\nErro. Tente novamente mais tarde.");
		return 1;	
	} 
	else{
		printf("\nSeja bem-vindo(a)!\n\nEntre ou cadastre-se.\n");
		printf("\t1. Criar nova conta.\n");
		printf("\t2. Entrar.\n");
		printf("\nInforme a opcao desejada: ");
		scanf(" %c", &op);
		
		if (op == '1'){
			cadastro();
		} 
		else if (op == '2'){
			char login[20];
			int loginCorreto = 0;
			do{
				pontArq = fopen("Dados.txt", "r"); // "r": lê o que está armazenado no arquivo.
				
				if (pontArq == NULL) {
					printf("\n\nErro. Tente novamente mais tarde.");
					exit(0);	
				}
				printf("\nInforme o login ou digite # para voltar: ");
				scanf(" %s", login);
				
				if (*login != '#') 
					loginCorreto = verificaLogin(pontArq, login);
				else
					main();	
				if (loginCorreto == 1)
					menuPrincipal();
				else
					printf("\nO login e/ou a senha estao incorretos. Tente novamente.\n");
			}
			while (loginCorreto == 0 && *login != '#');
		}
		else{
			printf("Opcao nao reconhecida.\n");
			printf("\nPressione qualquer tecla para tentar novamente.");
			getch();
			system("cls");
			main();
		}
	}
	printf("\nPressione qualquer tecla para continuar.");
	getch();
	return 0;		
}

void menuPrincipal(){
	char escolha;
	char temas[3][15] = {"Esportes", "Geografia", "Informatica"};
	system("cls");
	mostrarMenu();
	scanf(" %c", &escolha);
	switch (escolha){
		case '1':
			menuTemas(temas);
			int opcao = lerTema(); // Chama a função de ler o tema informado.
			if (opcao == 0)
				menuPrincipal();
			else{
				int pontos = jogar(temas, opcao); 
				salvarPontos(pontos);
				menuPrincipal();
			} 
			break;
		case '2': 
			pontuacoes();
			break;
		case '3':
			creditos();
			break;
		case '4':
			system("cls");
			exit(1);
		default:
			printf("Opcao nao reconhecida.\n");
			printf("\nPressione qualquer tecla para tentar novamente.");
			getch();
			menuPrincipal();
	}
}
