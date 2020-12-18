//
// Created by Mayank Parasar on 11/13/20.
//

#include "Topology.h"

using namespace  std;
// class-Topology member-functions definition
Topology::Topology(uint32_t mNumNodes, uint32_t mNumLinks,
                   vector<int> mBaseRing, bool mDebug,
                   Optimizer* mOptimizer)
        : m_num_nodes(mNumNodes), m_num_links(mNumLinks),
        m_base_ring(mBaseRing), m_debug(mDebug),
        m_optimizer(mOptimizer)
{
    // call the ctor of Nodes* and Link* class here
    /* Creating nodes */
    for (int node_id = 0; node_id < m_num_nodes; ++node_id) {
        Node* node_ = new Node(node_id);
        nodes.push_back(node_);
    }
    /* Creating Links */
    for (int link_id = 0; link_id < m_num_links; ++link_id) {
        Link* link_ = new Link(link_id, nullptr,
                               nullptr, 1);
        links.push_back(link_);
    }
    // start the process of generating a random topology and its associated
    // data structure here..
    create_ring(); // created a random ring

    create_topology(); // generated a random topology, based on the ring created
    return;
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
        Link* link_ = new Link(link_id, nullptr,
                               nullptr, 1);
        links.push_back(link_);
    }
    return;
};

// This creates the base: m_ring_topology
void
Topology::create_ring() {
    // traverse the m_base_ring vector and connect the nodes
    int link_id = 0; // start of link-ids
    for(int ii=0; ii < m_base_ring.size()-1; ii++) {
        nodes[m_base_ring[ii]]->outgoing_link.push_back(links[link_id]);
        // ii + 1 was the bug
        nodes[m_base_ring[ii + 1]]->incoming_link.push_back(links[link_id]);
        links[link_id]->m_src_node = nodes[m_base_ring[ii]];
        links[link_id]->m_dest_node = nodes[m_base_ring[ii + 1]];
        link_id++;
    }
    // make it a ring here
    nodes[m_base_ring[m_base_ring.size()-1]]->outgoing_link.\
                                            push_back(links[link_id]);
    nodes[m_base_ring[0]]->incoming_link.push_back(links[link_id]);
    links[link_id]->m_src_node = nodes[m_base_ring[m_base_ring.size() - 1]];
    links[link_id]->m_dest_node = nodes[m_base_ring[0]];

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
        assert(links[ii]->m_src_node != nullptr);
        assert(links[ii]->m_dest_node != nullptr);
        assert(links[ii]->m_src_node->node_id == m_base_ring[ii]);
    }
    for (ii = m_num_nodes; ii < m_num_links; ++ii) {
        // not connected yet
        assert(links[ii]->m_src_node == nullptr);
        assert(links[ii]->m_dest_node == nullptr);
    }

    for (int link_id = m_num_nodes; link_id < m_num_links; ++link_id) {
        /* Two different nodes at a time */
        int src_node_id, dest_node_id;
        do {
            src_node_id = rand()%m_num_nodes;
            dest_node_id = rand()%m_num_nodes;
        } while (/*if they are same node then repeat*/
                (src_node_id ==dest_node_id) ||
                /*if they are already connected then also repeat*/
                is_connected(nodes[src_node_id], nodes[dest_node_id]));
        /* Now we have different src and destination node for this link
         * connect it*/
        nodes[src_node_id]->outgoing_link.push_back(links[link_id]);
        nodes[dest_node_id]->incoming_link.push_back(links[link_id]);
        /*update the same in the link pointer as well*/
        links[link_id]->m_src_node = nodes[src_node_id];
        links[link_id]->m_dest_node = nodes[dest_node_id];
        if (m_debug) {
            cout << "link-" << link_id << " connects node(src): " << src_node_id
                 << " to node(dest): " << dest_node_id << endl;
        }
    }
    // Also generates the connectivity matrix here..
    // column is 'src'-node (sender) and row is 'dest'-node(receiver)
    for(int ii = 0; ii < m_num_nodes; ii++) {
        vector<int> v(m_num_nodes, 0); // all elements = 0
        for(auto k : nodes[ii]->outgoing_link)
            v[k->m_dest_node->node_id] = k->m_link_latency;
        m_connectivity_matrix.push_back(v);
    }

    // Generate hop matrix here as well
    for (int src_node_id_ = 0; src_node_id_ < m_num_nodes; ++src_node_id_) {
        populate_hop_matrix(src_node_id_);
    }
    // hop matrix has been generated for this topology
    return;
}

