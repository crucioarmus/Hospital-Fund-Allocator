// finalll compilation
#include <iostream>
using namespace std;

class Institution {
private:
    string name;
    int needLevel;

public:
    Institution(string name, int needs) {
        this->name = name;
        needLevel = needs;
    }

    string get_name() const {
        return name;
    }

    int get_needLevel() const {
        return needLevel;
    }

    // Base fund request display
    virtual void requestFunds() {
        cout << name << " is requesting funds. Need level: " << needLevel << endl;
    }

    virtual void report()  {
        cout << "Institution: " << name << endl;
        cout << "Need Level: " << needLevel << endl;
    }

    // request carry forward to mod 3
    virtual void forwardRequestToAllocator() = 0;

    virtual ~Institution() {}
};


class Hospital : public Institution {
private:
    int pendingRequests;   // NOT actual money, only count of requests

public:
    Hospital(string name, int needs)
        : Institution(name, needs) 
    {
        pendingRequests = 0;
    }

    // Override requestFunds() to increment pending count
    void requestFunds() override {
        pendingRequests++;
        cout << "Hospital " << get_name() << " requested funds." << endl;
        cout << "Total pending requests = " << pendingRequests << endl;
    }

    // This is called when Module 2 hands control to Module 3
    void forwardRequestToAllocator() override {
        cout << "Forwarding " << get_name() 
             << "'s request to Allocation Manager..." << endl;
    }

    void decreaseRequest() {
        if (pendingRequests > 0)
            pendingRequests--;
    }

    int getPendingRequests() const {
        return pendingRequests;
    }

    void report() {
        cout << "Hospital Report: " << get_name() << endl;
        cout << "Need Level: " << get_needLevel() << endl;
        cout << "Pending Requests: " << pendingRequests << endl;
    }
};
// Queue node and queue for hospitals (Module 2 queue)
class Qnode {
public:
    Hospital* data;
    Qnode* next;
    Qnode(Hospital* h) { data = h; next = NULL; }
};

class HospitalQueue {
private:
    Qnode* front;
    Qnode* rear;

public:
    HospitalQueue() { front = NULL; rear = NULL; }

    bool IsEmpty() const { return (front == NULL); }

    void enqueue(Hospital* h) {
        Qnode* temp = new Qnode(h);
        if (IsEmpty()) {
            front = temp;
            rear = temp;
        } else {
            rear->next = temp;
            rear = temp;
        }
        cout << h->get_name() << " added to funding queue." << endl;
    }

    void dequeue() {
        if (IsEmpty()) {
            cout << "Queue is empty." << endl;
            return;
        }
        Qnode* temp = front;
        front = front->next;
        if (front == NULL) rear = NULL;
        delete temp;
    }

    Hospital* getFront() const {
        if (IsEmpty()) return NULL;
        return front->data;
    }
};

//MODULE 1 (Allocation + DonationPool)//

// DonationPool singleton
class DonationPool {
private:
    double poolBalance;
    static DonationPool* instance;
    DonationPool() { 
        poolBalance = 0;
     }

public:
    static DonationPool* getInstance() {
        if (instance == nullptr) instance = new DonationPool();
        return instance;
    }

    void set_poolBalance(double s) {
         poolBalance = s; 
        }
    double get_poolBalance() {
        cout << "current pool balance is: " << poolBalance << endl;
        return poolBalance;
    }

    bool AllocateAmount(double amt) {
        if (poolBalance > amt) {
            poolBalance -= amt;
            return true;
        } 
        else if (poolBalance == amt) {
            poolBalance = 0;
            cout << "THIS HOSPITAL CONSUMED ALL OF THE FUNDS" << endl;
            return true;
        } 
        else {
            cout << "donation pool currently does not have sufficient balance" << endl;
            return false;
        }
    }

    void add_Amount(double amt) { 
        poolBalance += amt;
     }
};
DonationPool* DonationPool::instance = nullptr;

// Forward declaration of Hospital (we will define Hospital in Module 2 section)
// so AllocationManager can use Hospital*
class Hospital;

// Node for allocation tree (stores pointer to Hospital)
class Node {
public:
    Hospital* h;
    Node* left;
    Node* right;
    Node(Hospital* hos)
     { 
        h = hos; 
        left = right = nullptr; 
    }
};

// Strategy interface
class AllocationStrategy {
public:
    virtual double calculateAmount(Hospital* h) = 0;

    virtual ~AllocationStrategy() {}
};

class NeedBasedStrategy : public AllocationStrategy {
public:
    double calculateAmount(Hospital* h){
    return h->get_needLevel(); 
 }
};

class FixedStrategy : public AllocationStrategy {
public:
    double calculateAmount(Hospital* h) {
         return 500;
        }
};

