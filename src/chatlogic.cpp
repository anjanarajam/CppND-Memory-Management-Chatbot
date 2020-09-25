#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <iterator>
#include <tuple>
#include <algorithm>

#include "graphedge.h"
#include "graphnode.h"
#include "chatbot.h"
#include "chatlogic.h"


ChatLogic::ChatLogic()
{
    //// STUDENT CODE
    ////
/* An instance is already created in header file */
/* 
    // create instance of chatbot
    _chatBot = new ChatBot("../images/chatbot.png");
    // add pointer to chatlogic so that chatbot answers can be passed on to the GUI
    _chatBot->SetChatLogicHandle(this);
*/

    ////
    //// EOF STUDENT CODE
}

ChatLogic::~ChatLogic()
{
    //// STUDENT CODE
    ////

/* No chatbot instance is created in constructor. Nodes are an unique pointer. And instance of 
edge is not required since its craeted in a loop while passing it to  node. Therefore deletion of chatbot, node and 
edge is not required in deatructor */
/*
    delete _chatBot;

    for (auto it = std::begin(_nodes); it != std::end(_nodes); ++it)
    {
        delete *it;
    }


    for (auto it = std::begin(_edges); it != std::end(_edges); ++it)
    {
        delete *it;
    }
*/
    ////
    //// EOF STUDENT CODE
}

template <typename T>
void ChatLogic::AddAllTokensToElement(std::string tokenID, tokenlist &tokens, T &element)
{
    // find all occurences for current node
    auto token = tokens.begin();
    while (true)
    {
        token = std::find_if(token, tokens.end(), [&tokenID](const std::pair<std::string, std::string> &pair) { return pair.first == tokenID;; });
        if (token != tokens.end())
        {
            element.AddToken(token->second); // add new keyword to edge
            token++;                         // increment iterator to next element
        }
        else
        {
            break; // quit infinite while-loop
        }
    }
}

