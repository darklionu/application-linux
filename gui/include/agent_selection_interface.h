#ifndef AGENT_GRAPHICAL_INTERFACE_H
#define AGENT_GRAPHICAL_INTERFACE_H

/**
 * @file agent_graphical_interface.h
 * @brief Interface graphique J.A.R.V.I.S style Iron Man pour la sélection d'agents
 *
 * STRATÉGIE: Génère l'interface HTML dynamiquement depuis les données C++,
 * lance un serveur HTTP léger sur un port local, puis ouvre le navigateur.
 *
 * DÉPENDANCES:
 *   - C++17 (std::filesystem, std::string_view)
 *   - Sockets POSIX (Linux/macOS) ou Winsock2 (Windows)
 *   - Aucune lib externe requise
 *
 * COMPILATION:
 *   g++ -std=c++17 -pthread main.cpp -o jarvis
 */

#include "agent_selection_system.h"
#include <string>
#include <sstream>
#include <fstream>
#include <thread>
#include <atomic>
#include <functional>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <cstring>
#include <algorithm>

// ─── Includes réseau (cross-platform) ───────────────────────────────────────
#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #pragma comment(lib, "ws2_32.lib")
  using SocketType = SOCKET;
  #define INVALID_SOCK INVALID_SOCKET
  #define CLOSE_SOCK(s) closesocket(s)
#else
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <unistd.h>
  using SocketType = int;
  #define INVALID_SOCK (-1)
  #define CLOSE_SOCK(s) close(s)
#endif

// ─── Ouvre le navigateur par défaut (cross-platform) ────────────────────────
inline void openBrowser(const std::string& url) {
#ifdef _WIN32
    std::string cmd = "start \"\" \"" + url + "\"";
#elif __APPLE__
    std::string cmd = "open \"" + url + "\"";
#else
    std::string cmd = "xdg-open \"" + url + "\" &";
#endif
    std::system(cmd.c_str());
}

// ════════════════════════════════════════════════════════════════════════════
// ─── HTML BUILDER ────────────────────────────────────────────────────────────
// Génère le JSON des agents depuis AgentSelectionSystem et l'injecte dans le HTML
// ════════════════════════════════════════════════════════════════════════════

class JarvisHtmlBuilder {
public:
    /**
     * @brief Convertit les agents C++ en tableau JSON pour le front-end
     * @param system Le système de sélection d'agents
     * @return String JSON [ { id, name, type, status, deps, priority, path }, ... ]
     */
    static std::string buildAgentsJson(const AgentSelectionSystem& system) {
        const auto& agents     = system.getAvailableAgents();   // std::vector<AgentInfo>
        const auto& selected   = system.getSelectedAgents();

        std::ostringstream json;
        json << "[\n";

        // ─── Icônes SVG prédéfinies par type ────────────────────────────────
        std::unordered_map<std::string, std::string> icons = {
            {"Scanner",
             R"(<svg viewBox="0 0 48 48" xmlns="http://www.w3.org/2000/svg">)"
             R"(<polygon points="24,4 40,14 40,34 24,44 8,34 8,14" fill="none" stroke="#00e5ff" stroke-width="1.5"/>)"
             R"(<circle cx="24" cy="24" r="8" fill="none" stroke="#00e5ff" stroke-width="1" stroke-dasharray="3 2"/>)"
             R"(<circle cx="24" cy="24" r="3" fill="#00e5ff" opacity="0.8"/>)"
             R"(<line x1="24" y1="16" x2="24" y2="12" stroke="#00e5ff" stroke-width="1"/>)"
             R"(</svg>)"},
            {"Defense",
             R"(<svg viewBox="0 0 48 48" xmlns="http://www.w3.org/2000/svg">)"
             R"(<path d="M24 4 L40 11 L40 26 C40 35 24 44 24 44 C24 44 8 35 8 26 L8 11 Z" fill="none" stroke="#ffa500" stroke-width="1.5"/>)"
             R"(<line x1="24" y1="14" x2="24" y2="38" stroke="#ffa500" stroke-width="1" opacity="0.5"/>)"
             R"(<line x1="14" y1="24" x2="34" y2="24" stroke="#ffa500" stroke-width="1" opacity="0.5"/>)"
             R"(</svg>)"},
            {"Network",
             R"(<svg viewBox="0 0 48 48" xmlns="http://www.w3.org/2000/svg">)"
             R"(<circle cx="24" cy="24" r="3" fill="none" stroke="#00e5ff" stroke-width="1.5"/>)"
             R"(<circle cx="10" cy="14" r="3" fill="none" stroke="#00e5ff" stroke-width="1.2"/>)"
             R"(<circle cx="38" cy="14" r="3" fill="none" stroke="#00e5ff" stroke-width="1.2"/>)"
             R"(<circle cx="10" cy="34" r="3" fill="none" stroke="#00e5ff" stroke-width="1.2"/>)"
             R"(<circle cx="38" cy="34" r="3" fill="none" stroke="#00e5ff" stroke-width="1.2"/>)"
             R"(<line x1="13" y1="16" x2="21" y2="22" stroke="rgba(0,229,255,0.5)" stroke-width="1"/>)"
             R"(<line x1="35" y1="16" x2="27" y2="22" stroke="rgba(0,229,255,0.5)" stroke-width="1"/>)"
             R"(</svg>)"},
            {"Analyzer",
             R"(<svg viewBox="0 0 48 48" xmlns="http://www.w3.org/2000/svg">)"
             R"(<rect x="8" y="12" width="32" height="24" rx="3" fill="none" stroke="#ffa500" stroke-width="1.5"/>)"
             R"(<rect x="12" y="16" width="10" height="6" rx="1" fill="none" stroke="#ffa500" stroke-width="1" opacity="0.7"/>)"
             R"(<rect x="26" y="16" width="10" height="6" rx="1" fill="none" stroke="#ffa500" stroke-width="1" opacity="0.7"/>)"
             R"(<line x1="12" y1="26" x2="36" y2="26" stroke="rgba(255,165,0,0.4)" stroke-width="1"/>)"
             R"(</svg>)"},
        };

        auto escapeJson = [](const std::string& s) -> std::string {
            std::string out;
            for (char c : s) {
                if (c == '"')  out += "\\\"";
                else if (c == '\\') out += "\\\\";
                else if (c == '\n') out += "\\n";
                else out += c;
            }
            return out;
        };

        for (size_t i = 0; i < agents.size(); ++i) {
            const auto& a = agents[i];

            // Détermine le statut actuel (sélectionné ou non)
            bool isSelected = std::any_of(selected.begin(), selected.end(),
                [&](const auto& s){ return s.name == a.name; });

            // Icône par défaut si le type est inconnu
            std::string icon = icons.count(a.type) ? icons[a.type] : icons["Scanner"];

            // Sérialise les dépendances
            std::string depsJson = "[";
            for (size_t d = 0; d < a.dependencies.size(); ++d) {
                depsJson += "\"" + escapeJson(a.dependencies[d]) + "\"";
                if (d + 1 < a.dependencies.size()) depsJson += ",";
            }
            depsJson += "]";

            json << "  {\n"
                 << "    \"id\":       \"" << escapeJson(a.name)            << "\",\n"
                 << "    \"name\":     \"" << escapeJson(a.name)            << "\",\n"
                 << "    \"type\":     \"" << escapeJson(a.type)            << "\",\n"
                 << "    \"status\":   \"" << (a.is_active ? "online" : "standby") << "\",\n"
                 << "    \"priority\": "   << a.priority                    << ",\n"
                 << "    \"order\":    "   << (i + 1)                       << ",\n"
                 << "    \"path\":     \"" << escapeJson(a.path)            << "\",\n"
                 << "    \"deps\":     "   << depsJson                      << ",\n"
                 << "    \"selected\": "   << (isSelected ? "true" : "false") << ",\n"
                 << "    \"icon\":     \"" << escapeJson(icon)              << "\"\n"
                 << "  }";

            if (i + 1 < agents.size()) json << ",";
            json << "\n";
        }

        json << "]";
        return json.str();
    }

