//Necessidades para contrucao do Chip8.
//Preciso de uma memoria para armazenar as funcoes e opcodes
//(35 funcoes)

//Preciso de 16 registradores de dados de 8 bits
//De um stack para armazenar o endereco de retorno
//De 2 timers, para eventos de jogo e outro para o som.
//Do imput que seria o teclado.

//Grafico e som (Allegro 5 nesse caso)
//Opcode table, onde temos 35 opcodes:
//Hexa Simbols:
//  NNN: Address
//  NN : Constante de 8 bits
//  N: Constante de 4 bits

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#undef main
 
//Defino o tamanho da memoria para as funcionalidades do Chip8.
#define tammemo 4096

//Defino o tamanho inicial da tela para 640x320.
#define SCREEN_W 640
#define SCREEN_H 320

//Auxiliares.
#define SCREEN_BPP 32
#define W 64
#define H 32

//Faco o desenho basico de fontset de 0 a F.
//4x5 pixels.
unsigned char fontset[80] =
{
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

//Faco com que chip8 seja da struct CHIP8.
typedef struct chip8{
    FILE * game;
    
    //Vou utilizar para chamar a respectiva opcode.
    unsigned short opcode;

    //seto o memory com o tamanho que defino.
    unsigned char memory[tammemo];

    //X e Y: Identificador dos registradores de 4 bits.
    unsigned char V[0x10];

    //I: Registrador de 16 bits, para endereco de memoria.
    unsigned short I;

    //PC: Contador do programa
    unsigned short pc;

    //Para setar graficos do mesmo tamanho que vou utilizar na tela.
    unsigned char graphics[64 * 32];

    //Seto os chars de timers para o som e para o delay de jogo.
    unsigned char delay_timer;
    unsigned char sound_timer;

    //Stack para armazenar o endereco de retorno. 
    unsigned short stack[0x10];

    unsigned short sp;
    unsigned char key[0x10];
} CHIP8;

void inicializar(CHIP8 *, char *);
void executar(CHIP8 *);
void inicio();

int main(){
    inicio();
    return 0;
}

//Bloco para saber o nome do jogo que vai rodar a ROM
void inicio(){
    char name[50];
    printf("Nome do jogo: ");
    scanf("%s", name);
}

//Bloco que busca o jogo e carrega ele na memoria
void inicializar(CHIP8 * CH8, char * name){
    int i;
    CH8->game = fopen(name, "ESCOLHA O JOGO AQUI");

    if (!CH8->game)  {
        fflush(stdin); 
        getchar();
        exit(1);
    }

    fread(CH8->memory+0x200, 1, tammemo-0x200, CH8->game); // Carrega o jogo na memoria
 
    for(i = 0; i < 80; ++i)
            CH8->memory[i] = fontset[i]; // Carrego o fontset na memoria
        CH8->pc = 0x200;
        CH8->sp &= 0;
        CH8->opcode = 0x200;
}
 
//Bloco para inicializar as 35 opcodes, para rodar a ROM em cima
void executar(CHIP8 * CH8){

    Uint8 * keys;
    int y, x, vx, vy, times, i;
    unsigned height, pixel;

    for(times = 0; times < 10; times++){
        CH8->opcode = CH8->memory[CH8->pc] << 8 | CH8->memory[CH8->pc + 1];

        //Chamo os 35 opcodes.
        switch(CH8->opcode & 0xF000){    
            case 0x0000:
                switch(CH8->opcode & 0x000F){
                    case 0x0000: // 00E0 -> Limpa a tela
                        memset(CH8->graphics, 0, sizeof(CH8->graphics));
                        CH8->pc += 2;
                    break;
         
                    case 0x000E: // 00EE -> Retorno da subrotina  
                        CH8->pc = CH8->stack[(--CH8->sp)&0xF] + 2;
                    break;  
                }
            break;
           
            case 0x1000: // 1NNN -> Pula para o endereco NNN
                CH8->pc = CH8->opcode & 0x0FFF;
            break;  
               
            case 0x2000: // 2NNN -> Chama a subrotina NNN *(0xNNN)()
                CH8->stack[(CH8->sp++)&0xF] = CH8->pc;
                CH8->pc = CH8->opcode & 0x0FFF;
            break;
       
            case 0x3000: // 3XNN -> Pula a instrucao se VX == NN.
                if(CH8->V[(CH8->opcode & 0x0F00) >> 8] == (CH8->opcode & 0x00FF))
                    CH8->pc += 4;
                else
                    CH8->pc += 2;
            break;
       
            case 0x4000: // 4XNN -> Pula a instrucao se VX != NN.
                if(CH8->V[(CH8->opcode & 0x0F00) >> 8] != (CH8->opcode & 0x00FF))
                    CH8->pc += 4;
                else
                    CH8->pc += 2;
            break;
       
            case 0x5000: // 5XY0 -> Pula a intrucao se VX == Vy.
                if(CH8->V[(CH8->opcode & 0x0F00) >> 8] == CH8->V[(CH8->opcode & 0x00F0) >> 4])
                    CH8->pc += 4;
                else
                    CH8->pc += 2;
            break;
       
            case 0x6000: // 6XNN -> Seta o VX para o NN
                CH8->V[(CH8->opcode & 0x0F00) >> 8] = (CH8->opcode & 0x00FF);
                CH8->pc += 2;
            break;
       
            case 0x7000: // 7XNN -> adiciona NN ao VX
                CH8->V[(CH8->opcode & 0x0F00) >> 8] += (CH8->opcode & 0x00FF);
                CH8->pc += 2;
            break;
               
            case 0x8000:
                switch(CH8->opcode & 0x000F){
                    case 0x0000: // 8XY0 -> Seta VX ao valor de VY
                        CH8->V[(CH8->opcode & 0x0F00) >> 8] = CH8->V[(CH8->opcode & 0x00F0) >> 4];
                        CH8->pc += 2;
                    break;
       
                    case 0x0001: // 8XY1 -> Seto VX ao (VX ou VY) OU***
                        CH8->V[(CH8->opcode & 0x0F00) >> 8] = CH8->V[(CH8->opcode & 0x0F00) >> 8] | CH8->V[(CH8->opcode & 0x00F0) >> 4];
                        CH8->pc += 2;
                    break;
       
                    case 0x0002: // 8XY2 -> Seto VX ao (VX e VY) E***
                        CH8->V[(CH8->opcode & 0x0F00) >> 8] = CH8->V[(CH8->opcode & 0x0F00) >> 8] & CH8->V[(CH8->opcode & 0x00F0) >> 4];
                        CH8->pc += 2;
                    break;
       
                    case 0x0003: // 8XY3 -> Seto VX ao VX xor VY.
                        CH8->V[(CH8->opcode & 0x0F00) >> 8] = CH8->V[(CH8->opcode & 0x0F00) >> 8] ^ CH8->V[(CH8->opcode & 0x00F0) >> 4];
                        CH8->pc += 2;
                    break;
       
                    case 0x0004: // 8XY4 -> Adiciono VY ao VX. VF 1 = com Carry,VF 0 = sem Carry.
                        if(((int)CH8->V[(CH8->opcode & 0x0F00) >> 8 ] + (int)CH8->V[(CH8->opcode & 0x00F0) >> 4]) < 256)
                            CH8->V[0xF] &= 0;
                        else
                            CH8->V[0xF] = 1;

                        CH8->V[(CH8->opcode & 0x0F00) >> 8] += CH8->V[(CH8->opcode & 0x00F0) >> 4];
                        CH8->pc += 2;
                    break;
       
                    case 0x0005: // 8XY5 -> VY e subtraido do VX. VF = 0 subtrai, VF 1 = nao.
                        if(((int)CH8->V[(CH8->opcode & 0x0F00) >> 8 ] - (int)CH8->V[(CH8->opcode & 0x00F0) >> 4]) >= 0)
                            CH8->V[0xF] = 1;
                        else
                            CH8->V[0xF] &= 0;

                        CH8->V[(CH8->opcode & 0x0F00) >> 8] -= CH8->V[(CH8->opcode & 0x00F0) >> 4];
                        CH8->pc += 2;
                    break;
       
                    case 0x0006: // 8XY6 -> troca VX pra direta. VF e o ultimo numero significantes antes da troca de VX.
                        CH8->V[0xF] = CH8->V[(CH8->opcode & 0x0F00) >> 8] & 7;
                        CH8->V[(CH8->opcode & 0x0F00) >> 8] = CH8->V[(CH8->opcode & 0x0F00) >> 8] >> 1;
                        CH8->pc += 2;
                    break;
       
                    case 0x0007: // 8XY7 -> VX = VY - VX. VF = 0 subtrai, VF = 1 nao.
                        if(((int)CH8->V[(CH8->opcode & 0x0F00) >> 8] - (int)CH8->V[(CH8->opcode & 0x00F0) >> 4]) > 0)
                            CH8->V[0xF] = 1;
                        else
                            CH8->V[0xF] &= 0;

                        CH8->V[(CH8->opcode & 0x0F00) >> 8] = CH8->V[(CH8->opcode & 0x00F0) >> 4] - CH8->V[(CH8->opcode & 0x0F00) >> 8];
                        CH8->pc += 2;
                    break;
       
                    case 0x000E: // 8XYE -> troca VX pra esquerda. VF vira o valor mais significante antes da troca.
                        CH8->V[0xF] = CH8->V[(CH8->opcode & 0x0F00) >> 8] >> 7;
                        CH8->V[(CH8->opcode & 0x0F00) >> 8] = CH8->V[(CH8->opcode & 0x0F00) >> 8] << 1;
                        CH8->pc += 2;
                    break;
                }
            break;
       
            case 0x9000: // 9XY0 -> Pulo a proxima instrucao se VX nao for igual a VY.
                if(CH8->V[(CH8->opcode & 0x0F00) >> 8] != CH8->V[(CH8->opcode & 0x00F0) >> 4])
                    CH8->pc += 4;
                else
                    CH8->pc += 2;
            break;
       
            case 0xA000: // ANNN -> Seto I para o endereco NNN
                CH8->I = CH8->opcode & 0x0FFF;
                CH8->pc += 2;
            break;
       
            case 0xB000: // BNNN -> Pulo para o endereco NNN + V0.
                CH8->pc = (CH8->opcode & 0x0FFF) + CH8->V[0];
            break;
       
            case 0xC000: // CXNN -> Seto Vx ao resultado do (and) no numero randomico de NN.
                CH8->V[(CH8->opcode & 0x0F00) >> 8] = rand() & (CH8->opcode & 0x00FF);
                CH8->pc += 2;
            break;
       
            case 0xD000: // DXYN -> Desenho em VX e Vy*
                vx = CH8->V[(CH8->opcode & 0x0F00) >> 8];
                vy = CH8->V[(CH8->opcode & 0x00F0) >> 4];
                height = CH8->opcode & 0x000F;  
                CH8->V[0xF] &= 0;
           
                for(y = 0; y < height; y++){
                    pixel = CH8->memory[CH8->I + y];
                    for(x = 0; x < 8; x++){
                        if(pixel & (0x80 >> x)){
                            if(CH8->graphics[x+vx+(y+vy)*64])
                                CH8->V[0xF] = 1;
                            CH8->graphics[x+vx+(y+vy)*64] ^= 1;
                        }
                    }
                }
                CH8->pc += 2;
            break;
       
            case 0xE000:
                switch(CH8->opcode & 0x000F){
                    case 0x000E: // EX9E -> pulo a proxima instrucao se a chave em VX for pressionada.
                        int keys;
                        if(keys[keymap[CH8->V[(CH8->opcode & 0x0F00) >> 8]]])
                            CH8->pc += 4;
                        else
                            CH8->pc += 2;
                    break;
                             
                    case 0x0001: // EXA1 -> pula a proxima instrucao se a chave em VX nao for pressionada.
                        int keys;
                        if(!keys[keymap[CH8->V[(CH8->opcode & 0x0F00) >> 8]]])
                            CH8->pc += 4;
                        else
                            CH8->pc += 2;
                    break;
                }
            break;
       
            case 0xF000:
                switch(CH8->opcode & 0x00FF){
                    case 0x0007: // FX07 -> seto VX ao valor do delay.
                        CH8->V[(CH8->opcode & 0x0F00) >> 8] = CH8->delay_timer;
                        CH8->pc += 2;
                    break;
       
                    case 0x000A: // FX0A -> a chave pressionada e esperada, e guardada em VX.
                        int = keys;
                        for(i = 0; i < 0x10; i++)
                            if(keys[keymap[i]]){
                                CH8->V[(CH8->opcode & 0x0F00) >> 8] = i;
                                CH8->pc += 2;
                            }
                    break;
       
                    case 0x0015: // FX15 -> seto o valor do delay ao VX.
                        CH8->delay_timer = CH8->V[(CH8->opcode & 0x0F00) >> 8];
                        CH8->pc += 2;
                    break;
       
                    case 0x0018: // FX18 -> seto o timer de som ao VX.
                        CH8->sound_timer = CH8->V[(CH8->opcode & 0x0F00) >> 8];
                        CH8->pc += 2;
                    break;
       
                    case 0x001E: // FX1E -> adiciono VX ao I.
                        CH8->I += CH8->V[(CH8->opcode & 0x0F00) >> 8];
                        CH8->pc += 2;
                    break;

                    case 0x0029: // FX29 -> I se torna a localizacao do sprite do caracter no VX.
                        CH8->I = CH8->V[(CH8->opcode & 0x0F00) >> 8] * 5;
                        CH8->pc += 2;
                    break;
                    case 0x0033: // FX33 -> armazeno o codigo binario de VX.
                        CH8->memory[CH8->I] = CH8->V[(CH8->opcode & 0x0F00) >> 8] / 100;
                        CH8->memory[CH8->I+1] = (CH8->V[(CH8->opcode & 0x0F00) >> 8] / 10) % 10;
                        CH8->memory[CH8->I+2] = CH8->V[(CH8->opcode & 0x0F00) >> 8] % 10;
                        CH8->pc += 2;
                    break;
       
                    case 0x0055: // FX55 -> adiciono V0 ao VX, no endereco de inicio da memoria.
                        for(i = 0; i <= ((CH8->opcode & 0x0F00) >> 8); i++)
                            CH8->memory[CH8->I+i] = CH8->V[i];
                        CH8->pc += 2;
                    break;
       
                    case 0x0065: //FX65 -> preenche V0 ao VX com o valor de inicio da memoria.
                        for(i = 0; i <= ((CH8->opcode & 0x0F00) >> 8); i++)
                            CH8->V[i] = CH8->memory[CH8->I + i];
                        CH8->pc += 2;
                    break;
                }
            break;     
        }
    }     

}

//Bloco que faz o delay do jogo e o delay da musica.
void timers(CHIP8 * CH8){
    if(CH8->delay_timer > 0)
        CH8->delay_timer--;
    if(CH8->sound_timer > 0)
        CH8->sound_timer--;
    if(CH8->sound_timer != 0)
        printf("%c", 7);
}  
}