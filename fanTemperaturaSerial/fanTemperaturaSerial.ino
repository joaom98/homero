#include <FanController.h>      // Biblioteca que controla a velocidade de um cooler
#define SENSOR_PIN 2            // Pino do sensor é o Digital 2 ligado no fio verde do cooler
#define SENSOR_THRESHOLD 1000   // Constante para quantidade de leituras do tacômetro
#define PWM_PIN 9               // Pino de controle de velocidade do cooler ligado no fio azul

FanController fan(SENSOR_PIN, SENSOR_THRESHOLD, PWM_PIN); // Criando a instância do cooler

#define VELOCIDADE 0 // Percentual da velocidade do cooler de 0 a 100
/*
 * Velocidade máxima do cooler: 3200 RPM (100%)
 * 
 * Velocidade mínima do cooler: 800 RPM  (0%)
 * 
 */

#include <EduIntro.h>
#define LM35_PIN A0 // Pino de saida do LM35 (Fio Verde do LM35 ligado na porta analógica A0)
LM35 lm35(LM35_PIN);  // Criando o objeto 'lm35' no pino A0
double C;

double readTemperature (LM35 sensor, int amostras) // Função auxiliar para medir a temperatura (faz várias leituras e entrega uma média)
{
  double soma = 0;
   for ( int i=0 ; i < amostras ; i++ ) {
      soma += sensor.readCelsius(); 
   }

   return soma / amostras;
}

int velocidade_final = velocidadeMapeada(VELOCIDADE); // Utilizando variáveis globais para acessar seus valores em todas as funções
int velocidade_real;

int velocidadeMapeada( int velocidade ) // Função que relaciona a velocidade em um percentual com os valores do cooler
{ 
  velocidade_real = velocidade;
  return map(velocidade, 0, 100, 0, 65); // Valores descobertos empiricamente. O fan rodando a 65 na chamada de DutyCycle equilibra o lado frio em 1C
}

void setup()
{

  Serial.begin(9600); // Iniciando a comunicação serial
  
  fan.begin(); // Iniciando o fan

  fan.setDutyCycle(velocidade_final); // Seleção de velocidade (mudar VELOCIDADE para uma variável que seja um numero entre 0 e 100)

  Serial.println();

}

void loop()
{


  C = readTemperature(lm35, 400);       // Lendo a temperatura em Celsius
  Serial.write(C, 4);
  
  if (Serial.available() > 0) {
    // Parse speed
    int input = Serial.parseInt();

    // Constrain a 0-100 range
    byte target = max(min(input, 100), 0);

    // Set fan duty cycle
    fan.setDutyCycle(target);

    // Get duty cycle
    byte dutyCycle = fan.getDutyCycle();
    // Me livrando de uma leitura maluca
    int thrash = Serial.read();
  }

}
