#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <bits/stdc++.h>
using namespace std;
stack<int> st;
struct node
{
    char name[50];
    short int age, x; // x - height of node
    bool g;           // g- gender
    node *fc;         // Pointer to first child
    node *ns;         // Pointer to next sibiling
    node();
    void getData();
};

node::node()
{
    fc = ns = NULL;
    g = 0;
    strcpy(name, "");
    age = x = 0;
}

void node::getData()
{
    char ch;
    cout << "\nName of the Person: ";
    cin >> name;
    cout << "Age of " << name << ": ";
    cin >> age;
    cout << name << " is (m/f): ";
    cin >> ch;
    if (ch == 'm')
        g = 1;
}

class familyTree
{
public:
    node *start;

    familyTree();

    node *traverseDown(node *, char[]); // Search functions
    node *traverseRight(node *, char[]);
    node *search(char[]);

    void addSib(node *, node *); // Functions for adding new members
    void addChild(node *, node *);
    void addNew();

    void find();          // Function to find relations
    void show(node *);    // Function to show details of particular person
    void display(node *); // Function to display full tree
    void destroy(node *); // Function to destroy full tree
    void updateX(node *, int);
};

familyTree::familyTree()
{
    start = NULL;
}

void familyTree::destroy(node *ptr)
{
    node *temp = ptr;

    if (ptr == NULL)
        return;

    while (ptr != NULL)
    {
        destroy(ptr->fc);
        temp = ptr;
        ptr = ptr->ns;
        delete temp;
    }

    start = NULL;
}

void familyTree::show(node *ptr)
{
    char g[10];
    strcpy(g, "Female");
    if (ptr->g)
        strcpy(g, "Male");
    cout << "\n\nName: " << ptr->name << endl;
    cout << "Age: " << ptr->age << endl;
    cout << "Gender: " << g << endl;
}

void familyTree::display(node *ptr)
{
    // Traverses the full n-array tree by recursion to display names of all people
    if (ptr == NULL)
        return;
    while (ptr != NULL)
    {
        cout << ptr->name << "    ";
        display(ptr->ns);
        if (ptr->fc)
            cout << endl
                 << "next generation: ";
        ptr = ptr->fc;
    }
}

void familyTree::updateX(node *ptr, int x)
{
    // Traverses the full n-array tree by recursion and updates x value of all people

    if (ptr == NULL)
        return;

    while (ptr != NULL)
    {
        updateX(ptr->fc, x++);
        if (ptr->ns != NULL)
            ptr->ns->x = x;
        ptr = ptr->ns;
    }
}

