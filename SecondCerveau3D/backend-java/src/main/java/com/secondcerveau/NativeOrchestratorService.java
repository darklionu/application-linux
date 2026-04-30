package com.secondcerveau;

import org.springframework.stereotype.Service;
import org.springframework.web.socket.WebSocketSession;
import com.fasterxml.jackson.databind.ObjectMapper;
import java.util.*;
import java.util.concurrent.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

@Service
public class NativeOrchestratorService {
    private static final Logger logger = LoggerFactory.getLogger(NativeOrchestratorService.class);
    
    private final Map<String, AgentExecutor> agentExecutors;
    private final BlockingQueue<AgentTask> taskQueue;
    private final Map<Integer, AgentTaskResult> taskResults;
    private final Set<WebSocketSession> activeSessions;
    private final ObjectMapper objectMapper;
    private int nextTaskId = 1;
    
    public NativeOrchestratorService() {
        this.agentExecutors = new ConcurrentHashMap<>();
        this.taskQueue = new LinkedBlockingQueue<>();
        this.taskResults = new ConcurrentHashMap<>();
        this.activeSessions = ConcurrentHashMap.newKeySet();
        this.objectMapper = new ObjectMapper();
        
        initializeAgentExecutors();
        startTaskProcessor();
    }
    
    private void initializeAgentExecutors() {
        agentExecutors.put("red_recon", new AgentExecutor("red_recon"));
        agentExecutors.put("red_exploit", new AgentExecutor("red_exploit"));
        agentExecutors.put("red_blackhat", new AgentExecutor("red_blackhat"));
        agentExecutors.put("blue_monitor", new AgentExecutor("blue_monitor"));
        agentExecutors.put("blue_response", new AgentExecutor("blue_response"));
        
        logger.info("Agent executors initialized: {}", agentExecutors.keySet());
    }
    
    private void startTaskProcessor() {
        new Thread(() -> {
            while (true) {
                try {
                    AgentTask task = taskQueue.take();
                    executeTask(task);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    break;
                }
            }
        }, "TaskProcessor").start();
    }
    
    public synchronized int submitTask(String agentType, Map<String, Object> taskData) {
        int taskId = nextTaskId++;
        
        AgentTask task = new AgentTask();
        task.setId(taskId);
        task.setAgentType(agentType);
        task.setData(taskData);
        task.setStatus("SUBMITTED");
        task.setTimestamp(System.currentTimeMillis());
        
        try {
            taskQueue.put(task);
            logger.info("Task {} submitted for agent {}", taskId, agentType);
            broadcastUpdate(task);
        } catch (InterruptedException e) {
            logger.error("Failed to submit task", e);
            return -1;
        }
        
        return taskId;
    }
    
    private void executeTask(AgentTask task) {
        AgentExecutor executor = agentExecutors.get(task.getAgentType());
        
        if (executor == null) {
            logger.warn("No executor found for agent type: {}", task.getAgentType());
            task.setStatus("FAILED");
            return;
        }
        
        task.setStatus("RUNNING");
        broadcastUpdate(task);
        
        try {
            Map<String, Object> result = executor.execute(task.getData());
            
            AgentTaskResult taskResult = new AgentTaskResult();
            taskResult.setTaskId(task.getId());
            taskResult.setResult(result);
            taskResult.setStatus("SUCCESS");
            taskResult.setTimestamp(System.currentTimeMillis());
            
            taskResults.put(task.getId(), taskResult);
            task.setStatus("COMPLETED");
            
            logger.info("Task {} completed", task.getId());
            broadcastUpdate(task, result);
            
        } catch (Exception e) {
            logger.error("Task execution failed", e);
            task.setStatus("FAILED");
            broadcastUpdate(task);
        }
    }
    
    public AgentTaskResult getTaskResult(int taskId) {
        return taskResults.get(taskId);
    }
    
    public void registerSession(WebSocketSession session) {
        activeSessions.add(session);
        logger.info("WebSocket session registered: {}", session.getId());
    }
    
    public void unregisterSession(WebSocketSession session) {
        activeSessions.remove(session);
        logger.info("WebSocket session unregistered: {}", session.getId());
    }
    
    private void broadcastUpdate(AgentTask task) {
        broadcastUpdate(task, null);
    }
    
    private void broadcastUpdate(AgentTask task, Map<String, Object> result) {
        for (WebSocketSession session : activeSessions) {
            try {
                Map<String, Object> message = new LinkedHashMap<>();
                message.put("type", "task_update");
                message.put("taskId", task.getId());
                message.put("status", task.getStatus());
                message.put("agentType", task.getAgentType());
                if (result != null) message.put("result", result);
                
                session.sendMessage(new org.springframework.web.socket.TextMessage(
                    objectMapper.writeValueAsString(message)
                ));
            } catch (Exception e) {
                logger.error("Failed to broadcast update", e);
            }
        }
    }
    
    public List<AgentTask> listActiveAgents() {
        return new ArrayList<>(agentExecutors.keySet()).stream()
            .map(name -> {
                AgentTask task = new AgentTask();
                task.setAgentType(name);
                task.setStatus("ACTIVE");
                return task;
            })
            .toList();
    }
}
