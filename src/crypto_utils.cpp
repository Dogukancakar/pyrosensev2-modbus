#include "crypto_utils.h"

// AES blok boyutu
const int AES_BLOCK_SIZE = 16;

// Cihaz yapılandırması (AES ve HMAC anahtarları)
const DeviceConfig DEVICE_CONFIG = {
    .aes_key = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10}, // Örnek AES anahtarı
    .hmac_key = {0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
                 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20,
                 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
                 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30} // Örnek HMAC anahtarı
};

// Yardımcı fonksiyonlar
void print_hex_crypto(const char* label, const unsigned char* data, size_t len) {
    Serial.print(label);
    for (size_t i = 0; i < len; i++) {
        if (i % 16 == 0) Serial.println();
        Serial.printf("%02X ", data[i]);
    }
    Serial.println();
}

void print_mbedtls_error_crypto(const char* func_name, int ret_val) {
    char error_buf[100];
    mbedtls_strerror(ret_val, error_buf, sizeof(error_buf));
    Serial.printf("%s failed: -0x%04X - %s\n", func_name, -ret_val, error_buf);
}

// PKCS7 padding fonksiyonları
size_t pkcs7_pad_crypto(const unsigned char* input, size_t input_len, unsigned char* output, size_t output_buf_size) {
    size_t pad_len = AES_BLOCK_SIZE - (input_len % AES_BLOCK_SIZE);
    if (input_len + pad_len > output_buf_size) return 0;

    memcpy(output, input, input_len);
    memset(output + input_len, pad_len, pad_len);
    return input_len + pad_len;
}

int pkcs7_unpad_crypto(const unsigned char* input, size_t input_len, unsigned char* output, size_t output_buf_size) {
    if (input_len == 0 || input_len % AES_BLOCK_SIZE != 0) return -1;

    size_t pad_len = input[input_len - 1];
    if (pad_len > AES_BLOCK_SIZE || pad_len > input_len) return -1;

    size_t unpadded_len = input_len - pad_len;
    if (unpadded_len > output_buf_size) return -1;

    memcpy(output, input, unpadded_len);
    return unpadded_len;
}

// AES ve HMAC işlemleri
bool encrypt_lora_payload(const unsigned char* plaintext, size_t plaintext_len,
                          const DeviceConfig& settings,
                          unsigned char* output_payload_buffer, size_t output_buffer_max_len,
                          size_t* actual_payload_len) {
    // AES ve HMAC işlemleri burada yapılır
    // Örnek: AES şifreleme ve HMAC hesaplama
    return true;
}

bool decrypt_lora_payload(const unsigned char* received_payload, size_t received_payload_len,
                          const DeviceConfig& settings,
                          unsigned char* output_plaintext_buffer, size_t output_buffer_max_len,
                          size_t* actual_plaintext_len) {
    // AES ve HMAC doğrulama işlemleri burada yapılır
    return true;
}