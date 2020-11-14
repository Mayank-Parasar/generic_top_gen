//
// Created by Mayank Parasar on 11/13/20.
//

#include "Link.h"

// class-Link member definition
Link::Link() {
    m_link_id = -1;
    m_src_node = nullptr;
    m_dest_node = nullptr;
    return;
}

Link::Link(int linkId, Node *srcNode, Node *destNode,
           int mLinkLatency) {
    m_link_id = linkId;
    m_link_latency = mLinkLatency;
    m_src_node = srcNode;
    m_dest_node = destNode;
    return;
}