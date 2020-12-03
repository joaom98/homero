#include <FanController.h>      // Biblioteca que controla a velocidade de um cooler

#define SENSOR_PIN 2            // Pino do sensor é o Digital 2 ligado no fio verde do cooler
#define SENSOR_THRESHOLD 1000   // Constante para quantidade de leituras do tacômetro
#define PWM_PIN 9               // Pino de controle de velocidade do cooler ligado no fio azul

FanController fan(SENSOR_PIN, SENSOR_THRESHOLD, PWM_PIN); // Criando a instância do cooler

#define VELOCIDADE 0  // Percentual da velocidade do cooler de 0 a 100
/*
 * Velocidade máxima do cooler: 3200 RPM (100%)
 * 
 * Velocidade mínima do cooler: 800 RPM  (0%)
 * 
 */
void setup()
{

  Serial.begin(9600); // Iniciando a comunicação serial
  
  fan.begin(); // Iniciando o fan

}

void loop()
{
  
  fan.setDutyCycle(VELOCIDADE); // Seleção de velocidade (mudar VELOCIDADE para uma variável que seja um numero entre 0 e 100)

  Serial.print(VELOCIDADE);
  Serial.print("\t");
  Serial.print(fan.getSpeed());
  Serial.println("RPM");
}
