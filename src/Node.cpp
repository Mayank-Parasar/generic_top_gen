//
// Created by Mayank Parasar on 11/13/20.
//

#include "Node.h"
using namespace  std;
// class-Node member definition
Node::Node(int nodeId) {
    node_id = nodeId;
    return;
}

Node::Node(int nodeId, Link* outgoingLink, Link* incomingLink) {
    node_id = nodeId;
    outgoing_link.push_back(outgoingLink);
    incoming_link.push_back(incomingLink);
    return;
}

Node::Node(int nodeId, vector<Link*> outgoingLinks,
           vector<Link*> incomingLinks) {
    node_id = nodeId;
    for(auto i : outgoingLinks)
        outgoing_link.push_back(i);
    for(auto i : incomingLinks)
        incoming_link.push_back(i);

    return;
}
