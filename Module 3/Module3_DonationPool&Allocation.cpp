#include<iostream>
using namespace std;

//DUMMY CLASS FOR MODULE 3 ONLY
class Hospital{
    private:
    string name;
    int needLevel;
    public:
    Hospital(string n,int level){
        name=n;
        needLevel=level;
    }
    string get_name(){
        return name;
    }
    int get_needLevel(){
        return needLevel;
    }
};

class DonationPool{ 
    private:
    double poolBalance;
    static DonationPool* instance;
    DonationPool(){
        cout<<"singleton"<<endl;
    }
    public:
    static DonationPool*getInstance(){
        if(instance==nullptr){
            instance=new DonationPool();
            instance->poolBalance=0;
        }
        return instance;
    }
    void set_poolBalance(double s){
        poolBalance=s;
    }
    double get_poolBalance(){
        cout<<"current pool balance is:"<<poolBalance<<endl;
       return poolBalance;
    }
    bool AllocateAmount(double amt){
        if(poolBalance>amt){
            poolBalance =poolBalance-amt;
            return true;
        }
        else if(poolBalance==amt){
            poolBalance=0;
            cout<<"THIS HOSPITAL CONSUMED ALL OF THE FUNDS"<<endl;
            return true;
        }
        else{cout<<"donation pool currently does not have sufficient balance"<<endl;
            return false;}
    }
    void add_Amount(double amt){
        poolBalance=poolBalance+amt;
    }

};

//tree of hospital sorted by need level
//just a node structure for our tree
class Node{
    public:
    Hospital*h;
    Node*left;
    Node*right;
    Node(Hospital*hos){
        h=hos;
        right=left=nullptr;
    }

};

//to include polymorphism,abstraction,inheritance
//creating strategy to allocate funds;either fixed or need based
class AllocationStrategy {
public:
    virtual double calculateAmount(Hospital* h) = 0;  // pure virtual
};

//derived class
class NeedBasedStrategy : public AllocationStrategy {
public:
    double calculateAmount(Hospital* h)  {
        return h->get_needLevel();   // allocate equal to need
    }
};

class FixedStrategy : public AllocationStrategy {
public:
    double calculateAmount(Hospital* h)  {
        return 500;  // allocate fixed amount
    }
};

//CUSTOM STACK FOR ALLOCATIONS
class AllocationRecord {
public:
    Hospital* hospital;
    double amount;
    AllocationRecord* next;
    AllocationRecord(Hospital* h, double a) {
        hospital = h;
        amount = a;
        next = nullptr;
    }
};

class AllocationStack {
private:
    AllocationRecord* topNode;
public:
    AllocationStack() { topNode = nullptr; }

    void push(Hospital* h, double amt) {
        AllocationRecord* rec = new AllocationRecord(h, amt);
        rec->next = topNode; //top upgraded to the last node inserted
        topNode = rec;
    }

    AllocationRecord* pop() {
        if (isEmpty()) return nullptr;
        AllocationRecord* rec = topNode;
        topNode = topNode->next;
        rec->next = nullptr;
        return rec;
    }

    bool isEmpty() {
        return topNode == nullptr;
    }
};

//allocation manger
//to allocate accr to need levels
class AllocationManager{
    private :
    Node*root; //root of the hospital priotrity tree
    AllocationStrategy* strategy;
    AllocationStack allocationStack; //  stack for undo
    public:
    AllocationManager(){
        root=nullptr;
        strategy=new NeedBasedStrategy(); //default strategy
    }
    //recursion to sort/insert the tree; hos w higher need goes to left n vice versa
    Node*Request(Node*current,Hospital*h){
        if(current==nullptr){
            return new Node(h);
        }
        if(h->get_needLevel()>current->h->get_needLevel()){
            current->right=Request(current->right,h);}
        else{
            current->left=Request(current->left,h);
        } 
        return current;   
        }
    
    void insertHospital(Hospital*h){
        root =Request(root,h);}

    //now findiing highest need hospital
    Hospital*getHighestNeed(){
        if(root ==nullptr){
            return nullptr;
        }
        Node*current=root;
        while(current->left != nullptr){
            current=current->left;
        }
        return current->h; //leftmost hos w max need level
    }

    //removing the hos w highest need once its need is met
    Node*removeHighest(Node*current){
        if(current==nullptr){return nullptr;}
        if(current->left==nullptr){
            //means its the highest need node
            Node*rightchild=current->right;
            delete current;
            return rightchild;  
        }
        current->left=removeHighest(current->left);
        return current;
    }
    //allocating funds to highest need level hos
    void allocateToHighest(){
        if (root==nullptr){return;}
        Hospital*h=getHighestNeed();
        cout<<"ALLOCATING FUNDS TO:"<<  h->get_name()<<endl;
        DonationPool*pool=DonationPool::getInstance();
        double needed= h->get_needLevel();
        
        bool success=pool->AllocateAmount(needed);
        if (success){
            allocationStack.push(h, needed); // track allocation
            root=removeHighest(root);
        }
        else{
            cout<<"we are currently out of funds"<<endl;
        }
    }

    //undo last allocation
    void undoLastAllocation() {
        if (allocationStack.isEmpty()) {
            cout << "No allocation to undo." << endl;
            return;
        }
        AllocationRecord* last = allocationStack.pop();
        DonationPool* pool = DonationPool::getInstance();
        pool->add_Amount(last->amount);
        insertHospital(last->hospital);
        cout << "Undid allocation of " << last->amount
         << " to hospital " << last->hospital->get_name() << endl;
        delete last;
    }

};

DonationPool*DonationPool ::instance=nullptr;

int main() {
    cout <<    "DONATION ALLOCATION SYSTEM   " << endl;

    DonationPool::getInstance()->set_poolBalance(0); // start empty

    AllocationManager M;

    int choice;

    while (true) {
        cout << "MENU    " << endl;
        cout << "1. Add Funds to Donation Pool" << endl;
        cout << "2. Add a Hospital" << endl;
        cout << "3. Allocate to Highest Need Hospital" << endl;
        cout << "4. Undo Last Allocation" << endl;
        cout << "5. Show Current Pool Balance" << endl;
        cout << "6. Exit" << endl;
        cout << "pls enter choice: ";
        cin >> choice;

        if (choice == 1) {
            double amount;
            cout << "Enter amount to add: ";
            cin >> amount;
            DonationPool::getInstance()->add_Amount(amount);
            cout << "Funds added!" << endl;
        }

        else if (choice == 2) {
            string name;
            int need;

            cout << "Enter hospital name: ";
            cin >> ws;          
            

            cout << "Enter need level: ";
            cin >> need;

            Hospital* h = new Hospital(name, need);
            M.insertHospital(h);

            cout << "Hospital added successfully." << endl;
        }

        else if (choice == 3) {
            cout << "Allocating funds to highest-need hospital"<<endl;
            M.allocateToHighest();
        }

        else if (choice == 4) {
            cout << "Undoing last allocation"<<endl;
            M.undoLastAllocation();
        }

        else if (choice == 5) {
            DonationPool::getInstance()->get_poolBalance();
        }

        else if (choice == 6) {
            cout << "Exiting program"<<endl;
            break;
        }

        else {
            cout << "Invalid choice. Try again."<<endl;
        }
    }

    return 0;
}




