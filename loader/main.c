//Alunos: Gustavo Possebon e Bernardo de Cesaro

#include "SOIL.h" // SOIL é a biblioteca para leitura das imagens
#include <stdio.h> //Para manipular a entrada e saida em geral
#include <stdlib.h> //Para usar biblioteca de proposito geral da linguagem
#include <string.h> // Para usar biblioteca strings


// Um pixel RGB composto por r, g e b.
typedef struct {
    unsigned char r, g, b;
} RGB;

// Uma imagem em RGB composto por altura e largura, e um array de pixels
typedef struct {
    int width, height;
    RGB* img;
} Img;

// Controlar o índice dos pixels de Img
int pos = 0;

// Inicialização dos chars bits onde serão manipulados
char bit1, bit2, bit3, bit4, bit5, bit6, bit7, bit8 = 0;

// Protótipos
void load(char* name, Img* pic);
void blink(char* password);
void encrypt(int password, char text[], int argc, char** argv);
int testbit(int bit, int oi);
unsigned int setbit(Img pic, int bit);
unsigned int toLetter(Img pic);
unsigned int clearbit(int bit);
void decrypt(int password, int textTam, char** argv);

// Métodos implementados
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

//Verifica se o bit é o bit desejado
int testbit(int bit, int oi){
    if(((bit >> oi) & 0x01) == 0)
        return 0;
    return 1;
}

//Substitui o bit
unsigned int setbit(Img pic, int bit)
{
	unsigned int vueP = pic.img[pos].b;
	return ((vueP << 1) | bit);
}

//Transforma a posição do pos do pic em hexadecimal
unsigned int toLetter(Img pic){
	unsigned int vueP = pic.img[pos].b;
    if((vueP >> 0) & 0x01)
        return 1;
    return 0;
}

//Limpa o bit enviado
unsigned int clearbit(int bit)
{
    return 0 & ~(1 << bit);
}


// Executar saltos entre os pixels
void blink(char* password) {
    int hell = 0;
    for(int i=0; i<strlen(password); i++) {
        hell += password[i];
    }
    hell /= strlen(password);
    pos += hell;
}

// Método para codificar a mensagem na imagem
void encrypt(int password, char text[], int argc, char** argv) {

    Img pic; // Cria uma variavel pic da struct Img
    if(argc < 1) { //verifica se o argumento é invalido
        printf("loader [img]\n");
        exit(1);
    }

    load(argv[1], &pic); //carrega a saida no pic
    char gurupiMessage[350]; // Vetor para auxiliar da mensagem codificada
    char nowLetter; // Armazena o caractere atual da mensagem


    printf("\n");
    printf("----------IMAGEM CRIPTOGRAFADA----------\n");
    printf("\n");
    /*
    O princípio utilizado será que o caractere atual vai se transformar em um caractere a mais (+1)
    e será alocado na mesma posição so que no vetor da mensagem criptografada
    */
    for(int i = 0; i < strlen(text); i++) {
        nowLetter = text[i];
        gurupiMessage[i] = nowLetter + 2;
    }

    // printf("\nPixels da imagem anterior a criptografia:\n");
    // for(int i = 1; i <= 8*strlen(text); i++) {
    //     printf("[%02X %02X %02X] ", pic.img[i].r, pic.img[i].g, pic.img[i].b);
    //     if(i % 4 == 0) {
    //         printf("\n");
    //     }
    // }
    // printf("\n");

    //pegando bit a bit do caracter...
    for(int i = 0; i < strlen(gurupiMessage); i++) {
        nowLetter = gurupiMessage[i]; //pega o char da posição na mensagem codificada

        bit8 = testbit(nowLetter,7);
        bit7 = testbit(nowLetter,6);
        bit6 = testbit(nowLetter,5);
        bit5 = testbit(nowLetter,4);
        bit4 = testbit(nowLetter,3);
        bit3 = testbit(nowLetter,2);
        bit2 = testbit(nowLetter,1);
        bit1 = testbit(nowLetter,0);

        //executa 7 vezes
        for(int j = 0; j < 8; j++) {
            switch(j) {
                //pega cada posição substitui o bit para o que quer e depois pula
                case 0:
                    pic.img[pos].b = setbit(pic,bit8);
                    blink(password);
                    break;
                case 1:
                    pic.img[pos].b = setbit(pic,bit7);
                    blink(password);
                    break;
                case 2:
                    pic.img[pos].b = setbit(pic,bit6);
                    blink(password);
                    break;
                case 3:
                    pic.img[pos].b = setbit(pic,bit5);
                    blink(password);
                    break;
                case 4:
                    pic.img[pos].b = setbit(pic,bit4);
                    blink(password);
                    break;
                case 5:
                    pic.img[pos].b = setbit(pic,bit3);
                    blink(password);
                    break;
                case 6:
                    pic.img[pos].b = setbit(pic,bit2);
                    blink(password);
                    break;
                case 7:
                    pic.img[pos].b = setbit(pic,bit1);
                    blink(password);
                    break;

                default:
                    break;
            }
        }

    }

    // printf("\nPixels da imagem apos criptografia:\n");
    // for(int i = 1; i <= 8*strlen(gurupiMessage); i++) {
    //     printf("[%02X %02X %02X] ", pic.img[i].r, pic.img[i].g, pic.img[i].b+2);
    //     if(i % 4 == 0) {
    //         printf("\n");
    //     }
    // }
    // printf("\n\n");

    // Resetando os bits para 0
    bit8 = clearbit(bit8);
    bit7 = clearbit(bit7);
    bit6 = clearbit(bit6);
    bit5 = clearbit(bit5);
    bit4 = clearbit(bit4);
    bit3 = clearbit(bit3);
    bit2 = clearbit(bit2);
    bit1 = clearbit(bit1);

    // Salvando imagem criptografada
    SOIL_save_image("saida.bmp", SOIL_SAVE_TYPE_BMP, pic.width, pic.height, 3, pic.img);

    free(pic.img); //limpa imagem da memória
}

