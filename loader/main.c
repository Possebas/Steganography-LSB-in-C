#include <stdio.h>
#include <stdlib.h>
#include <string.h>		// Para usar strings

// SOIL � a biblioteca para leitura das imagens
#include "SOIL.h"

// Um pixel RGB
typedef struct {
    unsigned char r, g, b;
} RGB;

// Uma imagem em RGB
typedef struct {
    int width, height;
    RGB* img; //array de pixels
} Img;

int cont = 0; //Responsavel pelo controle do index dos pixels da imagem

// Prot�tipos
void load(char* name, Img* pic);
void encrypt(int password, char mensagem[],int argc, char** argv);
void decrypt(char string[], int number);

long long decimalBin(int num);
char* concatena(char* orig, char* extension);
char* insert_zeros(char* bits);
char* concatenaBin(char string[]);

int Bin(long long num);
void pegaLetra(char* bits, char string[]);

void save_to_image(char string[], int key, Img pic, int argc, char** argv);
void recover_from_image(int key, Img pic);


int testbit(unsigned int x, int bit){
    if((x & (1 << bit)) == 0)
        return 0;
    return 1;
}
unsigned int setbit(unsigned int x, int bit){
    return x | (1 << bit);
}



// Carrega uma imagem para a struct Img
void load(char* name, Img* pic)
{
    int chan;
    pic->img = (unsigned char*) SOIL_load_image(name, &pic->width, &pic->height, &chan, SOIL_LOAD_RGB);
    if(!pic->img)
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
        exit(1);
    }
    printf("Dimensoes da imagem: %d x %d x %d\n", pic->width, pic->height, chan);
}

long long decimalBin (int num){
    long long numBinario = 0;
    int r;
    int i = 1;
    int step = 1;

    while (num != 0){
        r = num % 2;
        step++;
        num /= 2;
        numBinario += r * i;
        i *= 10;
    }
    return numBinario;
}

char* concatena(char* orig, char* extension){
    char* aux = malloc(strlen(orig) + strlen(extension) + 2);
    int count = 0;

    for(int i = 0; i < strlen(orig); i++){
        if(orig[i] != '\0'){
            aux[i] = orig[i];
            count += 1;
        }
    }
    for(int i = 0; i < strlen(extension); i++){
        if (extension[i] != '\0'){
            aux[count] = extension[i];
            count += 1;
        }
    }
    aux[count] = '\0';
    return aux;
}

void encrypt(int pos, char mensagem[], int argc, char** argv)
{
    printf("\n");
    printf("----------CRIPTOGRAFANDO MENSAGEM----------");
    printf("\n");

    for(int i = 0; i < strlen(mensagem); i++){
        char character  = mensagem[i];

        if(character >= 'a' && character <= 'z'){
            character = character + pos;

            if(character > 'z'){
                character = character - 'z' + 'a' - 1;
            }

            mensagem[i] = character;
        }
        else if(character >= 'A' && character <= 'Z'){
            character = character + pos;

            if(character > 'Z'){
                character = character - 'Z' + 'A' - 1;
            }

            mensagem[i] = character;
        }
    }
    printf("\n\nTamanho da Mensagem: %d \n\n", strlen(mensagem) );
    printf("\nMENSAGEM CRIPTOGRAFADA: ");
    printf("%s\n", mensagem);

    printf("\n");
    printf("----------ESTEGANOGRAFANDO IMAGEM----------");
    printf("\n");

    //Carrega a imagem em pic
    Img pic;
    if(argc < 1) {
        printf("loader [img]\n");
        exit(1);
    }
    load(argv[1], &pic);

    printf("\nPixels da imagem antes da mudanca:\n");
    for(int i=0; i<8*strlen(mensagem); i++) {
        printf("[%02X %02X %02X] ", pic.img[i].r +1, pic.img[i].g, pic.img[i].b);
    }
    printf("\n\n");

    SOIL_save_image("saida.bmp", SOIL_SAVE_TYPE_BMP, pic.width, pic.height, 3, pic.img);

    free(pic.img);
}

