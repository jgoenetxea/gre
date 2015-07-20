#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>

using namespace std;

class Node
{
public:
    Node();
    ~Node();

    virtual void draw();
    void addChild( Node* c );
    Node* getChild( const unsigned int id );

private:
    std::vector<Node*> m_children;
};

#endif
