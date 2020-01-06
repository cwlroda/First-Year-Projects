#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iterator>

struct hufftreenode {
	char character;
	int frequency;
	hufftreenode* left;
	hufftreenode* right;
};

typedef hufftreenode* hufftreeptr;

//utility function
bool valid_hufftree(const hufftreeptr hufftree);
 // returns true if the input hufftree is a valid Huffman tree
 // i.e. all the terminal nodes (leaves) have characters,
 // all the non-leaf nodes have two sub-trees each,
 // and the occurrence frequency of a non-leaf node equals
 // the sum of the occurrence frequencies of its two sub-trees.

std::string huffencode(const std::string& message, std::map<char, int>& freqtable, hufftreeptr& hufftree, std::map<char, std::string>& hufftable);
// encodes an input message (a string of characters) into an encoded message (string of bits) using the Huffman coding
// builds the Huffman tree and its associated encoding table (i.e. the association between the characters in the tree leaves and their encodings (string of bits))

std::string huffdecode(const std::string& encodedmessage, const hufftreeptr& hufftree);
// decodes an encoded message (a string of bits) into the original message (a string of characters) using the Huffman tree built during the encoding

void build_freqtable(const std::string& message, std::map<char, int>& freqtable);
void build_tree(const std::string& message, std::map<char, int>& freqtable, hufftreeptr& hufftree, std::map<char, std::string>& hufftable);
void build_hufftable(hufftreeptr& hufftree, std::string& bit, std::map<char, std::string>& hufftable);
void traverse_tree(hufftreeptr& hufftree, bool& check);

int main() {
	std::string message, encodedmessage, decodedmessage;
	hufftreeptr hufftree;
	std::map<char, int> freqtable;
	std::map<char, std::string> hufftable;

    std::cout << "Enter a message:" << std::endl;
	std::getline(std::cin, message);
    std::cout << std::endl;

	// 1) encoding (compression)
    std::cout << "encoding OK" << std::endl << std::endl;
	encodedmessage = huffencode(message, freqtable, hufftree, hufftable);
	std::cout << encodedmessage << std::endl << std::endl;

    if(valid_hufftree(hufftree))  {
		std::cout << "valid Huffman tree." << std::endl;
	}
	else {
		std::cout << "not valid Huffman tree." << std::endl;
        return 0;
	}

	// 2) decoding (uncompression)
	decodedmessage = huffdecode(encodedmessage, hufftree);

	if(decodedmessage == message) {
		std::cout << "decoding OK." << std::endl << std::endl;
	}
	else {
		std::cout << "decoding not OK." << std::endl << std::endl;
	}

    std::cout << decodedmessage << std::endl << std::endl;

	return 0;
}

bool valid_hufftree(const hufftreeptr hufftree){
    bool check = true;
    hufftreeptr tmp = hufftree;
    traverse_tree(tmp, check);

    if(!check){
        return false;
    }

    if(hufftree == NULL){
        return false;
    }

    return true;
}

std::string huffencode(const std::string& message, std::map<char, int>& freqtable, hufftreeptr& hufftree, std::map<char, std::string>& hufftable){
    std::string code, bit;
    std::map<char, std::string>::iterator itr;
    std::map<char, int>::iterator itrs;

    build_freqtable(message, freqtable);

    if(freqtable.size() == 1){
        hufftree = new hufftreenode;
        hufftree->character = freqtable.begin()->first;
        hufftree->frequency = freqtable.begin()->second;
        hufftree->left = NULL;
        hufftree->right = NULL;

        for(int i = 0; i < message.size(); i++){
            code += "1";
        }

        return code;
    }

    build_tree(message, freqtable, hufftree, hufftable);

    if(!valid_hufftree(hufftree)){
        return NULL;
    }

    build_hufftable(hufftree, bit, hufftable);

    for(int i = 0; i < message.size(); i++){
        for(itr = hufftable.begin(); itr != hufftable.end(); itr++){
            if(itr->first == message[i]){
                code += itr->second;
            }
        }
    }

    return code;
}