void
Topology::populate_hop_matrix(int source_node_id) {
    /*
     * function populates the hop-matrix
     * using Dijkstra's algorithm
     * by finding the shortest path
     */
    int dist[m_num_nodes]; // The output array.  dist[i] will hold the shortest
    // distance from src to i

    bool sptSet[m_num_nodes]; // sptSet[i] will be true if vertex i is included
    // in shortest path tree or shortest distance from src to i is finalized

    // Initialize all distances as INFINITE and stpSet[] as false
    for (int i = 0; i < m_num_nodes; i++)
        dist[i] = INT_MAX, sptSet[i] = false;

    // Distance of source vertex from itself is always 0
    dist[source_node_id] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < m_num_nodes - 1; count++) {
        // Pick the minimum distance vertex from the set of vertices not
        // yet processed. u is always equal to src in the first iteration.
        int u = minDistance(dist, sptSet);

        // Mark the picked vertex as processed
        sptSet[u] = true;

        // Update dist value of the adjacent vertices of the picked vertex.
        for (int v = 0; v < m_num_nodes; v++)

            // Update dist[v] only if is not in sptSet, there is an edge from
            // u to v, and total weight of path from src to  v through u is
            // smaller than current value of dist[v]
            if (!sptSet[v] && m_connectivity_matrix[u][v] && dist[u] != INT_MAX
                && dist[u] + m_connectivity_matrix[u][v] < dist[v])
                dist[v] = dist[u] + m_connectivity_matrix[u][v];
    }

    vector<int> vect(dist, dist+m_num_nodes);
    m_hop_matrix.push_back(vect);
    return;
}
int Topology::minDistance(int *dist, bool *sptSet) {
    // Initialize min value
    int min = INT_MAX, min_index;

    for (int v = 0; v < m_num_nodes; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
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
     * if m_connectivity_matrix is SCC)
     * */
    return false;
}

const vector<Node *> &Topology::getNodes() const {
    return nodes;
}

void Topology::setNodes(const vector<Node *> &nodes) {
    Topology::nodes = nodes;
}

bool Topology::is_connected(Node *src_node, Node *dest_node) {
    // for all the outgoing links from src-node;
    // check if dest-node has same link-id for incoming node
    for( auto i : src_node->outgoing_link) {
        for( auto k : dest_node->incoming_link) {
            if(i->m_link_id == k->m_link_id)
                return true;
        }
    }
    // if control comes here... return false (these nodes are not connected)
    return false;
}

void Topology::print_topology() {
    cout << "Connectivity Matrix:" << endl;
    for(auto i : m_connectivity_matrix) {
        for (auto k : i) {
            cout << k << "\t";
        }
        cout << endl;
    }
    cout << "Hop Matrix: " << endl;
    for (auto i : m_hop_matrix) {
        for (auto k : i) {
            cout << k << "\t";
        }
        cout << endl;
    }
//    cout << endl;
    return;
}

const vector<std::vector<int>> &Topology::getMHopMatrix() const {
    return m_hop_matrix;
}

Topology::Topology() {
    // default
}

Mesh::Mesh(uint32_t mNumNodes, uint32_t mNumLinks, const vector<int> &mBaseRing,
           bool mDebug) : Topology(mNumNodes, mNumLinks, mBaseRing, mDebug) {
    // not used
    assert(0 && "This is not the right constructor to call to create Mesh "
                "Object Mesh objects are created independently from "
                "TopologyUniverse class");

}

