//
// Created by Mayank Parasar on 11/13/20.
//

#ifndef GENERIC_TOPO_GEN_LINK_H
#define GENERIC_TOPO_GEN_LINK_H

class Node;
class Link {
public:
    Link(); // default ctor
    Link(int linkId, Node *srcNode, Node *destNode, int mLinkLatency = 1);
    int m_link_id;
    int m_link_latency;
    Node* m_src_node;
    Node* m_dest_node;
};
#endif //GENERIC_TOPO_GEN_LINK_H
