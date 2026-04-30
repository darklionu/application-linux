package com.secondcerveau;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class AgentTaskResult {
    private int taskId;
    private Map<String, Object> result;
    private String status;
    private long timestamp;
    
    public int getTaskId() { return taskId; }
    public void setTaskId(int taskId) { this.taskId = taskId; }
    
    public Map<String, Object> getResult() { return result; }
    public void setResult(Map<String, Object> result) { 
        this.result = new ConcurrentHashMap<>(result); 
    }
    
    public String getStatus() { return status; }
    public void setStatus(String status) { this.status = status; }
    
    public long getTimestamp() { return timestamp; }
    public void setTimestamp(long timestamp) { this.timestamp = timestamp; }
}
