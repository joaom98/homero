#include <PID_v1.h>
#define RelayPin 6

#include <EduIntro.h>

LM35 lm35(A0);  // creating the object 'lm35' on pin A0

double C;

//Define Variables we'll be connecting to
double Desejado, Entrada, Saida;

//Specify the links and initial tuning parameters
PID myPID(&Entrada, &Saida, &Desejado, 2000, 500, 1, DIRECT);

int WindowSize = 10000;
unsigned long windowStartTime;
void setup()
{

  Serial.begin(9600);
  
  pinMode(RelayPin, OUTPUT);

  windowStartTime = millis();

  //initialize the variables we're linked to
  Desejado = 35;

  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WindowSize);

  //turn the PID on
  myPID.SetMode(AUTOMATIC);

}

void loop()
{
  C = lm35.readCelsius();       // Reading the temperature in Celsius degrees 
  Entrada = C;
  myPID.Compute();

  Serial.print(C);
  Serial.print("\n");

  /************************************************
     turn the output pin on/off based on pid output
   ************************************************/
  unsigned long now = millis();
  if (now - windowStartTime > WindowSize)
  { //time to shift the Relay Window
    windowStartTime += WindowSize;
  }
  if (Saida > now - windowStartTime) {
    digitalWrite(RelayPin, LOW);
    Serial.print("LIGADO\n");
  }
  else {
    digitalWrite(RelayPin, HIGH);
    Serial.print("DESLIGADO\n");
  }
}