void familyTree::find()
{
    /*
        Same hight: Sibiling if same parent, else cousin
        Difference of height = 1 - Parent if immediate, else uncule/aunt
        Difference oh height = 2 - Grand parents if same link, else idk
    */

    char name1[50], name2[50];
    cout << "Enter names of two persons:\n";
    cin >> name1 >> name2;
    node *ptr1 = search(name1);
    node *ptr2 = search(name2);
    node *ptr;
    node *ptrk = ptr1;
    node *ptrk1 = ptr2;

    switch (ptr1->x - ptr2->x)
    {

    case 0:
        char s[50];
        strcpy(s, "sister");
        if (ptr1->g)
            strcpy(s, "brother");

        ptr = ptr1;
        while (ptr != NULL)
        {
            if (ptr == ptr2)
            {
                cout << endl
                     << name1 << " is " << name2 << "'s " << s << endl;
                return;
            }
            ptr = ptr->ns;
        }
        ptr = ptr2;
        while (ptr != NULL)
        {
            if (ptr == ptr1)
            {
                cout << endl
                     << name1 << " is " << name2 << "'s " << s << endl;
                return;
            }
            ptr = ptr->ns;
        }
        cout << endl
             << name1 << " and " << name2 << " are Cousins";
        break;

    case 1:
        char str3[50];
        strcpy(str3, "daughter");
        if (ptr1->g)
            strcpy(str3, "son");
        ptr2 = ptr2->fc;
        while (ptr2 != NULL)
        {
            if (ptr2 == ptr1)
            {
                cout << endl
                     << name1 << " is " << name2 << "'s " << str3;
                return;
            }
            ptr2 = ptr2->ns;
        }
        strcpy(str3, "niece");
        if (ptr1->g)
            strcpy(str3, "nephew");
        cout << endl
             << name1 << " is " << name2 << "'s " << str3;
        break;
    case -1:
        char str[10];
        strcpy(str, "mother");
        if (ptrk->g)
            strcpy(str, "father");

        ptr = ptrk->fc;
        while (ptr != NULL)
        {
            if (ptr == ptrk1)
            {
                cout << endl
                     << name1 << " is " << name2 << "'s " << str;
                return;
            }
            ptr = ptr->ns;
        }
        strcpy(str, "aunt");
        if (ptrk->g)
            strcpy(str, "uncle");
        cout << endl
             << name1 << " is " << name2 << "'s " << str;
        break;

    case 2:
        char str1[50];
        strcpy(str1, "daughter");
        if (ptr2->g)
            strcpy(str1, "son");
        ptr2 = ptr2->fc->fc;
        while (ptr2 != NULL)
        {
            if (ptr2 == ptr1)
            {
                cout << endl
                     << name1 << " is grand " << str1 << " of " << name2;
                return;
            }
            ptr2 = ptr2->ns;
        }
        break;
    case -2:
        char str2[50];
        strcpy(str2, "mother");

        if (ptr1->g)
            strcpy(str2, "father");

        ptr1 = ptr1->fc->fc;

        while (ptr1 != NULL)
        {
            if (ptr1 == ptr2)
            {
                cout << endl
                     << name1 << " is grand " << str2 << " of " << name2;
                return;
            }
            ptr1 = ptr1->ns;
        }

        break;
    default:
        cout << "Too far relationship";
        break;
    }
}

node *familyTree::search(char s[50])
{
    /*
        Searches for the given name from start to it's sibilings and their children
        Returns the pointer of node where the name is present
    */

    node *ptr = start;
    if (strcmp(ptr->name, s) == 0)
        return ptr;
    else if (traverseRight(start, s) != NULL)
        return traverseRight(start, s);
    else if (traverseDown(start, s) != NULL)
        return traverseDown(start, s);
    else
    {
        return NULL;
        cout << "***Not found***8";
    }
}

node *familyTree::traverseDown(node *ptr, char s[50])
{
    // Searches all the children
    ptr = ptr->fc;
    while (ptr != NULL)
    {
        if (strcmp(ptr->name, s) == 0)
            return ptr;
        else if (traverseRight(ptr, s) != NULL)
            return traverseRight(ptr, s);
        else
            ptr = ptr->fc;
    }
    return NULL;
}

node *familyTree::traverseRight(node *ptr, char s[50])
{
    //  Searches all the sibilings
    ptr = ptr->ns;

    while (ptr != NULL)
    {
        if (strcmp(ptr->name, s) == 0)
            return ptr;
        else if (traverseDown(ptr, s) != NULL)
            return traverseDown(ptr, s);
        else
            ptr = ptr->ns;
    }
    return NULL;
}

void familyTree::addNew()
{
    node *temp = new node;
    temp->getData();

    if (start == NULL)
    {
        start = temp;
        temp->x = 0;
    }
    else
    {
        cout << "\nEnter any relation's name: ";
        char name[50];
        cin >> name;
        cout << "\n1. Child\n2. Sibiling\n\n"
             << temp->name << " is ____ to " << name << " : ";
        int opt;
        cin >> opt;
        switch (opt)
        {
        case 1:
            addChild(search(name), temp);
            break;
        case 2:
            addSib(search(name), temp);
            break;
        }
    }
    cout << "\nPerson sucessfully added.\n";
}

void familyTree::addSib(node *a, node *b)
{
    // b is added as sibling of a

    while (a->ns != NULL)
        a = a->ns;
    a->ns = b;
    b->x = a->x;
}

