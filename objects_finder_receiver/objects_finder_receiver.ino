/*
 * TITLE: OBJECTS FINDER RECEIVER 
 * 
 * DATE: 7TH NOV 2022
 * 
 * AUTHOR: SIMON
 */

// Includes all the libraries needed
#include <Arduino.h>


#include <SPI.h>
#include <LoRa.h>

const int push_button = 2; // initializes push button pin to pin no 2
const int buzzer = 5;
const int vibration_motor  = 6; 

int push_button_state = 0; // state to track push button presses 
int data_to_send = 4; 
long int press_track = 1; 
bool TOGGLE_PRESS = false; 
int lora_data_recevied = 0; // Variable store the data received from the LoRa sender

void setup(){
  Serial.begin(9600);         // Initialize serial and set the baud rate
  pinMode(push_button, INPUT);   // Sets this pin as an INPUT
  pinMode(buzzer, OUTPUT);
  pinMode(vibration_motor, OUTPUT) ;
  
   while (!Serial)
    ;
  Serial.println("LoRa Sender");
  if (!LoRa.begin(433E6))
  {
    Serial.println("Starting LoRa failed!");
    while (1);  
  }
}

void loop(){
  take_reading();
  receive_lora_message();
}

void take_reading(){
  push_button_state = digitalRead(push_button);
  Serial.print("Press State: ");
  Serial.println(push_button_state);
  delay(200); // to avoid debouncing 
}

bool check_send_status(){
  if(push_button_state == 1){
     press_track =+1;
  }
  if(press_track % 2 == 0){
    TOGGLE_PRESS = true; 
  } else {
    TOGGLE_PRESS = false; 
  }

  return TOGGLE_PRESS; 
}

void receive_lora_message()
{
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available())
    {
      lora_data_recevied = LoRa.read();
      Serial.print((char)lora_data_recevied);
      //      Serial.print((char)LoRa.read());
      (lora_data_recevied == '4') ? buzzer_vibrator_response() : buzzer_vibrator_off();
    }

    //         print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }

}

void buzzer_vibrator_response(){
  analogWrite(buzzer, 200);
  vibrator_trig(); 
}

void buzzer_vibrator_off(){
  analogWrite(buzzer, 0);
  digitalWrite(vibration_motor, LOW);
}

void vibrator_trig(){
    digitalWrite(vibration_motor, HIGH);
    delay(70);
    digitalWrite(vibration_motor, LOW);
    delay(70);
    digitalWrite(vibration_motor, HIGH);
    delay(70);
    digitalWrite(vibration_motor, LOW);
}
