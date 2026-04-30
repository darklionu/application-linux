# SecondCerveau3D - 3D Agent Orchestration Platform

## 📋 Overview

**SecondCerveau3D** is a comprehensive multi-language integration layer that extends your existing agent system with advanced capabilities:

```
                    ┌──────────────────────────┐
                    │    Electron 3D UI        │
                    │   (Node.js + React)      │
                    └────────────┬─────────────┘
                                 │
                    ┌────────────┴─────────────┐
                    │   Backend Orchestrator   │
                    │   (Spring Boot - Java)   │
                    └────────────┬─────────────┘
                                 │
            ┌────────────────────┼────────────────────┐
            │                    │                    │
        ┌───▼────┐          ┌────▼─────┐         ┌───▼────┐
        │ C Libs │          │ C++ Libs │         │ Python │
        │Scanner │          │ GraphSim │         │Analyzer│
        └────────┘          └──────────┘         └────────┘
            │                    │                    │
        ┌───▼────────────────────▼────────────────────▼───┐
        │        Existing Agent Red/Blue Team             │
        │     (agent_red_team, agent_blue_team)           │
        └───────────────────────────────────────────────┘
```

## 🏗️ Architecture

### Layer 1: Native Code (C/C++)
- **filescanner.c/h** - High-performance file scanning with threat analysis
- **graphlayout.cpp/h** - Force-directed graph layout for 3D visualization

### Layer 2: Integration Bridge (C#)
- **Program.cs** - P/Invoke bridge to native libraries
- Seamless interop with existing agent system

### Layer 3: Backend Orchestration (Java)
- **BackendApplication.java** - Spring Boot REST API server
- **NativeOrchestratorService.java** - Task coordination and scheduling
- **GraphController.java** - Graph data endpoint management

### Layer 4: Analysis Engine (Python)
- **analyze_notes.py** - Comprehensive threat analysis with pattern matching
- Threat indicator extraction (IPs, ports, hashes)
- Automated recommendations generation

### Layer 5: User Interface (Electron + React)
- **main.js** - Electron main process with WebSocket bridge
- **App.jsx** - React UI for agent management
- **Graph3D.jsx** - Three.js 3D graph visualization

## 🚀 Quick Start

### 1. Build All Components
```bash
cd SecondCerveau3D
chmod +x build_all.sh
./build_all.sh
```

Or using Make:
```bash
make clean
make all
make install
```

### 2. Start Backend
```bash
cd backend-java
mvn spring-boot:run
# Runs on http://localhost:8080
```

### 3. Start Electron UI
```bash
cd electron-ui
npm install
npm start
```

### 4. Run Python Analysis
```bash
python3 python/analyze_notes.py --file <your_file>
# Or with JSON agent data:
python3 python/analyze_notes.py --agent-data agents.json --format json
```

## 📊 Component Integration

### File Scanning → Agent System
```c
// C scanner detects threats and sends to orchestrator
send_scan_to_orchestrator(result);
// Integrates with blue_monitor agent
```

### Graph Layout → 3D Visualization
```cpp
// C++ computes force-directed layout
graph.compute_layout(100);
// Exported as JSON to Electron UI
std::string json = graph.export_to_json();
```

### Python Analysis → Backend
```bash
# Analyze agent outputs
python analyze_notes.py --agent-data orchestrator_output.json
# Generates threat assessment with recommendations
```

## 🔌 API Endpoints

| Endpoint | Method | Purpose |
|----------|--------|---------|
| `/api/agents/task` | POST | Submit agent task |
| `/api/agents/task/{id}` | GET | Get task result |
| `/api/agents/graph` | GET | Get current graph state |
| `/api/agents/graph/node` | POST | Add graph node |
| `/api/agents/graph/edge` | POST | Add graph edge |
| `/api/agents` | GET | List active agents |

## 🔒 Security Features

