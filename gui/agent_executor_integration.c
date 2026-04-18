/**
 * Exemple d'intégration du système GUI unifié avec les agents C
 * 
 * Ce fichier montre comment les agents C peuvent lire et utiliser
 * les configurations d'attaques générées par le GUI unifié.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TECHNIQUES 50
#define NAME_LEN 256

/**
 * Structure pour représenter une technique chargée
 */
typedef struct {
    char id[NAME_LEN];
    char phase[NAME_LEN];
    int loaded;
} LoadedTechnique;

/**
 * Charge les attaques configurées pour un agent spécifique
 * 
 * @param agent_id L'ID de l'agent (ex: "red_recon", "red_exploit")
 * @param techniques Array pour stocker les techniques chargées
 * @return Le nombre de techniques chargées
 */
int load_agent_attacks_from_gui_config(const char *agent_id, LoadedTechnique *techniques) {
    FILE *f = fopen("attack_config.txt", "r");
    if (!f) {
        fprintf(stderr, "[!] Impossible de lire attack_config.txt\n");
        fprintf(stderr, "[*] Assurez-vous d'avoir exécuté le GUI d'abord\n");
        return 0;
    }

    char line[256];
    char current_agent[NAME_LEN] = "";
    int technique_count = 0;

    printf("\n[*] Chargement des attaques pour l'agent: %s\n", agent_id);
    printf("╔════════════════════════════════════════════════════════╗\n");

    while (fgets(line, sizeof(line), f) && technique_count < MAX_TECHNIQUES) {
        // Trim trailing newline
        line[strcspn(line, "\r\n")] = 0;

        // Skip empty lines and comments
        if (strlen(line) == 0 || line[0] == '#') {
            continue;
        }

        // Check for agent section header
        if (line[0] == '[' && line[strlen(line)-1] == ']') {
            strncpy(current_agent, line + 1, strlen(line) - 2);
            current_agent[strlen(line) - 2] = '\0';
            printf("║ Agent trouvé: %s\n", current_agent);
            continue;
        }

        // If this is the agent we're looking for, load the technique
        if (strcmp(current_agent, agent_id) == 0 && strlen(line) > 0) {
            strncpy(techniques[technique_count].id, line, NAME_LEN - 1);
            techniques[technique_count].id[NAME_LEN - 1] = '\0';
            techniques[technique_count].loaded = 1;
            
            printf("║ [✓] Technique chargée: %s\n", line);
            technique_count++;
        }
    }

    fclose(f);
    printf("╚════════════════════════════════════════════════════════╝\n");
    printf("[✓] Total: %d technique(s) chargée(s)\n\n", technique_count);

    return technique_count;
}

/**
 * Affiche les techniques d'un agent
 */
void print_agent_techniques(const char *agent_id, const LoadedTechnique *techniques, int count) {
    printf("\n╔════════════════════════════════════════════════════════╗\n");
    printf("║ TECHNIQUES CONFIGURÉES POUR: %s\n", agent_id);
    printf("╠════════════════════════════════════════════════════════╣\n");
    
    if (count == 0) {
        printf("║ Aucune technique configurée\n");
    } else {
        for (int i = 0; i < count; i++) {
            printf("║ %d. %s\n", i + 1, techniques[i].id);
        }
    }
    
    printf("╚════════════════════════════════════════════════════════╝\n");
}

/**
 * Simule l'exécution des attaques configurées
 */
void simulate_agent_execution(const char *agent_name, 
                             const char *agent_id,
                             const LoadedTechnique *techniques, 
                             int count) {
    printf("\n╔════════════════════════════════════════════════════════╗\n");
    printf("║ SIMULATION D'EXÉCUTION - %s\n", agent_name);
    printf("╠════════════════════════════════════════════════════════╣\n");
    
    if (count == 0) {
        printf("║ [!] Aucune attaque configurée, agent inactif\n");
    } else {
        printf("║ Exécution de %d technique(s)...\n", count);
        printf("╠════════════════════════════════════════════════════════╣\n");
        
        for (int i = 0; i < count; i++) {
            int success = rand() % 100;
            char *result = success > 30 ? "[✓] Succès" : "[!] Échoué";
            
            printf("║ [%d/%d] %s - %s\n", i + 1, count, techniques[i].id, result);
        }
    }
    
    printf("╚════════════════════════════════════════════════════════╝\n");
}

/**
 * Exemple principal montrant l'intégration
 */
int main(int argc, char *argv[]) {
    printf("\n╔════════════════════════════════════════════════════════╗\n");
    printf("║    INTÉGRATION GUI - SYSTÈME D'EXÉCUTION D'AGENTS    ║\n");
    printf("║                  Exemple d'Intégration              ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n");

    // Définir les agents à charger
    const char *agent_ids[] = {"red_recon", "red_exploit", "red_blackhat"};
    const char *agent_names[] = {"Red Recon Agent", "Red Exploit Agent", "Red BlackHat Agent"};
    int num_agents = 3;

    LoadedTechnique techniques[MAX_TECHNIQUES];
    int technique_counts[3];

    printf("\n[*] Chargement des configurations depuis attack_config.txt\n");
    printf("[*] Assurez-vous d'avoir lancé le GUI d'abord!\n");

    // Charger les configurations pour chaque agent
    for (int i = 0; i < num_agents; i++) {
        technique_counts[i] = load_agent_attacks_from_gui_config(agent_ids[i], techniques);
    }

    printf("\n[*] Affichage des configurations chargées:\n");

    // Afficher et simuler l'exécution pour chaque agent
    for (int i = 0; i < num_agents; i++) {
        print_agent_techniques(agent_ids[i], techniques, technique_counts[i]);
        simulate_agent_execution(agent_names[i], agent_ids[i], techniques, technique_counts[i]);
        
        // Laisser un peu de temps entre les agents
        printf("\n");
    }

    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║              FIN DE LA SIMULATION                     ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n\n");

    return 0;
}

/**
 * INSTRUCTIONS DE COMPILATION ET D'UTILISATION:
 * 
 * 1. D'abord, lancez le GUI unifié et configurez les attaques:
 *    $ cd /workspaces/application-linux/gui
 *    $ make -f Makefile_unified run
 *    (Suivez le menu pour configurer attack_config.txt)
 * 
 * 2. Compilez cet exemple:
 *    $ gcc -o agent_executor agent_executor_integration.c -Wall -Wextra
 * 
 * 3. Exécutez le programme:
 *    $ ./agent_executor
 * 
 * RÉSULTAT ATTENDU:
 * 
 * - Le programme lit attack_config.txt
 * - Affiche les techniques configurées pour chaque agent
 * - Simule l'exécution des techniques
 * - Montre le succès/échec aléatoire
 * 
 * C'EST L'INTÉGRATION ENTRE LE GUI (C++) ET LES AGENTS (C)!
 */
