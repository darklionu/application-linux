package com.secondcerveau;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;
import org.springframework.http.ResponseEntity;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import java.util.*;

@RestController
@RequestMapping("/api/agents")
public class GraphController {
    private static final Logger logger = LoggerFactory.getLogger(GraphController.class);
    
    @Autowired
    private NativeOrchestratorService orchestratorService;
    
    /* Graph data representation */
    private Map<String, GraphNode> graphNodes = new ConcurrentHashMap<>();
    private List<GraphEdge> graphEdges = Collections.synchronizedList(new ArrayList<>());
    
    @PostMapping("/task")
    public ResponseEntity<Map<String, Object>> submitTask(@RequestBody Map<String, Object> request) {
        String agentType = (String) request.get("agentType");
        Map<String, Object> taskData = (Map<String, Object>) request.get("taskData");
        
        int taskId = orchestratorService.submitTask(agentType, taskData);
        
        Map<String, Object> response = new HashMap<>();
        response.put("taskId", taskId);
        response.put("status", "submitted");
        
        return ResponseEntity.ok(response);
    }
    
    @GetMapping("/task/{taskId}")
    public ResponseEntity<Map<String, Object>> getTaskResult(@PathVariable int taskId) {
        NativeOrchestratorService.AgentTaskResult result = orchestratorService.getTaskResult(taskId);
        
        if (result == null) {
            return ResponseEntity.notFound().build();
        }
        
        Map<String, Object> response = new HashMap<>();
        response.put("taskId", taskId);
        response.put("status", result.getStatus());
        response.put("result", result.getResult());
        
        return ResponseEntity.ok(response);
    }
    
    @GetMapping("/graph")
    public ResponseEntity<Map<String, Object>> getGraph() {
        Map<String, Object> graph = new HashMap<>();
        
        List<GraphNode> nodesList = new ArrayList<>(graphNodes.values());
        graph.put("nodes", nodesList);
        graph.put("edges", graphEdges);
        
        logger.info("Graph request: {} nodes, {} edges", nodesList.size(), graphEdges.size());
        
        return ResponseEntity.ok(graph);
    }
    
    @PostMapping("/graph/node")
    public ResponseEntity<Map<String, Object>> addGraphNode(@RequestBody GraphNode node) {
        graphNodes.put(node.getId(), node);
        
        Map<String, Object> response = new HashMap<>();
        response.put("nodeId", node.getId());
        response.put("status", "added");
        
        logger.info("Node added: {}", node.getId());
        
        return ResponseEntity.ok(response);
    }
    
    @PostMapping("/graph/edge")
    public ResponseEntity<Map<String, Object>> addGraphEdge(@RequestBody GraphEdge edge) {
        graphEdges.add(edge);
        
        Map<String, Object> response = new HashMap<>();
        response.put("edgeId", System.identityHashCode(edge));
        response.put("status", "added");
        
        return ResponseEntity.ok(response);
    }
    
    @GetMapping("/agents")
    public ResponseEntity<List<?>> listAgents() {
        List<?> agents = orchestratorService.listActiveAgents();
        return ResponseEntity.ok(agents);
    }
    
    public static class GraphNode {
        private String id;
        private String label;
        private String type;
        private int threatLevel;
        private Map<String, Object> position;
        
        public String getId() { return id; }
        public void setId(String id) { this.id = id; }
        
        public String getLabel() { return label; }
        public void setLabel(String label) { this.label = label; }
        
        public String getType() { return type; }
        public void setType(String type) { this.type = type; }
        
        public int getThreatLevel() { return threatLevel; }
        public void setThreatLevel(int threatLevel) { this.threatLevel = threatLevel; }
        
        public Map<String, Object> getPosition() { return position; }
        public void setPosition(Map<String, Object> position) { this.position = position; }
    }
    
    public static class GraphEdge {
        private String source;
        private String target;
        private String type;
        private float weight;
        
        public String getSource() { return source; }
        public void setSource(String source) { this.source = source; }
        
        public String getTarget() { return target; }
        public void setTarget(String target) { this.target = target; }
        
        public String getType() { return type; }
        public void setType(String type) { this.type = type; }
        
        public float getWeight() { return weight; }
        public void setWeight(float weight) { this.weight = weight; }
    }
}
