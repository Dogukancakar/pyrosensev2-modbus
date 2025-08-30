#ifndef CRYPTO_UTILS_H
#define CRYPTO_UTILS_H

#include <Arduino.h>
#include "mbedtls/aes.h"
#include "mbedtls/md.h"
#include "mbedtls/platform.h"
#include "mbedtls/error.h"

// AES blok boyutu sabiti
extern const int AES_BLOCK_SIZE;
#define HMAC_SIZE 32 // SHA256 HMAC çıktısı için 32 byte

// DeviceConfig yapısı: AES ve HMAC anahtarlarını içerir
struct DeviceConfig {
    unsigned char aes_key[16];  // AES-128 için 16 byte anahtar
    unsigned char hmac_key[32]; // HMAC-SHA256 için 32 byte anahtar
};

// Cihaz yapılandırması (AES ve HMAC anahtarları)
extern const DeviceConfig DEVICE_CONFIG;

// Yardımcı Fonksiyon Bildirimleri
void print_hex_crypto(const char* label, const unsigned char* data, size_t len);
void print_mbedtls_error_crypto(const char* func_name, int ret_val);
size_t pkcs7_pad_crypto(const unsigned char* input, size_t input_len, unsigned char* output, size_t output_buf_size);
int pkcs7_unpad_crypto(const unsigned char* input, size_t input_len, unsigned char* output, size_t output_buf_size);

/**
 * @brief Verilen düz metni AES-128-CBC ile şifreler, HMAC-SHA256 hesaplar ve LoRa için bir payload oluşturur.
 * Payload Formatı: IV (16 byte) + ŞifreliMetin (değişken uzunluk) + HMAC (32 byte)
 * 
 * @param plaintext Şifrelenecek düz metin.
 * @param plaintext_len Düz metnin uzunluğu.
 * @param settings Cihaz ayarları (AES ve HMAC anahtarlarını içerir).
 * @param output_payload_buffer Oluşturulan kriptolu payload'un yazılacağı buffer.
 * @param output_buffer_max_len output_payload_buffer'ın maksimum boyutu.
 * @param actual_payload_len Oluşturulan payload'un gerçek uzunluğu (çıkış parametresi).
 * @return bool İşlem başarılıysa true, değilse false.
 */
bool encrypt_lora_payload(const unsigned char* plaintext, size_t plaintext_len,
                          const DeviceConfig& settings,
                          unsigned char* output_payload_buffer, size_t output_buffer_max_len,
                          size_t* actual_payload_len);

/**
 * @brief Gelen LoRa payload'unun HMAC'ini doğrular ve AES-128-CBC ile şifresini çözer.
 * Payload Formatı: IV (16 byte) + ŞifreliMetin (değişken uzunluk) + HMAC (32 byte)
 * 
 * @param received_payload Alınan kriptolu payload.
 * @param received_payload_len Alınan payload'un uzunluğu.
 * @param settings Cihaz ayarları (AES ve HMAC anahtarlarını içerir).
 * @param output_plaintext_buffer Çözülen düz metnin yazılacağı buffer.
 * @param output_buffer_max_len output_plaintext_buffer'ın maksimum boyutu.
 * @param actual_plaintext_len Çözülen düz metnin gerçek uzunluğu (çıkış parametresi).
 * @return bool HMAC doğrulaması başarılı ve şifre çözme başarılıysa true, değilse false.
 */
bool decrypt_lora_payload(const unsigned char* received_payload, size_t received_payload_len,
                          const DeviceConfig& settings,
                          unsigned char* output_plaintext_buffer, size_t output_buffer_max_len,
                          size_t* actual_plaintext_len);

#endif // CRYPTO_UTILS_H