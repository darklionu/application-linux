/**
 * TUNNEL DE COMMUNICATION CHIFFRÉ
 * Communication sécurisée entre les agents
 */

#ifndef ENCRYPTED_TUNNEL_H
#define ENCRYPTED_TUNNEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX_MESSAGE_LEN 2048
#define MAX_TUNNEL_CONNECTIONS 50
#define TUNNEL_PORT 9999
#define KEY_SIZE 32

/**
 * États du tunnel
 */
typedef enum {
    TUNNEL_DISCONNECTED,
    TUNNEL_CONNECTING,
    TUNNEL_CONNECTED,
    TUNNEL_AUTHENTICATED,
    TUNNEL_ERROR
} TunnelState;

/**
 * Types de chiffrement
 */
typedef enum {
    CIPHER_NONE = 0,
    CIPHER_XOR = 1,
    CIPHER_SHIFT = 2,
    CIPHER_MIXED = 3
} CipherType;

/**
 * Clé de chiffrement
 */
typedef struct {
    unsigned char key[KEY_SIZE];
    CipherType cipher_type;
    int key_id;
    time_t created_at;
} EncryptionKey;

/**
 * Message chiffré
 */
typedef struct {
    char sender[64];
    char recipient[64];
    time_t timestamp;
    CipherType cipher;
    char encrypted_data[MAX_MESSAGE_LEN];
    int encrypted_size;
    unsigned int checksum;
} EncryptedMessage;

/**
 * Configuration d'un tunnel
 */
typedef struct {
    int id;
    TunnelState state;
    char local_addr[32];
    char remote_addr[32];
    int local_port;
    int remote_port;
    EncryptionKey key;
    time_t connection_time;
    int bytes_sent;
    int bytes_received;
    int messages_sent;
    int messages_received;
} TunnelConnection;

/**
 * Moteur de tunnel chiffré
 */
typedef struct {
    TunnelConnection tunnels[MAX_TUNNEL_CONNECTIONS];
    int tunnels_count;
    EncryptionKey master_key;
    int listening_port;
    int active;
    FILE* log_file;
    int total_messages_encrypted;
    int total_messages_decrypted;
} EncryptedTunnelEngine;

// Fonctions de tunnel
EncryptedTunnelEngine* tunnel_init(int port);
void tunnel_free(EncryptedTunnelEngine* engine);
TunnelConnection* tunnel_create_connection(EncryptedTunnelEngine* engine, const char* remote_addr, int remote_port);
int tunnel_connect(TunnelConnection* conn);
int tunnel_disconnect(EncryptedTunnelEngine* engine, int tunnel_id);

// Fonctions de chiffrement
EncryptionKey* key_generate(CipherType cipher);
void key_free(EncryptionKey* key);
unsigned char* encrypt_data(const unsigned char* plaintext, int plaintext_len, EncryptionKey* key, int* cipher_len);
unsigned char* decrypt_data(const unsigned char* ciphertext, int cipher_len, EncryptionKey* key, int* plaintext_len);

// Fonctions de messages
EncryptedMessage* message_create(const char* sender, const char* recipient, const char* content);
void message_encrypt(EncryptedMessage* msg, EncryptionKey* key);
void message_decrypt(EncryptedMessage* msg, EncryptionKey* key);
void message_free(EncryptedMessage* msg);

// Port knocking
int port_knocking_init(int ports[], int ports_count);
int port_knocking_verify(int sequence[], int ports[], int ports_count);

// Affichage
void tunnel_print_status(EncryptedTunnelEngine* engine);
void tunnel_print_connections(EncryptedTunnelEngine* engine);

#endif
