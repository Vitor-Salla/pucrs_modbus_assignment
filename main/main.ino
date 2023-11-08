#include <ModbusRTUSlave.h>
#include <SoftwareSerial.h>

const uint8_t ledPins[3] = {13, 12, 11}; // leds
const uint8_t switchPins[3] = {4, 3, 2}; // dip switch
const byte potenciometroPin = A0; // potênciometro
const byte lm35Pin = A1;

const uint8_t rxPin = 7;
const uint8_t txPin = 5;
const uint8_t dePin = 6; // pino do enable

SoftwareSerial mySerial(rxPin, txPin);
ModbusRTUSlave modbus(mySerial, dePin); // porta serial com pino do enable rs485

bool leds[3];
bool switchs[3];
uint16_t potenciometros[1];

union Lm35Union {
  float floatValue;
  uint16_t uint16Value[2];
};

Lm35Union lm35values;

void setup() {
  pinMode(ledPins[0], OUTPUT);
  pinMode(ledPins[1], OUTPUT);
  pinMode(ledPins[2], OUTPUT);
  pinMode(switchPins[0], INPUT_PULLUP);
  pinMode(switchPins[1], INPUT_PULLUP);
  pinMode(switchPins[2], INPUT_PULLUP);
  pinMode(potenciometroPin, INPUT);

  modbus.configureCoils(leds, 3);
  modbus.configureDiscreteInputs(switchs, 3);
  modbus.configureInputRegisters(potenciometros, 1); // potenciometro int
  modbus.configureHoldingRegisters(lm35values.uint16Value, 2); // potenciometro float
  modbus.begin(1, 9600);

  digitalWrite(ledPins[0], 1);
  delay(500);
  digitalWrite(ledPins[1], 1);
  delay(300);
  digitalWrite(ledPins[2], 1);
  delay(500);
  digitalWrite(ledPins[0], 0);
  digitalWrite(ledPins[1], 0);
  digitalWrite(ledPins[2], 0);
  delay(500);
  digitalWrite(ledPins[0], 1);
  delay(250);
  digitalWrite(ledPins[1], 1);
  delay(100);
  digitalWrite(ledPins[2], 1);
  delay(500);
  digitalWrite(ledPins[0], 0);
  digitalWrite(ledPins[1], 0);
  digitalWrite(ledPins[2], 0);
}

void loop() {
  switchs[0] = !digitalRead(switchPins[0]);
  switchs[1] = !digitalRead(switchPins[1]);
  switchs[2] = !digitalRead(switchPins[2]);
  potenciometros[0] = analogRead(potenciometroPin);
  float lm35voltage = (analogRead(lm35Pin)/1024.0)*5.0;
  lm35values.floatValue = lm35voltage/0.01;

  modbus.poll();

  digitalWrite(ledPins[0], leds[0]);
  digitalWrite(ledPins[1], leds[1]);
  digitalWrite(ledPins[2], leds[2]);
}
