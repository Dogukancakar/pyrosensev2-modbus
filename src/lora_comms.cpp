#include "lora_comms.h"
#include "crypto_utils.h" // Kriptografi işlemleri için

// LoRa yapılandırma sabitleri
#define TX_POWER 18 // dBm cinsinden iletim gücü
#define LORA_FREQUENCY 433E6 // 433 MHz
#define LORA_BANDWIDTH 125E3 // 125 kHz
#define LORA_SPREADING_FACTOR 7
#define LORA_PREAMBLE_LENGTH 8
#define LORA_SYNC_WORD 0x12

bool lora_setup() {
    Serial.println(F("LoRa Modülü Başlatılıyor (lora_comms.cpp)..."));

    // --- EN GÜVENİLİR BAŞLATMA SIRASI ---

    // Adım 1: LoRa kütüphanesine kullanacağı pinleri söyle.
    LoRa.setPins(LORA_SS_PIN, LORA_RST_PIN, LORA_DIO0_PIN);
    Serial.println(F("LoRa.setPins çağrıldı."));
    Serial.print(F("Kullanılan Pinler - SS: ")); Serial.print(LORA_SS_PIN);
    Serial.print(F(", RST: ")); Serial.print(LORA_RST_PIN);
    Serial.print(F(", DIO0: ")); Serial.println(LORA_DIO0_PIN);

    // Adım 2: LoRa modülünü donanımsal olarak sıfırla.
    Serial.println(F("LoRa modülü donanımsal olarak sıfırlanıyor..."));
    pinMode(LORA_RST_PIN, OUTPUT);
    digitalWrite(LORA_RST_PIN, LOW);
    delay(20); // Reset sinyali için bekle
    digitalWrite(LORA_RST_PIN, HIGH);
    delay(50); // Modülün kendine gelmesi ve stabilize olması için bekle

    // Adım 3: SPI veri yolunu başlat.
    SPI.begin(LORA_SCK_PIN, LORA_MISO_PIN, LORA_MOSI_PIN, LORA_SS_PIN);
    Serial.println(F("SPI.begin çağrıldı."));

    // Adım 4: LoRa kütüphanesini başlat.
    Serial.print(F("LoRa.begin(")); Serial.print(LORA_FREQUENCY); Serial.println(F(") çağrılıyor..."));
    if (!LoRa.begin(LORA_FREQUENCY)) {
        Serial.println(F("HATA: LoRa.begin() false döndü!"));
        return false;
    }

    // LoRa ayarlarını elle yapılandır
    LoRa.setTxPower(TX_POWER);
    LoRa.setSignalBandwidth(LORA_BANDWIDTH);
    LoRa.setSpreadingFactor(LORA_SPREADING_FACTOR);
    LoRa.setPreambleLength(LORA_PREAMBLE_LENGTH);
    LoRa.setSyncWord(LORA_SYNC_WORD);

    Serial.print(F("LoRa başarıyla başlatıldı. Frekans: "));
    Serial.print(LORA_FREQUENCY / 1E6);
    Serial.println(F(" MHz"));

    return true;
}

bool lora_send_data(const uint8_t* data, size_t length) {
    if (length == 0) {
        Serial.println(F("LoRa Gönderme Hatası: Veri uzunluğu 0."));
        return false;
    }
    if (length > 255) { // LoRa paket boyutu sınırı
        Serial.print(F("LoRa Gönderme Hatası: Veri çok uzun (maks 255 byte): "));
        Serial.println(length);
        return false;
    }

    if (LoRa.beginPacket()) {
        LoRa.write(data, length);
        if (LoRa.endPacket()) {
            Serial.print(F("LoRa Paketi gönderildi, boyut: "));
            Serial.println(length);
            return true;
        } else {
            Serial.println(F("LoRa Paketi gönderme hatası (endPacket false döndü)."));
            return false;
        }
    } else {
        Serial.println(F("LoRa meşgul veya gönderme başlatılamadı (beginPacket false döndü)."));
        return false;
    }
}

void lora_sleep() {
    LoRa.sleep();
    Serial.println(F("LoRa modülü uyku moduna alındı."));
}

void lora_idle() {
    LoRa.idle();
    Serial.println(F("LoRa modülü bekleme moduna alındı."));
}

int lora_get_packet_rssi() {
    return LoRa.packetRssi();
}

float lora_get_packet_snr() {
    return LoRa.packetSnr();
}

void perform_lora_send(const char* plaintext_to_send) {
    Serial.println(F("\n--- Veri Gönderme İşlemi Başlatılıyor ---"));

    Serial.print(F("Gönderilecek Orijinal Mesaj: "));
    Serial.println(plaintext_to_send);

    unsigned char lora_payload_buffer[255];
    size_t lora_payload_len = 0;

    // Şifreleme işlemi
    bool success = encrypt_lora_payload(
        (const unsigned char*)plaintext_to_send,
        strlen(plaintext_to_send),
        DEVICE_CONFIG, // Kripto ayarları
        lora_payload_buffer,
        sizeof(lora_payload_buffer),
        &lora_payload_len
    );

    if (success && lora_payload_len > 0) {
        Serial.print(F("Kriptolu LoRa Mesajı Gönderiliyor, toplam boyut: "));
        Serial.println(lora_payload_len);
        lora_send_data(lora_payload_buffer, lora_payload_len);
    } else {
        Serial.println(F("HATA: Mesaj kriptolanamadı. Gönderilmiyor."));
    }

    Serial.println(F("--- Veri Gönderme İşlemi Tamamlandı ---"));
}