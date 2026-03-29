
#include <iostream>
using namespace std;

// Donation Node (Linked List) where each node contains amount and next ptr
class DonationNode {
public:
    double amount;
    DonationNode* next;

    DonationNode(double a) {
        amount = a;
        next = NULL;
    }
};

// Recursive function to sum all donations//helps to sum up donations by a particular person
double recursiveSum(DonationNode* head) {
    if (head == NULL)	return 0;
    return head->amount + recursiveSum(head->next);
}

//Person (Base Class) 
class Person {
protected:
    string name;

public:
    Person(string n = "Unknown") {
        name = n;
    }

    virtual void displayInfo() = 0;  // pure virtual function
};

// Donor
class Donor : public Person { //child class
protected:
    DonationNode* head; // donation history LL

public:
    Donor(string n = "Unknown") : Person(n) {
        head = NULL;
    }

    
    virtual double generateDonation(double billAmount) = 0; //virtual function later will be modified accr to type of donor

    // Add donation amount to linked list//adding at the start of the linked list
    void addDonation(double v) {
        DonationNode* n = new DonationNode(v);
        n->next = head;
        head = n;
    }

    // Show donation history
    void showHistory() {
        DonationNode* t = head;
        if (!t) {
            cout << "No donations yet."<<endl;
            return;
        }
        cout << "Donation history of " << name << ":"<<endl;
        while (t != NULL) {
            cout << t->amount << endl;
            t = t->next;
        }
    }

    // Total donation using recursion
    double totalDonations() {
        return recursiveSum(head);
    }

    virtual void displayInfo() {
        cout << "Donor: " << name << endl;
    }
};

//  Individual Donor (child of person class)
class IndividualDonor : public Donor {
public:
    IndividualDonor(string n) : Donor(n) {
        
    } 
    
    
    
    //function to round off nearest 100
    double roundUpTo100(double x) {
    int bill = (int)x;

    // if already a perfect 100
    if (bill % 100 == 0)
        return bill;

    // otherwise calculate next multiple of 100
    int next100 = ((bill / 100) + 1) * 100;
    return next100;
}


    // Round-up strategy for individuals
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

//  Corporate Donor (second child of person class)
class CorporateDonor : public Donor {
public:
    CorporateDonor(string n) : Donor(n) {}

    // Corporate donors donate 2% of bill
    double generateDonation(double billAmount)  {
        double donation = billAmount * 0.02;
        addDonation(donation);
        return donation;
    }

    void displayInfo()  {
        cout << "Corporate Donor: " << name << endl;
    }
};




//  MAIN (Demo)
int main() {
    

    IndividualDonor d1("Ali");
    CorporateDonor d2("DonX");

    double bill;

    cout << "Enter bill for Ali: ";
    cin >> bill;
    double dA = d1.generateDonation(bill);
   

    cout << "Enter bill for DonX: ";
    cin >> bill;
    double dB = d2.generateDonation(bill);
   

    cout << " Donor Info "<<endl;
    d1.displayInfo();
    d1.showHistory();
    cout << "Total: " << d1.totalDonations() << endl;

    d2.displayInfo();
    d2.showHistory();
    cout << "Total: " << d2.totalDonations() << endl;
    
    return 0;}