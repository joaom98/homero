#include <StateSpaceControl.h>

/*
 * This example shows how StateSpaceControl can be used to control the position of a cart while it balances a pole attached to it using a passive rotational joint.
 * The model being used comes from this analysis: http://ctms.engin.umich.edu/CTMS/index.php?example=InvertedPendulum&section=SystemModeling
 * which defines the state as: x = [cart_position, cart_velocity, stick_angle, stick_angular_rate]^T . It's assumed that the cart position and the stick angle are directly observable 
 * and the rest of the state is recovered using an estimator.
 */

// Start by defining a state space model, This particular model describes the cart pole system but you can define any model by just declaring a Model object and then
// filling out the state matrices. See Model.h for examples on how to do this (and feel free to PR to add your own!).
Peltier model( 12 );

// Next define a state space controller. The cart pole model uses 4 states, 1 input and 2 outputs so we'll need to specify these inside the <> brackets when declaring the controller.
// The controller also uses state estimation so to enable this, supply true as the fourth template parameter, like so:
StateSpaceController<2,1,1, true, true> controller(model);

// Lastly, since the controller isn't controlling a cart pole, we'll need to simulate one to show how the controller works. The Simulation class handles this by accepting 
// the control inputs generated by the controller (u) and returning observations from the motor (y).
Simulation<2,1,1> sim(model);

Matrix<1> y;
const float dt = 0.001;

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
  Serial.begin(115200);

  // To parameterise the controller, we'll need to fill out the control law matrix K, and the estimator matrix L.
  // K defines feedback gains which are loosely similar to PID gains while L is equivilent to the Kalman Gain of a Kalman Filter.
  // If you're wondering where these numbers came from, head over to TuneThoseGains.ipynb
    controller.K << 99.9592, 4.0002;
    controller.L << 0.3192,
                    0.0833;
    controller.I << -464.1719;

  // Once the system and gain matrices are filled in, the controller needs to be initialised so that it can precalculate Nbar
  controller.initialise();

  // Try to bring the cart to rest at 3.5m from it's home position
  controller.r << 20;

  // Let's also set some initial conditions on the simulation so that the controller's estimator has to work for it
  sim.x << 0, 0;

  fan.begin(); // Iniciando o fan

  fan.setDutyCycle(velocidade_final); // Seleção de velocidade (mudar VELOCIDADE para uma variável que seja um numero entre 0 e 100)

  Serial.println();
  Serial.println("VELOCIDADE_REAL\tPERCENTUAL_ACIONAMENTO\tRPM\tTEMPERATURA");

  
}

// Now we can start the control loop
void loop()
{
  // Firstly generate some measurements from the simulator. Since this model assumes that only part of the state can be can be directly observed,
  // y contains two elements containing cart position and stick angle. If we were controlling an actual cart pole, these observations would come, for example,
  // from a set of encoders attached to the cart's wheels and stick joint.
  C = readTemperature(lm35, 400);       // Lendo a temperatura em Celsius
  Serial.println(C);
  y = C;

  // Now update the state space controller, which causes it to update its u member (the control input). When controlling an actual system, the updated control input
  // would be used to command the cart's motor driver or similar.
  controller.update(y, dt);

  // Print the current system output to serial. If the controller is doing its job properly then y should settle at whatever r was set to after a short transient.
  Serial << "temperatura = " << y(0) << " acionamento = "  << controller.u(0) << '\n';


  delay(dt * 1000);
}
