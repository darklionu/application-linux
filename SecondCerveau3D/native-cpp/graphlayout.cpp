#include "graphlayout.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cstring>

GraphLayout::GraphLayout() 
    : damping(0.85f), stiffness(0.5f), repulsion(50.0f), max_iterations(100) {
}

GraphLayout::~GraphLayout() {
    nodes.clear();
    edges.clear();
    node_map.clear();
}

int GraphLayout::add_node(const std::string &label, const std::string &type, int threat) {
    static int next_id = 0;
    int node_id = next_id++;
    
    Node node;
    node.id = node_id;
    node.label = label;
    node.position = glm::vec3(
        static_cast<float>(rand()) / RAND_MAX * 10.0f - 5.0f,
        static_cast<float>(rand()) / RAND_MAX * 10.0f - 5.0f,
        static_cast<float>(rand()) / RAND_MAX * 10.0f - 5.0f
    );
    node.velocity = glm::vec3(0.0f);
    node.mass = 1.0f;
    node.threat_level = threat;
    node.node_type = type;
    
    nodes.push_back(node);
    node_map[node_id] = &nodes.back();
    
    return node_id;
}

void GraphLayout::remove_node(int node_id) {
    auto it = std::find_if(nodes.begin(), nodes.end(), 
        [node_id](const Node &n) { return n.id == node_id; });
    
    if (it != nodes.end()) {
        nodes.erase(it);
        node_map.erase(node_id);
        
        /* Remove edges connected to this node */
        edges.erase(std::remove_if(edges.begin(), edges.end(),
            [node_id](const Edge &e) { 
                return e.source == node_id || e.target == node_id; 
            }), edges.end());
    }
}

Node* GraphLayout::get_node(int node_id) {
    auto it = node_map.find(node_id);
    return it != node_map.end() ? it->second : nullptr;
}

void GraphLayout::add_edge(int source, int target, float strength, const std::string &type) {
    Edge edge;
    edge.source = source;
    edge.target = target;
    edge.strength = strength;
    edge.connection_type = type;
    edge.weight = 1.0f / (1.0f + std::abs(source - target));
    
    edges.push_back(edge);
}

void GraphLayout::remove_edge(int source, int target) {
    edges.erase(std::remove_if(edges.begin(), edges.end(),
        [source, target](const Edge &e) {
            return (e.source == source && e.target == target) ||
                   (e.source == target && e.target == source);
        }), edges.end());
}

void GraphLayout::apply_forces() {
    /* Force-directed layout: Fruchterman-Reingold algorithm */
    
    for (auto &node : nodes) {
        node.velocity = glm::vec3(0.0f);
    }
    
    /* Repulsive forces between all node pairs */
    for (size_t i = 0; i < nodes.size(); i++) {
        for (size_t j = i + 1; j < nodes.size(); j++) {
            glm::vec3 delta = nodes[i].position - nodes[j].position;
            float dist = glm::length(delta);
            
            if (dist < 0.1f) dist = 0.1f;
            
            glm::vec3 force = glm::normalize(delta) * (repulsion / (dist * dist));
            nodes[i].velocity += force / nodes[i].mass;
            nodes[j].velocity -= force / nodes[j].mass;
        }
    }
    
    /* Attractive forces along edges */
    for (const auto &edge : edges) {
        Node *source = get_node(edge.source);
        Node *target = get_node(edge.target);
        
        if (source && target) {
            glm::vec3 delta = target->position - source->position;
            float dist = glm::length(delta);
            
            if (dist < 0.01f) dist = 0.01f;
            
            glm::vec3 force = glm::normalize(delta) * stiffness * dist;
            source->velocity += force / source->mass;
            target->velocity -= force / target->mass;
        }
    }
}

void GraphLayout::update_positions(float dt) {
    for (auto &node : nodes) {
        node.velocity *= damping;
        node.position += node.velocity * dt;
    }
}

void GraphLayout::detect_collisions() {
    for (size_t i = 0; i < nodes.size(); i++) {
        for (size_t j = i + 1; j < nodes.size(); j++) {
            glm::vec3 delta = nodes[j].position - nodes[i].position;
            float dist = glm::length(delta);
            float min_dist = 1.0f;
            
            if (dist < min_dist) {
                glm::vec3 normal = glm::normalize(delta);
                nodes[i].position -= normal * (min_dist - dist) * 0.5f;
                nodes[j].position += normal * (min_dist - dist) * 0.5f;
            }
        }
    }
}

void GraphLayout::compute_layout(int iterations) {
    for (int i = 0; i < iterations; i++) {
        apply_forces();
        update_positions(0.016f);
        detect_collisions();
    }
}

void GraphLayout::simulate_step(float dt) {
    apply_forces();
    update_positions(dt);
    detect_collisions();
}

void GraphLayout::update_from_agent_data(const char *json_data) {
    /* Parse agent telemetry and update graph */
    std::cout << "[GraphLayout] Processing agent data" << std::endl;
    
    /* Integration point with agent_orchestrator output */
}

std::string GraphLayout::export_to_json() const {
    std::stringstream ss;
    ss << "{\"nodes\":[";
    
    for (size_t i = 0; i < nodes.size(); i++) {
        if (i > 0) ss << ",";
        ss << "{\"id\":" << nodes[i].id 
           << ",\"label\":\"" << nodes[i].label << "\""
           << ",\"threat\":" << nodes[i].threat_level << "}";
    }
    
    ss << "],\"edges\":[";
    for (size_t i = 0; i < edges.size(); i++) {
        if (i > 0) ss << ",";
        ss << "{\"source\":" << edges[i].source 
           << ",\"target\":" << edges[i].target << "}";
    }
    ss << "]}";
    
    return ss.str();
}
