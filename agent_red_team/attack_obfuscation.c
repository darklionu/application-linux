/**
 * IMPLÉMENTATION SYSTÈME DE MASQUAGE D'ATTAQUES
 */

#include "attack_obfuscation.h"
#include <stdarg.h>

static const char* base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/**
 * Initialiser le moteur de masquage
 */
AttackObfuscationEngine* obfuscation_init(void) {
    AttackObfuscationEngine* engine = (AttackObfuscationEngine*)malloc(sizeof(AttackObfuscationEngine));
    if (!engine) return NULL;
    
    engine->techniques_count = 0;
    engine->active_techniques = 0;
    engine->detection_avoidance = 0;
    engine->last_attack_time = 0;
    
    engine->log_file = fopen("/tmp/obfuscation.log", "a");
    if (engine->log_file) {
        fprintf(engine->log_file, "[%ld] Moteur de masquage initialisé\n", time(NULL));
        fflush(engine->log_file);
    }
    
    // Ajouter les techniques par défaut
    obfuscation_add_technique(engine, OBFUSCATION_TIMING, 75);
    obfuscation_add_technique(engine, OBFUSCATION_SIGNATURE_EVASION, 80);
    obfuscation_add_technique(engine, OBFUSCATION_ENCODING, 70);
    obfuscation_add_technique(engine, OBFUSCATION_FRAGMENTATION, 85);
    obfuscation_add_technique(engine, OBFUSCATION_PROXY, 65);
    
    printf("[+] Moteur de masquage d'attaques initialisé\n");
    return engine;
}

/**
 * Libérer le moteur de masquage
 */
void obfuscation_free(AttackObfuscationEngine* engine) {
    if (!engine) return;
    
    if (engine->log_file) {
        fprintf(engine->log_file, "[%ld] Moteur de masquage terminé\n", time(NULL));
        fclose(engine->log_file);
    }
    
    free(engine);
}

/**
 * Ajouter une technique de masquage
 */
void obfuscation_add_technique(AttackObfuscationEngine* engine, ObfuscationTechnique tech, int effectiveness) {
    if (!engine || engine->techniques_count >= MAX_OBFUSCATION_TECHNIQUES) return;
    
    ObfuscationConfig* config = &engine->techniques[engine->techniques_count];
    
    config->technique = tech;
    config->enabled = 1;
    config->effectiveness = effectiveness;
    
    switch (tech) {
        case OBFUSCATION_TIMING:
            snprintf(config->description, sizeof(config->description) - 1, 
                     "Délais aléatoires entre les paquets (efficacité: %d%%)", effectiveness);
            break;
        case OBFUSCATION_FRAGMENTATION:
            snprintf(config->description, sizeof(config->description) - 1, 
                     "Fragmentation du trafic (efficacité: %d%%)", effectiveness);
            break;
        case OBFUSCATION_SIGNATURE_EVASION:
            snprintf(config->description, sizeof(config->description) - 1, 
                     "Évasion de signatures (efficacité: %d%%)", effectiveness);
            break;
        case OBFUSCATION_PROTOCOL_MIXING:
            snprintf(config->description, sizeof(config->description) - 1, 
                     "Mélange de protocoles (efficacité: %d%%)", effectiveness);
            break;
        case OBFUSCATION_ENCODING:
            snprintf(config->description, sizeof(config->description) - 1, 
                     "Encodage des données (efficacité: %d%%)", effectiveness);
            break;
        case OBFUSCATION_PROXY:
            snprintf(config->description, sizeof(config->description) - 1, 
                     "Utilisation de proxy (efficacité: %d%%)", effectiveness);
            break;
        case OBFUSCATION_ENCRYPTION:
            snprintf(config->description, sizeof(config->description) - 1, 
                     "Chiffrement des données (efficacité: %d%%)", effectiveness);
            break;
        default:
            snprintf(config->description, sizeof(config->description) - 1, 
                     "Technique inconnue (efficacité: %d%%)", effectiveness);
    }
    
    engine->techniques_count++;
}

/**
 * Activer toutes les techniques de masquage
 */
