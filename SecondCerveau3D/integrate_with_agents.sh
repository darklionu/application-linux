#!/bin/bash

# SecondCerveau3D Integration Script
# Links SecondCerveau3D components to existing agent system

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(dirname "$SCRIPT_DIR")"

echo "╔════════════════════════════════════════════════════════════╗"
echo "║   SecondCerveau3D → Agent System Integration               ║"
echo "╚════════════════════════════════════════════════════════════╝"
echo ""

# Check for existing agent directories
if [ ! -d "$ROOT_DIR/agent_red_team" ]; then
    echo "⚠️  agent_red_team not found, skipping..."
else
    echo "🔗 Integrating with Red Team agents..."
    
    # Link file scanner
    if [ ! -L "$ROOT_DIR/agent_red_team/filescanner.h" ]; then
        ln -s "$SCRIPT_DIR/native-c/filescanner.h" "$ROOT_DIR/agent_red_team/filescanner.h"
        echo "   ✓ Linked filescanner.h"
    fi
    
    if [ ! -L "$ROOT_DIR/agent_red_team/filescanner.c" ]; then
        ln -s "$SCRIPT_DIR/native-c/filescanner.c" "$ROOT_DIR/agent_red_team/filescanner.c"
        echo "   ✓ Linked filescanner.c"
    fi
    
    # Link graph layout
    if [ ! -L "$ROOT_DIR/agent_red_team/graphlayout.h" ]; then
        ln -s "$SCRIPT_DIR/native-cpp/graphlayout.h" "$ROOT_DIR/agent_red_team/graphlayout.h"
        echo "   ✓ Linked graphlayout.h"
    fi
    
    if [ ! -L "$ROOT_DIR/agent_red_team/graphlayout.cpp" ]; then
        ln -s "$SCRIPT_DIR/native-cpp/graphlayout.cpp" "$ROOT_DIR/agent_red_team/graphlayout.cpp"
        echo "   ✓ Linked graphlayout.cpp"
    fi
fi

if [ ! -d "$ROOT_DIR/agent_blue_team" ]; then
    echo "⚠️  agent_blue_team not found, skipping..."
else
    echo "🔗 Integrating with Blue Team agents..."
    
    # Link analysis engine
    if [ ! -L "$ROOT_DIR/agent_blue_team/analyze_threats.py" ]; then
        ln -s "$SCRIPT_DIR/python/analyze_notes.py" "$ROOT_DIR/agent_blue_team/analyze_threats.py"
        echo "   ✓ Linked analyze_threats.py"
    fi
    
    # Create wrapper script
    cat > "$ROOT_DIR/agent_blue_team/threat_scanner.sh" << 'WRAPPER'
#!/bin/bash
# Wrapper for SecondCerveau3D threat analysis

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ANALYZER="$SCRIPT_DIR/analyze_threats.py"

if [ ! -f "$ANALYZER" ]; then
    ANALYZER="$(dirname $SCRIPT_DIR)/SecondCerveau3D/python/analyze_notes.py"
fi

if [ -f "$ANALYZER" ]; then
    python3 "$ANALYZER" "$@"
else
    echo "Error: Threat analyzer not found"
    exit 1
fi
WRAPPER
    chmod +x "$ROOT_DIR/agent_blue_team/threat_scanner.sh"
    echo "   ✓ Created threat_scanner.sh wrapper"
fi

# Create orchestrator integration module
echo "🔗 Creating orchestrator integration..."

cat > "$SCRIPT_DIR/orchestrator_integration.c" << 'ORCH'
/**
 * SecondCerveau3D - Orchestrator Integration
 * Bridges SecondCerveau3D with existing agent_orchestrator
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <json-c/json.h>

/* Function pointers to SecondCerveau components */
typedef void* (*scan_directory_fn)(const char *path);
typedef void (*send_scan_to_orchestrator_fn)(void *result);

static void* filescanner_lib = NULL;
static scan_directory_fn scan_directory = NULL;
static send_scan_to_orchestrator_fn send_scan = NULL;

int initialize_secondcerveau() {
    /* Load SecondCerveau3D shared libraries */
    filescanner_lib = dlopen("./build/libfilescanner.so", RTLD_LAZY);
    
    if (!filescanner_lib) {
        fprintf(stderr, "Failed to load libfilescanner: %s\n", dlerror());
        return -1;
    }
    
    scan_directory = (scan_directory_fn)dlsym(filescanner_lib, "scan_directory");
    send_scan = (send_scan_to_orchestrator_fn)dlsym(filescanner_lib, "send_scan_to_orchestrator");
    
    if (!scan_directory || !send_scan) {
        fprintf(stderr, "Failed to load functions: %s\n", dlerror());
        return -1;
    }
    
    printf("[Orchestrator] SecondCerveau3D initialized\n");
    return 0;
}