void ChatLogic::LoadAnswerGraphFromFile(std::string filename)
{
    // load file with answer graph elements
    std::ifstream file(filename);

    // check for file availability and process it line by line
    if (file)
    {
        // loop over all lines in the file
        std::string lineStr;
        while (getline(file, lineStr))
        {
            // extract all tokens from current line
            tokenlist tokens;
            while (lineStr.size() > 0)
            {
                // extract next token
                int posTokenFront = lineStr.find("<");
                int posTokenBack = lineStr.find(">");
                if (posTokenFront < 0 || posTokenBack < 0)
                    break; // quit loop if no complete token has been found
                std::string tokenStr = lineStr.substr(posTokenFront + 1, posTokenBack - 1);

                // extract token type and info
                int posTokenInfo = tokenStr.find(":");
                if (posTokenInfo != std::string::npos)
                {
                    std::string tokenType = tokenStr.substr(0, posTokenInfo);
                    std::string tokenInfo = tokenStr.substr(posTokenInfo + 1, tokenStr.size() - 1);

                    // add token to vector
                    tokens.push_back(std::make_pair(tokenType, tokenInfo));
                }

                // remove token from current line
                lineStr = lineStr.substr(posTokenBack + 1, lineStr.size());
            }

            // process tokens for current line
            auto type = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "TYPE"; });
            if (type != tokens.end())
            {
                // check for id
                auto idToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "ID"; });
                if (idToken != tokens.end())
                {
                    // extract id from token
                    int id = std::stoi(idToken->second);

                    // node-based processing
                    if (type->second == "NODE")
                    {
                        //// STUDENT CODE
                        ////

                        // check if node with this ID exists already
                        auto newNode = std::find_if(_nodes.begin(), _nodes.end(), [&id](std::unique_ptr<GraphNode> &node) { return node->GetID() == id; });

                        // create new element if ID does not yet exist
                        if (newNode == _nodes.end())
                        {
                         	std::unique_ptr<GraphNode> new_node = std::make_unique<GraphNode>(id);
                            _nodes.emplace_back(std::move(new_node));
                            newNode = _nodes.end() - 1; // get iterator to last element

                            // add all answers to current node
                            // newNode is an iterator, therefore *newNode points to pointer node, *newNode pointer points 
                            // to value pointed by pointer node which is of type GraphNode
                            AddAllTokensToElement("ANSWER", tokens, **newNode);
                        }

                        ////
                        //// EOF STUDENT CODE
                    }

                    // edge-based processing
                    if (type->second == "EDGE")
                    {
                        //// STUDENT CODE
                        ////

                        // find tokens for incoming (parent) and outgoing (child) node
                        auto parentToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "PARENT"; });
                        auto childToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "CHILD"; });

                        if (parentToken != tokens.end() && childToken != tokens.end())
                        {
                            // get iterator on incoming and outgoing node via ID search
                            // when reference to unique pointer is used, an alias of the passed pointer variable is created, which prevents creating a local copy of the 
                            // pointer variable, so that its value changes outside the scope of function like SetChildNode and SetParentNode. Thus it does not transfer the 
                            // ownership to the other classes. And unique pointer does not allow copy either.
                            auto parentNode = std::find_if(_nodes.begin(), _nodes.end(), [&parentToken](const std::unique_ptr<GraphNode> &node) { return node->GetID() == 		std::stoi(parentToken->second); });
                            auto childNode = std::find_if(_nodes.begin(), _nodes.end(), [&childToken](const std::unique_ptr<GraphNode> &node) { return node->GetID() == std::stoi(childToken->second); });

                            // create new edge
                          	
                            std::unique_ptr<GraphEdge> edge = std::make_unique<GraphEdge>(id);
                            edge->SetChildNode(childNode->get());
                            edge->SetParentNode(parentNode->get());
                            // not required as no object of edge is created
                            //_edges.push_back(edge.get());

                            // find all keywords for current node
                            AddAllTokensToElement("KEYWORD", tokens, *edge);

                            // store reference in child node and parent node
                            (*childNode)->AddEdgeToParentNode(edge.get());
                            (*parentNode)->AddEdgeToChildNode(std::move(edge));
                        }
                      
                        ////
                        //// EOF STUDENT CODE
                    }
                }
                else
                {
                    std::cout << "Error: ID missing. Line is ignored!" << std::endl;
                }
            }
        } // eof loop over all lines in the file

        file.close();

    } // eof check for file availability
    else
    {
        std::cout << "File could not be opened!" << std::endl;
        return;
    }

    //// STUDENT CODE
    ////

    // identify root node
     GraphNode *rootNode = nullptr;
  
    for (auto it = std::begin(_nodes); it != std::end(_nodes); ++it)
    {
        // search for nodes which have no incoming edges
        if ((*it)->GetNumberOfParents() == 0)
        {

            if (rootNode == nullptr)
            {
                rootNode = it->get(); 
            }
            else
            {
                std::cout << "ERROR : Multiple root nodes detected" << std::endl;
            }
        }
    }

    /* Create a local member of chatbot */
    ChatBot* chatBot =  new ChatBot("../images/chatbot.png");
    /* Pass chat bot to chat logic */
    SetChatbotHandle(chatBot); 
    /* Pass chatbot to chatlogic so that it can display messages in GUI */
    chatBot->SetChatLogicHandle(this);
    /* |Set the root node in chatbot */
    chatBot->SetRootNode(rootNode);
  	/* Move chatbot to the rootnode */
    rootNode->MoveChatbotHere(std::move(chatBot));
    
    ////
    //// EOF STUDENT CODE
}

void ChatLogic::SetPanelDialogHandle(ChatBotPanelDialog *panelDialog)
{
    _panelDialog = panelDialog;
}

void ChatLogic::SetChatbotHandle(ChatBot *chatbot)
{
    _chatBot = chatbot;
}

void ChatLogic::SendMessageToChatbot(std::string message)
{
    _chatBot->ReceiveMessageFromUser(message);
}

void ChatLogic::SendMessageToUser(std::string message)
{
    _panelDialog->PrintChatbotResponse(message);
}

wxBitmap *ChatLogic::GetImageFromChatbot()
{
    return _chatBot->GetImageHandle();
}
