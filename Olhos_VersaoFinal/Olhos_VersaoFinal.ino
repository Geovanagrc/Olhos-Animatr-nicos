/*
* OBSERVAÇÕES:
*  é necessário arrumar um modo de descobrir se o ângulo para fechar o olho esta correto, pois, se não estiver, acredito que há perigo de forçar demais os servos e 
*  as palpebras do olho e acabar quebrando algo
*  
*  SEMPRE CONECTE O RX NO PINO 3 E O TX NO PINO 2
* 
*  NÃO CONECTE COM O BLUETOOTH DO CEL, VÁ DIRETO AO APP E CONECTE POR LÁ! CASO ESTEJA PAREADO COLOQUE PARA ESQUECER.
*  
*  ACREDITO QUE FECHAR E ABRIR OS OLHOS ESTEJA EXIGINDO MUITA ENERGIA. POR ESTE MOTIVO O BLUETOOTH SOFRE UM RESET ACIDENTAL E DESCONECTA
*  
*  
*  SE TUDO DER ERRADO VERIFIQUE SE AS BIBLIOTECAS ESTÃO ATUALIZADAS
*/
#include <Dabble.h>
#include <Adafruit_PWMServoDriver.h>  // Importa a biblioteca para controlar servos
#include <Wire.h>
 
 
// Cria um objeto da classe Adafruit_PWMServoDriver para controlar o driver de servos
Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver();
 
// Definindo constantes para os pinos dos servos
#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
 
// Definindo constantes para os pinos dos servos
int palpe_ce = 0;  // Servo para controlar a pálpebra superior esquerda
int palpe_be = 1;  // Servo para controlar a pálpebra inferior esquerda
int palpe_cd = 2;  // Servo para controlar a pálpebra superior direita
int palpe_bd = 3;  // Servo para controlar a pálpebra inferior direita
int Yarm = 4;      // Servo para controlar o movimento da íris no eixo Y
int Xarm = 5;      // Servo para controlar o movimento da íris no eixo X
 
 
 
//================== FUNÇÃO PRINCIPAL ================== 
void setup() {
  Dabble.begin(9600);
  Serial.begin(9600);   // Inicializa a comunicação serial com velocidade de 9600 bps
  servos.begin();   // Inicializa o driver de servo
  servos.setPWMFreq(60); // Define a frequência PWM para 60 Hz, ideal para servos
  
  eye_ball_centert();
  open_eye();
}
 
//================== LAÇO DE REPETIÇÃO (LOOP) ==================
void loop() {
  
  
  Dabble.processInput(); // Processa a entrada de dados do GamePad
  //================== VARIÁVEIS DO GAMEPAD ==================
 
  // Valores dos eixos X e Y do joystick do GamePad 
  float eixox = GamePad.getXaxisData();  // Valor do eixo X do joystick
  float eixoy = GamePad.getYaxisData();  // Valor do eixo Y do joystick
  
  // Valores do botoes do Gamepad
  int botx = GamePad.isCrossPressed();   // Valor do botão ✗ do controle
  int boto = GamePad.isCirclePressed();  // Valor do botão ⭕ do controle
  int botq = GamePad.isSquarePressed();  // Valor do botão □ do controle
  int bott = GamePad.isTrianglePressed(); //Valor do botão Δ do controle
  //lógica para a movimentação dos servos com base no GamePad
  
  // Se apertar o botão ✗ pisca
  if (botx) 
  {
   Serial.println("✗ pressionado");
   eye_ball_centert();
   close_eye();
   delay(1000);
   open_eye();
   
  
  }
 
  
  // Se apertar o ⭕ fecha o olho
  if(boto){
    Serial.println("⭕ pressionado");
    eye_ball_centert();
    close_eye();
  }
 
  
  // Se apertar o □ abre o olho
  if(botq){
    Serial.println("□ pressionado");
    open_eye();
  }
  if (bott){
    Serial.println("Δ pressionado");
    eye_ball_centert();
    pisca_esq();
  }
 
//===========================MOVIMENTACAO EIXO X ==========================
// converte o valor do gamepad pra angulo do servo
 
 
  if ( eixox!=0) {
     int valorformatx = map(eixox,-7,7,36,164); //map do gamepad usando os limites do servo
     angu_ticks(Xarm,valorformatx,36,164,240,560);
     Serial.print("Eixo x: "); 
     Serial.println(valorformatx);

  }
 
   if ( eixoy != 0) {
     int valorformaty = map(eixoy,-7,7,0,20); //map do gamepad usando os limites do servo
     angu_ticks(Yarm,valorformaty,0,16,150,200);
     Serial.print("Eixo y: "); 
     Serial.println(valorformaty);
     
  }
}
 
 
 
//==================== FUNÇÕES PARA CONTROLAR OS OLHOS ==================
// Função para abrir os olhos
void open_eye() {
  servos.setPWM(palpe_ce,0,155);  // Ângulo de abertura para pálpebra superior esquerda
  servos.setPWM(palpe_be,0,550);  // Ângulo de abertura para pálpebra inferior esquerda
  servos.setPWM(palpe_cd,0,250);  // Ângulo de abertura para pálpebra superior direita 68GRAUS
  servos.setPWM(palpe_bd,0,300); // Ângulo de abertura para pálpebra inferior direita 40GRAUS
}
 
// Função para fechar os olhos
void close_eye() {
  servos.setPWM(palpe_ce,0,265); // Ângulo de fechamento para pálpebra superior esquerda 46GRAUS
  servos.setPWM(palpe_be,0,275); // Ângulo de fechamento para pálpebra inferior esquerda 50GRAUS
  servos.setPWM(palpe_bd,0,500);   // Ângulo de fechamento para pálpebra inferior direita 132GRAUS
  servos.setPWM(palpe_cd,0,150);   // Ângulo de fechamento para pálpebra superior direita 0GRAUS
  
}
 
// Função para centralizar a íris 
void eye_ball_centert() {
  servos.setPWM(Xarm,0,420);   // Centraliza a íris no eixo X 108GRAUS
  servos.setPWM(Yarm,0,160);  // Centraliza a íris no eixo Y  4GRAUS
}
//Função que pisca somente o olho esquerdo
void pisca_esq (){
  //fecha
  servos.setPWM(palpe_ce,0,265); // Ângulo de fechamento para pálpebra superior esquerda
  servos.setPWM(palpe_be,0,287.5); // Ângulo de fechamento para pálpebra inferior esquerda
  delay(500);
  //abre
  servos.setPWM(palpe_ce,0,155);  // Ângulo de abertura para pálpebra superior esquerda
  servos.setPWM(palpe_be,0,550);  // Ângulo de abertura para pálpebra inferior esquerda
}
 
 
//==================== FUNCAO DE CONVERSÃO DOS TICKS DO DRIVE PWM ======= 
void angu_ticks(int servo, int angulo, int anguloMin , int anguloMax , int pwmMin, int pwmMax ) {
  // Converte o ângulo desejado  para ticks (entre 150 e 600)
   int valorTicks = map(angulo, anguloMin, anguloMax, pwmMin, pwmMax);
  
  // Aplica o valor calculado para o canal específico do servo (por exemplo, o canal 5)
  servos.setPWM(servo, 0, valorTicks);  // Move o servo para o valor correspondente de ticks
}
 
 
//================== FIM DO PROGRAMA==================