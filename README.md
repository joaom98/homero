# homero
Ajuda no trabalho de realimentados do Homero, perigosíssimo

Dependências:
* EduIntro
* FanControlle


# Testes realizados durante o projeto

## Controlar lado quente do peltier

* Não é a finalidade ideal do circuito, podendo ser trocada por um resistor ou outro acionador voltado para geração de calor

## Controlar lado frio do peltier usando um relé como acionador em um sistema PID

* Não podia utilizar um sistema "on-off", apesar do controle ser feito pela modulação de tempo de ativação do relé utilizando um PID.

## Controlar o lado frio esquentando o lado quente e usando um cooler PWM

* Implementação não ideal, pois existem muitas variáveis envolvidas

* Dificuldade em controlar os parâmetros do peltier diretamente

* Limitações na montagem
