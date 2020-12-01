#include <FanController.h>

#define SENSOR_PIN 2 // Fio Verde
#define SENSOR_THRESHOLD 1000
#define PWM_PIN 9 // Fio Azul
FanController fan(SENSOR_PIN, SENSOR_THRESHOLD, PWM_PIN);

#define VELOCIDADE 100 // Numero de 0 a 100 que define a velocidade do cooler

void setup()
{

  Serial.begin(9600);
  
  fan.begin();

  // Get duty cycle
  byte dutyCycle = fan.getDutyCycle();
  Serial.print("Duty cycle: ");
  Serial.println(dutyCycle, DEC);

}

void loop()
{
  
  fan.setDutyCycle(VELOCIDADE);

  Serial.print(VELOCIDADE);
  Serial.print("\t");
  Serial.print(fan.getSpeed());
  Serial.println("RPM");
}
