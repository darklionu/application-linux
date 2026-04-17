#ifndef BLUE_AGENT_H
#define BLUE_AGENT_H

#define BLUE_NAME_LEN 64

typedef enum {
    BLUE_RESULT_DETECTED,
    BLUE_RESULT_ALERT,
    BLUE_RESULT_STABLE
} BlueResult;

typedef struct {
    char name[BLUE_NAME_LEN];
    int alert_level;
    int readiness;
} BlueAgent;

void blue_agent_init(BlueAgent *agent, const char *name);
void blue_agent_print_status(const BlueAgent *agent);
BlueResult blue_agent_action(BlueAgent *agent, const char *action_name, int success_rate);

#endif // BLUE_AGENT_H
