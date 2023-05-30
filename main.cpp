/*-------------------------------------------
Program 6: Graphs
Course: CS 141, Spring 2023, UIC
System: Visual Studio Code on Windows 11
Author: Jason Liang, starter code provided by CS 141 Professors
------------------------------------------- */

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

//declares and initializes the Node class
class Node{

    //public functions
    public:

        //default constructor for the class. takes in zero parameters. used for the head junk node of a linked list
        Node(){
            value = -99;
            deg = 0;
            next = nullptr;
        }

        //constructor for the class. takes in an integer parameter
        Node(int value){
            this->value = value; //initializes the data member value 
            this->next = nullptr;
        }

        int getValue() const {return this->value;} //returns the data member value
        int getDeg() const {return this->deg;}  //returns the data member deg
        Node* getNext() const {return this->next;} //returns the data member pointer next

        //sets the pointer to the next node
        void setNext(Node* next){
            this->next = next;
        }

        //increases the data member deg by 1
        void increaseDeg(){
            this->deg++;
        }

    //private data members
    private:
        int deg; //used to store the degree of a node in an edge list
        int value; //used to store which node the Node class object represents
        Node* next; //pointer to the next Node object in a linked list
};

/*this function inserts a node into a linked list in the ascending order of the data member value
takes in 3 parameters:
1. a vector of type Node* holding the heads of an adjency list
2. an integer indicating which head or linked list to insert into
3. a Node* pointer that is being inserted*/
void insert_in_order(vector<Node*>& heads_list, int head_pos, Node* new_node){

    //initializes pointers to be used in insertion
    Node* current = heads_list.at(head_pos);
    Node* temp;

    //checks if the head at the chosen is a null pointer
    if (!heads_list.at(head_pos)){
        Node* new_head = new Node(); //creates a head node 
        heads_list.at(head_pos) = new_head; //sets the chosen head to the newly created head
        current = heads_list.at(head_pos); //adjusts current
    }

    heads_list.at(head_pos)->increaseDeg(); //increases the the degree of the head node by 1 everytime a new node is added

    //for cases where a head node is created but points to no next node
    if (!current->getNext()){
        temp = current->getNext(); 
        current->setNext(new_node); //inserts the new node into the linked list
        new_node->setNext(temp);
        return; //returns from the function
    }

    //adjusts current to skip the head node
    current = current->getNext();

    //while loop that traverses the linked list
    while (current){
        
        //tries to insert if new_node's value is greater than current's
        if (new_node->getValue() > current->getValue()){

            //checks if the end of the linked list is reached
            if (current->getNext()){

                //insertion will only happen only if new_node's value is less than the next node's value
                if (new_node->getValue() < current->getNext()->getValue()){
                    temp = current->getNext();
                    current->setNext(new_node);
                    new_node->setNext(temp);
                    return; //returns from the function
                }
            }
            
            //inserts the new node if the end is reached
            else{
                temp = current->getNext();
                current->setNext(new_node);
                new_node->setNext(temp);
                return; //returns from the function
            }
        }
        current = current->getNext(); //moves on to next node in linked list
    }
    
    //for the cases where new_node's value will be smaller than every value of the linked list
    temp = heads_list.at(head_pos)->getNext();
    heads_list.at(head_pos)->setNext(new_node);
    new_node->setNext(temp);
}

/*fucntion checks if a node is already present in a linked list
takes in 3 parameters:
1. a vector of type Node* storing the heads of a adjency list
2. an integer indicating which head or linked list will be checked
3. an integer indictating the value of the node that is being checked for*/
bool already_present(vector<Node*>& heads_list, int head_pos, int new_node){
    
    Node* current = heads_list.at(head_pos); //initializes pointer for traversal

    //while loop to traverses the linked list
    while (current){
        if (current->getValue() == new_node){
            return true; //returns true if the data member value matches new_node
        }
        current = current->getNext(); //moves on to next node
    }

    return false; //returns false if no match occurs
}