Mesh::Mesh(uint32_t mNumNodes, uint32_t mNumLinks, uint32_t mNumRows,
           uint32_t mNumCols) : Topology(mNumNodes,
                                         mNumLinks) {
    // not used
    assert(0 && "This is not the right constructor to call to create Mesh "
                "Object Mesh objects are created independently from "
                "TopologyUniverse class");
    m_rows = mNumRows;
    m_cols = mNumCols;
    // Nodes and links objects have been populated now...
    // Connect them in the form of 'Mesh' here
    // Then,
    // Generate the connectivity matrix here..
}

Mesh::Mesh(uint32_t mRows, uint32_t mCols) : m_rows(mRows), m_cols(mCols) {
    m_num_nodes = m_rows * m_cols; // Number of Nodes in Mesh Topology
    // Number of links in Mesh topology (uni-directional)
    m_num_links = 2* ((m_rows - 1) * m_cols + m_rows * (m_cols - 1));
    // call the ctor of Nodes* and Link* class here
    /* Creating nodes */
    for (int node_id = 0; node_id < m_num_nodes; ++node_id) {
        Node* node_ = new Node(node_id);
        nodes.push_back(node_);
    }
    /* Creating Links */
    for (int link_id = 0; link_id < m_num_links; ++link_id) {
        Link* link_ = new Link(link_id, nullptr,
                               nullptr, 1);
        links.push_back(link_);
    }

    create_mesh();
    // Generate connectivity Matrix here
    // column is 'src'-node (sender) and row is 'dest'-node(receiver)
    for(int ii = 0; ii < m_num_nodes; ii++) {
        vector<int> v(m_num_nodes, 0); // all elements = 0
        for(auto k : nodes[ii]->outgoing_link)
            v[k->m_dest_node->node_id] = k->m_link_latency;
        m_connectivity_matrix.push_back(v);
    }

    // Generate hop matrix here as well
    for (int src_node_id_ = 0; src_node_id_ < m_num_nodes; ++src_node_id_) {
        populate_hop_matrix(src_node_id_);
    }

    return;
}

void Mesh::create_mesh() {

    int link_id = 0;
    int src_node_id = -1;
    int dest_node_id = -1;

    // (East to West)-links
    for (int row_ = 0; row_ < m_rows; ++row_) {
        for (int col_ = 0; col_ < m_cols; ++col_) {
            if(col_ + 1 < m_cols) {
                src_node_id = col_ + (row_ * m_cols);
                dest_node_id = col_+ 1 + (row_ * m_cols);
                assert(links[link_id] != nullptr);
                nodes[src_node_id]->outgoing_link.push_back(links[link_id]);
                nodes[dest_node_id]->incoming_link.push_back(links[link_id]);
                /*update the same in the link pointer as well*/
                links[link_id]->m_src_node = nodes[src_node_id];
                links[link_id]->m_dest_node = nodes[dest_node_id];
                link_id++;
            }
        }
    }

    // (West to East)-links
    for (int row_ = 0; row_ < m_rows; ++row_) {
        for (int col_ = 0; col_ < m_cols; ++col_) {
            if(col_ + 1 < m_cols) {
                dest_node_id = col_ + (row_ * m_cols);
                src_node_id = (col_+ 1) + (row_ * m_cols);
                assert(links[link_id] != nullptr);
                nodes[src_node_id]->outgoing_link.push_back(links[link_id]);
                nodes[dest_node_id]->incoming_link.push_back(links[link_id]);
                /*update the same in the link pointer as well*/
                links[link_id]->m_src_node = nodes[src_node_id];
                links[link_id]->m_dest_node = nodes[dest_node_id];
                link_id++;
            }
        }
    }

    // (North to South)-links
    for (int col_ = 0; col_ < m_cols; ++col_) {
        for (int row_ = 0; row_ < m_rows; ++row_) {
            if (row_ + 1 < m_rows) {
                src_node_id = col_ + (row_ * m_cols);
                dest_node_id = col_ + ((row_+1) * m_cols);
                assert(links[link_id] != nullptr);
                nodes[src_node_id]->outgoing_link.push_back(links[link_id]);
                nodes[dest_node_id]->incoming_link.push_back(links[link_id]);
                /*update the same in the link pointer as well*/
                links[link_id]->m_src_node = nodes[src_node_id];
                links[link_id]->m_dest_node = nodes[dest_node_id];
                link_id++;
            }

        }
    }

    // (South to North)-links
    for (int col_ = 0; col_ < m_cols; ++col_) {
        for (int row_ = 0; row_ < m_rows; ++row_) {
            if (row_ + 1 < m_rows) {
                dest_node_id = col_ + (row_ * m_cols);
                src_node_id = col_ + ((row_+1) * m_cols);
                assert(links[link_id] != nullptr);
                nodes[src_node_id]->outgoing_link.push_back(links[link_id]);
                nodes[dest_node_id]->incoming_link.push_back(links[link_id]);
                /*update the same in the link pointer as well*/
                links[link_id]->m_src_node = nodes[src_node_id];
                links[link_id]->m_dest_node = nodes[dest_node_id];
                link_id++;
            }
        }
    }

    assert(link_id == (m_num_links));

    return;
}

