#include "graphedge.h"
#include "graphnode.h"

GraphNode::GraphNode(int id)
{
    _id = id;
}

GraphNode::~GraphNode()
{
    //// STUDENT CODE
    ////

    //delete _chatBot; 

    ////
    //// EOF STUDENT CODE
}

// Tokens are nothing but answers
void GraphNode::AddToken(std::string token)
{
    _answers.push_back(token);
}

// Adding edges to parent node from answersheet 
void GraphNode::AddEdgeToParentNode(GraphEdge* edge)
{
    _parentEdges.push_back(edge);
}

// Adding edges to child node from answersheet
void GraphNode::AddEdgeToChildNode(std::unique_ptr<GraphEdge> edge)
{
    _childEdges.push_back(std::move(edge));
}

//// STUDENT CODE
////
// Moving the chatbot from either root or any other node to this node 
void GraphNode::MoveChatbotHere(ChatBot chatbot)
{
    _chatBot = std::move(chatbot);
  	// Set this node as the current node
    _chatBot.SetCurrentNode(this);
}

// Move chatbot to a new node using MoveChatbotHere() function
void GraphNode::MoveChatbotToNewNode(GraphNode* newNode)
{
    newNode->MoveChatbotHere(std::move(_chatBot));
    // since chatbot is moved - coomenting this line since we are using std::move to pass the chatbot
    //_chatBot = nullptr; // invalidate pointer at source
}
////
//// EOF STUDENT CODE

GraphEdge* GraphNode::GetChildEdgeAtIndex(int index) {
    //// STUDENT CODE
    ////

    return _childEdges[index].get();

    ////
    //// EOF STUDENT CODE
}