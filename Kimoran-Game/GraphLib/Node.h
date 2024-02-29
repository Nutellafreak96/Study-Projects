#pragma once
#include <string>
#include <list>


// forward-deklaration
class Edge;


class Node
{
public:


	Node();

	Node(std::string id) : m_id(id) { s_numInstances++; }
	
	virtual ~Node() { }
	
	std::string getId() const { return m_id; }

    std::list<Edge*>& getOutEdges() { return m_outgoingEdges; }
    std::list<Edge*>& getInEdges()  { return m_incomingEdges; }


private:

	std::string m_id;

    std::list<Edge*> m_outgoingEdges;
    std::list<Edge*> m_incomingEdges;

	static int s_numInstances;

};