Torus::Torus(uint32_t mRows, uint32_t mCols) : m_rows(mRows), m_cols(mCols) {
    m_num_nodes = m_rows * m_cols; // Number of Nodes in Torus Topology
    // Number of links in Torus topology (uni-directional)
    m_num_links = 2 * ((m_rows * m_cols) + (m_rows * m_cols));
    // call the ctor of Nodes* and Link* class here
    /* Creating nodes */
    for (int node_id = 0; node_id < m_num_nodes; ++node_id) {
        Node* node_ = new Node(node_id);
        nodes.push_back(node_);
    }
    /* Creating Links */
    for (int link_id = 0; link_id < m_num_links; ++link_id) {
        Link* link_ = new Link(link_id, nullptr,
                               nullptr, 1);
        links.push_back(link_);
    }

    create_torus();
    // Generate connectivity Matrix here
    // column is 'src'-node (sender) and row is 'dest'-node(receiver)
    for(int ii = 0; ii < m_num_nodes; ii++) {
        vector<int> v(m_num_nodes, 0); // all elements = 0
        for(auto k : nodes[ii]->outgoing_link)
            v[k->m_dest_node->node_id] = k->m_link_latency;
        m_connectivity_matrix.push_back(v);
    }

    // Generate hop matrix here as well
    for (int src_node_id_ = 0; src_node_id_ < m_num_nodes; ++src_node_id_) {
        populate_hop_matrix(src_node_id_);
    }

    return;
}

