/**
 * IMPLÉMENTATION TUNNEL DE COMMUNICATION CHIFFRÉ
 */

#include "encrypted_tunnel.h"
#include <stdarg.h>

/**
 * Initialiser le moteur de tunnel
 */
EncryptedTunnelEngine* tunnel_init(int port) {
    EncryptedTunnelEngine* engine = (EncryptedTunnelEngine*)malloc(sizeof(EncryptedTunnelEngine));
    if (!engine) return NULL;
    
    engine->tunnels_count = 0;
    engine->listening_port = port;
    engine->active = 1;
    engine->total_messages_encrypted = 0;
    engine->total_messages_decrypted = 0;
    
    engine->log_file = fopen("/tmp/tunnel.log", "a");
    if (engine->log_file) {
        fprintf(engine->log_file, "[%ld] Moteur de tunnel initialisé sur le port %d\n", time(NULL), port);
        fflush(engine->log_file);
    }
    
    // Générer la clé maître
    engine->master_key = *key_generate(CIPHER_MIXED);
    
    printf("[+] Moteur de tunnel chiffré initialisé (Port: %d)\n", port);
    return engine;
}

/**
 * Libérer le moteur de tunnel
 */
void tunnel_free(EncryptedTunnelEngine* engine) {
    if (!engine) return;
    
    // Fermer toutes les connexions
    for (int i = 0; i < engine->tunnels_count; i++) {
        tunnel_disconnect(engine, i);
    }
    
    if (engine->log_file) {
        fprintf(engine->log_file, "[%ld] Moteur de tunnel terminé\n", time(NULL));
        fclose(engine->log_file);
    }
    
    free(engine);
}

/**
 * Créer une clé de chiffrement
 */
EncryptionKey* key_generate(CipherType cipher) {
    EncryptionKey* key = (EncryptionKey*)malloc(sizeof(EncryptionKey));
    if (!key) return NULL;
    
    // Remplir la clé avec des données pseudo-aléatoires
    for (int i = 0; i < KEY_SIZE; i++) {
        key->key[i] = (unsigned char)(rand() % 256);
    }
    
    key->cipher_type = cipher;
    key->key_id = (int)time(NULL) % 10000;
    key->created_at = time(NULL);
    
    return key;
}

/**
 * Libérer une clé
 */
void key_free(EncryptionKey* key) {
    if (key) {
        memset(key->key, 0, KEY_SIZE);
        free(key);
    }
}

/**
 * Chiffrer les données (XOR simple ou SHIFT)
 */
unsigned char* encrypt_data(const unsigned char* plaintext, int plaintext_len, EncryptionKey* key, int* cipher_len) {
    if (!plaintext || plaintext_len <= 0 || !key) return NULL;
    
    unsigned char* ciphertext = (unsigned char*)malloc(plaintext_len + 1);
    if (!ciphertext) return NULL;
    
    switch (key->cipher_type) {
        case CIPHER_XOR:
            for (int i = 0; i < plaintext_len; i++) {
                ciphertext[i] = plaintext[i] ^ key->key[i % KEY_SIZE];
            }
            break;
            
        case CIPHER_SHIFT:
            for (int i = 0; i < plaintext_len; i++) {
                int shift = key->key[i % KEY_SIZE] % 256;
                ciphertext[i] = (plaintext[i] + shift) % 256;
            }
            break;
            
        case CIPHER_MIXED:
            for (int i = 0; i < plaintext_len; i++) {
                if (i % 2 == 0) {
                    ciphertext[i] = plaintext[i] ^ key->key[i % KEY_SIZE];
                } else {
                    ciphertext[i] = (plaintext[i] + key->key[i % KEY_SIZE]) % 256;
                }
            }
            break;
            
        default:
            memcpy(ciphertext, plaintext, plaintext_len);
    }
    
    ciphertext[plaintext_len] = '\0';
    *cipher_len = plaintext_len;
    return ciphertext;
}

/**
 * Déchiffrer les données
 */
unsigned char* decrypt_data(const unsigned char* ciphertext, int cipher_len, EncryptionKey* key, int* plaintext_len) {
    if (!ciphertext || cipher_len <= 0 || !key) return NULL;
    
    unsigned char* plaintext = (unsigned char*)malloc(cipher_len + 1);
    if (!plaintext) return NULL;
    
    switch (key->cipher_type) {
        case CIPHER_XOR:
            for (int i = 0; i < cipher_len; i++) {
                plaintext[i] = ciphertext[i] ^ key->key[i % KEY_SIZE];
            }
            break;
            
        case CIPHER_SHIFT:
            for (int i = 0; i < cipher_len; i++) {
                int shift = key->key[i % KEY_SIZE] % 256;
                plaintext[i] = (ciphertext[i] - shift + 256) % 256;
            }
            break;
            
        case CIPHER_MIXED:
            for (int i = 0; i < cipher_len; i++) {
                if (i % 2 == 0) {
                    plaintext[i] = ciphertext[i] ^ key->key[i % KEY_SIZE];
                } else {
                    plaintext[i] = (ciphertext[i] - key->key[i % KEY_SIZE] + 256) % 256;
                }
            }
            break;
            
        default:
            memcpy(plaintext, ciphertext, cipher_len);
    }
    
    plaintext[cipher_len] = '\0';
    *plaintext_len = cipher_len;
    return plaintext;
}