void familyTree::addChild(node *a, node *b)
{

    // b is added as child as a (or) b is added as sibiling to last child of a

    if (a->fc == NULL)
        a->fc = b;
    else
        addSib(a->fc, b);
    b->x = a->x + 1;
}

void connect(familyTree *T1, familyTree *T2)
{
    char name[50];
    int opt;
    int x;
    cout << "Name of person in 1st tree to merge: ";
    cin >> name;
    cout << T2->start->name << " is __ to " << name << "\n1. Child 2. Sibling - ";
    ;
    cin >> opt;
    node *ptr = T1->search(name);
    switch (opt)
    {
    case 1:
        T1->addChild(ptr, T2->start);
        x = ptr->x + 1;
        break;
    case 2:
        T1->addSib(ptr, T2->start);
        x = ptr->x;
        break;
    }
    T2->updateX(T2->start, x);
    T2->destroy(T2->start);
    cout << "\nMerged\n";
}

void push_digits(int number)
{
    while (number != 0)
    {
        st.push(number % 10);
        number = number / 10;
    }
}
int sum_number(int number)
{
    push_digits(number);
    int sum = 0;
    int c = 0;
    while (!st.empty())
    {
        sum = sum + st.top();
        st.pop();
        c++;
    }
    if (c > 1)

    {
        return sum_number(sum);
    }
    else
    {
        return sum;
    }
}
int main()
{
    system("Color 5E");
    int choice;
    cout << "CHOOSE" << endl
         << "1. NUMEROLOGY ANALYSIS " << endl
         << "2. MATCH MAKING " << endl
         << "3. FAMILY TREE \n"
         << endl;
    cin >> choice;
    switch (choice)
    {
    case 1:
        {
        int k;
        int dob;
        cout << "\n\nEnter date of birth in form of number (For eg 10072003 for 10 July 2003): ";
        cin >> dob;
        k = sum_number(dob);
        cout << endl
             << endl
             << "According to your date of birth your numerology number is: " << k << endl
             << endl;
        switch (k)
        {
        case 9:
            cout << "Nine's-\t always seems willing to lend a helping hand or a strong voice to a worthy social cause.At their best, Nines are selfless, compassionate, healers. At their worst, they are emotionally distant, resentful, and complicated.";
            break;
        case 8:
            cout << "Eight's-\t strive for success and they want you to know about it.At their best, Eights are visionaries, goal-oriented, and hard workers. At their worst, they are blunt, power-hungry, and workaholics.";
            break;
        case 7:
            cout << "Seven's-\t have an air of mystery around them.At their best, Sevens are analytical, spiritual, and wise. At their worst, they are secretive, aloof, and pessimistic";
            break;
        case 6:
            cout << "Sixe's-\t are responsible people and natural caretakers.At their best, Sixes are reliable, supportive, and loving. At their worst, they are self-righteous, meddlesome, and a bit of a martyr.";
            break;
        case 5:
            cout << "Five's-\t cannot be tied down to one place, person, or idea�as they are restless hearts who know that change is the only constant in life.At their best, Fives are persuasive, outgoing, and free-spirited. At their worst, Fives are gossiping, non-committal, and inconsistent.";
            break;
        case 4:
            cout << "Four's-\t are methodical, practical, and down-to-earth.At their best, Fours are orderly, disciplined, and dependable. At their worst, they are strict, stubborn, and blunt.";
            break;
        case 3:
            cout << "Three's-\t treats all the world is a stage, they can be pretty dramatic. Above all else, Threes live life to the fullest. Threes are optimistic, generous, and charismatic. At their worst, they are directionless, superficially, and moody.";
            break;
        case 2:
            cout << "Two's-\t crave unconditional love above all else. They are also one of the most intuitive individuals.At their best, Twos are compassionate, honest, and excellent friends. At their worst, they are overly sensitive, indecisive, and self-sacrificing.";
            break;
        case 1:
            cout << "One's-\t are independent, strong-willed people�which makes compromising with them difficult.At their best, Ones are motivated, determined, and driven to greatness. At their worst, they are bossy, stubborn, and self-critical.";
            break;
        default:
            cout << "invalid";
            break;
        }
        return 0;
    }
    case 2:
        {
        string name1,name2;
        int k1,k2;
        int dob1,dob2;
        cout<<"Enter name of male: ";
        cin>>name1;
        cout << "\n\nEnter date of birth of Male in form of number (For eg 10072003 for 10 July 2003): ";
        cin >> dob1;
        k1 = sum_number(dob1);
        cout << endl<< endl<< name1<<" numerology number is: " << k1 << endl<< endl;
        cout<<"Enter name of female: ";
        cin>>name2;
        cout << "\n\nEnter date of birth of Female in form of number (For eg 10072003 for 10 July 2003): ";
        cin >> dob2;
        k2 = sum_number(dob2);
        cout << endl<< endl<< name2<<" numerology number is: " << k2 << endl<< endl;
    if(k1 == 1 || k2 == 5)
    {
    cout<<name1<<" and "<<name2<<" : Your compatibility matches";
    return 0;
    }
    if((k1 == 2 && k2 == 4) || (k1 ==2 && k2 == 6) || (k1 == 2 && k2 == 9) )
    {
    cout<<name1<<" and "<<name2<<" : Your compatibility matches";
    return 0;
    }
    if(k1 == 3 && k2 == 5)
    {
    cout<<name1<<" and "<<name2<<" : Your compatibility matches";
    return 0;
    }
    if(k1 == 4 && k2 == 2)
    {
    cout<<name1<<" and "<<name2<<" : Your compatibility matches";
    return 0;
    }
    if((k1 == 5 && k2 == 1) || (k1 == 5 && k2 == 3) || (k1 == 5 && k2 == 7))
    {
    cout<<name1<<" and "<<name2<<" : Your compatibility matches";
    return 0;
    }
    if((k1 == 6 && k2 == 2) || (k1 ==6 && k2 == 8))
    {
    cout<<name1<<" and "<<name2<<" : Your compatibility matches";
    return 0;
    }
    if(k1 == 7 && k2 == 5)
    {
    cout<<name1<<" and "<<name2<<" : Your compatibility matches";
    return 0;
    }
    if(k1 == 8 && k2 == 6)
    {
    cout<<name1<<" and "<<name2<<" : Your compatibility matches";
    return 0;
    }
    if((k1 == 9 && k2 == 2) || (k1 == 9 && k2 == 9))
    {
    cout<<name1<<" and "<<name2<<" : Your compatibility matches";
    return 0;
    }
    else
        cout<<"Sorry "<<name1<<"and "<<name2<<"are not a compatible match.";
    return 0;
    }
case 3:
    {
        familyTree T[100];
        int opt, n, n1, n2;
        char c, name[50];
        cout << "\nEnter the family tree number = ";
        cin >> n;
        while (1)
        {
#ifdef WINDOWS
            system("cls");
#endif
            cout << "\n\n\n\tFamily tree no = " << n << "\n\n\t1. Add new person\n\t2. Find relationship b/w two persons\n\t3. Search\n\t4. Destroy\n\t5. Display\n\t6. Change family tree\n\t7. Connect two family trees\n\t8. Exit\n\n\tEnter your choice = ";
            cin >> opt;
            cout << endl;
            switch (opt)
            {
            default:
                cout << "Invalid input";
                break;
            case 1:
                T[n].addNew();
                break;

            case 2:
                T[n].find();
                break;

            case 3:
                cout << "Enter name of person to search: ";
                cin >> name;
                T[n].show(T[n].search(name));
                break;

            case 4:
                T[n].destroy(T[n].start);
                cout << "Tree " << n << " has been destroyed sucessfully";
                break;

            case 5:
                T[n].display(T[n].start);
                break;

            case 6:
                cout << "Enter family tree number: ";
                cin >> n;
                break;

            case 7:
                cout << "Merge __ to __ \n";
                cin >> n2 >> n1;
                connect(&T[n1], &T[n2]);
                break;
            case 8:
                return 0;
            }
            cout << "\n\nPress any key to continue.....";
            cin >> c;
        }
    }
    }
}
