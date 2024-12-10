#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <unordered_map>

#define MAX 256

using namespace std;

struct HuffmanNode {
    char character;
    int frequency;
    HuffmanNode* leftChild;
    HuffmanNode* rightChild;

    HuffmanNode(char character, int frequency) : character(character), frequency(frequency), leftChild(NULL), rightChild(NULL) {}
};

class PriorityQueue {
private:
    HuffmanNode* nodes[MAX];
    int currentSize;

public:
    PriorityQueue() : currentSize(0) {}
    void enqueue(HuffmanNode* newNode);
    HuffmanNode* dequeue();
    bool isEmpty() const;
    int getSize() const { return currentSize; }
};

class HuffmanFunctions {
private:
    string inputText;
    string huffmanCodes[MAX];
    int characterFrequency[MAX] = {0};
    HuffmanNode* root = NULL;

public:
    void createNewTextFile();
    void calculateCharacterFrequencies();
    void displayResultsMenu();
    int displayMainMenu();
    void executeHuffmanAlgorithm();
    void generateHuffmanCodes(HuffmanNode* node, const string& currentCode);
    void saveToFile();
    void decompressAndDisplay();
};

int main() {
    HuffmanFunctions tools;
    bool isRunning = true;

    do {
        switch (tools.displayMainMenu()) {
            case 1:
                tools.createNewTextFile();
                break;
            case 2:
                tools.decompressAndDisplay();
                break;
            case 3:
                isRunning = false;
                cout << "Thank you for using the program! See you next time!" << endl;
                system("pause");
                break;
            default:
                cout << "Oops! That's not a valid option. Please try again!" << endl;
                system("pause");
                break;
        }
    } while (isRunning);

    return 0;
}

void HuffmanFunctions::createNewTextFile() {
    system("cls");
    cout << "Please enter your sentence or paragraph below:" << endl << "> ";
    cin.ignore();
    getline(cin, inputText);
    cout << "Text entered successfully!" << endl;
    calculateCharacterFrequencies();
    executeHuffmanAlgorithm();
    saveToFile();
}

void HuffmanFunctions::calculateCharacterFrequencies() {
    fill(begin(characterFrequency), end(characterFrequency), 0);
    for (char ch : inputText) {
        if (ch != '\n') characterFrequency[ch]++;
    }
}

void HuffmanFunctions::displayResultsMenu() {
    system("cls");
    cout << left << setw(20) << "Character"
         << setw(20) << "ASCII Code"
         << setw(20) << "Frequency"
         << "Huffman Code\n" << endl;
    cout << string(120, '=') << "\n" << endl;

    for (int i = 0; i < MAX; ++i) {
        if (characterFrequency[i] > 0) {
            cout << left << setw(20);
            if (i == ' ') {
                cout << "(space)";
            } else {
                cout << char(i);
            }
            cout << setw(20) << i
                 << setw(20) << characterFrequency[i]
                 << huffmanCodes[i] << endl;
        }
    }
    cout << "\n" << string(120, '=') << endl;
}

void HuffmanFunctions::saveToFile() {
    system("cls");
    ofstream compressedFile("compressed_students.txt");
    if (compressedFile) {
        compressedFile << "Character Mappings:\n";
        for (int i = 0; i < MAX; ++i) {
            if (characterFrequency[i] > 0) {
                if (i == ' ') {
                    compressedFile << "(space) ";
                } else {
                    compressedFile << char(i) << " ";
                }
                compressedFile << huffmanCodes[i] << endl;
            }
        }

        compressedFile << "\nEncoded Text:\n";
        for (char ch : inputText) {
            if (ch != '\n') {
                compressedFile << huffmanCodes[ch];
            }
        }

        compressedFile.close();
        cout << "Data saved to 'compressed_students.txt'!" << endl;
    } else {
        cout << "Error opening file!" << endl;
    }
    system("pause");
}

void HuffmanFunctions::decompressAndDisplay() {
    ifstream compressedFile("compressed_students.txt");
    if (!compressedFile) {
        cout << "Error reading the file!" << endl;
        return;
    }

    string line;
    bool isMappingSection = true;
    stringstream encodedTextStream;
    unordered_map<string, char> reverseHuffmanCodes;

    while (getline(compressedFile, line)) {
        if (line == "Encoded Text:") {
            isMappingSection = false;
            break;
        }

        if (isMappingSection) {
            stringstream ss(line);
            string character;
            string code;
            ss >> character >> code;
            if (character == "(space)") character = " ";
            reverseHuffmanCodes[code] = character[0];
        }
    }

    string encodedText;
    while (getline(compressedFile, line)) {
        encodedText += line;
    }
    compressedFile.close();

    string currentCode = "";
    string decodedText = "";

    for (char ch : encodedText) {
        currentCode += ch;

        if (reverseHuffmanCodes.find(currentCode) != reverseHuffmanCodes.end()) {
            decodedText += reverseHuffmanCodes[currentCode];
            currentCode = "";
        }
    }

    cout << "Decoded Text: " << decodedText << endl;
    system("pause");
}

HuffmanNode* PriorityQueue::dequeue() {
    if (isEmpty()) return NULL;
    HuffmanNode* minNode = nodes[0];
    for (int i = 1; i < currentSize; i++) {
        nodes[i - 1] = nodes[i];
    }
    currentSize--;
    return minNode;
}

void PriorityQueue::enqueue(HuffmanNode* newNode) {
    int i = currentSize - 1;
    while (i >= 0 && nodes[i]->frequency > newNode->frequency) {
        nodes[i + 1] = nodes[i];
        i--;
    }
    nodes[i + 1] = newNode;
    currentSize++;
}

bool PriorityQueue::isEmpty() const {
    return currentSize == 0;
}

void HuffmanFunctions::executeHuffmanAlgorithm() {
    PriorityQueue pq;
    for (int i = 0; i < MAX; ++i) {
        if (characterFrequency[i] > 0) {
            HuffmanNode* newNode = new HuffmanNode(char(i), characterFrequency[i]);
            pq.enqueue(newNode);
        }
    }
    while (pq.getSize() > 1) {
        HuffmanNode* leftChild = pq.dequeue();
        HuffmanNode* rightChild = pq.dequeue();
        HuffmanNode* mergedNode = new HuffmanNode('\0', leftChild->frequency + rightChild->frequency);
        mergedNode->leftChild = leftChild;
        mergedNode->rightChild = rightChild;
        pq.enqueue(mergedNode);
    }
    root = pq.dequeue();
    generateHuffmanCodes(root, "");
}

void HuffmanFunctions::generateHuffmanCodes(HuffmanNode* node, const string& currentCode) {
    if (!node) return;

    if (!node->leftChild && !node->rightChild) {
        huffmanCodes[node->character] = currentCode;
    }

    generateHuffmanCodes(node->leftChild, currentCode + "0");
    generateHuffmanCodes(node->rightChild, currentCode + "1");
}

int HuffmanFunctions::displayMainMenu() {
    system("cls");
    int userChoice;
    cout << "==================== Main Menu ====================\n" << endl;
    cout << "1. Create a New Text File" << endl;
    cout << "2. Decompress and Display Text" << endl;
    cout << "3. Exit\n" << endl;
    cout << "===================================================" << endl;
    cout << "Enter your choice: ";
    cin >> userChoice;
    return userChoice;
}