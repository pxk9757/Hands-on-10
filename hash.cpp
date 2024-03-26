#include <iostream>
#include <cstdlib>

// Node for the doubly linked list
struct Node {
    int key, value;
    Node* prev;
    Node* next;

    Node(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

// Hash table class
class HashTable {
private:
    int size;
    int capacity;
    Node** table;
    int numElements;

    // Doubly linked list functions
    void insertNode(Node** head, int key, int value) {
        Node* newNode = new Node(key, value);
        newNode->next = *head;
        if (*head != nullptr)
            (*head)->prev = newNode;
        *head = newNode;
    }

    void deleteNode(Node** head, Node* node) {
        if (node->prev != nullptr)
            node->prev->next = node->next;
        else
            *head = node->next;

        if (node->next != nullptr)
            node->next->prev = node->prev;

        delete node;
    }

    // Hash function (multiplication and division method)
    int hashFunction(int key) {
        return (key * 2654435761) % capacity;
    }

    // Rehash function
    void rehash(int newCapacity) {
        Node** oldTable = table;
        int oldSize = size;

        capacity = newCapacity;
        size = capacity / 2;
        table = new Node*[capacity];
        for (int i = 0; i < capacity; i++)
            table[i] = nullptr;
        numElements = 0;

        for (int i = 0; i < oldSize; i++) {
            Node* curr = oldTable[i];
            while (curr != nullptr) {
                Node* next = curr->next;
                int index = hashFunction(curr->key);
                insertNode(&table[index], curr->key, curr->value);
                curr = next;
            }
        }

        for (int i = 0; i < oldSize; i++) {
            Node* curr = oldTable[i];
            while (curr != nullptr) {
                Node* next = curr->next;
                delete curr;
                curr = next;
            }
        }

        delete[] oldTable;
    }

public:
    HashTable(int cap = 16) : capacity(cap), size(cap / 2), numElements(0) {
        table = new Node*[capacity];
        for (int i = 0; i < capacity; i++)
            table[i] = nullptr;
    }

    ~HashTable() {
        for (int i = 0; i < capacity; i++) {
            Node* curr = table[i];
            while (curr != nullptr) {
                Node* next = curr->next;
                delete curr;
                curr = next;
            }
        }
        delete[] table;
    }

    void insert(int key, int value) {
        int index = hashFunction(key);
        insertNode(&table[index], key, value);
        numElements++;

        // Rehash if load factor exceeds 0.75
        if (numElements > 0.75 * size) {
            rehash(2 * capacity);
        }
    }

    void remove(int key) {
        int index = hashFunction(key);
        Node* curr = table[index];
        while (curr != nullptr) {
            if (curr->key == key) {
                deleteNode(&table[index], curr);
                numElements--;
                break;
            }
            curr = curr->next;
        }

        // Rehash if load factor drops below 0.25
        if (numElements < 0.25 * size && capacity > 16) {
            rehash(capacity / 2);
        }
    }

    int get(int key) {
        int index = hashFunction(key);
        Node* curr = table[index];
        while (curr != nullptr) {
            if (curr->key == key)
                return curr->value;
            curr = curr->next;
        }
        return -1; // Key not found
    }

    void print() {
        for (int i = 0; i < capacity; i++) {
            std::cout << "Bucket " << i << ": ";
            Node* curr = table[i];
            while (curr != nullptr) {
                std::cout << "(" << curr->key << "," << curr->value << ") ";
                curr = curr->next;
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    HashTable ht;

    ht.insert(10, 100);
    ht.insert(20, 200);
    ht.insert(30, 300);
    ht.insert(40, 400);
    ht.insert(50, 500);

    ht.print(); // Print the hash table

    std::cout << "Value for key 20: " << ht.get(20) << std::endl; // Output: 200
    std::cout << "Value for key 60: " << ht.get(60) << std::endl; // Output: -1 (key not found)

    ht.remove(20);
    ht.print(); // Print the hash table after removal

    return 0;
}