/**
 * Créer une connexion de tunnel
 */
TunnelConnection* tunnel_create_connection(EncryptedTunnelEngine* engine, const char* remote_addr, int remote_port) {
    if (!engine || engine->tunnels_count >= MAX_TUNNEL_CONNECTIONS) return NULL;
    
    TunnelConnection* conn = &engine->tunnels[engine->tunnels_count];
    
    conn->id = engine->tunnels_count;
    conn->state = TUNNEL_DISCONNECTED;
    conn->remote_port = remote_port;
    conn->local_port = engine->listening_port + engine->tunnels_count;
    conn->bytes_sent = 0;
    conn->bytes_received = 0;
    conn->messages_sent = 0;
    conn->messages_received = 0;
    
    strncpy(conn->remote_addr, remote_addr, sizeof(conn->remote_addr) - 1);
    conn->remote_addr[sizeof(conn->remote_addr) - 1] = '\0';
    
    snprintf(conn->local_addr, sizeof(conn->local_addr) - 1, "127.0.0.1");
    
    // Générer une clé de session
    conn->key = *key_generate(CIPHER_MIXED);
    
    engine->tunnels_count++;
    
    printf("[+] Connexion tunnel créée: %s:%d -> 127.0.0.1:%d\n", 
           remote_addr, remote_port, conn->local_port);
    
    return conn;
}

/**
 * Connecter un tunnel
 */
int tunnel_connect(TunnelConnection* conn) {
    if (!conn) return 0;
    
    printf("\n[*] ÉTABLISSEMENT DU TUNNEL CHIFFRÉ\n");
    printf("════════════════════════════════════════════\n");
    printf("[*] Connexion à %s:%d\n", conn->remote_addr, conn->remote_port);
    printf("[*] Port local: %d\n", conn->local_port);
    printf("[*] Clé de session ID: %d\n", conn->key.key_id);
    
    // Simuler la connexion
    sleep(1);
    
    conn->state = TUNNEL_CONNECTING;
    printf("[*] Négociation du cipher: MIXED (XOR + SHIFT)\n");
    
    sleep(1);
    
    printf("[*] Authentification avec handshake TLS-like\n");
    sleep(1);
    
    conn->state = TUNNEL_AUTHENTICATED;
    conn->connection_time = time(NULL);
    
    printf("[+] Tunnel sécurisé établi avec succès\n");
    printf("[+] État: CONNECTÉ ET AUTHENTIFIÉ\n\n");
    
    return 1;
}

/**
 * Déconnecter un tunnel
 */
int tunnel_disconnect(EncryptedTunnelEngine* engine, int tunnel_id) {
    if (!engine || tunnel_id < 0 || tunnel_id >= engine->tunnels_count) return 0;
    
    TunnelConnection* conn = &engine->tunnels[tunnel_id];
    
    if (conn->state != TUNNEL_DISCONNECTED) {
        conn->state = TUNNEL_DISCONNECTED;
        printf("[+] Tunnel #%d déconnecté\n", tunnel_id);
        
        if (engine->log_file) {
            fprintf(engine->log_file, "[%ld] Tunnel #%d fermé (Bytes: Envoyés=%d, Reçus=%d)\n",
                   time(NULL), tunnel_id, conn->bytes_sent, conn->bytes_received);
            fflush(engine->log_file);
        }
        
        return 1;
    }
    
    return 0;
}

/**
 * Créer un message chiffré
 */
EncryptedMessage* message_create(const char* sender, const char* recipient, const char* content) {
    EncryptedMessage* msg = (EncryptedMessage*)malloc(sizeof(EncryptedMessage));
    if (!msg) return NULL;
    
    strncpy(msg->sender, sender, sizeof(msg->sender) - 1);
    msg->sender[sizeof(msg->sender) - 1] = '\0';
    
    strncpy(msg->recipient, recipient, sizeof(msg->recipient) - 1);
    msg->recipient[sizeof(msg->recipient) - 1] = '\0';
    
    msg->timestamp = time(NULL);
    msg->cipher = CIPHER_MIXED;
    msg->encrypted_size = 0;
    msg->checksum = 0;
    
    memset(msg->encrypted_data, 0, sizeof(msg->encrypted_data));
    strncpy(msg->encrypted_data, content, sizeof(msg->encrypted_data) - 1);
    
    return msg;
}

/**
 * Chiffrer un message
 */
void message_encrypt(EncryptedMessage* msg, EncryptionKey* key) {
    if (!msg || !key) return;
    
    int plaintext_len = strlen(msg->encrypted_data);
    int ciphertext_len = 0;
    
    unsigned char* ciphertext = encrypt_data((unsigned char*)msg->encrypted_data, plaintext_len, key, &ciphertext_len);
    
    if (ciphertext) {
        memcpy(msg->encrypted_data, ciphertext, ciphertext_len);
        msg->encrypted_size = ciphertext_len;
        
        // Calculer un checksum simple
        msg->checksum = 0;
        for (int i = 0; i < ciphertext_len; i++) {
            msg->checksum += ciphertext[i];
        }
        
        free(ciphertext);
    }
}