void obfuscation_enable_all(AttackObfuscationEngine* engine) {
    if (!engine) return;
    
    printf("\n[*] ACTIVATION DE TOUTES LES TECHNIQUES DE MASQUAGE\n");
    printf("════════════════════════════════════════════════════\n");
    
    for (int i = 0; i < engine->techniques_count; i++) {
        engine->techniques[i].enabled = 1;
        engine->active_techniques++;
        printf("[+] %s\n", engine->techniques[i].description);
    }
    
    printf("\n");
}

/**
 * Délai aléatoire
 */
int random_delay(int min_ms, int max_ms) {
    if (min_ms < 0) min_ms = 0;
    if (max_ms < min_ms) max_ms = min_ms;
    
    int delay = min_ms + (rand() % (max_ms - min_ms + 1));
    usleep(delay * 1000);  // Convertir en microsecondes
    return delay;
}

/**
 * Coder en base64
 */
char* base64_encode(const unsigned char* data, int data_len) {
    if (!data || data_len <= 0) return NULL;
    
    int output_len = ((data_len + 2) / 3) * 4 + 1;
    char* output = (char*)malloc(output_len);
    if (!output) return NULL;
    
    int i = 0, j = 0;
    unsigned char buf[3];
    
    while (i < data_len) {
        buf[0] = data[i++];
        buf[1] = (i < data_len) ? data[i++] : 0;
        buf[2] = (i < data_len) ? data[i++] : 0;
        
        output[j++] = base64_chars[buf[0] >> 2];
        output[j++] = base64_chars[((buf[0] & 0x03) << 4) | (buf[1] >> 4)];
        output[j++] = (i - 1 < data_len) ? base64_chars[((buf[1] & 0x0f) << 2) | (buf[2] >> 6)] : '=';
        output[j++] = (i < data_len) ? base64_chars[buf[2] & 0x3f] : '=';
    }
    
    output[j] = '\0';
    return output;
}

/**
 * Générer un User-Agent aléatoire
 */
void randomize_user_agent(char* ua_buffer, int buffer_size) {
    const char* user_agents[] = {
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36",
        "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36",
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:89.0) Gecko/20100101 Firefox/89.0",
        "Mozilla/5.0 (X11; Linux x86_64; rv:89.0) Gecko/20100101 Firefox/89.0",
        "Mozilla/5.0 (compatible; curl/7.78.0)",
        "Mozilla/5.0 (compatible; Wget/1.21.1)"
    };
    
    int idx = rand() % (sizeof(user_agents) / sizeof(user_agents[0]));
    strncpy(ua_buffer, user_agents[idx], buffer_size - 1);
    ua_buffer[buffer_size - 1] = '\0';
}

/**
 * Appliquer le masquage à une attaque
 */
int obfuscation_apply_masking(AttackObfuscationEngine* engine, const char* original_payload, 
                               char* masked_payload, int max_len) {
    if (!engine || !original_payload || !masked_payload) return 0;
    
    memset(masked_payload, 0, max_len);
    strncpy(masked_payload, original_payload, max_len - 1);
    
    int masking_applied = 0;
    int total_effectiveness = 0;
    
    printf("\n[*] APPLICATION DES TECHNIQUES DE MASQUAGE\n");
    printf("═════════════════════════════════════════════════\n");
    
    // Appliquer le timing
    for (int i = 0; i < engine->techniques_count; i++) {
        if (!engine->techniques[i].enabled) continue;
        
        switch (engine->techniques[i].technique) {
            case OBFUSCATION_TIMING: {
                printf("[+] Timing: Délai de ");
                int delay = random_delay(100, 500);
                printf("%d ms appliqué\n", delay);
                masking_applied = 1;
                total_effectiveness += engine->techniques[i].effectiveness;
                break;
            }
            
            case OBFUSCATION_ENCODING: {
                printf("[+] Encodage: Conversion base64\n");
                // Simuler l'encodage
                char temp[512];
                snprintf(temp, sizeof(temp) - 1, "[ENCODED:%s]", original_payload);
                strncpy(masked_payload, temp, max_len - 1);
                masking_applied = 1;
                total_effectiveness += engine->techniques[i].effectiveness;
                break;
            }
            
            case OBFUSCATION_SIGNATURE_EVASION: {
                printf("[+] Évasion: Modification des signatures détectées\n");
                // Ajouter du bruit
                for (int j = 0; j < (int)strlen(masked_payload) && j < max_len - 10; j++) {
                    if (rand() % 10 == 0) {
                        masked_payload[j] = (masked_payload[j] + (rand() % 5)) % 256;
                    }
                }
                masking_applied = 1;
                total_effectiveness += engine->techniques[i].effectiveness;
                break;
            }
            
            case OBFUSCATION_FRAGMENTATION: {
                printf("[+] Fragmentation: Découpage de l'attaque\n");
                masking_applied = 1;
                total_effectiveness += engine->techniques[i].effectiveness;
                break;
            }
            
            case OBFUSCATION_PROXY: {
                printf("[+] Proxy: Tunnel via serveur relais\n");
                masking_applied = 1;
                total_effectiveness += engine->techniques[i].effectiveness;
                break;
            }
            
            default:
                break;
        }
    }
    
    engine->detection_avoidance = (total_effectiveness / (engine->active_techniques > 0 ? engine->active_techniques : 1));
    
    printf("\n[*] Score d'évasion: %d%%\n\n", engine->detection_avoidance);
    
    if (engine->log_file) {
        fprintf(engine->log_file, "[%ld] Masquage appliqué - Évasion: %d%%\n", time(NULL), engine->detection_avoidance);
        fflush(engine->log_file);
    }
    
    return masking_applied;
}

