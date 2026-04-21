/**
 * SYSTÈME DE MASQUAGE DES ATTAQUES
 * Techniques d'obfuscation et d'évasion
 */

#ifndef ATTACK_OBFUSCATION_H
#define ATTACK_OBFUSCATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_OBFUSCATION_TECHNIQUES 20

/**
 * Techniques de masquage
 */
typedef enum {
    OBFUSCATION_NONE,
    OBFUSCATION_TIMING,          // Délais aléatoires
    OBFUSCATION_FRAGMENTATION,   // Fragmentation du trafic
    OBFUSCATION_SIGNATURE_EVASION, // Évasion de signatures
    OBFUSCATION_PROTOCOL_MIXING,  // Mélange de protocoles
    OBFUSCATION_ENCODING,         // Encodage des données
    OBFUSCATION_PROXY,            // Utilisation de proxy
    OBFUSCATION_ENCRYPTION        // Chiffrement
} ObfuscationTechnique;

/**
 * Configuration de masquage
 */
typedef struct {
    ObfuscationTechnique technique;
    int enabled;
    int effectiveness;           // 0-100: efficacité du masquage
    char description[256];
} ObfuscationConfig;

/**
 * Moteur de masquage d'attaques
 */
typedef struct {
    ObfuscationConfig techniques[MAX_OBFUSCATION_TECHNIQUES];
    int techniques_count;
    int active_techniques;
    int detection_avoidance;     // Score de détection (0-100)
    FILE* log_file;
    time_t last_attack_time;
} AttackObfuscationEngine;

// Fonctions
AttackObfuscationEngine* obfuscation_init(void);
void obfuscation_free(AttackObfuscationEngine* engine);
void obfuscation_add_technique(AttackObfuscationEngine* engine, ObfuscationTechnique tech, int effectiveness);
void obfuscation_enable_all(AttackObfuscationEngine* engine);
int obfuscation_apply_masking(AttackObfuscationEngine* engine, const char* original_payload, char* masked_payload, int max_len);
int obfuscation_add_timing_evasion(void);
char* obfuscation_encode_payload(const char* payload);
void obfuscation_fragment_attack(const char* attack_data, int fragment_size);
void obfuscation_print_status(AttackObfuscationEngine* engine);

// Utilitaires
int random_delay(int min_ms, int max_ms);
char* base64_encode(const unsigned char* data, int data_len);
void randomize_user_agent(char* ua_buffer, int buffer_size);

#endif
