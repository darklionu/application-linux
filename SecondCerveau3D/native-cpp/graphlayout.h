#ifndef GRAPHLAYOUT_H
#define GRAPHLAYOUT_H

#include <vector>
#include <string>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Node {
    int id;
    std::string label;
    glm::vec3 position;
    glm::vec3 velocity;
    float mass;
    int threat_level;
    std::string node_type; // "agent", "file", "process", "network"
};

struct Edge {
    int source;
    int target;
    float strength;
    std::string connection_type; // "communication", "dependency", "threat"
    float weight;
};

class GraphLayout {
private:
    std::vector<Node> nodes;
    std::vector<Edge> edges;
    std::map<int, Node*> node_map;
    
    float damping;
    float stiffness;
    float repulsion;
    int max_iterations;
    
    void apply_forces();
    void update_positions(float dt);
    void detect_collisions();
    
public:
    GraphLayout();
    ~GraphLayout();
    
    /* Node operations */
    int add_node(const std::string &label, const std::string &type, int threat = 0);
    void remove_node(int node_id);
    Node* get_node(int node_id);
    
    /* Edge operations */
    void add_edge(int source, int target, float strength, const std::string &type);
    void remove_edge(int source, int target);
    
    /* Layout computation */
    void compute_layout(int iterations = 100);
    void simulate_step(float dt = 0.016f);
    
    /* Getters */
    const std::vector<Node>& get_nodes() const { return nodes; }
    const std::vector<Edge>& get_edges() const { return edges; }
    
    /* Integration with agents */
    void update_from_agent_data(const char *json_data);
    std::string export_to_json() const;
    
    /* 3D visualization parameters */
    void set_damping(float d) { damping = d; }
    void set_stiffness(float s) { stiffness = s; }
    void set_repulsion(float r) { repulsion = r; }
};

#endif
