/*
  Author: Jônatas Oliveira
  Projeto: Sistema Automático de Irrigação - TCC Eletrotécnica - Senai
*/

// Inclusão de bibliotecas
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Definições
#define endereco  0x27 // Endereços comuns: 0x27, 0x3F
#define colunas   16
#define linhas    2
#define umidadeSoloDefault 865
#define ledBombaOn 10
#define ledBombaOff 9
#define ledReservatorioBaixo 8
#define sinalMiniBomba 13
#define sinalSensorChuva 4
#define sinalSensorNivel 2
#define valorSensorUmidadeSolo A0

// Criação de instância de objeto do display
LiquidCrystal_I2C lcd(endereco, colunas, linhas);

void setup()
{
  lcd.init(); // Inicia a comunicação com a display
  lcd.backlight(); // Liga a iluminação do display
  lcd.clear(); // Limpa o display

  Serial.begin(9600);
  pinMode(sinalMiniBomba, OUTPUT); // Sinal de liga ou desliga mini bomba [1 - Sol, 0 - Chuva]
  pinMode(ledBombaOn, OUTPUT); // LED - Mini bomba ligada
  pinMode(ledBombaOff, OUTPUT); // LED - Mini bomba desligada
  pinMode(ledReservatorioBaixo, OUTPUT); // LED - Reservatório baixo
  pinMode(sinalSensorChuva, INPUT); // Sensor de chuva
  pinMode(sinalSensorNivel, INPUT); // Sensor de nível de água
  pinMode(valorSensorUmidadeSolo, INPUT); // Sensor de solo - higrômetro [ > 865 solo está seco]

  digitalWrite(sinalMiniBomba, HIGH); // Desliga mini bomba
  lcd.print("Proj. Irrigacao");
  lcd.setCursor(0, 1); // Posiciona o cursor na primeira coluna da linha 2
  lcd.print(" Eletrotecnica");
  delay(3000); // aguarda 3000ms, ou seja, 3 segundos
  Serial.println("Setup OK");
  // Impressão no monitor serial
  //Serial.println(digitalRead(sinalSensorChuva));
  //Serial.println(analogRead(valorSensorUmidadeSolo));
}

void loop()
{
  // Impressão no monitor serial
  Serial.println(digitalRead(sinalSensorNivel));
  Serial.println(analogRead(valorSensorUmidadeSolo));

  while (!digitalRead(sinalSensorChuva)) {
    digitalWrite(sinalMiniBomba, HIGH); // Desliga mini bomba
    digitalWrite(ledBombaOn, LOW); // LED - Motor Ligado
    digitalWrite(ledBombaOff, HIGH); // LED - Motor desligado
    digitalWrite(ledReservatorioBaixo, LOW); // LED - Reservatório Baixo
    // Impressão no display
    lcd.clear(); // Limpa o display
    lcd.print("Bomba Desligada");
    lcd.setCursor(0, 1); // Posiciona o cursor na primeira coluna da linha 2
    lcd.print("     CHUVA");
    delay(500);
  }
  while (analogRead(valorSensorUmidadeSolo) < umidadeSoloDefault) {
    Serial.println(analogRead(valorSensorUmidadeSolo));
    digitalWrite(sinalMiniBomba, HIGH); // Desliga mini bomba
    digitalWrite(ledBombaOn, LOW); // LED - Motor Ligado
    digitalWrite(ledBombaOff, HIGH); // LED - Motor desligado
    digitalWrite(ledReservatorioBaixo, LOW); // LED - Reservatório Baixo
    // Impressão no display
    lcd.clear(); // Limpa o display
    lcd.print("Bomba Desligada");
    lcd.setCursor(0, 1); // Posiciona o cursor na primeira coluna da linha 2
    lcd.print("   SOLO UMIDO");
    delay(500);
  }
  if (digitalRead(sinalSensorChuva) || analogRead(valorSensorUmidadeSolo) >= umidadeSoloDefault) {
    if (digitalRead(sinalSensorNivel)) {
      digitalWrite(sinalMiniBomba, LOW); // Liga mini bomba
      digitalWrite(ledBombaOn, HIGH); // LED - Motor Ligado
      digitalWrite(ledBombaOff, LOW); // LED - Motor desligado
      digitalWrite(ledReservatorioBaixo, LOW); // LED - Reservatório Baixo
      // Impressão no display
      lcd.clear(); // Limpa o display
      lcd.print("  Bomba Ligada");
      lcd.setCursor(0, 1); // Posiciona o cursor na primeira coluna da linha 2
      lcd.print("  Irrigando.");
      delay(500);
      lcd.setCursor(0, 1); // Posiciona o cursor na primeira coluna da linha 2
      lcd.print("  Irrigando..");
      delay(500);
      lcd.setCursor(0, 1); // Posiciona o cursor na primeira coluna da linha 2
      lcd.print("  Irrigando...");
    }
    else if(!digitalRead(sinalSensorNivel)) {
      digitalWrite(sinalMiniBomba, HIGH); // Desliga mini bomba
      digitalWrite(ledBombaOn, LOW); // LED - Motor Ligado
      digitalWrite(ledBombaOff, HIGH); // LED - Motor desligado
      digitalWrite(ledReservatorioBaixo, HIGH); // LED - Reservatório Baixo
      // Impressão no display
      lcd.clear(); // Limpa o display
      lcd.print("  Reservatorio");
      lcd.setCursor(0, 1);
      lcd.print("     Baixo");
      delay(500);
    }
  }
  
  Serial.println("...");
  delay(500); // Delay a little bit to improve simulation performance
}