void decrypt(int password, int textTam, char** argv) {

    printf("\n");
    printf("----------DECODIFICANDO IMAGEM----------\n");
    printf("\n");
    char* text = calloc(300, sizeof text); //alocar espaço para os itens e retorna um ponteiro
    char* textSteo = calloc(300, sizeof textSteo); //alocar espaço para os itens e retorna um ponteiro
    Img pic; // instancia uma variavel da estrutura Imgs
    load("saida.bmp", &pic); //carrega a saida no pic
    pos = 0; //reinicia essa variavel
    int contCarac = 0;

    //Transforma cada posição desse vetor de pixel a partir do binário em letra
    for(int i = 0; i < textTam; i++) {
        bit8 = toLetter(pic);
        blink(password); // pula posição
        bit7 = toLetter(pic); //vira letra
        blink(password);
        bit6 = toLetter(pic);
        blink(password);
        bit5 = toLetter(pic);
        blink(password);
        bit4 = toLetter(pic);
        blink(password);
        bit3 = toLetter(pic);
        blink(password);
        bit2 = toLetter(pic);
        blink(password);
        bit1 = toLetter(pic);
        blink(password);

        textSteo[i] = (bit8 << 7) + (bit7 << 6) + (bit6 << 5) + (bit5 << 4) + (bit4 << 3) + (bit3 << 2) + (bit2 << 1) + (bit1 << 0);
    }

    //Vai descriptografando cara posição do vetor criptografado e volta ao normal
    for(int i=0; i<strlen(textSteo); i++){
        text[i] = textSteo[i] - 2;
    }
    printf("\nMensagem criptografada: %s\n", textSteo);
    printf("\nMensagem descriptografada: %s\n", text);

    free(text); //limpa da memória
    free(textSteo); //limpa da memória
    free(pic.img); //limpa da memória
}


int main(int argc, char** argv) {
    char sentence[250]; //frase digitada pelo usuário
    char password[50]; //senha original
    char newPassoword[50]; //senha para descriptografar
    char c,d,f;
    int i = 0;
    int g = 0;
    int j = 0;
    puts("Digite a senha para criptografar: ");
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

    encrypt(password, sentence, argc, argv); //chama o método para criptografar a imagem com texto e senha recebidos

    puts("Digite a senha para descriptografar: ");
    while ((f = getchar()) != '\n'){
             newPassoword[j++] = f;
            }
    newPassoword[j] = '\0';

    //so executa a primeira senha em todos os caracteres forem iguais aos da segunda senha solicitada
    if (strcmp (password,newPassoword) == 0 )  {
        printf("\nAs senhas conferem entao sera decodificado\n");
        decrypt(password, strlen(sentence), argv);
    }
    else{
        printf("As senhas nao conferem entao sera decodificado\n");
    }
}