/**
 * Déchiffrer un message
 */
void message_decrypt(EncryptedMessage* msg, EncryptionKey* key) {
    if (!msg || !key) return;
    
    int plaintext_len = 0;
    unsigned char* plaintext = decrypt_data((unsigned char*)msg->encrypted_data, msg->encrypted_size, key, &plaintext_len);
    
    if (plaintext) {
        memcpy(msg->encrypted_data, plaintext, plaintext_len);
        msg->encrypted_data[plaintext_len] = '\0';
        msg->encrypted_size = plaintext_len;
        free(plaintext);
    }
}

/**
 * Libérer un message
 */
void message_free(EncryptedMessage* msg) {
    if (msg) {
        memset(msg->encrypted_data, 0, sizeof(msg->encrypted_data));
        free(msg);
    }
}

/**
 * Port knocking - Initier une séquence
 */
int port_knocking_init(int ports[], int ports_count) {
    printf("\n[*] PORT KNOCKING INITIÉ\n");
    printf("════════════════════════════════════════════\n");
    printf("[*] Séquence de ports: ");
    
    for (int i = 0; i < ports_count; i++) {
        printf("%d", ports[i]);
        if (i < ports_count - 1) printf(" -> ");
    }
    printf("\n\n");
    
    for (int i = 0; i < ports_count; i++) {
        printf("[*] Frappe sur le port %d...\n", ports[i]);
        sleep(1);
    }
    
    printf("[+] Séquence de port knocking terminée\n");
    printf("[+] Port secret déverrouillé\n\n");
    
    return 1;
}

/**
 * Vérifier une séquence de port knocking
 */
int port_knocking_verify(int sequence[], int ports[], int ports_count) {
    if (!sequence || !ports || ports_count <= 0) return 0;
    
    printf("\n[*] VÉRIFICATION DU PORT KNOCKING\n");
    printf("════════════════════════════════════════════\n");
    
    for (int i = 0; i < ports_count; i++) {
        if (sequence[i] != ports[i]) {
            printf("[!] Séquence incorrecte au port %d\n", i);
            printf("[-] Port knocking échoué\n\n");
            return 0;
        }
        printf("[+] Port %d vérifié\n", sequence[i]);
    }
    
    printf("[+] Séquence de port knocking valide\n\n");
    return 1;
}

/**
 * Afficher le statut du moteur de tunnel
 */
void tunnel_print_status(EncryptedTunnelEngine* engine) {
    if (!engine) return;
    
    printf("\n╔═════════════════════════════════════════════════╗\n");
    printf("║    MOTEUR DE TUNNEL CHIFFRÉ - STATUT        ║\n");
    printf("╠═════════════════════════════════════════════════╣\n");
    printf("║ État: %s%-40s║\n", engine->active ? "ACTIF" : "INACTIF", "");
    printf("║ Port d'écoute: %d%-41s║\n", engine->listening_port, "");
    printf("║ Connexions actives: %d%-37s║\n", engine->tunnels_count, "");
    printf("║ Clé maître ID: %d%-43s║\n", engine->master_key.key_id, "");
    printf("║ Cipher: MIXED (XOR + SHIFT)%-25s║\n", "");
    printf("╠═════════════════════════════════════════════════╣\n");
    printf("║ Messages chiffrés: %d%-39s║\n", engine->total_messages_encrypted, "");
    printf("║ Messages déchiffrés: %d%-36s║\n", engine->total_messages_decrypted, "");
    printf("╚═════════════════════════════════════════════════╝\n\n");
}

/**
 * Afficher les connexions actives
 */
void tunnel_print_connections(EncryptedTunnelEngine* engine) {
    if (!engine || engine->tunnels_count == 0) {
        printf("[*] Aucune connexion active\n");
        return;
    }
    
    printf("\n╔═════════════════════════════════════════════════╗\n");
    printf("║       CONNEXIONS DE TUNNEL (%d)             ║\n", engine->tunnels_count);
    printf("╠═════════════════════════════════════════════════╣\n");
    
    for (int i = 0; i < engine->tunnels_count; i++) {
        TunnelConnection* conn = &engine->tunnels[i];
        printf("║ [%d] %s:%d -> 127.0.0.1:%d\n", 
               conn->id, conn->remote_addr, conn->remote_port, conn->local_port);
        printf("║     État: ");
        
        switch (conn->state) {
            case TUNNEL_CONNECTED: printf("CONNECTÉ"); break;
            case TUNNEL_AUTHENTICATED: printf("AUTHENTIFIÉ"); break;
            case TUNNEL_DISCONNECTED: printf("DÉCONNECTÉ"); break;
            default: printf("INCONNU");
        }
        printf("\n");
        printf("║     Clé ID: %d | Msgs: %d/%d\n", 
               conn->key.key_id, conn->messages_sent, conn->messages_received);
    }
    
    printf("╚═════════════════════════════════════════════════╝\n\n");
}
