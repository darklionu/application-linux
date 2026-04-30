const { app, BrowserWindow, ipcMain } = require('electron');
const path = require('path');
const ws = require('ws');

let mainWindow;
let wsConnection;

const createWindow = () => {
    mainWindow = new BrowserWindow({
        width: 1400,
        height: 900,
        webPreferences: {
            nodeIntegration: false,
            contextIsolation: true,
            preload: path.join(__dirname, 'preload.js')
        }
    });

    mainWindow.loadFile(path.join(__dirname, 'src', 'index.html'));

    mainWindow.webContents.openDevTools();

    setupIPC();
    connectToBackend();
};

const connectToBackend = () => {
    console.log('[Electron] Connecting to WebSocket backend...');
    
    wsConnection = new ws('ws://localhost:8080/ws');
    
    wsConnection.on('open', () => {
        console.log('[Electron] Connected to backend');
        mainWindow?.webContents.send('backend-connected');
    });
    
    wsConnection.on('message', (data) => {
        console.log('[Electron] Received:', data);
        try {
            const message = JSON.parse(data);
            mainWindow?.webContents.send('agent-update', message);
        } catch (e) {
            console.error('Failed to parse message:', e);
        }
    });
    
    wsConnection.on('error', (error) => {
        console.error('[Electron] WebSocket error:', error);
        mainWindow?.webContents.send('backend-error', error.message);
    });
};

const setupIPC = () => {
    ipcMain.handle('submit-task', async (event, agentType, taskData) => {
        const request = {
            type: 'task',
            agentType: agentType,
            taskData: taskData
        };
        
        wsConnection?.send(JSON.stringify(request));
        return { status: 'sent' };
    });
    
    ipcMain.handle('get-graph', async (event) => {
        return new Promise((resolve) => {
            setTimeout(() => {
                resolve({
                    nodes: [
                        { id: 'agent1', label: 'Red Recon', type: 'agent', threat: 2 },
                        { id: 'agent2', label: 'Blue Monitor', type: 'agent', threat: 1 },
                        { id: 'target', label: 'Target System', type: 'system', threat: 5 }
                    ],
                    edges: [
                        { source: 'agent1', target: 'target', type: 'scanning' },
                        { source: 'agent2', target: 'agent1', type: 'monitoring' }
                    ]
                });
            }, 100);
        });
    });
};

app.on('ready', createWindow);

app.on('window-all-closed', () => {
    if (process.platform !== 'darwin') {
        app.quit();
    }
});

module.exports = { wsConnection };