// Allocation manager
class AllocationManager {
private:
    Node* root;
    AllocationStrategy* strategy;

public:
    AllocationManager() { 
        root = nullptr;
        strategy = new NeedBasedStrategy(); 
    }

    Node* Request(Node* current, Hospital* h) {
        if (current == nullptr) return new Node(h);

        if (h->get_needLevel() > current->h->get_needLevel()) {
            current->left = Request(current->left, h);
        } 
        else {
            current->right = Request(current->right, h);
        }
        return current;
    }

    void insertHospital(Hospital* h) {
         root = Request(root, h);
         }

    Hospital* getHighestNeed() {
        if (root == nullptr) return nullptr;
        Node* current = root;

        while (current->left != nullptr) current = current->left;
        return current->h;
    }

    Node* removeHighest(Node* current) {
        if (current == nullptr){
            return nullptr;
        }
        if (current->left == nullptr) {
            Node* rightchild = current->right;
            delete current;
            return rightchild;
        }
        current->left = removeHighest(current->left);
        return current;
    }

    void allocateToHighest() {
        if (root == nullptr) {
            cout << "No hospitals in allocation tree." << endl;
            return;
        }
        Hospital* h = getHighestNeed();
        if (h == nullptr) {
            cout << "No hospital found."<< endl;
            return;
        }
        cout << "ALLOCATING FUNDS TO: " << h->get_name() 
        << " (need " << h->get_needLevel() 
        << ")" << endl;

        DonationPool* pool = DonationPool::getInstance();
        double needed = strategy->calculateAmount(h);
        bool success = pool->AllocateAmount(needed);
        
        if (success) {
            root = removeHighest(root);
            cout << "Allocation successful.\n";
        } 
        else {
            cout << "we are currently out of funds or insufficient funds\n";
        }
    }

    bool isEmpty() { 
        return root == nullptr; 
    }

    
};




//MODULE 3 (Donors) //

// Donation linked list node
class DonationNode {
public:
    double amount;
    DonationNode* next;

    DonationNode(double a) {
         amount = a; 
         next = NULL;
         }
};

double recursiveSum(DonationNode* head) {
    if (head == NULL) return 0;
    return head->amount + recursiveSum(head->next);
}

// Person base
class Person {
protected:
    string name;
public:
    Person (){
        name = "Unknown";
    }
    Person(string n ) { 
        name = n; 
    }
    virtual void displayInfo() = 0;

    virtual ~Person() {}
};

// Donor base
class Donor : public Person {
protected:
    DonationNode* head;
public:
    Donor(string n = "Unknown") : Person(n) { head = NULL; }
    virtual double generateDonation(double billAmount) = 0;

    void addDonation(double v) {
        DonationNode* n = new DonationNode(v);
        n->next = head;
        head = n;
    }

    void showHistory() {
        DonationNode* t = head;
        if (!t) { cout << "No donations yet." <<endl; 
            return; 
        }
        cout << "Donation history of " << name << ": "<< endl;
        while (t != NULL) {
            cout << t->amount << endl;
            t = t->next;
        }
    }

    double totalDonations() { 
        return recursiveSum(head); 
    }

    void displayInfo() override { 
        cout << "Donor: " << name << endl; 
    }
};

class IndividualDonor : public Donor {
public:
    IndividualDonor(string n) : Donor(n) {}

    double roundUpTo100(double x) {
        int bill = (int)x;
        if (bill % 100 == 0) return bill;
        int next100 = ((bill / 100) + 1) * 100;
        return next100;
    }
    double generateDonation(double billAmount) {
        double rounded = roundUpTo100(billAmount);
        double donation = rounded - billAmount;
        addDonation(donation);
        return donation;
    }
    void displayInfo() override { 
        cout << "Individual Donor: " << name << endl; 
    }
};

class CorporateDonor : public Donor {
public:
    CorporateDonor(string n) : Donor(n) {}
    double generateDonation(double billAmount) {
        double donation = billAmount * 0.02;
        addDonation(donation);
        return donation;
    }
    void displayInfo() override {
         cout << "Corporate Donor: " << name << endl; 
        }
};

//MODULE 4 (Payment System) //

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

class StackSimple {
private:
    string arr[50];
    int top;
public:
    StackSimple() { 
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
        for (int i = 0; i <= top; i++) {
            if (arr[i] == x) {
                return true;
            }
        }
        return false;
    }
};

class QueuePayments {
private:
    PaymentRequest arr[50];
    int front, rear;
public:
    QueuePayments() { 
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
        if (isFull()) { 
            cout << "Queue Overflow!" << endl; 
            return; 
        }
        rear++; 
        arr[rear] = p;
        if (front == -1){
         front = 0;
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
        left = right = nullptr; }
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
        else {
            node->right = insert(node->right, msg, time);
        }
        return node;
    }
    
