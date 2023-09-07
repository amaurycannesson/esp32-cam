#include <Arduino.h>
#include <SPI.h>

#define RX_MODE 0
#define TX_MODE 1
#define SELECTED_MODE 1
#define SELECTED_PIN 2

#define REPEAT 10
#define MS_DELAY_BETWEEN_REPEAT 5

#define PAYLOAD_SIZE 136
#define PERIOD 480

// HackRF (data + pause)     => 1000001110100011101110100011101110100011101110100011101110111011101110111011101000111011101110111010001000100010001000100010001000111010000000000000

bool hasStarted = false;
char receivedData[PAYLOAD_SIZE];
int dataToSend[] = {1,0,0,0,0,0,1,1,1,0,1,0,0,0,1,1,1,0,1,1,1,0,1,0,0,0,1,1,1,0,1,1,1,0,1,0,0,0,1,1,1,0,1,1,1,0,1,0,0,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,0,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,1,1,0,1,0};
int i = 0;

void setup()
{
  Serial.begin(9600);

  if (SELECTED_MODE == RX_MODE)
  {
    pinMode(SELECTED_PIN, INPUT);
  }
  else if (SELECTED_MODE == TX_MODE)
  {
    pinMode(SELECTED_PIN, OUTPUT);
  }

  Serial.println("Init done.");
}

void displayReceivedData()
{
  int pinValue = digitalRead(SELECTED_PIN);

  if (pinValue == 1 && !hasStarted)
  {
    hasStarted = true;
    delayMicroseconds(PERIOD / 2);
  }

  if (hasStarted && i < PAYLOAD_SIZE)
  {
    receivedData[i] = pinValue == 1 ? '1' : '0';
    delayMicroseconds(PERIOD);
    i++;
  }

  if (i == PAYLOAD_SIZE)
  {
    Serial.println(receivedData);
    Serial.println("Restart in 5 seconds...");
    hasStarted = false;
    i = 0;
    delay(5000);
  }
}

void sendData()
{
  unsigned int remaningRepeat = REPEAT;
  while (remaningRepeat > 0)
  {
    while (i < PAYLOAD_SIZE)
    {
      digitalWrite(2, dataToSend[i]);
      delayMicroseconds(PERIOD);
      i++;
    }
    delay(MS_DELAY_BETWEEN_REPEAT);
    i = 0;
    remaningRepeat--;
  }

  Serial.println("Data sent.");
  Serial.println("Restart in 5 seconds...");
  delay(5000);
}

void loop()
{
  if (SELECTED_MODE == RX_MODE)
  {
    displayReceivedData();
  }
  else if (SELECTED_MODE == TX_MODE)
  {
    sendData();
  }
}
