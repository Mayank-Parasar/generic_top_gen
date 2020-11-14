//
// Created by Mayank Parasar on 11/13/20.
//

#ifndef GENERIC_TOPO_GEN_NODE_H
#define GENERIC_TOPO_GEN_NODE_H
#include "Link.h"
#include <vector>

class Node {
public:
    Node(); // default ctor
    Node(int nodeId, Link* outgoingLink, Link* incomingLink);
    Node(int nodeId, std::vector<Link*> outgoingLinks,
         std::vector<Link*> incomingLinks);

    Node(int nodeId);

    int node_id;
    std::vector<Link*> outgoing_link;
    std::vector<Link*> incoming_link;
};

#endif //GENERIC_TOPO_GEN_NODE_H
