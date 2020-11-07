//
// Created by Mayank Parasar on 11/1/20.
//

#include "generic_topo_gen.h"

using namespace  std;

// class-TopologyUniverse member-functions definitions
TopologyUniverse::TopologyUniverse(uint32_t mNumNodes, uint32_t mNumLinks,
                                   uint32_t mNumTopology,
                                   const vector<Topology *> &topologies)
        : m_num_nodes(mNumNodes), m_num_links(mNumLinks),
          m_num_topology(mNumTopology), topologies(topologies) {

}


// class-Topology member-functions definition
Topology::Topology(uint32_t num_nodes, uint32_t num_links)
: m_num_nodes(num_nodes), m_num_links(num_links)
{
    // call the ctor of Nodes* and Link* class here
    /* Creating nodes */
    for (int node_id = 0; node_id < m_num_nodes; ++node_id) {
        Node* node_ = new Node(node_id);
        nodes.push_back(node_);
    }
    /* Creating Links */
    for (int link_id = 0; link_id < m_num_links; ++link_id) {
        Link* link_ = new Link(link_id, nullptr, nullptr);
        links.push_back(link_);
    }
};

void
Topology::create_topology() {
//    cout << "This is the template of the function: create_topology()"<< endl;
//    cout << "Num-nodes: " << get_num_nodes() << endl;
//    cout << "Num-links: " << get_num_links() << endl;

    /*
     * Algorithm:
     * From the pool of free links, take one link
     * and randomly connect it to any src-node and
     * then to any different dest-node.
     *
     * Keep doing it until all the links are exhausted.
     * Then call the function: Topology::is_strongly_connected()
     * to determine, if the topology is valid. If yes the output
     * this topology otherwise repeat this function from scratch
     * again creating a new topology.
     * */
    srand((unsigned) time(0));
    for (int link_id = 0; link_id < m_num_links; ++link_id) {
        /* Two different nodes at a time */
        int src_node_id, dest_node_id;
        do {
            src_node_id = rand()%m_num_nodes;
            dest_node_id = rand()%m_num_nodes;
        } while (src_node_id == dest_node_id);
        /* Now we have different src and destination node for this link
         * connect it*/
        nodes[src_node_id]->outgoing_link.push_back(links[link_id]);
        nodes[dest_node_id]->incoming_link.push_back(links[link_id]);
        /*update the same in the link pointer as well*/
        links[link_id]->src_node = nodes[src_node_id];
        links[link_id]->dest_node = nodes[dest_node_id];
        cout << "link-"<< link_id << " connects node(src): " << src_node_id
            << " to node(dest): " << dest_node_id << endl;
    }
    /* Here we should have a random topology: check if it is a valid one? */
    return;
}

void
Topology::set_params(int nodes, int links) {
    this->m_num_nodes = nodes;
    this->m_num_links = links;
    return;
}

bool
Topology::is_strongly_connected(Topology * t) {
    cout << "This function checks if the given topology "
            "generated is strongly connected" << endl;
    /*Algorithm:
     * Starting with the nod_id-0, start traversing the
     * topology using link connections. If we comeback
     * to the node-0 after traversing all the links then
     * topology is strongly connected..
     * (Use a standard algorithm for finding
     * if graph is SCC)
     * */
    return false;
}

void
Topology::populate_hop_matrix() {
    /*
     * function populates the hop-matrix
     * using Dijkstra's algorithm
     * by finding the shortest path
     */

    return;
}

const vector<Node *> &Topology::getNodes() const {
    return nodes;
}

void Topology::setNodes(const vector<Node *> &nodes) {
    Topology::nodes = nodes;
}

void Topology::create_rings(std::vector<Node *> nodes_, int start, int end) {

}

// class-Node member definition
Node::Node(int nodeId) {
    node_id = nodeId;
//    outgoing_link = nullptr;
//    incoming_link = nullptr;
}

Node::Node(int nodeId, Link* outgoingLink, Link* incomingLink) {
    node_id = nodeId;
    outgoing_link.push_back(outgoingLink);
    incoming_link.push_back(incomingLink);
}

Node::Node(int nodeId, vector<Link*> outgoingLinks,
           vector<Link*> incomingLinks) {
    node_id = nodeId;
    for(auto i : outgoingLinks)
        outgoing_link.push_back(i);
    for(auto i : incomingLinks)
        incoming_link.push_back(i);
}

// class-Link member definition
Link::Link() {
    link_id = -1;
    src_node = nullptr;
    dest_node = nullptr;
}

Link::Link(int linkId, Node* srcNode, Node* destNode) {
    link_id = linkId;
    src_node = srcNode;
    dest_node = destNode;
}


