package com.secondcerveau;

import org.springframework.stereotype.Component;
import org.springframework.web.socket.CloseStatus;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;
import org.springframework.web.socket.handler.TextWebSocketHandler;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import java.util.*;

@Component
public class AgentWebSocketHandler extends TextWebSocketHandler {
    private static final Logger logger = LoggerFactory.getLogger(AgentWebSocketHandler.class);
    private final ObjectMapper objectMapper = new ObjectMapper();
    private final NativeOrchestratorService orchestratorService;
    
    public AgentWebSocketHandler(NativeOrchestratorService orchestratorService) {
        this.orchestratorService = orchestratorService;
    }
    
    @Override
    public void afterConnectionEstablished(WebSocketSession session) throws Exception {
        orchestratorService.registerSession(session);
        logger.info("WebSocket connection established: {}", session.getId());
        
        // Send initial status
        Map<String, Object> greeting = new HashMap<>();
        greeting.put("type", "connection");
        greeting.put("status", "connected");
        greeting.put("agents", orchestratorService.listActiveAgents().size());
        
        session.sendMessage(new TextMessage(objectMapper.writeValueAsString(greeting)));
    }
    
    @Override
    public void handleTextMessage(WebSocketSession session, TextMessage message) throws Exception {
        try {
            Map<String, Object> request = objectMapper.readValue(
                message.getPayload(), 
                new com.fasterxml.jackson.core.type.TypeReference<Map<String, Object>>() {}
            );
            
            String messageType = (String) request.get("type");
            
            if ("task".equals(messageType)) {
                String agentType = (String) request.get("agentType");
                Map<String, Object> taskData = (Map<String, Object>) request.get("taskData");
                
                int taskId = orchestratorService.submitTask(agentType, taskData);
                
                Map<String, Object> response = new HashMap<>();
                response.put("type", "task_submitted");
                response.put("taskId", taskId);
                response.put("status", "submitted");
                
                session.sendMessage(new TextMessage(objectMapper.writeValueAsString(response)));
            }
            
        } catch (Exception e) {
            logger.error("Error handling WebSocket message", e);
        }
    }
    
    @Override
    public void afterConnectionClosed(WebSocketSession session, CloseStatus status) throws Exception {
        orchestratorService.unregisterSession(session);
        logger.info("WebSocket connection closed: {}", session.getId());
    }
}