- **Threat Detection**: Automatic analysis of suspicious patterns
- **Hash Verification**: SHA-256 validation for critical files
- **Multi-layer Monitoring**: File, process, network surveillance
- **Correlation Analysis**: Detects attack patterns across events

## 📁 File Structure

```
SecondCerveau3D/
├── native-c/
│   ├── filescanner.h
│   └── filescanner.c
├── native-cpp/
│   ├── graphlayout.h
│   └── graphlayout.cpp
├── bridge-csharp/
│   └── Program.cs
├── backend-java/
│   ├── pom.xml
│   └── src/main/java/com/secondcerveau/
│       ├── BackendApplication.java
│       ├── NativeOrchestratorService.java
│       ├── GraphController.java
│       ├── AgentTask.java
│       ├── AgentExecutor.java
│       └── AgentTaskResult.java
├── python/
│   └── analyze_notes.py
├── electron-ui/
│   ├── package.json
│   ├── main.js
│   └── src/
│       ├── App.jsx
│       └── Graph3D.jsx
├── build_all.sh
└── Makefile
```

## 🔗 Integration with Existing Agents

### Red Team Integration
```bash
# Copy or link scanner to red team agents
ln -s SecondCerveau3D/native-c/filescanner.c agent_red_team/

# Use in exploitation workflows
./agent_red_team/agent_manager --scan-target --use-filescanner
```

### Blue Team Integration
```bash
# Link analysis engine
ln -s SecondCerveau3D/python/analyze_notes.py agent_blue_team/analyze_threats.py

# Analyze agent findings
./agent_blue_team/blue_monitor --analyze-with-python
```

## 🧪 Testing

```bash
# Build test
make test

# Integration test
./install/test_integration.sh

# Python analyzer test
python3 python/analyze_notes.py --text "backdoor malware shellcode" --format text

# Backend connectivity
curl http://localhost:8080/api/agents

# UI test
npm start  # Opens http://localhost:3000
```

## 📊 Example Usage

### Submit Reconnaissance Task
```bash
curl -X POST http://localhost:8080/api/agents/task \
  -H "Content-Type: application/json" \
  -d '{
    "agentType": "red_recon",
    "taskData": {
      "target": "192.168.1.100",
      "ports": [22, 80, 443]
    }
  }'
```

### Get Task Result
```bash
curl http://localhost:8080/api/agents/task/1
```

### Threat Analysis
```bash
python3 SecondCerveau3D/python/analyze_notes.py \
  --agent-data task_result.json \
  --format json
```

## 🎯 Key Features

✅ **Multi-Language Support** - C, C++, C#, Java, Python, JavaScript  
✅ **Force-Directed Graph** - Advanced network visualization  
✅ **Real-time Threat Analysis** - Automated pattern detection  
✅ **WebSocket Communication** - Live UI updates  
✅ **REST API** - Full agent control  
✅ **3D Visualization** - Interactive agent network  
✅ **Seamless Integration** - Works with existing agent system  

## 🔄 Communication Flow

```
1. Electron UI → Backend API (REST)
2. Backend ← Orchestrator Service (Java)
3. Service → Native Libraries (C/C++ via P/Invoke)
4. Native Code → Agent System (shared memory/sockets)
5. Results ← Python Analyzer (threat assessment)
6. Display → 3D Graph (Three.js)
```

## 🛠️ Build Customization

Edit `build_all.sh` or `Makefile` to:
- Change compiler flags
- Add additional dependencies
- Modify installation paths
- Include custom components

## 📝 License

Part of the agent-based security system. Integrated with existing red/blue team infrastructure.

## 🆘 Troubleshooting

| Issue | Solution |
|-------|----------|
| Missing libcrypto | `sudo apt install libssl-dev` |
| Maven not found | Install Maven or skip Java build |
| npm modules fail | Delete package-lock.json and retry |
| WebSocket timeout | Ensure backend is running on 8080 |

---

**SecondCerveau3D** - Orchestrating your agents in 3D 🧠✨