/**
 * Ajouter l'évasion de timing
 */
int obfuscation_add_timing_evasion(void) {
    printf("[*] Ajout d'évasion de timing...\n");
    
    int delays[] = {0, 100, 250, 500, 1000};
    int delay = delays[rand() % (sizeof(delays) / sizeof(delays[0]))];
    
    if (delay > 0) {
        printf("[+] Délai aléatoire appliqué: %d ms\n", delay);
        usleep(delay * 1000);
        return 1;
    }
    
    return 0;
}

/**
 * Fragmenter une attaque
 */
void obfuscation_fragment_attack(const char* attack_data, int fragment_size) {
    if (!attack_data || fragment_size <= 0) return;
    
    printf("\n[*] FRAGMENTATION DE L'ATTAQUE\n");
    printf("════════════════════════════════════════════\n");
    
    int data_len = strlen(attack_data);
    int fragments = (data_len + fragment_size - 1) / fragment_size;
    
    printf("[*] Taille originale: %d bytes\n", data_len);
    printf("[*] Taille des fragments: %d bytes\n", fragment_size);
    printf("[*] Nombre de fragments: %d\n\n", fragments);
    
    for (int i = 0; i < fragments; i++) {
        int start = i * fragment_size;
        int end = (start + fragment_size < data_len) ? start + fragment_size : data_len;
        int len = end - start;
        
        printf("[Fragment %d] Offset: %d, Taille: %d bytes\n", i+1, start, len);
        
        // Simuler le délai entre fragments
        if (i < fragments - 1) {
            int delay = random_delay(50, 200);
            printf("             Délai avant suivant: %d ms\n", delay);
        }
    }
    
    printf("\n[+] Fragmentation terminée\n\n");
}

/**
 * Afficher le statut du moteur de masquage
 */
void obfuscation_print_status(AttackObfuscationEngine* engine) {
    if (!engine) return;
    
    printf("\n╔═════════════════════════════════════════════════╗\n");
    printf("║     MOTEUR DE MASQUAGE D'ATTAQUES           ║\n");
    printf("╠═════════════════════════════════════════════════╣\n");
    printf("║ Techniques disponibles: %d                    ║\n", engine->techniques_count);
    printf("║ Techniques actives: %d                        ║\n", engine->active_techniques);
    printf("║ Score d'évasion: %d%%                         ║\n", engine->detection_avoidance);
    printf("╠═════════════════════════════════════════════════╣\n");
    
    for (int i = 0; i < engine->techniques_count && i < 8; i++) {
        printf("║ [%s] %s\n", 
               engine->techniques[i].enabled ? "X" : " ",
               engine->techniques[i].description);
    }
    
    printf("╚═════════════════════════════════════════════════╝\n\n");
}