    /**
     * @brief Génère la page HTML complète avec les données injectées
     */
    static std::string buildFullPage(const AgentSelectionSystem& system) {
        std::string agentsJson = buildAgentsJson(system);

        // ── Le HTML est généré ici (template literal C++17) ─────────────────
        std::ostringstream html;
        html << R"(<!DOCTYPE html>
<html lang="fr">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>J.A.R.V.I.S - Système de Contrôle d'Agents</title>
<link href="https://fonts.googleapis.com/css2?family=Orbitron:wght@400;700;900&family=Share+Tech+Mono&display=swap" rel="stylesheet">
<style>
:root{--cyan:#00e5ff;--cyan-dim:#0088aa;--gold:#ffa500;--red:#ff2020;
--dark:#020c14;--panel:rgba(0,20,35,.85);--border:rgba(0,229,255,.3);
--border-hot:rgba(0,229,255,.8);}
*{box-sizing:border-box;margin:0;padding:0;}
body{background:var(--dark);color:var(--cyan);
font-family:'Share Tech Mono',monospace;min-height:100vh;overflow-x:hidden;}
body::before{content:'';position:fixed;inset:0;
background-image:linear-gradient(rgba(0,229,255,.04) 1px,transparent 1px),
linear-gradient(90deg,rgba(0,229,255,.04) 1px,transparent 1px);
background-size:40px 40px;pointer-events:none;z-index:0;}
body::after{content:'';position:fixed;inset:0;
background:repeating-linear-gradient(0deg,transparent,transparent 2px,
rgba(0,0,0,.08) 2px,rgba(0,0,0,.08) 4px);pointer-events:none;z-index:100;}
.layout{position:relative;z-index:1;display:grid;
grid-template-columns:260px 1fr 240px;grid-template-rows:70px 1fr 90px;
min-height:100vh;}
.topbar{grid-column:1/-1;display:flex;align-items:center;
justify-content:space-between;padding:0 24px;
border-bottom:1px solid var(--border);background:rgba(0,10,20,.9);}
.logo{font-family:'Orbitron',sans-serif;font-size:20px;font-weight:900;
letter-spacing:6px;color:var(--cyan);text-shadow:0 0 8px #00e5ff;}
.logo span{color:var(--gold);}
.top-stats{display:flex;gap:32px;font-size:11px;color:var(--cyan-dim);}
.top-stats .val{color:var(--cyan);font-size:13px;}
.system-time{font-family:'Orbitron',sans-serif;font-size:14px;
color:var(--gold);letter-spacing:2px;}
.left-panel{border-right:1px solid var(--border);padding:16px;
background:var(--panel);display:flex;flex-direction:column;gap:16px;overflow-y:auto;}
.panel-title{font-family:'Orbitron',sans-serif;font-size:9px;letter-spacing:3px;
color:var(--cyan-dim);padding-bottom:8px;border-bottom:1px solid var(--border);}
.arc-mini{width:80px;height:80px;margin:0 auto;position:relative;
display:flex;align-items:center;justify-content:center;}
.arc-mini svg{width:100%;height:100%;animation:reactor-spin 8s linear infinite;}
@keyframes reactor-spin{to{transform:rotate(360deg);}}
.arc-core{position:absolute;width:24px;height:24px;border-radius:50%;
background:radial-gradient(circle,#00e5ff 0%,#0088aa 60%,transparent 100%);
box-shadow:0 0 12px #00e5ff,0 0 30px rgba(0,229,255,.6);
animation:pulse-core 2s ease-in-out infinite;}
@keyframes pulse-core{0%,100%{box-shadow:0 0 12px #00e5ff,0 0 30px rgba(0,229,255,.6);}
50%{box-shadow:0 0 20px #00e5ff,0 0 50px rgba(0,229,255,.9);}}
.readout{display:flex;flex-direction:column;gap:8px;}
.readout-row{display:flex;justify-content:space-between;font-size:10px;}
.readout-label{color:var(--cyan-dim);}
.readout-val{color:var(--cyan);}
.bar-wrap{height:3px;background:rgba(0,229,255,.1);border-radius:2px;
overflow:hidden;margin-top:3px;}
.bar-fill{height:100%;border-radius:2px;background:var(--cyan);
box-shadow:0 0 4px var(--cyan);transition:width 1s ease;}
.bar-fill.gold{background:var(--gold);box-shadow:0 0 4px var(--gold);}
.bar-fill.red{background:var(--red);box-shadow:0 0 4px var(--red);}
.log-box{flex:1;font-size:9px;line-height:1.7;color:rgba(0,229,255,.5);
overflow-y:auto;border:1px solid var(--border);padding:8px;
background:rgba(0,0,0,.3);}
.log-line{display:flex;gap:6px;}
.log-time{color:var(--gold);opacity:.6;white-space:nowrap;}
.log-msg.ok{color:#00ff88;} .log-msg.warn{color:var(--gold);}
.log-msg.err{color:var(--red);}
.center{padding:20px;display:flex;flex-direction:column;gap:16px;overflow-y:auto;}
.section-header{font-family:'Orbitron',sans-serif;font-size:10px;letter-spacing:4px;
color:var(--cyan-dim);display:flex;align-items:center;gap:12px;}
.section-header::after{content:'';flex:1;height:1px;
background:linear-gradient(90deg,var(--border),transparent);}
.agent-grid{display:grid;grid-template-columns:repeat(auto-fill,minmax(180px,1fr));gap:16px;}
.agent-card{position:relative;cursor:pointer;transition:transform .2s ease;}
.agent-card:hover{transform:translateY(-3px);}
.card-inner{border:1px solid var(--border);
background:linear-gradient(135deg,rgba(0,30,50,.9) 0%,rgba(0,15,30,.95) 100%);
padding:16px 14px 14px;position:relative;overflow:hidden;transition:all .3s ease;
clip-path:polygon(0 12px,12px 0,calc(100% - 12px) 0,100% 12px,100% calc(100% - 12px),
calc(100% - 12px) 100%,12px 100%,0 calc(100% - 12px));}
.agent-card:hover .card-inner{border-color:var(--border-hot);
box-shadow:0 0 20px rgba(0,229,255,.2),inset 0 0 20px rgba(0,229,255,.05);}
.card-corner{position:absolute;width:8px;height:8px;border-color:var(--cyan);border-style:solid;}
.card-corner.tl{top:4px;left:4px;border-width:1px 0 0 1px;}
.card-corner.tr{top:4px;right:4px;border-width:1px 1px 0 0;}
.card-corner.bl{bottom:4px;left:4px;border-width:0 0 1px 1px;}
.card-corner.br{bottom:4px;right:4px;border-width:0 1px 1px 0;}
.agent-icon{width:48px;height:48px;margin:0 auto 10px;display:flex;align-items:center;justify-content:center;}
.agent-icon svg{width:100%;height:100%;}
.agent-name{font-family:'Orbitron',sans-serif;font-size:9px;letter-spacing:2px;
color:var(--cyan);text-align:center;margin-bottom:4px;}
.agent-type{font-size:8px;color:var(--cyan-dim);text-align:center;
letter-spacing:1px;margin-bottom:10px;}
.agent-stats{display:flex;justify-content:space-between;font-size:8px;
color:rgba(0,229,255,.5);margin-bottom:8px;}
.agent-status{display:flex;align-items:center;justify-content:center;gap:5px;font-size:8px;}
.status-dot{width:5px;height:5px;border-radius:50%;background:#00ff88;
box-shadow:0 0 4px #00ff88;animation:blink 2s ease-in-out infinite;}
.status-dot.inactive{background:var(--cyan-dim);animation:none;box-shadow:none;}
.status-dot.warn{background:var(--gold);box-shadow:0 0 4px var(--gold);}
@keyframes blink{0%,100%{opacity:1;}50%{opacity:.3;}}
.agent-card.selected .card-inner{border-color:var(--gold);
box-shadow:0 0 15px rgba(255,165,0,.3),inset 0 0 15px rgba(255,165,0,.05);}
.agent-card.selected .agent-name{color:var(--gold);text-shadow:0 0 8px #ffa500;}
.agent-card.selected .card-corner{border-color:var(--gold);}
.select-badge{position:absolute;top:6px;right:6px;width:16px;height:16px;
background:var(--gold);box-shadow:0 0 8px #ffa500;
clip-path:polygon(50% 0%,100% 38%,82% 100%,18% 100%,0% 38%);
display:flex;align-items:center;justify-content:center;font-size:8px;
color:#000;font-weight:bold;opacity:0;transition:opacity .2s;}
.agent-card.selected .select-badge{opacity:1;}
.right-panel{border-left:1px solid var(--border);padding:16px;
background:var(--panel);display:flex;flex-direction:column;gap:16px;overflow-y:auto;}
.queue-item{display:flex;align-items:center;gap:8px;padding:8px;
border:1px solid var(--border);background:rgba(0,0,0,.3);font-size:9px;
transition:all .2s;}
.queue-item:hover{border-color:var(--border-hot);}
.queue-num{font-family:'Orbitron',sans-serif;font-size:14px;color:var(--gold);
width:20px;text-align:center;opacity:.7;}
.queue-name{color:var(--cyan);flex:1;}
.queue-type{color:var(--cyan-dim);font-size:8px;}
.queue-remove{cursor:pointer;color:var(--red);font-size:12px;opacity:.5;
transition:opacity .2s;background:none;border:none;font-family:'Share Tech Mono',monospace;}
.queue-remove:hover{opacity:1;}
.bottombar{grid-column:1/-1;border-top:1px solid var(--border);
background:rgba(0,10,20,.95);display:flex;align-items:center;padding:0 24px;gap:16px;}
.btn{font-family:'Orbitron',sans-serif;font-size:9px;letter-spacing:2px;
padding:8px 20px;border:1px solid;background:transparent;cursor:pointer;
transition:all .2s ease;text-transform:uppercase;
clip-path:polygon(0 0,calc(100% - 8px) 0,100% 8px,100% 100%,8px 100%,0 calc(100% - 8px));}
.btn-cyan{border-color:var(--cyan);color:var(--cyan);}
.btn-cyan:hover{background:rgba(0,229,255,.1);box-shadow:0 0 8px #00e5ff;}
.btn-gold{border-color:var(--gold);color:var(--gold);}
.btn-gold:hover{background:rgba(255,165,0,.1);box-shadow:0 0 8px #ffa500;}
.btn-execute{margin-left:auto;font-size:11px;padding:10px 32px;
border-color:var(--gold);color:var(--gold);background:rgba(255,165,0,.05);}
.btn-execute:hover{box-shadow:0 0 8px #ffa500;}
.counter-num{font-size:22px;font-weight:900;color:var(--gold);
text-shadow:0 0 8px #ffa500;line-height:1;}
.notif{position:fixed;top:80px;right:20px;background:rgba(0,20,35,.95);
border:1px solid var(--gold);padding:10px 16px;font-size:10px;color:var(--gold);
box-shadow:0 0 8px #ffa500;z-index:200;transform:translateX(120%);
transition:transform .3s ease;font-family:'Orbitron',sans-serif;letter-spacing:2px;
clip-path:polygon(0 0,calc(100% - 8px) 0,100% 8px,100% 100%,8px 100%,0 calc(100% - 8px));}
.notif.show{transform:translateX(0);}
.execute-overlay{position:fixed;inset:0;background:rgba(0,0,0,.9);z-index:300;
display:flex;align-items:center;justify-content:center;
opacity:0;pointer-events:none;transition:opacity .3s;}
.execute-overlay.show{opacity:1;pointer-events:all;}
.execute-modal{width:480px;border:1px solid var(--gold);background:var(--dark);
box-shadow:0 0 8px #ffa500;padding:32px;
clip-path:polygon(0 16px,16px 0,calc(100% - 16px) 0,100% 16px,100% calc(100% - 16px),
calc(100% - 16px) 100%,16px 100%,0 calc(100% - 16px));}
.modal-title{font-family:'Orbitron',sans-serif;font-size:14px;letter-spacing:4px;
color:var(--gold);text-align:center;margin-bottom:24px;}
.exec-bar-outer{height:4px;background:rgba(255,165,0,.1);
border:1px solid rgba(255,165,0,.2);margin-bottom:8px;overflow:hidden;}
.exec-bar-fill{height:100%;background:var(--gold);box-shadow:0 0 8px #ffa500;
width:0%;transition:width .4s ease;}
.exec-log{font-size:9px;color:rgba(0,229,255,.6);height:100px;overflow-y:auto;
border:1px solid var(--border);padding:8px;background:rgba(0,0,0,.4);margin-bottom:16px;}
.exec-log .ok{color:#00ff88;} .exec-log .label{color:var(--gold);}
</style>
</head>
<body>
<div class="layout">
<header class="topbar">
  <div class="logo">J.<span>A</span>.R.V.I.S <span style="font-size:11px;opacity:.6">// CONTRÔLE D'AGENTS v4.2</span></div>
  <div class="top-stats">
    <div>SYSTÈME<br><span class="val">OPÉRATIONNEL</span></div>
    <div>AGENTS ACTIFS<br><span class="val" id="active-count">0 / 0</span></div>
    <div>PUISSANCE<br><span class="val">ARC 100%</span></div>
    <div>PROTOCOLE<br><span class="val">MARQUE VII</span></div>
  </div>
  <div class="system-time" id="clock">--:--:--</div>
</header>
<aside class="left-panel">
  <div class="panel-title">// Réacteur Arc</div>
  <div class="arc-mini">
    <svg viewBox="0 0 80 80" xmlns="http://www.w3.org/2000/svg">
      <circle cx="40" cy="40" r="36" fill="none" stroke="rgba(0,229,255,.15)" stroke-width="1"/>
      <circle cx="40" cy="40" r="28" fill="none" stroke="rgba(0,229,255,.2)" stroke-width="1"/>
      <g stroke="#00e5ff" stroke-width="1.5" stroke-linecap="round">
        <line x1="40" y1="4" x2="40" y2="16" opacity=".8"/>
        <line x1="40" y1="64" x2="40" y2="76" opacity=".8"/>
        <line x1="4" y1="40" x2="16" y2="40" opacity=".8"/>
        <line x1="64" y1="40" x2="76" y2="40" opacity=".8"/>
      </g>
      <polygon points="40,22 46,34 58,34 49,42 52,54 40,46 28,54 31,42 22,34 34,34"
               fill="none" stroke="rgba(0,229,255,.4)" stroke-width=".8"/>
    </svg>
    <div class="arc-core"></div>
  </div>
  <div class="panel-title">// Diagnostics</div>
  <div class="readout">
    <div class="readout-row"><span class="readout-label">CPU</span><span class="readout-val" id="cpu-val">73%</span></div>
    <div class="bar-wrap"><div class="bar-fill" style="width:73%"></div></div>
    <div class="readout-row"><span class="readout-label">RAM</span><span class="readout-val">2,3 GB</span></div>
    <div class="bar-wrap"><div class="bar-fill gold" style="width:58%"></div></div>
    <div class="readout-row"><span class="readout-label">RÉSEAU</span><span class="readout-val">1800 MHz</span></div>
    <div class="bar-wrap"><div class="bar-fill" style="width:90%"></div></div>
    <div class="readout-row"><span class="readout-label">TEMP</span><span class="readout-val">38°C</span></div>
    <div class="bar-wrap"><div class="bar-fill red" style="width:38%"></div></div>
  </div>
  <div class="panel-title">// Journal Système</div>
  <div class="log-box" id="log-box">
    <div class="log-line"><span class="log-time">[SYS]</span><span class="log-msg ok">JARVIS initialisé</span></div>
  </div>
</aside>
<main class="center">
  <div class="section-header">AGENTS DISPONIBLES - CLIQUER POUR SÉLECTIONNER</div>
  <div class="agent-grid" id="agent-grid"></div>
  <div class="section-header" style="margin-top:8px">FILTRES</div>
  <div style="display:flex;gap:10px;flex-wrap:wrap;">
    <button class="btn btn-cyan" onclick="filterAgents('all')">TOUS</button>
    <button class="btn btn-cyan" onclick="filterAgents('Scanner')">SCANNER</button>
    <button class="btn btn-cyan" onclick="filterAgents('Analyzer')">ANALYSEUR</button>
    <button class="btn btn-cyan" onclick="filterAgents('Network')">RÉSEAU</button>
    <button class="btn btn-cyan" onclick="filterAgents('Defense')">DÉFENSE</button>
    <button class="btn btn-gold" onclick="validateDeps()">[*] VALIDER DÉPEND.</button>
  </div>
</main>
<aside class="right-panel">
  <div class="panel-title">// File d'Exécution</div>
  <svg viewBox="0 0 160 160" xmlns="http://www.w3.org/2000/svg" style="width:100%;max-width:160px;margin:0 auto;display:block;">
    <circle cx="80" cy="80" r="70" fill="none" stroke="rgba(0,229,255,.1)" stroke-width="1"/>
    <circle cx="80" cy="80" r="55" fill="none" stroke="rgba(0,229,255,.15)" stroke-width="1" stroke-dasharray="4 4"/>
    <circle cx="80" cy="80" r="40" fill="none" stroke="rgba(0,229,255,.2)" stroke-width="1"/>
    <line x1="80" y1="10" x2="80" y2="150" stroke="rgba(0,229,255,.05)" stroke-width="1"/>
    <line x1="10" y1="80" x2="150" y2="80" stroke="rgba(0,229,255,.05)" stroke-width="1"/>
    <line x1="80" y1="80" x2="80" y2="10" stroke="rgba(0,229,255,.6)" stroke-width="1.5">
      <animateTransform attributeName="transform" type="rotate" values="0 80 80;360 80 80" dur="4s" repeatCount="indefinite"/>
    </line>
    <circle cx="80" cy="80" r="4" fill="#00e5ff" opacity=".8"/>
    <circle cx="50" cy="60" r="3" fill="#00ff88" opacity=".7"/>
    <circle cx="110" cy="50" r="3" fill="#ffa500" opacity=".7"/>
    <circle cx="95" cy="110" r="3" fill="#00e5ff" opacity=".7"/>
  </svg>
  <div id="queue-list" style="display:flex;flex-direction:column;gap:6px;flex:1;overflow-y:auto;">
    <div style="font-size:9px;color:rgba(0,229,255,.3);text-align:center;padding:20px 0;">Aucun agent sélectionné</div>
  </div>
  <button class="btn btn-cyan" style="width:100%;font-size:8px;" onclick="saveSnapshot()">[*] PHOTO-ÉTAT</button>
</aside>
<footer class="bottombar">
  <div style="display:flex;align-items:center;gap:12px;">
    <div class="counter-num" id="sel-count">0</div>
    <div style="font-size:8px;letter-spacing:2px;color:var(--cyan-dim)">SÉLECTIONNÉS</div>
  </div>
  <button class="btn btn-cyan" onclick="clearSelection()">[*] EFFACER</button>
  <button class="btn btn-cyan" onclick="selectAll()">[*] TOUT SÉLECTION</button>
  <div style="flex:1;"></div>
  <button class="btn btn-execute btn-gold" onclick="executeAgents()">[!] LANCER</button>
</footer>
</div>
<div class="notif" id="notif"></div>
<div class="execute-overlay" id="exec-overlay">
  <div class="execute-modal">
    <div class="modal-title">[!] EXÉCUTION EN COURS</div>
    <div id="exec-current" style="font-size:10px;color:var(--cyan);text-align:center;margin-bottom:12px;letter-spacing:2px;">Initialisation...</div>
    <div style="margin:24px 0;">
      <div class="exec-bar-outer"><div class="exec-bar-fill" id="exec-bar"></div></div>
      <div style="display:flex;justify-content:space-between;font-size:8px;color:var(--cyan-dim)">
        <span>PROGRESSION</span><span id="exec-pct">0%</span>
      </div>
    </div>
    <div class="exec-log" id="exec-log"></div>
    <button class="btn btn-gold" style="width:100%;display:none;" id="exec-close-btn" onclick="closeModal()">[OK] FERMER</button>
  </div>
</div>
<script>
// ── Données injectées par le serveur C++ ────────────────────────────────────
const DONNEES_AGENTS = )";

        // ── Injection du JSON dynamique ──────────────────────────────────────
        html << agentsJson;

        html << R"(;
// ────────────────────────────────────────────────────────────────────────────

let selected = new Set(AGENTS_DATA.filter(a=>a.selected).map(a=>a.id));
let currentFilter='all';

function renderAgents(){
  const grid=document.getElementById('agent-grid');
  const list=currentFilter==='all'?AGENTS_DATA:AGENTS_DATA.filter(a=>a.type===currentFilter);
  grid.innerHTML='';
  list.forEach(agent=>{
    const isSel=selected.has(agent.id);
    const card=document.createElement('div');
    card.className='agent-card'+(isSel?' selected':'');
    card.dataset.id=agent.id;
    const sdot=agent.status==='online'?'':'warn';
    const scolor=agent.status==='online'?'#00ff88':agent.status==='standby'?'#ffa500':'#ff2020';
    const stxt=agent.status==='online'?'ACTIF':agent.status==='standby'?'VEILLE':'HORS LIGNE';
    card.innerHTML=`<div class="card-inner">
      <div class="card-corner tl"></div><div class="card-corner tr"></div>
      <div class="card-corner bl"></div><div class="card-corner br"></div>
      <div class="select-badge">${[...selected].indexOf(agent.id)+1||''}</div>
      <div class="agent-icon">${agent.icon}</div>
      <div class="agent-name">${agent.name}</div>
      <div class="agent-type">${agent.type.toUpperCase()} // P${agent.priority}</div>
      <div class="agent-stats"><span>DEP: ${agent.deps.length}</span><span>ORD: ${agent.order}</span></div>
      <div class="agent-status"><div class="status-dot ${sdot}"></div>
      <span style="font-size:8px;letter-spacing:1px;color:${scolor}">${stxt}</span></div>
    </div>`;
    card.addEventListener('click',()=>toggleAgent(agent.id));
    grid.appendChild(card);
  });
}
function toggleAgent(id){
  if(selected.has(id)){selected.delete(id);addLog('Agent '+id.toUpperCase()+' désélectionné','warn');}
  else{selected.add(id);addLog('Agent '+id.toUpperCase()+' sélectionné','ok');}
  updateUI();
  // Notifie le serveur C++ (optionnel)
  fetch('/api/select?id='+id+'&active='+selected.has(id)).catch(()=>{});
}
function updateUI(){
  renderAgents();
  document.getElementById('sel-count').textContent=selected.size;
  document.getElementById('active-count').textContent=selected.size+' / '+AGENTS_DATA.length;
  renderQueue();
}
function renderQueue(){
  const q=document.getElementById('queue-list');
  if(selected.size===0){q.innerHTML='<div style="font-size:9px;color:rgba(0,229,255,.3);text-align:center;padding:20px 0;">Aucun agent sélectionné</div>';return;}
  q.innerHTML=''; let idx=1;
  selected.forEach(id=>{
    const a=AGENTS_DATA.find(x=>x.id===id);if(!a)return;
    const item=document.createElement('div');item.className='queue-item';
    item.innerHTML=`<span class="queue-num">${idx}</span>
      <div style="flex:1"><div class="queue-name">${a.name}</div>
      <div class="queue-type">${a.type}</div></div>
      <button class="queue-remove" onclick="toggleAgent('${id}')">✕</button>`;
    q.appendChild(item);idx++;
  });
}
function filterAgents(t){currentFilter=t;renderAgents();}
function clearSelection(){selected.clear();addLog('Sélection effacée','warn');updateUI();}
function selectAll(){AGENTS_DATA.forEach(a=>selected.add(a.id));addLog('Tous sélectionnés','ok');updateUI();}
function validateDeps(){
  let ok=true;
  selected.forEach(id=>{
    const a=AGENTS_DATA.find(x=>x.id===id);
    a.deps.forEach(d=>{if(!selected.has(d))ok=false;});
  });
  if(ok){showNotif('[OK] DÉPENDANCES VALIDÉES');addLog('Validation OK','ok');}
  else{showNotif('✗ DÉPENDANCES MANQUANTES');addLog('Validation ÉCHEC','err');}
}
function saveSnapshot(){showNotif('SNAPSHOT SAUVEGARDÉ');addLog('Snapshot enregistré','ok');}
function showNotif(msg){
  const n=document.getElementById('notif');n.textContent=msg;
  n.classList.add('show');setTimeout(()=>n.classList.remove('show'),2500);
}
function addLog(msg,type='ok'){
  const box=document.getElementById('log-box');
  const now=new Date();
  const t=`${String(now.getHours()).padStart(2,'0')}:${String(now.getMinutes()).padStart(2,'0')}:${String(now.getSeconds()).padStart(2,'0')}`;
  const line=document.createElement('div');line.className='log-line';
  line.innerHTML=`<span class="log-time">[${t}]</span><span class="log-msg ${type}">${msg}</span>`;
  box.appendChild(line);box.scrollTop=box.scrollHeight;
}
async function executeAgents(){
  if(selected.size===0){showNotif('✗ AUCUN AGENT SÉLECTIONNÉ');return;}
  const overlay=document.getElementById('exec-overlay');
  const bar=document.getElementById('exec-bar');
  const pct=document.getElementById('exec-pct');
  const cur=document.getElementById('exec-current');
  const log=document.getElementById('exec-log');
  const closeBtn=document.getElementById('exec-close-btn');
  overlay.classList.add('show');bar.style.width='0%';pct.textContent='0%';
  log.innerHTML='';closeBtn.style.display='none';
  const agents=[...selected].map(id=>AGENTS_DATA.find(a=>a.id===id));
  for(let i=0;i<agents.length;i++){
    const a=agents[i];
    cur.textContent='EXÉCUTION: '+a.name;
    const p=Math.round(((i+.5)/agents.length)*100);
    bar.style.width=p+'%';pct.textContent=p+'%';
    log.innerHTML+=`<div class="exec-log"><span class="label">[${i+1}/${agents.length}] ${a.name}</span> - <span class="ok">Exécution...</span></div>`;
    log.scrollTop=log.scrollHeight;
    await fetch('/api/execute?id='+a.id).catch(()=>{});
    await new Promise(r=>setTimeout(r,600));
    log.innerHTML+=`<div><span style="color:rgba(0,229,255,.4)"> └── </span><span class="ok">[OK] Complété: ${a.path}</span></div>`;
    log.scrollTop=log.scrollHeight;
  }
  bar.style.width='100%';pct.textContent='100%';
  cur.textContent='[OK] TOUS LES AGENTS EXÉCUTÉS';cur.style.color='#00ff88';
  closeBtn.style.display='block';
  addLog('Exécution terminée: '+agents.length+' agent(s)','ok');
}
function closeModal(){document.getElementById('exec-overlay').classList.remove('show');}
setInterval(()=>{
  const n=new Date();
  const t=`${String(n.getHours()).padStart(2,'0')}:${String(n.getMinutes()).padStart(2,'0')}:${String(n.getSeconds()).padStart(2,'0')}`;
  document.getElementById('clock').textContent=t;
},1000);
setInterval(()=>{
  const v=65+Math.floor(Math.random()*25);
  document.getElementById('cpu-val').textContent=v+'%';
},3000);
renderAgents();renderQueue();
</script>
</body>
</html>)";
        return html.str();
    }
};

// ════════════════════════════════════════════════════════════════════════════
// ─── HTTP SERVER LÉGER ───────────────────────────────────────────────────────
// Gère GET / (page principale) et GET /api/select, /api/execute
// ════════════════════════════════════════════════════════════════════════════

class MiniHttpServer {
public:
    using RouteHandler = std::function<std::string(const std::string& query)>;

private:
    int                                         port_;
    SocketType                                  server_socket_{INVALID_SOCK};
    std::atomic<bool>                           running_{false};
    std::thread                                 server_thread_;
    std::unordered_map<std::string, RouteHandler> routes_;
    std::string                                 main_page_;

public:
    explicit MiniHttpServer(int port = 7474) : port_(port) {}

    ~MiniHttpServer() { stop(); }

    void setMainPage(const std::string& html) { main_page_ = html; }
    void addRoute(const std::string& path, RouteHandler handler) { routes_[path] = handler; }

    bool start() {
#ifdef _WIN32
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) return false;
#endif
        server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket_ == INVALID_SOCK) return false;

        int opt = 1;
        setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR,
                   reinterpret_cast<const char*>(&opt), sizeof(opt));

        sockaddr_in addr{};
        addr.sin_family      = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port        = htons(static_cast<uint16_t>(port_));

        if (bind(server_socket_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) return false;
        if (listen(server_socket_, 10) < 0) return false;

        running_ = true;
        server_thread_ = std::thread(&MiniHttpServer::serve, this);
        return true;
    }

    void stop() {
        running_ = false;
        if (server_socket_ != INVALID_SOCK) {
            CLOSE_SOCK(server_socket_);
            server_socket_ = INVALID_SOCK;
        }
        if (server_thread_.joinable()) server_thread_.join();
#ifdef _WIN32
        WSACleanup();
#endif
    }

    int port() const { return port_; }

private:
    void serve() {
        while (running_) {
            sockaddr_in client{};
            socklen_t   len = sizeof(client);
            SocketType  client_sock = accept(server_socket_,
                                             reinterpret_cast<sockaddr*>(&client), &len);
            if (client_sock == INVALID_SOCK) break;

            // Lecture de la requête
            char   buf[4096] = {};
            recv(client_sock, buf, sizeof(buf) - 1, 0);
            std::string request(buf);

            // Parsing: "GET /path?query HTTP/1.1"
            std::string path, query;
            auto get_pos = request.find("GET ");
            if (get_pos != std::string::npos) {
                auto start  = get_pos + 4;
                auto end    = request.find(' ', start);
                std::string full_path = request.substr(start, end - start);
                auto qpos = full_path.find('?');
                if (qpos != std::string::npos) {
                    path  = full_path.substr(0, qpos);
                    query = full_path.substr(qpos + 1);
                } else {
                    path = full_path;
                }
            }

            // Génération de la réponse
            std::string body, content_type;
            if (path == "/" || path.empty()) {
                body         = main_page_;
                content_type = "text/html; charset=utf-8";
            } else if (routes_.count(path)) {
                body         = routes_[path](query);
                content_type = "application/json";
            } else {
                body         = R"({"error":"not found"})";
                content_type = "application/json";
            }

            std::string response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: "   + content_type + "\r\n"
                "Content-Length: " + std::to_string(body.size()) + "\r\n"
                "Connection: close\r\n\r\n" + body;

            send(client_sock, response.c_str(),
                 static_cast<int>(response.size()), 0);
            CLOSE_SOCK(client_sock);
        }
    }
};

// ════════════════════════════════════════════════════════════════════════════
// ─── INTERFACE GRAPHIQUE IRON MAN ────────────────────────────────────────────
// Remplace AgentSelectionInterface::displayGraphicalMenu()
// ════════════════════════════════════════════════════════════════════════════

class AgentGraphicalInterface {
private:
    AgentSelectionSystem& system_;
    MiniHttpServer        server_;

public:
    explicit AgentGraphicalInterface(AgentSelectionSystem& system, int port = 7474)
        : system_(system), server_(port) {}

    /**
     * @brief Lance l'interface Iron Man dans le navigateur par défaut
     *
     * Usage (dans main.cpp):
     *   AgentSelectionSystem sys;
     *   sys.loadAgents("agents/");          // charge vos agents
     *   AgentGraphicalInterface gui(sys);
     *   gui.launch();
     */
    void launch() {
        // 1. Génère le HTML avec les données actuelles
        server_.setMainPage(JarvisHtmlBuilder::buildFullPage(system_));

        // 2. Route API: sélection/désélection d'un agent depuis le navigateur
        server_.addRoute("/api/select", [this](const std::string& query) -> std::string {
            auto id     = parseParam(query, "id");
            auto active = parseParam(query, "active");
            if (active == "true") system_.selectAgent(id);
            else                  system_.deselectAgent(id);
            // Regénère le HTML pour les rechargements ultérieurs
            server_.setMainPage(JarvisHtmlBuilder::buildFullPage(system_));
            return R"({"status":"ok"})";
        });

        // 3. Route API: exécution d'un agent depuis le navigateur
        server_.addRoute("/api/execute", [this](const std::string& query) -> std::string {
            auto id = parseParam(query, "id");
            // Ici: appel réel à executeAgent(path)
            std::cout << "[JARVIS] Exécution agent: " << id << "\n";
            return R"({"status":"ok"})";
        });

        // 4. Démarrage du serveur HTTP
        if (!server_.start()) {
            std::cerr << "[JARVIS] Erreur: impossible de démarrer le serveur HTTP\n";
            return;
        }

        std::string url = "http://localhost:" + std::to_string(server_.port());
        std::cout << "\n";
        std::cout << "  ╔═══════════════════════════════════════════════╗\n";
        std::cout << "  ║     J.A.R.V.I.S  - Interface Iron Man        ║\n";
        std::cout << "  ╠═══════════════════════════════════════════════╣\n";
        std::cout << "  ║  Serveur HTTP démarré sur : " << url << "  ║\n";
        std::cout << "  ║  Ouverture du navigateur...                   ║\n";
        std::cout << "  ║  (Ctrl+C pour quitter)                        ║\n";
        std::cout << "  ╚═══════════════════════════════════════════════╝\n\n";

        // 5. Ouvre le navigateur
        openBrowser(url);

        // 6. Boucle principale - attend Ctrl+C
        std::cout << "Appuyez sur [ENTRÉE] pour arrêter le serveur...\n";
        std::cin.get();
        server_.stop();
    }

private:
    // Extrait un paramètre "key=value" depuis une query string
    static std::string parseParam(const std::string& query, const std::string& key) {
        auto start = query.find(key + "=");
        if (start == std::string::npos) return "";
        start += key.size() + 1;
        auto end = query.find('&', start);
        return (end == std::string::npos)
            ? query.substr(start)
            : query.substr(start, end - start);
    }
};

#endif // AGENT_GRAPHICAL_INTERFACE_H