void Torus::create_torus() {

    int link_id = 0;
    int src_node_id = -1;
    int dest_node_id = -1;

    // (East to West)-links
    for (int row_ = 0; row_ < m_rows; ++row_) {
        for (int col_ = 0; col_ < m_cols; ++col_) {
            if(col_ + 1 < m_cols) {
                src_node_id = col_ + (row_ * m_cols);
                dest_node_id = col_+ 1 + (row_ * m_cols);
                assert(links[link_id] != nullptr);
                nodes[src_node_id]->outgoing_link.push_back(links[link_id]);
                nodes[dest_node_id]->incoming_link.push_back(links[link_id]);
                /*update the same in the link pointer as well*/
                links[link_id]->m_src_node = nodes[src_node_id];
                links[link_id]->m_dest_node = nodes[dest_node_id];
                link_id++;
            }
            // Add wrap-around link here
            if (col_ + 1 == m_cols) {
                src_node_id = col_ + (row_ * m_cols);
                dest_node_id = (row_ * m_cols);
                assert(links[link_id] != nullptr);
                nodes[src_node_id]->outgoing_link.push_back(links[link_id]);
                nodes[dest_node_id]->incoming_link.push_back(links[link_id]);
                /*update the same in the link pointer as well*/
                links[link_id]->m_src_node = nodes[src_node_id];
                links[link_id]->m_dest_node = nodes[dest_node_id];
                link_id++;
            }
        }
    }

    // (West to East)-links
    for (int row_ = 0; row_ < m_rows; ++row_) {
        for (int col_ = 0; col_ < m_cols; ++col_) {
            if(col_ + 1 < m_cols) {
                dest_node_id = col_ + (row_ * m_cols);
                src_node_id = (col_+ 1) + (row_ * m_cols);
                assert(links[link_id] != nullptr);
                nodes[src_node_id]->outgoing_link.push_back(links[link_id]);
                nodes[dest_node_id]->incoming_link.push_back(links[link_id]);
                /*update the same in the link pointer as well*/
                links[link_id]->m_src_node = nodes[src_node_id];
                links[link_id]->m_dest_node = nodes[dest_node_id];
                link_id++;
            }
            // Add wrap-around link here
            if (col_ + 1 == m_cols) {
                dest_node_id = col_ + (row_ * m_cols);
                src_node_id = (row_ * m_cols);
                assert(links[link_id] != nullptr);
                nodes[src_node_id]->outgoing_link.push_back(links[link_id]);
                nodes[dest_node_id]->incoming_link.push_back(links[link_id]);
                /*update the same in the link pointer as well*/
                links[link_id]->m_src_node = nodes[src_node_id];
                links[link_id]->m_dest_node = nodes[dest_node_id];
                link_id++;
            }
        }
    }

    // (North to South)-links
    for (int col_ = 0; col_ < m_cols; ++col_) {
        for (int row_ = 0; row_ < m_rows; ++row_) {
            if (row_ + 1 < m_rows) {
                src_node_id = col_ + (row_ * m_cols);
                dest_node_id = col_ + ((row_+1) * m_cols);
                assert(links[link_id] != nullptr);
                nodes[src_node_id]->outgoing_link.push_back(links[link_id]);
                nodes[dest_node_id]->incoming_link.push_back(links[link_id]);
                /*update the same in the link pointer as well*/
                links[link_id]->m_src_node = nodes[src_node_id];
                links[link_id]->m_dest_node = nodes[dest_node_id];
                link_id++;
            }
            // Add wrap around link here
            if (row_ + 1 == m_rows) {
                src_node_id = col_ + (row_ * m_cols);
                dest_node_id = col_;
                assert(links[link_id] != nullptr);
                nodes[src_node_id]->outgoing_link.push_back(links[link_id]);
                nodes[dest_node_id]->incoming_link.push_back(links[link_id]);
                /*update the same in the link pointer as well*/
                links[link_id]->m_src_node = nodes[src_node_id];
                links[link_id]->m_dest_node = nodes[dest_node_id];
                link_id++;
            }
        }
    }

    // (South to North)-links
    for (int col_ = 0; col_ < m_cols; ++col_) {
        for (int row_ = 0; row_ < m_rows; ++row_) {
            if (row_ + 1 < m_rows) {
                dest_node_id = col_ + (row_ * m_cols);
                src_node_id = col_ + ((row_+1) * m_cols);
                assert(links[link_id] != nullptr);
                nodes[src_node_id]->outgoing_link.push_back(links[link_id]);
                nodes[dest_node_id]->incoming_link.push_back(links[link_id]);
                /*update the same in the link pointer as well*/
                links[link_id]->m_src_node = nodes[src_node_id];
                links[link_id]->m_dest_node = nodes[dest_node_id];
                link_id++;
            }
            // Add wrap around link here
            if (row_ + 1 == m_rows) {
                dest_node_id = col_ + (row_ * m_cols);
                src_node_id = col_;
                assert(links[link_id] != nullptr);
                nodes[src_node_id]->outgoing_link.push_back(links[link_id]);
                nodes[dest_node_id]->incoming_link.push_back(links[link_id]);
                /*update the same in the link pointer as well*/
                links[link_id]->m_src_node = nodes[src_node_id];
                links[link_id]->m_dest_node = nodes[dest_node_id];
                link_id++;
            }
        }
    }

    assert(link_id == (m_num_links));

    return;
}

