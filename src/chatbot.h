#ifndef CHATBOT_H_
#define CHATBOT_H_

#include <wx/bitmap.h>
#include <string>
#include <iostream>

class GraphNode; // forward declaration
class ChatLogic; // forward declaration

class ChatBot
{
private:
    // data handles (owned)
    wxBitmap *_image; // avatar image

    // data handles (not owned)
    GraphNode *_currentNode;
    GraphNode *_rootNode;
    ChatLogic *_chatLogic;

    // proprietary functions
    int ComputeLevenshteinDistance(std::string s1, std::string s2);

public:
    // constructors / destructors
    ChatBot();                     // constructor WITHOUT memory allocation
    ChatBot(std::string filename); // constructor WITH memory allocation
    ~ChatBot();

    //// STUDENT CODE
    ////
    /* Copy Constructor */
    ChatBot(const ChatBot& obj);
    
    /* Copy Assisgnment operator */
    ChatBot& operator=(const ChatBot& obj) {
    	std::cout << "Chatbot:Performing deep copy assignment operator" << std::endl;
        
        if (this == &obj) 
        	return *this;
            
        _rootNode = obj._rootNode;
        _chatLogic = obj._chatLogic;
            
        delete _image;
        
        _image = new wxBitmap(*(obj._image));  
        _image = obj._image;
        
        return *this;       
    }
    
    /* Move Constructor */
    ChatBot(ChatBot&& obj);
    
     /* Move Assignment operator */
	ChatBot& operator=(ChatBot&& obj) {
    	std::cout << "Chatbot:Move assignment operator" << std::endl;
        
        if (this == &obj) 
        	return *this;
            
        _rootNode = obj._rootNode;
        _chatLogic = obj._chatLogic;
            
        delete _image;
        
    	_image = obj._image;
        
    	obj._image = nullptr;
        _rootNode = nullptr;
        _chatLogic = nullptr;
        
        return *this; 
	}

    ////
    //// EOF STUDENT CODE

    // getters / setters
    void SetCurrentNode(GraphNode *node);
    void SetRootNode(GraphNode *rootNode) { _rootNode = rootNode; }
    void SetChatLogicHandle(ChatLogic *chatLogic) { _chatLogic = chatLogic; }
    wxBitmap *GetImageHandle() { return _image; }

    // communication
    void ReceiveMessageFromUser(std::string message);
};

#endif /* CHATBOT_H_ */