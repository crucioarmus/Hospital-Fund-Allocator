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

    string get_name() {
        return name;
    }

    int get_needLevel() {
        return needLevel;
    }

    
    virtual void requestFunds() {
        cout << name << " is requesting funds. Need level: " << needLevel << endl;
    }

    virtual void report() {
        cout << "Institution: " << name << endl;
        cout << "Need Level: " << needLevel << endl;
    }

   
    virtual void forwardRequestToAllocator() = 0; //module 3 ke liye

    virtual ~Institution() {} 
};


class Hospital : public Institution {
private:
    int pendingRequests;   //  only count of requests

public:
    Hospital(string name, int needs)
        : Institution(name, needs)
    {
        pendingRequests = 0;
    }

    
    void requestFunds(){
        pendingRequests++;
        cout << "Hospital " << get_name() << " requested funds." << endl;
        cout << "Total pending requests = " << pendingRequests << endl;
    }

    // Mod 2 hands cntrl to Mod 3
    void forwardRequestToAllocator(){
        cout << "Forwarding " << get_name()
            << "'s request to Allocation Manager..." << endl;
    }

    void decreaseRequest() {
        if (pendingRequests > 0)
            pendingRequests--;
    }

    int getPendingRequests(){
        return pendingRequests;
    }

    void report() {
        cout << "Hospital Report: " << get_name() << endl;
        cout << "Need Level: " << get_needLevel() << endl;
        cout << "Pending Requests: " << pendingRequests << endl;
    }
};




class Qnode {
public:
    Hospital* data;
    Qnode* next;

    Qnode(Hospital* h) {
        data = h;
        next = NULL;
    }
};

class HospitalQueue {
private:
    Qnode* front;
    Qnode* rear;

public:
    HospitalQueue() {
        front = NULL;
        rear = NULL;
    }

    bool IsEmpty() {
        return (front == NULL);
    }

    void enqueue(Hospital* h) {      //FCFS
        Qnode* temp = new Qnode(h);
        if (IsEmpty()) {
            front = temp;
            rear = temp;
        }
        else {
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

        if (front == NULL)
            rear = NULL;

        delete temp;
    }

    Hospital* getFront() {
        if (IsEmpty())
            return NULL;
        return front->data;
    }
};




void processRequests(HospitalQueue& q) {
    if (q.IsEmpty()) return;   // Base case

    Hospital* h = q.getFront();
    h->forwardRequestToAllocator();

    q.dequeue();  // Remove q se

    processRequests(q);  // Recursively call
}




int main() {
    HospitalQueue fundQueue;

    Hospital h1("City Hospital", 8);
    Hospital h2("Al Shifa Hospital", 5);

    int choice;

    while (true) {
        cout << " Hospital Funding Menu" << endl;
        cout << "1. City Hospital Request Funds"<<endl;
        cout << "2. Al Shifa Hospital Request Funds"<<endl;
        cout << "3. Process All Requests"<<endl;
        cout << "4. Show Hospital Reports"<<endl;
        cout << "5. Exit"<<endl;
        cout << "Enter choice: "<<endl;
        cin >> choice;

        if (choice == 1) {
            h1.requestFunds();
            fundQueue.enqueue(&h1);
        }
        else if (choice == 2) {
            h2.requestFunds();
            fundQueue.enqueue(&h2);
        }
        else if (choice == 3) {
            cout << "Processing all hospital fund requests..."<<endl;
            processRequests(fundQueue);
            cout << "Requests forwarded to Allocation System."<<endl;
        }
        else if (choice == 4) {
            cout << "--- Hospital Reports ---"<<endl;
            h1.report();
            h2.report();
        }
        else if (choice == 5) {
            cout << "Exiting program."<<endl;
            break;
        }
        else {
            cout << "Invalid choice. Try again."<<endl;
        }
    }

    return 0;
}