/*function appends nullptr to a vector in order to create enough indices that can be accessed
takes in 2 parameters:
1. a vector of type Node* storing the heads of a adjency list
2. an integer indicating the index position the vector needs to reach*/
void increase_size(vector<Node*>& heads_list, const int target_idx){
    int num_heads = target_idx - heads_list.size() + 1;
    for (auto i = 0; i < num_heads; i++){
        heads_list.push_back(nullptr);
    }
    
}

/*function produces the adjaceny list as a text file
takes in 2 parameters:
1. a vector of type Node* storing the heads of a adjency list
2. an ofstream object that the adjency list is outputted to*/
int produce_adjaceny_file(vector<Node*>& heads_list, ofstream& destination){

    Node* current; //Node* pointer for traversal
    Node* temp; //Node* pointer for temporary storage

    int count = 0, node_nums = 0; //integer variables to keep count of the index position and the number of nodes

    //for loop to access each element in the heads_list vector
    for (Node* pointer : heads_list){

        //checks if the element is a nullptr
        if (!pointer){
            count++;
            continue; //skips the element
        }
        
        node_nums++; //increases the number of nodes counted by 1

        current = pointer->getNext(); //moves onto the next node in the linked list

        //checks if the current node is a valid node
        if (current){
            
            //outputs the linked list to 
            destination << count << ":";
            while (current){
                destination << " " << current->getValue();
                temp = current->getNext();
                delete current; //deletes current node to free memory
                current = temp;
            }
            destination << endl;

        }
        count++;
    }

    return node_nums; //returns the number of nodes

}

/*function acts as the driver function for the program
take in 2 parameters:
1. a string parameter indicating the file path being read from
2. a string parameter indicating the file path being written to*/
void run(const string& src_path, const string& dst_path){

    ifstream input{src_path}; //creates input file stream using src_path

    //initializes and declares variables to be used
    int head, node, edges = 0, max = -1;
    bool present;

    vector<Node*> heads_list = {nullptr}; //initializes a vector with a single nullptr element
                                          //each element will be the head junk node of a linked list

    //for loop that reads in each line of the file
    for (string line; getline(input, line);){

        stringstream line_input{line}; //creates a string stream for easy reading of each line

        line_input >> head >> node; //inputs each line as a head and node

        //checks if the position will be out of range
        if (head > heads_list.size() - 1){
            increase_size(heads_list, head); //increases vector size if so
        }

        present = already_present(heads_list, head, node); //checks if the node already exists in the adjaceny list

        //inserts the new node if not present
        if (!present){
            Node* to_insert = new Node(node); //creates new node
            insert_in_order(heads_list, head, to_insert); //inserts it using insert_in_order

            max = (heads_list.at(head)->getDeg() > max) ? heads_list.at(head)->getDeg() : max; //updates the maximum degree count as needed
            edges++; //increases the number of edges by 1

            //checks if the position will be out of range
            if (node > heads_list.size() - 1){
                increase_size(heads_list, node); //increases vector size if so
            }

            present = already_present(heads_list, node, head); //checks if the reverse node already exists

            //inserts if not present. same process as before
            if (!present){
                to_insert = new Node(head);
                insert_in_order(heads_list, node, to_insert);
                max = (heads_list.at(node)->getDeg() > max) ? heads_list.at(node)->getDeg() : max;
            }
        }
    }

    ofstream destination{dst_path}; //creates an output file stream using dst_path

    int node_nums = produce_adjaceny_file(heads_list, destination); //calls produce_adjaceny_file to produce a text file and get the number of nodes

    //outputs the statistics of the edge list
    cout << "Number of nodes: " << node_nums << endl
         << "Maximum degree: " << max << endl
         << "Number of edges: " << edges << endl;
}

int main(){

    run("youtube.txt", "data.txt");
    return 0;
}