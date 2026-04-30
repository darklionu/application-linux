package com.secondcerveau;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class AgentTask {
    private int id;
    private String agentType;
    private Map<String, Object> data;
    private String status;
    private long timestamp;
    
    public int getId() { return id; }
    public void setId(int id) { this.id = id; }
    
    public String getAgentType() { return agentType; }
    public void setAgentType(String agentType) { this.agentType = agentType; }
    
    public Map<String, Object> getData() { return data; }
    public void setData(Map<String, Object> data) { this.data = new ConcurrentHashMap<>(data); }
    
    public String getStatus() { return status; }
    public void setStatus(String status) { this.status = status; }
    
    public long getTimestamp() { return timestamp; }
    public void setTimestamp(long timestamp) { this.timestamp = timestamp; }
}
