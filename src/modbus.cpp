#include "modbus.h"
#include "lora_comms.h"

ModbusMaster node;
HardwareSerial mySerial(2);

void setup_modbus() {
    // Seri haberleşmeyi başlat
    mySerial.begin(9600, SERIAL_8N1, 10, 11); // RX=10, TX=11 pinleri
}

void read_ss(uint8_t slaveAddress) {      
    node.begin(slaveAddress, mySerial);
    uint8_t result;
    result = node.readInputRegisters(256, 1); // Adres 0x0000'dan 1 kayıt oku

    if (result == node.ku8MBSuccess) {
        uint16_t value = node.getResponseBuffer(0x00);
        Serial.print("Slave SS: ");
        Serial.println(value); // Okunan değeri yazdır
        
        char valueStr[16];
        sprintf(valueStr, "%u", value);
        perform_lora_send(valueStr);
        uint16_t akim1; 
        node.readInputRegisters(257 , 1);
        akim1 = node.getResponseBuffer(0x00);  
        char akimstr1[16];
        sprintf(akimstr1, "%u", akim1);
        perform_lora_send(akimstr1);

    } else {
        Serial.print("Modbus Read Error: ");
        Serial.println(result, HEX);
        
        // Hata kodunu string'e çevir
        char errorStr[16];
        sprintf(errorStr, "Error:%02X", result);
        perform_lora_send(errorStr);
    }
}



void read_flow(uint8_t slaveAddress) {
    node.begin(slaveAddress, mySerial);
    uint8_t result;
    result = node.readHoldingRegisters(100, 2); // Adres 0x0001'den 2 kayıt oku

    if (result == node.ku8MBSuccess) {
        uint16_t value = node.getResponseBuffer(0x00);
        Serial.print("Slave Flow: ");
        Serial.println(value); // Okunan değeri yazdır
        
        // İsteğe bağlı: Flow değerini de LoRa ile gönder
        char valueStr[16];
        sprintf(valueStr, "Flow:%u", value);
        perform_lora_send(valueStr);
    } else {
        Serial.print("Modbus Read Error: ");
        Serial.println(result, HEX);
    }
}

void write_slave_ss(uint8_t slaveAddress, uint16_t value) {
    node.begin(slaveAddress, mySerial);
    uint8_t result;
    result = node.writeSingleRegister(0x0000, value); // Adres 0x0000'a değeri yaz

    if (result == node.ku8MBSuccess) {
        Serial.print("Slave SS Write Success: ");
        Serial.println(value);
        
        // Yazma işlemi başarılı mesajı gönder
        char successStr[32];
        sprintf(successStr, "Write:%u", value);
        perform_lora_send(successStr);
    } else {
        Serial.print("Modbus Write Error: ");
        Serial.println(result, HEX);
        
        // Hata mesajı gönder
        char errorStr[32];
        sprintf(errorStr, "WriteError:%02X", result);
        perform_lora_send(errorStr);
    }
}