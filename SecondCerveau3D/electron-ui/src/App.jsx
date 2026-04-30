import React, { useEffect, useState } from 'react';
import Graph3D from './Graph3D';

const App = () => {
    const [agents, setAgents] = useState([]);
    const [selectedAgent, setSelectedAgent] = useState(null);
    const [taskData, setTaskData] = useState('');
    const [connected, setConnected] = useState(false);
    const [updates, setUpdates] = useState([]);

    useEffect(() => {
        // Listen for backend connection
        window.electronAPI?.onBackendConnected(() => {
            console.log('Backend connected');
            setConnected(true);
        });

        window.electronAPI?.onAgentUpdate((data) => {
            setUpdates(prev => [...prev, data]);
        });

        // Load initial agent list
        loadAgents();
    }, []);

    const loadAgents = async () => {
        try {
            const response = await fetch('http://localhost:8080/api/agents');
            const agentList = await response.json();
            setAgents(agentList);
        } catch (error) {
            console.error('Failed to load agents:', error);
        }
    };

    const submitTask = async () => {
        if (!selectedAgent || !taskData) {
            alert('Please select an agent and enter task data');
            return;
        }

        try {
            const result = await window.electronAPI?.submitTask(selectedAgent, JSON.parse(taskData));
            console.log('Task submitted:', result);
            setTaskData('');
        } catch (error) {
            console.error('Failed to submit task:', error);
        }
    };

    return (
        <div style={styles.container}>
            <header style={styles.header}>
                <h1>🧠 SecondCerveau3D - Agent Orchestrator</h1>
                <span style={{ color: connected ? '#4caf50' : '#ff6b6b' }}>
                    {connected ? '● Connected' : '● Disconnected'}
                </span>
            </header>

            <div style={styles.content}>
                <div style={styles.sidebar}>
                    <h2>Task Submission</h2>
                    
                    <div style={styles.section}>
                        <label>Select Agent:</label>
                        <select 
                            value={selectedAgent || ''}
                            onChange={(e) => setSelectedAgent(e.target.value)}
                            style={styles.select}
                        >
                            <option value="">-- Select Agent --</option>
                            {agents.map(agent => (
                                <option key={agent.agentType} value={agent.agentType}>
                                    {agent.agentType}
                                </option>
                            ))}
                        </select>
                    </div>

                    <div style={styles.section}>
                        <label>Task Data (JSON):</label>
                        <textarea
                            value={taskData}
                            onChange={(e) => setTaskData(e.target.value)}
                            placeholder='{"target": "localhost", "port": 22}'
                            style={styles.textarea}
                        />
                    </div>

                    <button onClick={submitTask} style={styles.button}>
                        Submit Task
                    </button>

                    <div style={styles.updateLog}>
                        <h3>Recent Updates</h3>
                        {updates.slice(-5).map((update, i) => (
                            <div key={i} style={styles.updateItem}>
                                <strong>{update.agentType}</strong> - {update.status}
                            </div>
                        ))}
                    </div>
                </div>

                <div style={styles.mainArea}>
                    <Graph3D />
                </div>
            </div>
        </div>
    );
};

const styles = {
    container: {
        display: 'flex',
        flexDirection: 'column',
        height: '100vh',
        fontFamily: 'Segoe UI, Tahoma, Geneva, Verdana, sans-serif',
        backgroundColor: '#1a1a1a',
        color: '#fff'
    },
    header: {
        backgroundColor: '#0d1b2a',
        padding: '20px',
        borderBottom: '2px solid #00d4ff',
        display: 'flex',
        justifyContent: 'space-between',
        alignItems: 'center'
    },
    content: {
        display: 'flex',
        flex: 1,
        gap: '20px',
        padding: '20px'
    },
    sidebar: {
        width: '300px',
        backgroundColor: '#2a2a2a',
        padding: '20px',
        borderRadius: '8px',
        border: '1px solid #444',
        overflowY: 'auto'
    },
    section: {
        marginBottom: '15px'
    },
    select: {
        width: '100%',
        padding: '8px',
        marginTop: '5px',
        backgroundColor: '#3a3a3a',
        color: '#fff',
        border: '1px solid #555',
        borderRadius: '4px'
    },
    textarea: {
        width: '100%',
        height: '80px',
        padding: '8px',
        marginTop: '5px',
        backgroundColor: '#3a3a3a',
        color: '#fff',
        border: '1px solid #555',
        borderRadius: '4px',
        fontFamily: 'monospace',
        fontSize: '12px',
        resize: 'vertical'
    },
    button: {
        width: '100%',
        padding: '10px',
        backgroundColor: '#00d4ff',
        color: '#000',
        border: 'none',
        borderRadius: '4px',
        fontWeight: 'bold',
        cursor: 'pointer',
        marginTop: '10px'
    },
    updateLog: {
        marginTop: '30px',
        padding: '10px',
        backgroundColor: '#1a1a1a',
        borderRadius: '4px',
        border: '1px solid #444'
    },
    updateItem: {
        fontSize: '12px',
        padding: '5px 0',
        borderBottom: '1px solid #444'
    },
    mainArea: {
        flex: 1,
        backgroundColor: '#2a2a2a',
        borderRadius: '8px',
        border: '1px solid #444',
        position: 'relative'
    }
};

export default App;