int binarioDec(long long num){
    int numDecimal = 0;
    int i = 0;
    int r;
    while (num!=0)
    {
        r = num % 10;
        num /= 10;
        numDecimal += r * pow(2, i);
        ++i;
    }
    return numDecimal;
}

void pegaLetra(char* bits, char string[]){
    int pos = 0;
    for(int i = 0; i < strlen(bits); i += 8){
        // get 8 bits from the bit array
        char aux[9];
        int count = 0;
        for(int j = i; j < i + 8; j++){
            aux[count] = bits[j];
            count += 1;
        }
        aux[count] = '\0';

        // convert the 8 bits to long long
        long long bin = atol(aux);


        //
        char decimal = (char) binarioDec(bin);
        string[pos] = decimal;
        pos += 1;
    }
    string[pos] = '\0';
}

void save_to_image(char string[], int key, Img pic, int argc, char** argv){
    encrypt(key,string,argc,argv);
    char* bits = concatenaBin(string);
    char one = 0x01;
    for(int i = 0; i < strlen(bits); i++) {
        if (bits[i] == '1'){
            pic.img[i].b = pic.img[i].b | one;
        }
    }
}
void ImageRecover(int key, Img pic){
    char* bits = "";
    char aux[9];
    int count = 0;
    for(int i = 0; i < sizeof(pic.img);i++){

        aux[count] = testbit(pic.img[i].b, 0);


        if (count == 8){
          
            aux[count+1] = '\0';

        
            bits = concatena(bits, aux);

            // restart counter
            count = 0;


            if(binarioDec(atol(aux)) == '\0'){
                break;
            }
        }
        count += 1;
    }

    char mensagemCripto[strlen(bits)];
    pegaLetra(bits, mensagemCripto);

    printf("Recovered string = %s", mensagemCripto);
}

char* insert_zeros(char* bits){
    int qt_zeros = 8 - strlen(bits);
    if (qt_zeros == 0){
        return bits;
    }
    else if(qt_zeros < 0){
        return "erro";
    }
    else{
        char* aux = malloc(qt_zeros);
        int count = 0;
        for(int i = 0; i < qt_zeros; i++){
            aux[i] = (char) 48;
            count += 1;
        }
        aux[count] = '\0';

        return concatena(aux, bits);
    }
}

char* concatenaBin(char string[]){
    char* concBin = "";

    for(int i = 0; i< strlen(string); i++){
        // Converte para bin�rio
        long long bin = decimalBin(string[i]);

        // Converte o bin�rio para um array de char
        char bits[256];
        sprintf(bits, "%lli", bin); // sprintf sprintf escreve em um buffer de caracteres definido por bits.

        // Vai adicionando 0 at� que complete os 8 bits.
        char* Adiciona0arr = insert_zeros(bits);

        concBin = concatena(concBin, Adiciona0arr);
    }
    return concBin;
}

void decrypt(char string[], int num){
    for(int i = 0; i < strlen(string); i++){
		char character = string[i];

		if(character >= 'a' && character <= 'z'){
			character = character - num;

			if(character < 'a'){
				character = character + 'z' - 'a' + 1;
			}

			string[i] = character;
		}
		else if(character >= 'A' && character <= 'Z'){
			character = character - num;

			if(character < 'A'){
				character = character + 'Z' - 'A' + 1;
			}

			string[i] = character;
		}
	}
}


int main(int argc, char** argv){

    //Carrega a imagem em pic
    Img pic;
    if(argc < 1) {
        printf("loader [img]\n");
        exit(1);
    }
    load(argv[1], &pic);

     int choice;
     char string[35];
     do {
        printf("1.Criptografar\n2.Decodificar\n3.Sair\n");
        printf("Opcao: ");
        scanf("%d", &choice);
        switch(choice){
            case 1:
                printf("Digite a mensagem a ser escondida: ");
                scanf("%s", string);
                encrypt(1,string,argc,argv);
                break;
            case 2:
                decrypt(1, string);
                break;
            default:
                printf("ATE A PROXIMA\n==== EH US GURI ====\n");
                exit(0);
}
}while (choice != 0);
}