    void inorder(AuditNode* node) {
        if (node == nullptr){
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
    QueuePayments q;
    StackSimple s;
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
            cout <<"Duplicate found! Request rejected" << endl; return; 
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
            cout << "No pending requests" << endl; 
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

//global objects

AllocationManager globalAllocator; // central allocator
PaymentSystem paymentSystem;       // payment system instance

// Bridge: when a hospital request is forwarded, insert into global allocator
void bridge_forwardRequestToAllocator(Hospital* h) {
    if (h == nullptr) return;
    // Insert the existing Hospital* into allocator tree (no duplicate class)
    globalAllocator.insertHospital(h);
    cout << "Bridge: inserted " << h->get_name() 
         << " into AllocationManager (need "
         << h->get_needLevel() 
         << ")" <<endl;
}

//MAIN: single program menu integrating all modules //

int main() {
    // initial donation pool funds
    DonationPool::getInstance()->add_Amount(10000);

    // create several Hospital objects to use in the queue and reports
    Hospital city("City Hospital", 400);
    Hospital alshifa("AL Shifa Hospital", 800);
    Hospital metro("Metro Hospital", 500);

    HospitalQueue fundQueue;

    // donors
    IndividualDonor ind1("Ali");
    CorporateDonor corp1("DonX");

    int choice = 0;
    while (true) {
        cout << "\n INTEGRATED SYSTEM MENU = " << endl;
        cout << "1. City Hospital Request Funds"<< endl;
        cout << "2. AL Shifa Request Funds"<< endl;
        cout << "3. Metro Hospital Request Funds"<< endl;
        cout << "4. Process All Requests (forward queue to allocator)"<< endl;
        cout << "5. Run Allocation to Highest Need (one allocation)"<< endl;
        cout << "6. Add Donation (Module 3 donors)"<< endl;
        cout << "7. Show Donation Pool Balance"<< endl;
        cout << "8. Show Donor Histories"<< endl;
        cout << "9. Payment System Menu"<< endl;
        cout << "10. Exit"<< endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            city.requestFunds();
            fundQueue.enqueue(&city);
        }
         else if (choice == 2) {
            alshifa.requestFunds();
            fundQueue.enqueue(&alshifa);
        } 
        else if (choice == 3) {
            metro.requestFunds();
            fundQueue.enqueue(&metro);
        } 
        else if (choice == 4) {
            cout << "Processing all hospital fund requests..."<< endl;
            while (!fundQueue.IsEmpty()) {
                Hospital* cur = fundQueue.getFront();
                if (cur) {
                    cur->forwardRequestToAllocator(); // message
                    bridge_forwardRequestToAllocator(cur); // actual insertion
                }
                fundQueue.dequeue();
            }
            cout << "Requests forwarded to Allocation System."<< endl;
        } 
        else if (choice == 5) {
            globalAllocator.allocateToHighest();
        } 
        else if (choice == 6) {
            cout << "Add Donation: 1=Individual (round-up), 2=Corporate (2%) : ";
            int t; 
            double bill; 
            cin >> t;
            cout << "Enter bill amount: "; 
            cin >> bill;
            double donated = 0;
            if (t == 1) {
                donated = ind1.generateDonation(bill);
                DonationPool::getInstance()->add_Amount(donated);
                cout << "Individual donated: " << donated << endl;
            } 
            else {
                donated = corp1.generateDonation(bill);
                DonationPool::getInstance()->add_Amount(donated);
                cout << "Corporate donated: " << donated << endl;
            }
        } 
        else if (choice == 7) {
            DonationPool::getInstance()->get_poolBalance();
        } 
        else if (choice == 8) {
            cout << "Donor info: "<< endl;
            ind1.displayInfo();
            ind1.showHistory(); 
            cout << "Total: " << ind1.totalDonations() << endl;
            corp1.displayInfo(); 
            corp1.showHistory(); 
            cout << "Total: " << corp1.totalDonations() << endl;
        } 
        else if (choice == 9) {
            int pchoice = 0;
            while (true) {
                cout << "\nPAYMENT SYSTEM MENU"<< endl;
                cout << "1. Add Payment Request"<< endl;
                cout << "2. Process Request"<< endl;
                cout << "3. Show Audit Log"<< endl;
                cout << "4. Back"<< endl;
                cout << "Enter choice: "<< endl;
                cin >> pchoice;
                if (pchoice == 1) {
                    paymentSystem.addPayment();
                }
                else if (pchoice == 2) {
                    paymentSystem.processRequest();
                }
                else if (pchoice == 3) {
                    paymentSystem.showAudit();
                }
                else if (pchoice == 4) {
                    break;
                }
                else {
                    cout << "Invalid choice" << endl;
                }
            }
        } 
        else if (choice == 10) {
            cout << "Exiting program."<< endl;
            break;
        } 
        else {
            cout << "Invalid choice. Try again."<< endl;
        }
    }

    return 0;
}
