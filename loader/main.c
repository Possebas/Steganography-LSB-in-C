#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Para usar strings
#include "SOIL.h" // SOIL é a biblioteca para leitura das imagens

// Um pixel RGB
typedef struct {
    unsigned char r, g, b;
} RGB;

// Uma imagem em RGB
typedef struct {
    int width, height;
    RGB* img; //array de pixels
} Img;

// Responsável pelo controle do index dos pixels da imagem
int cont = 0;
char bit1, bit2, bit3, bit4, bit5, bit6, bit7, bit8 = 0;

// Protótipos
void load(char* name, Img* pic);
void salta(char* password);
void encrypt(int password, char message[], int argc, char** argv);
void decrypt(int password, int passwordLength, char** argv);

// Carrega uma imagem para a struct Img
void load(char* name, Img* pic) {
    int chan;
    pic -> img = (unsigned char*) SOIL_load_image(name, &pic -> width, &pic -> height, &chan, SOIL_LOAD_RGB);
    if(!pic -> img) {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
        exit(1);
    }
    printf("Tamanho da imagem: %d x %d x %d\n", pic->width, pic->height, chan);
    printf("\n");
}

// Algoritmo de salto entre pixels
void salta(char* password) {
    int salto = 0;
    for(int i=0; i<strlen(password); i++) {
        salto += password[i];
    }

    salto /= strlen(password);

    cont += salto;
}

void encrypt(int password, char message[], int argc, char** argv) {

    // Armazena a mensagem apos a cifra de Cesar
    char mensagemCriptografada[348];

    printf("\n");
    printf("----------ENCRIPTANDO IMAGEM----------\n");
    printf("\n");

    // Armazena o caractere atual da mensagem
    char caractereAtual;

    // AUMENTANDO 3 LETRAS NO ALFABETO PARA CADA LETRA DA MENSAGEM (CRIPTOGRAFANDO COM CIFRA DE CESAR).
    for(int i = 0; i < strlen(message); i++) {
        caractereAtual = message[i];
        mensagemCriptografada[i] = caractereAtual + 3;
    }

    // Carrega a imagem em pic
    Img pic;

    if(argc < 1) {
        printf("loader [img]\n");
        exit(1);
    }

    load(argv[1], &pic);

    for(int i = 0; i < strlen(mensagemCriptografada); i++) {
        caractereAtual = mensagemCriptografada[i];

        //pegando bit a bit do caracter...
        bit8 = (caractereAtual >> 7) & 0x01;
        bit7 = (caractereAtual >> 6) & 0x01;
        bit6 = (caractereAtual >> 5) & 0x01;
        bit5 = (caractereAtual >> 4) & 0x01;
        bit4 = (caractereAtual >> 3) & 0x01;
        bit3 = (caractereAtual >> 2) & 0x01;
        bit2 = (caractereAtual >> 1) & 0x01;
        bit1 = (caractereAtual >> 0) & 0x01;

        for(int j = 0; j < 8; j++) {
            switch(j) {
                case 0:
                    pic.img[cont].r = (pic.img[cont].r << 1) | bit8;
                    salta(password);
                    break;
                case 1:
                    pic.img[cont].r = (pic.img[cont].r << 1) | bit7;
                    salta(password);
                    break;
                case 2:
                    pic.img[cont].r = (pic.img[cont].r << 1) | bit6;
                    salta(password);
                    break;
                case 3:
                    pic.img[cont].r = (pic.img[cont].r << 1) | bit5;
                    salta(password);
                    break;
                case 4:
                    pic.img[cont].r = (pic.img[cont].r << 1) | bit4;
                    salta(password);
                    break;
                case 5:
                    pic.img[cont].r = (pic.img[cont].r << 1) | bit3;
                    salta(password);
                    break;
                case 6:
                    pic.img[cont].r = (pic.img[cont].r << 1) | bit2;
                    salta(password);
                    break;
                case 7:
                    pic.img[cont].r = (pic.img[cont].r << 1) | bit1;
                    salta(password);
                    break;

                default:
                    break;
            }
        }

    }

    // Limpando bits
    bit8 = 0;
    bit7 = 0;
    bit6 = 0;
    bit5 = 0;
    bit4 = 0;
    bit3 = 0;
    bit2 = 0;
    bit1 = 0;

    // Salvando imagem criptografada
    SOIL_save_image("saida.bmp", SOIL_SAVE_TYPE_BMP, pic.width, pic.height, 3, pic.img);

    free(pic.img);
}

void decrypt(int password, int passwordLength, char** argv) {
    printf("\n");
    printf("----------DECODIFICANDO IMAGEM----------\n");
    printf("\n");
    char* message = calloc(300, sizeof message);
    char* messageCriptografada = calloc(300, sizeof messageCriptografada);

    Img pic;
    load("saida.bmp", &pic);

    // MONTA A LETRA ATRAVÉS DO BINARIO
    cont = 0;
    int contCarac = 0;
    for(int i = 0; i < passwordLength; i++) {
        bit8 = (pic.img[cont].r >> 0)  & 0x01;
        salta(password);
        bit7 = (pic.img[cont].r >> 0) & 0x01;
        salta(password);
        bit6 = (pic.img[cont].r >> 0) & 0x01;
        salta(password);
        bit5 = (pic.img[cont].r >> 0) & 0x01;
        salta(password);
        bit4 = (pic.img[cont].r >> 0) & 0x01;
        salta(password);
        bit3 = (pic.img[cont].r >> 0) & 0x01;
        salta(password);
        bit2 = (pic.img[cont].r >> 0) & 0x01;
        salta(password);
        bit1 = (pic.img[cont].r >> 0) & 0x01;
        salta(password);

        messageCriptografada[i] = (bit8 << 7) + (bit7 << 6) + (bit6 << 5) + (bit5 << 4) + (bit4 << 3) + (bit3 << 2) + (bit2 << 1) + (bit1 << 0);
    }

    // REDUZINDO 3 LETRAS NO ALFABETO PARA CADA LETRA DA MENSAGEM (DESCRIPTOGRAFANDO COM CIFRA DE CESAR)
    for(int i=0; i<strlen(messageCriptografada); i++){
        message[i] = messageCriptografada[i] - 3;
    }

    printf("\nMENSAGEM DESCRIPTOGRAFADA: %s\n", message);

    free(message);
    free(messageCriptografada);
    free(pic.img);
}


int main(int argc, char** argv) {
    char sentence [250];
    char password [50];
    char newPassoword[50];
    char c,d,f;
    int i = 0;
    int g = 0;
    int j = 0;
    puts("Digite a senha: ");
    while ((d = getchar()) != '\n'){
             password[g++] = d;
            }
    password[g] = '\0';
    puts("Digite a mensagem a ser escondida: ");
    while ((c = getchar()) != '\n'){
             sentence[i++] = c;
            }
    sentence[i] = '\0';
    printf("\n\nTamanho da Mensagem: %d \n\n", strlen(sentence) );
    encrypt(password, sentence, argc, argv);
    puts("Digite a senha para descriptografar: ");
    while ((f = getchar()) != '\n'){
             newPassoword[j++] = f;
            }
    newPassoword[j] = '\0';
    if (strcmp (password,newPassoword) == 0 )  {
        printf("\nAs senhas conferem\n");
        decrypt(password, strlen(sentence), argv);
    }
    else{
        printf("As senhas não conferem então sera decodificado\n");
    }
}
// }
//     encrypt("testeSenh@gigante123", "sera que essa merda vai dar bom dessa vez?", argc, argv);
//     decrypt("testeSenh@gigante123", 42, argv);

// }
