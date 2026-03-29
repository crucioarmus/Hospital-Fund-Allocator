#include <iostream>
using namespace std;

class PaymentBase {
public:
    virtual void display() = 0; 
};

class PaymentRequest : public PaymentBase {
public:
    string name;
    int amount;
    string method;

    PaymentRequest() {}

    PaymentRequest(string n, int a, string m) {
        name = n;
        amount = a;
        method = m;
    }

    void display() override {
        cout << "Payment by: " << name
             << " - Amount: " << amount
             << " - Method: " << method << endl;
    }
};

class Stack {
private:
    string arr[50];
    int top;

public:
    Stack() { 
        top = -1; 
        
    }
    
    bool isFull(){
        if (top == 49){
            return true;
        }
            return false;
    }
    
    void push(string x) {
        if (isFull()) {
            cout << "Stack Overflow!" << endl;
            return;
        }
        top++;
        arr[top] = x;
    }

    bool exists(string x) {
        for (int i = top; i => 0; i--) {
            if (arr[i] == x){
                return true;
            }
        }
        return false;
    }
};

class Queue {
private:
    PaymentRequest arr[50];
    int front, rear;

public:
    Queue() {
        front = -1;
        rear = -1;
    }
    
    bool isFull (){
        if (rear == 49){
            return true;
        }
            return false;
    }
    
    void enqueue(PaymentRequest p) {
        if (isFull()){
            cout << "Queue Overflow!" << endl;
        }
            rear++;
            arr[rear] = p;
            if (front == -1){
                front ++;
            }
            cout << "New Payment added" << endl;
     }
    

    bool isEmpty() {
        if (front ==-1 || front > rear){
            return true;
        }
            return false;
    }

    PaymentRequest dequeue() {
        if (isEmpty()) {
            cout << "Queue Underflow!" << endl;
            return PaymentRequest();
        }
        PaymentRequest p = arr[front];
        front++;
        return p;
    }
};

class AuditNode {
public:
    string message;
    int timestamp;
    AuditNode* left;
    AuditNode* right;

    AuditNode(string msg, int t) {
        message = msg;
        timestamp = t;
        left = right = nullptr;
    }
};

class AuditTree {
private:
    AuditNode* root;

    AuditNode* insert(AuditNode* node, string msg, int time) {
        if (node == nullptr){
            return new AuditNode(msg, time);
        }

        if (time < node->timestamp){
            node->left = insert(node->left, msg, time);
        }
        else{
            node->right = insert(node->right, msg, time);
        }

        return node;
    }

    void inorder(AuditNode* node) {
        if (node == nullptr) {
            return;
        }

        inorder(node->left);
        cout << node->timestamp << " : " << node->message << endl;
        inorder(node->right);
    }

public:
    AuditTree() { 
        root = nullptr; 
    }

    void addEntry(string msg, int time) {
        root = insert(root, msg, time);
    }

    void showLog() {
        cout << "The Audit Log is = " << endl;
        inorder(root);
    }
};

class PaymentSystem {
private:
    Queue q;
    Stack s;
    AuditTree audit;
    int timeCounter;

public:
    PaymentSystem() { 
        timeCounter = 1;
        }

    void addPayment() {
        string name, method;
        int amount;

        cout << "Enter Customer Name: ";
        cin >> name;

        if (s.exists(name)) {
            cout << "Duplicate found! Request rejected" << endl;
            return;
        }
        s.push(name);

        cout << "Enter Amount: ";
        cin >> amount;

        cout << "Payment Method (Card/Wallet/Bank): ";
        cin >> method;

        PaymentRequest p(name, amount, method);
        q.enqueue(p);

        string msg = "New request by ";
        msg += name;
        audit.addEntry(msg, timeCounter++);
    }

    void processRequest() {
        if (q.isEmpty()) {
            cout << "No pending requests" <<endl;
            return;
        }

        PaymentRequest p = q.dequeue();
        p.display();
        
        cout << "Processing request for " << p.name << endl;

        string msg = "Processed request of ";
        msg += p.name;
        audit.addEntry(msg, timeCounter++);
    }

    void showAudit() {
        audit.showLog();
    }
};

int main() {
    PaymentSystem sys;
    int choice;

    while (true) {
        cout << "\nPAYMENT SYSTEM MENU =" <<endl;
        cout << "1. Add Payment Request" <<endl;
        cout << "2. Process Request" << endl;
        cout << "3. Show Audit Log" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1){
            sys.addPayment();
        }
        else if (choice == 2){
            sys.processRequest();
        }
        else if (choice == 3){
            sys.showAudit();
        }
        else if (choice == 4) {
            break;
        }
        else {
            cout << "Invalid choice" << endl;
        }
    }

    return 0;
}