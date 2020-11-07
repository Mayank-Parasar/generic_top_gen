//
// Created by Mayank Parasar on 11/1/20.
//

#include "generic_topo_gen.h"

using namespace  std;

// class-TopologyUniverse member-functions definitions
TopologyUniverse::TopologyUniverse(uint32_t mNumNodes, uint32_t mNumLinks,
                                   uint32_t mNumTopology)
        : m_num_nodes(mNumNodes), m_num_links(mNumLinks),
          m_num_topology(mNumTopology) {
    vector<int> node_order;
    for(int i = 0; i < mNumNodes; i++) {
        node_order.push_back(i);
    }
    populate_unique_rings(node_order);
    // By now all the unique rings have been populated.
    // for 'm_num_topology' distribute one base-ring instance for each
    for (uint32_t mTopology = 0; mTopology < m_num_topology; ++mTopology) {
        Topology *topology_ = new Topology(m_num_nodes, m_num_links,
                                           m_unique_rings[mTopology]);
        m_topologies.push_back(topology_);
    }

}

void
TopologyUniverse::populate_unique_rings(std::vector<int> node_order) {
    srand((unsigned) time(0));
    int sizeOfVector = node_order.size();
    // shuffle
    shuffle:
    /* Fisher-Yates shuffle algorithm */
    for (int k = 0; k < sizeOfVector; k++) {
        int r = k + rand() % (sizeOfVector - k); // careful here!
        swap(node_order[k], node_order[r]);
    }
    // check
    for(int i = 0; i < node_order.size(); i++) {
        rotate(node_order.begin(), node_order.begin()+1, node_order.end());
        for(auto i : m_unique_rings)
            if (i == node_order)
                goto shuffle;
    }
    //store
    if(m_unique_rings.size() < m_num_topology) {
        m_unique_rings.push_back(node_order);
        goto shuffle;
    }
    else
        return;
}


// class-Topology member-functions definition
Topology::Topology(uint32_t mNumNodes, uint32_t mNumLinks,
                   vector<int> mBaseRing)
        : m_num_nodes(mNumNodes), m_num_links(mNumLinks), m_base_ring(mBaseRing)
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

Topology::Topology(uint32_t mNumNodes, uint32_t mNumLinks)
: m_num_nodes(mNumNodes), m_num_links(mNumLinks)
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

// This creates the base: m_ring_topology
void
Topology::create_ring() {
    // traverse the m_base_ring vector and connect the nodes
    int link_id = 0; // start of link-ids
    for(int ii=0; ii < m_base_ring.size()-1; ii++) {
        nodes[m_base_ring[ii]]->outgoing_link.push_back(links[link_id]);
        nodes[m_base_ring[ii]]->incoming_link.push_back(links[link_id]);
        links[link_id]->src_node = nodes[m_base_ring[ii]];
        links[link_id]->dest_node = nodes[m_base_ring[ii+1]];
        link_id++;
    }
    // make it a ring here
    nodes[m_base_ring[m_base_ring.size()-1]]->outgoing_link.\
                                            push_back(links[link_id]);
    nodes[m_base_ring[0]]->incoming_link.push_back(links[link_id]);
    links[link_id]->src_node = nodes[m_base_ring[m_base_ring.size()-1]];
    links[link_id]->dest_node = nodes[m_base_ring[0]];

    return;
}

// works on the base_ring with adding remaining links randomly
// NOTE: no ling should be added between already connected nodes
// put an assert for that...
void
Topology::create_topology() {
    int ii;
    for(ii=0; ii < m_num_nodes; ii++) {
        // connected to form the ring
        assert(links[ii]->src_node != nullptr);
        assert(links[ii]->dest_node != nullptr);
    }
    for (ii = m_num_nodes; ii < m_num_links; ++ii) {
        // not connected yet
        assert(links[ii]->src_node == nullptr);
        assert(links[ii]->dest_node == nullptr);
    }


    // Also generates the connectivity matrix here..

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
    // srand((unsigned) time(0));
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


