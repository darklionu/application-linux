#ifndef AGENT_H
#define AGENT_H

#define MAX_TECHNIQUES  50
#define MAX_TOOLS       20
#define NAME_LEN        64
#define PATH_LEN        256

// Résultat d'une action
typedef enum {
    RESULT_SUCCESS,
    RESULT_PARTIAL,
    RESULT_FAILURE
} ActionResult;

// Mode d'évolution
typedef enum {
    MODE_MANUAL,
    MODE_AUTO
} EvolutionMode;

// Phases d'attaque (kill chain simplifiée)
typedef enum {
    PHASE_RECON      = 0,
    PHASE_SCAN       = 1,
    PHASE_EXPLOIT    = 2,
    PHASE_LATERAL    = 3,
    PHASE_PERSIST    = 4
} Phase;

// Un outil système disponible
typedef struct {
    char        name[NAME_LEN];
    char        path[PATH_LEN];
    Phase       phase;
    int         enabled;
} Tool;

// Une technique offensive
typedef struct {
    char        name[NAME_LEN];
    char        tool_name[NAME_LEN];  // outil associé
    Phase       phase;
    int         success_rate;
    int         unlocked;
} Technique;

// L'agent red team
typedef struct {
    char        name[NAME_LEN];
    int         stealth;
    int         techniques_count;
    int         tools_count;
    int         auto_mode;
    Technique   techniques[MAX_TECHNIQUES];
    Tool        tools[MAX_TOOLS];
} Agent;

// Fonctions déclarées
int load_tools(Agent *a, const char *tools_dir);
int load_techniques_from_file(const char *filename);
void create_default_techniques(void);
ActionResult agent_execute(Agent *a, int tech_index);
int agent_auto_choose(Agent *a, Phase target_phase);
void agent_auto_progress(Agent *a, int iterations);
void agent_print_status(const Agent *a);
void agent_init(Agent *a, const char *name);

#endif