/*
 * TITLE: OBJECTS FINDER TRANSMITTER 
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
int push_button_state = 0; // state to track push button presses 
int data_to_send = 4; 
long int press_track = 1; 
bool TOGGLE_PRESS = false; 

void setup(){
  Serial.begin(9600);         // Initialize serial and set the baud rate
  pinMode(push_button, INPUT);   // Sets this pin as an INPUT

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
  if(check_send_status()){
    send_signal(); 
  } else{
    Serial.println("Waiting for input to send signal");
  }
  
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

void send_signal(){
  Serial.print("Sending packet: ");
  Serial.println(data_to_send);

  // send packet
  LoRa.beginPacket();
  LoRa.print(data_to_send);
  LoRa.endPacket();
}
