/**
 * SecondCerveau3D - Electron Renderer Process
 * Handles UI interactions and 3D graph rendering
 */

const { ipcRenderer } = require('electron');

// Global state
const state = {
    agents: [],
    selectedAgent: null,
    updates: [],
    graphData: { nodes: [], edges: [] }
};

// Initialize when DOM is ready
document.addEventListener('DOMContentLoaded', () => {
    console.log('[Renderer] Initializing...');
    
    loadAgents();
    setupEventListeners();
    connectToBackend();
    loadGraphData();
});

// Load available agents
async function loadAgents() {
    try {
        const response = await fetch('http://localhost:8080/api/agents');
        state.agents = await response.json();
        
        const select = document.getElementById('agentSelect');
        select.innerHTML = '';
        
        state.agents.forEach(agent => {
            const option = document.createElement('option');
            option.value = agent.agentType || agent;
            option.textContent = agent.agentType || agent;
            select.appendChild(option);
        });
        
        console.log('[Renderer] Loaded agents:', state.agents.length);
    } catch (error) {
        console.error('[Renderer] Failed to load agents:', error);
    }
}

// Setup event listeners
function setupEventListeners() {
    const agentSelect = document.getElementById('agentSelect');
    agentSelect.addEventListener('change', (e) => {
        state.selectedAgent = e.target.value;
    });
}

// Connect to WebSocket backend
function connectToBackend() {
    console.log('[Renderer] Connecting to WebSocket...');
    
    const ws = new WebSocket('ws://localhost:8080/ws');
    
    ws.onopen = () => {
        console.log('[Renderer] WebSocket connected');
        updateStatus(true);
    };
    
    ws.onmessage = (event) => {
        try {
            const message = JSON.parse(event.data);
            console.log('[Renderer] Message received:', message);
            
            if (message.type === 'task_update') {
                addUpdate(message);
            }
        } catch (error) {
            console.error('[Renderer] Failed to parse message:', error);
        }
    };
    
    ws.onerror = (error) => {
        console.error('[Renderer] WebSocket error:', error);
        updateStatus(false);
    };
    
    ws.onclose = () => {
        console.log('[Renderer] WebSocket closed');
        updateStatus(false);
        // Reconnect after delay
        setTimeout(connectToBackend, 3000);
    };
    
    window.wsConnection = ws;
}

// Submit task
async function submitTask() {
    if (!state.selectedAgent) {
        alert('Please select an agent');
        return;
    }
    
    const taskDataText = document.getElementById('taskData').value;
    
    if (!taskDataText) {
        alert('Please enter task data');
        return;
    }
    
    try {
        const taskData = JSON.parse(taskDataText);
        
        const payload = {
            agentType: state.selectedAgent,
            taskData: taskData
        };
        
        const response = await fetch('http://localhost:8080/api/agents/task', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(payload)
        });
        
        const result = await response.json();
        console.log('[Renderer] Task submitted:', result);
        
        addUpdate({
            type: 'task_submitted',
            agentType: state.selectedAgent,
            taskId: result.taskId,
            status: 'submitted'
        });
        
        // Clear input
        document.getElementById('taskData').value = '';
        
        // Reload graph
        setTimeout(loadGraphData, 1000);
        
    } catch (error) {
        console.error('[Renderer] Error submitting task:', error);
        alert('Failed to submit task: ' + error.message);
    }
}

// Load graph data
async function loadGraphData() {
    try {
        const response = await fetch('http://localhost:8080/api/agents/graph');
        if (!response.ok) return;
        
        state.graphData = await response.json();
        console.log('[Renderer] Graph loaded:', 
            state.graphData.nodes.length, 'nodes,',
            state.graphData.edges.length, 'edges');
        
        updateGraphStats();
        renderGraph();
        
    } catch (error) {
        console.error('[Renderer] Failed to load graph:', error);
    }
}

// Update graph statistics
function updateGraphStats() {
    const threatCount = state.graphData.nodes.filter(n => n.threat > 5).length;
    
    document.getElementById('nodeCount').textContent = state.graphData.nodes.length;
    document.getElementById('edgeCount').textContent = state.graphData.edges.length;
    document.getElementById('threatCount').textContent = threatCount;
}

// Render graph on canvas
function renderGraph() {
    const canvas = document.getElementById('canvas');
    if (!canvas) return;
    
    const ctx = canvas.getContext('2d');
    const width = canvas.width = canvas.offsetWidth;
    const height = canvas.height = canvas.offsetHeight;
    
    // Clear canvas
    ctx.fillStyle = '#0a0e27';
    ctx.fillRect(0, 0, width, height);
    
    const centerX = width / 2;
    const centerY = height / 2;
    const radius = Math.min(width, height) / 3;
    
    // Draw edges
    ctx.strokeStyle = '#00d4ff';
    ctx.lineWidth = 1;
    
    state.graphData.edges.forEach((edge, i) => {
        const sourceIdx = state.graphData.nodes.findIndex(n => n.id === edge.source);
        const targetIdx = state.graphData.nodes.findIndex(n => n.id === edge.target);
        
        if (sourceIdx >= 0 && targetIdx >= 0) {
            const angle1 = (sourceIdx / state.graphData.nodes.length) * Math.PI * 2;
            const angle2 = (targetIdx / state.graphData.nodes.length) * Math.PI * 2;
            
            const x1 = centerX + Math.cos(angle1) * radius;
            const y1 = centerY + Math.sin(angle1) * radius;
            const x2 = centerX + Math.cos(angle2) * radius;
            const y2 = centerY + Math.sin(angle2) * radius;
            
            ctx.beginPath();
            ctx.moveTo(x1, y1);
            ctx.lineTo(x2, y2);
            ctx.stroke();
        }
    });
    
    // Draw nodes
    state.graphData.nodes.forEach((node, i) => {
        const angle = (i / state.graphData.nodes.length) * Math.PI * 2;
        const x = centerX + Math.cos(angle) * radius;
        const y = centerY + Math.sin(angle) * radius;
        
        // Color based on threat level
        if (node.threat >= 7) ctx.fillStyle = '#ff0000';
        else if (node.threat >= 4) ctx.fillStyle = '#ffaa00';
        else ctx.fillStyle = '#00ff00';
        
        ctx.beginPath();
        ctx.arc(x, y, 8, 0, Math.PI * 2);
        ctx.fill();
        
        // Draw label
        ctx.fillStyle = '#fff';
        ctx.font = 'bold 10px Arial';
        ctx.textAlign = 'center';
        ctx.fillText(node.label, x, y + 20);
    });
}

// Add update to log
function addUpdate(update) {
    state.updates.unshift(update);
    if (state.updates.length > 10) state.updates.pop();
    
    const updateList = document.getElementById('updateList');
    updateList.innerHTML = state.updates.map(u => 
        `<div class="update-item"><strong>${u.agentType}</strong> - ${u.status}</div>`
    ).join('');
}

// Update connection status
function updateStatus(connected) {
    const indicator = document.getElementById('statusIndicator');
    const text = document.getElementById('statusText');
    
    if (connected) {
        indicator.className = 'status-indicator connected';
        text.textContent = 'Connected';
    } else {
        indicator.className = 'status-indicator disconnected';
        text.textContent = 'Disconnected';
    }
}

// Auto-refresh graph
setInterval(loadGraphData, 5000);

// Expose functions to window
window.submitTask = submitTask;