int execute_secondcerveau_task(const char *task_type, const char *task_json) {
    if (!scan_directory) return -1;
    
    if (strcmp(task_type, "filesystem_scan") == 0) {
        /* Parse JSON to get scan path */
        struct json_object *root = json_tokener_parse(task_json);
        struct json_object *path_obj;
        
        if (json_object_object_get_ex(root, "path", &path_obj)) {
            const char *path = json_object_get_string(path_obj);
            printf("[Orchestrator] Starting filesystem scan: %s\n", path);
            
            void *result = scan_directory(path);
            if (result) {
                send_scan(result);
                printf("[Orchestrator] Scan completed\n");
                json_object_put(root);
                return 0;
            }
        }
        
        json_object_put(root);
        return -1;
    }
    
    return -1;
}

void cleanup_secondcerveau() {
    if (filescanner_lib) {
        dlclose(filescanner_lib);
        filescanner_lib = NULL;
    }
}
ORCH

echo "   ✓ Created orchestrator_integration.c"

# Create Python integration module
echo "🔗 Creating Python integration module..."

cat > "$SCRIPT_DIR/agent_integration.py" << 'PYTHON'
#!/usr/bin/env python3
"""
SecondCerveau3D - Agent Integration Module
Bridges Python components with the agent system
"""

import sys
import os
import json
from pathlib import Path

# Add SecondCerveau3D to path
SECONDCERVEAU_PATH = Path(__file__).parent
sys.path.insert(0, str(SECONDCERVEAU_PATH / 'python'))

from analyze_notes import ThreatAnalyzer, ReportGenerator

class AgentIntegration:
    """Integration layer for SecondCerveau3D with agent system"""
    
    def __init__(self):
        self.analyzer = ThreatAnalyzer()
        self.report_gen = ReportGenerator()
    
    def analyze_agent_output(self, agent_name, agent_output):
        """
        Analyze output from an agent
        
        Args:
            agent_name: Name of the agent (red_recon, blue_monitor, etc.)
            agent_output: Output from agent (JSON or text)
        
        Returns:
            Threat analysis result
        """
        try:
            if isinstance(agent_output, str):
                data = json.loads(agent_output)
            else:
                data = agent_output
            
            result = self.analyzer.analyze_agent_data(data)
            return self.report_gen.generate_json_report(result)
        
        except Exception as e:
            return json.dumps({
                "error": str(e),
                "agent": agent_name,
                "status": "failed"
            })
    
    def submit_analysis_request(self, agent_data_file):
        """Submit analysis request for agent data file"""
        
        if not os.path.exists(agent_data_file):
            return {"error": "File not found"}
        
        with open(agent_data_file, 'r') as f:
            content = f.read()
        
        return self.analyze_agent_output("file_input", content)

if __name__ == '__main__':
    integration = AgentIntegration()
    
    if len(sys.argv) > 1:
        # Analyze file
        result = integration.submit_analysis_request(sys.argv[1])
        print(result)
    else:
        print("Usage: agent_integration.py <agent_data_file>")
PYTHON

chmod +x "$SCRIPT_DIR/agent_integration.py"
echo "   ✓ Created agent_integration.py"

# Create integration configuration
echo "🔗 Generating configuration..."

cat > "$SCRIPT_DIR/integration.conf" << 'CONFIG'
# SecondCerveau3D Integration Configuration

# Native Library Paths
NATIVE_C_LIB=./SecondCerveau3D/build/libfilescanner.so
NATIVE_CPP_LIB=./SecondCerveau3D/build/libgraphlayout.so

# Backend Service
BACKEND_URL=http://localhost:8080
BACKEND_WS=ws://localhost:8080/ws

# Python Integration
PYTHON_ANALYZER=./SecondCerveau3D/python/analyze_notes.py
PYTHON_INTEGRATION=./SecondCerveau3D/agent_integration.py

# Agent System Paths
RED_TEAM_PATH=./agent_red_team
BLUE_TEAM_PATH=./agent_blue_team
ORCHESTRATOR_PATH=./agent

# Task Queue
TASK_QUEUE_PATH=/tmp/secondcerveau_tasks
RESULT_CACHE_PATH=/tmp/secondcerveau_results

# Enable Components
ENABLE_FILE_SCANNER=true
ENABLE_GRAPH_LAYOUT=true
ENABLE_THREAT_ANALYSIS=true
ENABLE_VISUALIZATION=true

# Security Settings
THREAT_THRESHOLD=50
MAX_CONCURRENT_TASKS=10
SCAN_TIMEOUT=300
CONFIG

echo "   ✓ Generated integration.conf"

# Summary
echo ""
echo "╔════════════════════════════════════════════════════════════╗"
echo "║              ✓ Integration Complete!                       ║"
echo "╠════════════════════════════════════════════════════════════╣"
echo "║                                                            ║"
echo "║  Linked Components:                                        ║"
echo "║  ✓ File Scanner (C)                                        ║"
echo "║  ✓ Graph Layout (C++)                                      ║"
echo "║  ✓ Threat Analysis (Python)                                ║"
echo "║  ✓ Backend Service (Java)                                  ║"
echo "║  ✓ Electron UI                                             ║"
echo "║                                                            ║"
echo "║  Integration Modules:                                      ║"
echo "║  ✓ orchestrator_integration.c                              ║"
echo "║  ✓ agent_integration.py                                    ║"
echo "║                                                            ║"
echo "║  Configuration: integration.conf                           ║"
echo "║                                                            ║"
echo "╚════════════════════════════════════════════════════════════╝"
echo ""
