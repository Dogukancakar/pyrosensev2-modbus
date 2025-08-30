#ifndef LORA_COMMS_H
#define LORA_COMMS_H

#include <Arduino.h>
#include <LoRa.h> // LoRa kütüphanesini dahil et

// LoRa Modülü Pin Tanımları (Kullandığınız modüle göre güncelleyin)
#define LORA_SCK_PIN   12  // GPIO5  -- SX1278's SCK
#define LORA_MISO_PIN  14 // GPIO19 -- SX1278's MISO
#define LORA_MOSI_PIN  13 // GPIO27 -- SX1278's MOSI
#define LORA_SS_PIN    17 // GPIO18 -- SX1278's CS
#define LORA_RST_PIN   5 // GPIO14 -- SX1278's RESET
#define LORA_DIO0_PIN  6 // GPIO26 -- SX1278's IRQ(Interrupt Request)

// LoRa ayarları (isteğe bağlı, varsayılanlar da kullanılabilir)
#define LORA_FREQUENCY          433E6  // RA-02 için doğru frekans
#define LORA_BANDWIDTH          125E3 // 125 kHz
#define LORA_SPREADING_FACTOR   7     // SF7
#define LORA_CODING_RATE        4/5   // 4/5
#define LORA_PREAMBLE_LENGTH    8     // Varsayılan 8
#define LORA_SYNC_WORD          0x34  // Varsayılan 0x12, farklı ağlar için değiştirin
#define LORA_TX_POWER           17    // dBm cinsinden (örn: 2-20)

// Fonksiyon Bildirimleri
void perform_lora_send(const char* plaintext_to_send);
/**
 * @brief LoRa modülünü başlatır ve ayarlar.
 * 
 * @return bool Başlatma başarılıysa true, değilse false.
 */
bool lora_setup();

/**
 * @brief Verilen byte dizisini LoRa üzerinden gönderir.
 * 
 * @param data Gönderilecek veri işaretçisi.
 * @param length Gönderilecek verinin uzunluğu (byte cinsinden).
 * @return bool Gönderme işlemi başlatıldıysa true, değilse false.
 *         Bu fonksiyon genellikle asenkron çalışır, verinin ulaştığını garanti etmez.
 */
bool lora_send_data(const uint8_t* data, size_t length);

/**
 * @brief LoRa üzerinden gelen bir paketi kontrol eder ve alır.
 *        Bu fonksiyon non-blocking olmalı veya bir callback ile çalışmalıdır.
 *        Bu örnekte basit bir polling (kontrol etme) yapısı sunulmuştur.
 * 
 * @param buffer Gelen verinin kopyalanacağı buffer.
 * @param max_length Buffer'ın maksimum boyutu.
 * @return int Alınan byte sayısı, paket yoksa 0, hata varsa negatif bir değer.
 */
int lora_receive_data(uint8_t* buffer, size_t max_length);

/**
 * @brief LoRa modülünü uyku moduna alır (güç tasarrufu için).
 */
void lora_sleep();

/**
 * @brief LoRa modülünü bekleme (idle/standby) moduna alır.
 */
void lora_idle();

/**
 * @brief Alınan son paketin RSSI (Received Signal Strength Indicator) değerini döndürür.
 * @return int RSSI değeri.
 */
int lora_get_packet_rssi();

/**
 * @brief Alınan son paketin SNR (Signal-to-Noise Ratio) değerini döndürür.
 * @return float SNR değeri.
 */
float lora_get_packet_snr();

// İsteğe bağlı: Gelen paketler için bir callback fonksiyonu tanımlanabilir
// typedef void (*LoraRxCallback)(int packetSize);
// void lora_on_receive(LoraRxCallback callback);
void perform_lora_send_without_setup(const char* plaintext_to_send); // YENİ FONKSİYON
#endif // LORA_COMMS_H