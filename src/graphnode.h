#ifndef GRAPHNODE_H_
#define GRAPHNODE_H_

#include <vector>
#include <string>
#include "chatbot.h"
#include <memory>


// forward declarations
class GraphEdge;

class GraphNode
{
private:
    //// STUDENT CODE
    ////

    // data handles (owned) --  these are outgoing edges
    std::vector<std::unique_ptr<GraphEdge>> _childEdges;  // edges to subsequent nodes

    // data handles (not owned) -- these are the incoming edges
    std::vector<GraphEdge*> _parentEdges; // edges to preceding nodes 
    ChatBot _chatBot;

    ////
    //// EOF STUDENT CODE

    // proprietary members
    int _id;
    std::vector<std::string> _answers;

public:
    // constructor / destructor
    GraphNode(int id);
    ~GraphNode();

    // getter / setter
    int GetID() { return _id; }
    int GetNumberOfChildEdges() { return _childEdges.size(); }
    GraphEdge* GetChildEdgeAtIndex(int index);
    std::vector<std::string> GetAnswers() { return _answers; }
    int GetNumberOfParents() { return _parentEdges.size(); }

    // proprietary functions

    ////
    //// STUDENT CODE    
    void AddToken(std::string token); // add answers to list
    void AddEdgeToParentNode(GraphEdge* edge);
    void AddEdgeToChildNode(std::unique_ptr<GraphEdge> edge);
    ////
    //// EOF STUDENT CODE


    //// STUDENT CODE
    ////

    void MoveChatbotHere(ChatBot chatbot);

    ////
    //// EOF STUDENT CODE

    void MoveChatbotToNewNode(GraphNode* newNode);
};

#endif /* GRAPHNODE_H_ */