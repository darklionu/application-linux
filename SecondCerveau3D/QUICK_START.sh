#!/bin/bash

# Quick start guide for SecondCerveau3D

cat << 'EOF'

╔════════════════════════════════════════════════════════════╗
║      SecondCerveau3D - Quick Start Guide                  ║
╚════════════════════════════════════════════════════════════╝

🚀 STARTING THE SYSTEM:

  1. BUILD NATIVE COMPONENTS:
     ┌──────────────────────────────────────────────────────┐
     │ $ cd SecondCerveau3D                                 │
     │ $ chmod +x build_all.sh && ./build_all.sh           │
     │   or                                                 │
     │ $ make clean && make all && make install            │
     └──────────────────────────────────────────────────────┘

  2. START BACKEND SERVICE:
     ┌──────────────────────────────────────────────────────┐
     │ $ cd backend-java                                    │
     │ $ mvn spring-boot:run                               │
     │ 🔵 Server runs on http://localhost:8080             │
     └──────────────────────────────────────────────────────┘

  3. START USER INTERFACE:
     ┌──────────────────────────────────────────────────────┐
     │ $ cd electron-ui                                     │
     │ $ npm install                                        │
     │ $ npm start                                          │
     │ 🎨 UI opens at http://localhost:3000                │
     └──────────────────────────────────────────────────────┘

  4. RUN ANALYSIS TOOLS:
     ┌──────────────────────────────────────────────────────┐
     │ $ cd python                                          │
     │ $ python3 analyze_notes.py --help                   │
     │ $ python3 analyze_notes.py --file data.txt          │
     └──────────────────────────────────────────────────────┘

🔌 API ENDPOINTS:

  Task Management:
  POST   http://localhost:8080/api/agents/task
  GET    http://localhost:8080/api/agents/task/{id}
  
  Graph Operations:
  GET    http://localhost:8080/api/agents/graph
  POST   http://localhost:8080/api/agents/graph/node
  POST   http://localhost:8080/api/agents/graph/edge
  
  System Info:
  GET    http://localhost:8080/api/agents

📊 EXAMPLE COMMANDS:

  Submit Reconnaissance Task:
  ┌──────────────────────────────────────────────────────────┐
  │ curl -X POST http://localhost:8080/api/agents/task \\   │
  │   -H "Content-Type: application/json" \\               │
  │   -d '{                                                 │
  │     "agentType": "red_recon",                          │
  │     "taskData": {                                       │
  │       "target": "192.168.1.100",                       │
  │       "ports": [22, 80, 443]                           │
  │     }                                                   │
  │   }'                                                    │
  └──────────────────────────────────────────────────────────┘

  Get Task Results:
  ┌──────────────────────────────────────────────────────────┐
  │ curl http://localhost:8080/api/agents/task/1            │
  └──────────────────────────────────────────────────────────┘

  Threat Analysis:
  ┌──────────────────────────────────────────────────────────┐
  │ python3 python/analyze_notes.py \\                      │
  │   --agent-data result.json \\                           │
  │   --format json                                         │
  └──────────────────────────────────────────────────────────┘

🧬 INTEGRATION WITH AGENTS:

  Link to Red Team:
  $ ln -s SecondCerveau3D/native-c/filescanner.c \\
         agent_red_team/

  Link to Blue Team:
  $ ln -s SecondCerveau3D/python/analyze_notes.py \\
         agent_blue_team/analyze_threats.py

🔍 TROUBLESHOOTING:

  Issue: Port 8080 already in use
  Fix:   $ lsof -i :8080 && kill -9 <PID>
  
  Issue: Missing npm modules
  Fix:   $ cd electron-ui && npm install --force
  
  Issue: libcrypto not found
  Fix:   $ sudo apt install libssl-dev
  
  Issue: Maven not installed
  Fix:   $ sudo apt install maven

✅ VERIFICATION:

  Check Backend:
  $ curl http://localhost:8080/api/agents | jq

  Check Python:
  $ python3 python/analyze_notes.py --text "malware exploit"

  Check C Libraries:
  $ file build/lib*.so

📁 FILES STRUCTURE:

  SecondCerveau3D/
  ├── native-c/         ← File scanning engine
  ├── native-cpp/       ← Graph visualization
  ├── bridge-csharp/    ← C# integration layer
  ├── backend-java/     ← REST API & orchestration
  ├── python/           ← Threat analysis
  ├── electron-ui/      ← 3D interface
  ├── build/            ← Compiled binaries
  ├── install/          ← Installation artifacts
  └── build_all.sh      ← Automated builder

🎯 NEXT STEPS:

  1. Open browser at http://localhost:3000
  2. Select an agent from dropdown
  3. Enter task configuration in JSON
  4. Click "Submit Task"
  5. Watch the 3D graph update in real-time
  6. Check threat analysis results

💡 TIPS:

  • Use WebSocket for real-time updates
  • Graph auto-refreshes every 5 seconds
  • Threat score displayed in node colors
  • All API responses are JSON formatted
  • Python analyzer supports multiple input formats

═══════════════════════════════════════════════════════════════

EOF

# Show current status
echo ""
echo "📊 SYSTEM STATUS:"
echo ""

if command -v java &> /dev/null; then
    echo "✅ Java $(java -version 2>&1 | head -n 1 | awk '{print $3}')"
else
    echo "❌ Java not found"
fi

if command -v npm &> /dev/null; then
    echo "✅ Node $(node -v) / npm $(npm -v)"
else
    echo "❌ Node/npm not found"
fi

if command -v python3 &> /dev/null; then
    echo "✅ Python3 $(python3 --version 2>&1 | awk '{print $2}')"
else
    echo "❌ Python3 not found"
fi

if [ -f "build_all.sh" ]; then
    echo "✅ Build script present"
else
    echo "❌ Build script not found"
fi

echo ""
echo "🎯 To get started, run: ./build_all.sh"
echo ""