std::string huffdecode(const std::string& encoded, const hufftreeptr& hufftree){
    std::string decoded;
    hufftreeptr tmp = hufftree;

    if(tmp->character != '\0'){
        for(int i = 0; i < encoded.size(); i++){
            decoded += tmp->character;
        }

        return decoded;
    }

    for(int i = 0; i < encoded.size(); i++){
        if(encoded[i] == '0'){
            tmp = tmp->left;
        }

        else{
            tmp = tmp->right;
        }

        if(tmp->left == NULL && tmp->right == NULL){
            decoded += tmp->character;
            tmp = hufftree;
        }
    }

    return decoded;
}

void build_freqtable(const std::string& message, std::map<char, int>& freqtable){
    std::map<char, int>::iterator itr;
    freqtable[message[0]] = 1;

    for(int i = 1; i < message.length(); i++){
        bool add = false;

        for(itr = freqtable.begin(); itr != freqtable.end(); itr++){
            if(itr->first == message[i]){
                itr->second++;
                add = true;
            }
        }

        if(!add){
            freqtable[message[i]] = 1;
        }
    }
}

void build_tree(const std::string& message, std::map<char, int>& freqtable, hufftreeptr& hufftree, std::map<char, std::string>& hufftable){
    std::vector<hufftreeptr> sorted;
    std::map<char, int> tmp = freqtable;
    std::map<char, int>::iterator itr;
    int size = tmp.size();

    while(sorted.size() != size){
        std::map<char, int>::iterator itmp = tmp.begin();

        for(itr = tmp.begin(); itr != tmp.end(); itr++){
            if(itr->second > itmp->second){
                itmp = itr;
            }
        }

        hufftreeptr node = new hufftreenode;
        node->character = itmp->first;
        node->frequency = itmp->second;
        node->left = NULL;
        node->right = NULL;

        sorted.push_back(node);
        tmp.erase(itmp);
    }

    while(sorted.size() > 1){
        hufftreeptr childA = sorted[sorted.size()-1];
        int index = sorted.size()-1;

        for(int i = sorted.size()-1; i >= 0; i--){
            if(sorted[i]->frequency < childA->frequency){
                childA = sorted[i];
                index = i;
            }
        }

        sorted.erase(sorted.begin()+index);

        hufftreeptr childB = sorted[sorted.size()-1];
        index = sorted.size()-1;

        for(int i = sorted.size()-1; i >= 0; i--){
            if(sorted[i]->frequency < childB->frequency){
                childB = sorted[i];
                index = i;
            }
        }

        sorted.erase(sorted.begin()+index);

        hufftreeptr parent = new hufftreenode;
        parent->character = '\0';
        parent->frequency = childA->frequency + childB->frequency;
        parent->left = childB;
        parent->right = childA;

        sorted.insert(sorted.begin()+index, parent);

        for(int i = sorted.size()-1; i >= 0; i--){
            for(int j = i-1; j >= 0; j--){
                if(sorted[i]->frequency > sorted[j]->frequency){
                    hufftreeptr swap = sorted[i];
                    sorted[i] = sorted[j];
                    sorted[j] = swap;
                    i = j+1;
                    break;
                }
            }
        }
    }

    hufftree = sorted[0];
}

void build_hufftable(hufftreeptr& hufftree, std::string& bit, std::map<char, std::string>& hufftable){
    if(hufftree != NULL){
        bit += "0";
        build_hufftable(hufftree->left, bit, hufftable);

        if(hufftree->left == NULL && hufftree->right == NULL){
            hufftable[hufftree->character] = bit;
        }

        bit += "1";
        build_hufftable(hufftree->right, bit, hufftable);
    }

    bit.pop_back();
}

void traverse_tree(hufftreeptr& hufftree, bool& check){
    if(hufftree != NULL){
        traverse_tree(hufftree->left, check);

        if(hufftree->left == NULL && hufftree->right == NULL){
            if(hufftree->character == '\0'){
                check = false;
            }
        }

        else if(hufftree->character == '\0'){
            if(hufftree->left == NULL || hufftree->right == NULL){
                check = false;
            }

            if(hufftree->frequency != hufftree->left->frequency + hufftree->right->frequency){
                check = false;
            }
        }

        traverse_tree(hufftree->right, check);
    }
}