FlattenedButterfly::FlattenedButterfly(uint32_t mRows, uint32_t mCols)
        : m_rows(mRows), m_cols(mCols){
    m_num_nodes = m_rows * m_cols;
    // update the 'm_num_links' in the create_flattened_butterfly topology
    /* Creating nodes */
    for (int node_id = 0; node_id < m_num_nodes; ++node_id) {
        Node* node_ = new Node(node_id);
        nodes.push_back(node_);
    }

    this->create_flattened_butterfly();
    // Generate connectivity Matrix here
    // column is 'src'-node (sender) and row is 'dest'-node(receiver)
    for(int ii = 0; ii < m_num_nodes; ii++) {
        vector<int> v(m_num_nodes, 0); // all elements = 0
        for(auto k : nodes[ii]->outgoing_link)
            v[k->m_dest_node->node_id] = k->m_link_latency;
        m_connectivity_matrix.push_back(v);
    }

    // Generate hop matrix here as well
    for (int src_node_id_ = 0; src_node_id_ < m_num_nodes; ++src_node_id_) {
        populate_hop_matrix(src_node_id_);
    }

    return;
}

void FlattenedButterfly::create_flattened_butterfly() {
    int link_id = 0;
    int src_node_id = -1;
    int dest_node_id = -1;

    // W to E
    for (int row_ = 0; row_ < m_rows; ++row_) {
        for (int col_ = 0; col_ < m_cols; ++col_) {
            src_node_id = col_ + (row_ * m_cols);
            for (int dest_ = src_node_id+1; dest_ < (row_ * m_cols + m_cols);
            ++dest_) {
                dest_node_id = dest_;
                // create a new link
                // link is created with its src and destinations
                Link* link_ = new Link(link_id, nodes[src_node_id],
                                       nodes[dest_node_id], 1);
                links.push_back(link_);
                nodes[src_node_id]->outgoing_link.push_back(links[link_id]);
                nodes[dest_node_id]->incoming_link.push_back(links[link_id]);
                link_id++;
                 // cout << "connected src_node: " << src_node_id
                 // << " to dest_node: " << dest_node_id << endl;
            }
        }
    }

    // E to W
    for (int row_ = 0; row_ < m_rows; ++row_) {
        for (int col_ = 0; col_ < m_cols; ++col_) {
            dest_node_id = col_ + (row_ * m_cols);
            for (int dest_ = dest_node_id+1; dest_ < (row_ * m_cols + m_cols);
                 ++dest_) {
                src_node_id = dest_;
                // create a new link
                // link is created with its src and destinations
                Link* link_ = new Link(link_id, nodes[src_node_id],
                                       nodes[dest_node_id], 1);
                links.push_back(link_);
                nodes[src_node_id]->outgoing_link.push_back(links[link_id]);
                nodes[dest_node_id]->incoming_link.push_back(links[link_id]);
                link_id++;
                 // cout << "connected src_node: " << src_node_id
                 //      << " to dest_node: " << dest_node_id << endl;
            }
        }
    }

    for (int col_ = 0; col_ < m_cols; ++col_) {
        for (int row_ = 0; row_ < m_rows; ++row_) {
            src_node_id = col_ + (row_ * m_cols);
            int k = col_;
            while (k < src_node_id) {
                dest_node_id = k;
                k += m_cols;
                // create a new link
                // link is created with its src and destinations
                Link* link_ = new Link(link_id, nodes[src_node_id],
                                       nodes[dest_node_id], 1);
                links.push_back(link_);
                nodes[src_node_id]->outgoing_link.push_back(links[link_id]);
                nodes[dest_node_id]->incoming_link.push_back(links[link_id]);
                link_id++;
                 // cout << "connected src_node: " << src_node_id
                 //      << " to dest_node: " << dest_node_id << endl;
            }
        }
    }

    for (int col_ = 0; col_ < m_cols; ++col_) {
        for (int row_ = 0; row_ < m_rows; ++row_) {
            dest_node_id = col_ + (row_ * m_cols);
            int k = col_;
            while (k < dest_node_id) {
                src_node_id = k;
                k += m_cols;
                // create a new link
                // link is created with its src and destinations
                Link* link_ = new Link(link_id, nodes[src_node_id],
                                       nodes[dest_node_id], 1);
                links.push_back(link_);
                nodes[src_node_id]->outgoing_link.push_back(links[link_id]);
                nodes[dest_node_id]->incoming_link.push_back(links[link_id]);
                link_id++;
                 // cout << "connected src_node: " << src_node_id
                 //      << " to dest_node: " << dest_node_id << endl;
            }
        }
    }

    m_num_links = link_id;
    return;
}


