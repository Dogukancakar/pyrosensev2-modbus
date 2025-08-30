#include <Arduino.h>
#include "modbus.h"
#include "lora_comms.h"
#include "crypto_utils.h"


#define mosfet_pin 7
 
void setup() {
    Serial.begin(9600);
    pinMode(mosfet_pin, OUTPUT);
    digitalWrite(mosfet_pin, LOW); // MOSFET'i aç
    delay(50); // 50 ms bekle
    setup_modbus();
    lora_setup();

    Serial.println("Setup tamamlandı.");
}

void loop() {

    read_ss(2); // Slave adresi 2 olan cihazdan SS oku

    read_ss(4); // Slave adresi 4 olan cihazdan SS oku

    read_ss(6); // Slave adresi 6 olan cihazdan SS oku

    read_flow(8); // Slave adresi 8 olan cihazdan Flow oku
}