#include <LiquidCrystal.h>			// inclusão da biblioteca do LCD.
#include <Servo.h> 				// inclusão da biblioteca do Servo motor.
#include <Keypad.h> 				// Biblioteca do codigo.
#include <EEPROM.h> 			// inclusão da biblioteca da memória externa.

// Declaração da variável senha.

int  senha, senhaCorreta = 1234; 

// Declaração dos Sensores

int pir		= 11;		//Nomeação do sensor PIR na porta digital D11.
int ldr		= A0;		//Nomeação do sensor LDR na porta analógica A0.

// Declaração das linhas e colunas do teclado matricial.

const byte linhas  = 4;		//Declaração da quantidade de linhas.
const byte colunas = 4;	//Declaração da quantidade de colunas.

// Mapeamento da matriz do teclado.

char matriz_teclado[linhas][colunas] =
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Nomeação da pinagem do teclado matricial
 
byte Pinoslinhas[linhas]   = {9, 8, 7, 6};	//Nomeação dos pinos linhas do teclado.
byte Pinoscolunas[colunas] = {5, 4, 3, 2};	//Nomeação dos pinos colunas do teclado.

// Instanciando um objeto teclado  que define mapeamento, pinagem, linhas e colunas.

Keypad teclado = Keypad(makeKeymap(matriz_teclado), Pinoslinhas, Pinoscolunas, linhas, colunas);

// Declaração dos atuadores

int lampada = 12;				//Nomeação da lâmpada na porta digital D12.
Servo fechadura;				//Instanciando um objeto fechadura no servomotor.
LiquidCrystal lcd(13,A1,A2,A3,A4,A5);	//Declaração de pinagem do LCD.


void setup()
{
  
 pinMode(lampada,OUTPUT); 	//   Lâmpada é nomeada como saída.
 pinMode(pir,INPUT);		//  Sensor de presença PIR é nomeado como entrada.
 pinMode(A0,INPUT);		// Sensor de luminosidade LDR é nomeado como entrada.
 lcd.begin(16,2);			// Inicialização do LCD 16 x 2.
 fechadura.attach(10);			// Anexa a variável servo a porta digital D10.
 teclado.setHoldTime(1000); 		// Estabelece 1s para a espera de uma nova tecla.
 EEPROM.put(0, senhaCorreta);	// Insere a senha na posição 0 da memória EEPROM.
 Serial.begin(9600);
  
}

void loop()
{
int senhaCadastrada = EEPROM.get(0,senhaCorreta); // Lê na memória EEPROM a senha cadastrada.

     ldr 			= analogRead(A0);		// Verifica luminosidade do ambiente.
bool presenca 		= sensor Presenca();		// Verifica presença no ambiente.
   
//Escreve “PRESSIONE QUALQUER TECLA” no LCD.
    lcd.setCursor(0,0);					/
    lcd.print("   PRESSIONE   ");
    lcd.setCursor(0,1);
    lcd.print(" QUALQUER TECLA");
  
//Se houver presença de usuário e a luminosidade for menor igual 900 a lâmpada acende
  if ( presenca == true && ldr <= 900 )
  {
    
    digitalWrite(lampada, HIGH);
    
  }else	// Se não ela não acende
  {
    
    digitalWrite(lampada, LOW);
    
  }
  
char tecla_pressionada = teclado.getKey();		// Verifica se o teclado foi pressionado
  
  if( tecla_pressionada)  //Se alguma tecla for pressionada solicita para digitar uma senha de 4 digítos
  {
    
  senha = solicitacaoSenha();
    
  if ( senha == senhaCadastrada )	// Se senha for correta abre a porta e printa ABERTO no LCD.
  	{
    
    senha = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("     ABERTO     ");
	abrirPorta();
    delay(1000);
    
  } else{ 				// Se senha for incorreta printa FECHADO no LCD.
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("     FECHADO     ");
    delay(1000);
    
  	}
  }
  

  
}

  
//Solicita ao usuário para digitar uma senha no teclado e retorna seu valor em inteiro.
int solicitacaoSenha(void)
{

int senhaDigitada, i = 0; 		// Declara variável local senhaDigitada  como inteiro.
String palavraChave; 			// Declara variável local palavraChave como string.

// Escreve no LCD “Digite a senha:”.
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Digite a senha:");
lcd.setCursor(0,1);
  
// Solicita para o usuário digitar uma string de 4 dígitos no teclado e a converte em inteiro.
  while ( i < 4 )
  {
	char	tecla_pressionada = teclado.getKey(); 
    
    if(tecla_pressionada)
    {
    lcd.print("*");
	palavraChave += tecla_pressionada;
  	senhaDigitada = palavraChave.toInt();
	i++;
    }
    
  }
  
//retorna senha de 4 dígitos inteiro.
return (senhaDigitada);

}


//Verifica se há presença no ambiente e retorna se é verdadeiro ou falso.
bool sensorPresenca(void)
{
  
bool presenca; 
  
  if (digitalRead(pir) == HIGH)	
  {
    presenca = true;
  }else{
    presenca = false;
  }

return (presenca);
}

//Rotaciona a tranca (servomotor) 180º e o retorna a posição inicial.
void abrirPorta()
{
  	for (int i = 0; i<180; i++)
    {
      fechadura.write(i);
      delay(10);
    }
    for (int i = 180; i>0; i--)
    {
      fechadura.write(i);
      delay(10);
    }
}
