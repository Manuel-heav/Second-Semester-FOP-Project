#include <iostream>
#include <fstream>

using namespace std;

string server = "MANUAEL\\SQLEXPRESS01"; // Escape backslash
string database = "project";
string username = "sa";
string password = "ilikesql0989";

string connStr = "sqlcmd -S " + server + " -d " + database + " -U " + username + " -P " + password;

class shopping
{
private:
    int pcode;
    float price;
    float discount;
    string pname; // p stands for product
public:
    void menu();
    void administrator();
    void buyer();
    void add();
    void edit();
    void rem(); // a function to delete products
    void display();
    void receipt();
};

void shopping::menu()
{
m: // is a level we put for our goto function
    int choice;
    string email;
    string password; // this will be used for the login purpose

    cout << "\t\t\t\t______________________\n";
    cout << "\t\t\t\t                      \n";
    cout << "\t\t\t\t    MAIN MENU         \n";
    cout << "\t\t\t\t                      \n";
    cout << "\t\t\t\t______________________\n";
    cout << "\t\t\t\t                      \n";
    cout << "\t\t\t\t 1)Adiminstrator |\n";
    cout << "\t\t\t\t                 |    \n";
    cout << "\t\t\t\t 2)Buyer         |\n";
    cout << "\t\t\t\t                 |     \n";
    cout << "\t\t\t\t 3)exit          |\n";
    cout << "\t\t\t\t                 |     \n";
    cout << "\t\t\t\t please select!";

    cin >> choice;

    switch (choice)
    {
    case 1:
        cout << "please login \n";
        cout << "Enter Email \n";
        cin >> email;
        cout << "Password:  \n";
        cin >> password;

        if (email == "123" && password == "123")
        {
            administrator();
        }
        else
        {
            cout << "Invalid email or password";
        }
        break;
    case 2:
        buyer();
        break;
    case 3:
        exit(0);
    default: // default case will execute if a person selects other than 1,2 or 3
        cout << "please select from the given options";
    }

    goto m; // go to is a jump statement, so it will help us jumping to the main menu again after the completion of the switch case
}

void shopping::administrator()
{
m:
    int choice;
    cout << "\n\n\n\t\t\t Administrator Menu";
    cout << "\n\t\t\t|____1)Add a product_____|";
    cout << "\n\t\t\t|                        |";
    cout << "\n\t\t\t|____2)Modifiy a product_|";
    cout << "\n\t\t\t|                        |";
    cout << "\n\t\t\t|____3)Delete a product__|";
    cout << "\n\t\t\t|                        |";
    cout << "\n\t\t\t|____4)Back to main menu_|";
    cout << "\n\t\t\t|                        |";
    cout << "\n\t\t\t|please enter your choice|";

    cin >> choice;

    switch (choice)
    {
    case 1:
        add();
        break;
    case 2:
        edit();
        break;
    case 3:
        rem();
        break;
    case 4:
        menu();
        break;
    default:
        cout << "invalid choice!";
    }
    goto m;
}

void shopping::buyer()
{
m:
    int choice;
    cout << "\t\t\t   Buyer \n";
    cout << " _______________________ \n";
    cout << "\t\t\t __ 1)Buy product __\n";
    cout << "_______________________\n";
    cout << "\t\t\t __ 2)Go back _____ \n";
    cout << "\t\t Enter your choice:\n";
    cin >> choice;

    switch (choice)
    {
    case 1:
        receipt();
        break;
    case 2:
        menu();
        break;
    default:
        cout << "invalid choice!";
    }
    goto m;
}

void shopping::add()
{
    int token = 0;
    string line;

    cout << "\n\n\t\t\t Add a product  ";
    cout << "\n\n\t Product code of the product: ";
    cin >> pcode;
    cout << "\n\n\t Name of product: ";
    cin >> pname;
    cout << "\n\n\t Price of product: $";
    cin >> price;
    cout << "\n\n\t Discount on product: %";
    cin >> discount;
    line = to_string(pcode);

    // SQL query to check if there's another product with the same product code
    string check_query = connStr + " -Q \"SELECT COUNT(*) FROM productss WHERE product_code = " + to_string(pcode) + "\" -h-1 -W";

    // Execute the SQL query to check the count of products with the same product code
    FILE *pipe = _popen(check_query.c_str(), "r");
    if (!pipe)
    {
        cout << "Failed to run query.\n";
        return;
    }
    char buffer[128];
    string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != NULL)
    {
        result += buffer;
    }
    _pclose(pipe);

    int count = stoi(result);
    if (count > 0)
    {
        cout << "\n\n\t Duplicate product code. Please try again.";
        return; // Exit the function
    }

    // SQL query to insert the product into the database
    string insert_query = connStr + " -Q \"INSERT INTO productss (product_code, product_name, product_price, product_discount) VALUES (" + to_string(pcode) + ", '" + pname + "', " + to_string(price) + ", " + to_string(discount) + ")\"";

    // Execute the SQL query to insert the product into the database
    system(insert_query.c_str());

    cout << "\n\n\t Product added successfully.";
}

void shopping::edit()
{
    int pkey;
    int new_code;
    float new_price, new_discount;
    string new_name, code;
    bool found = false;

    cout << "\n\t\t\t Modify the record";
    cout << "\n\t\t\t Product code: ";
    cin >> pkey;
    cin.ignore(); // Ignore leftover newline character

    code = to_string(pkey);

    // SQL query to check if the product exists with the given product code
    string check_query = connStr + " -Q \"SELECT COUNT(*) FROM productss WHERE product_code = " + to_string(pkey) + "\" -h-1 -W";

    // Execute the SQL query to check if the product exists
    FILE *pipe = _popen(check_query.c_str(), "r");
    if (!pipe)
    {
        cout << "Failed to run query.\n";
        return;
    }
    char buffer[128];
    string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != NULL)
    {
        result += buffer;
    }
    _pclose(pipe);

    int count = stoi(result);
    if (count == 0)
    {
        cout << "\n\n\t Product not found.";
        return; // Exit the function
    }

    // Product exists, proceed with editing
    found = true;
    cout << "\n\t\t New product code: ";
    cin >> new_code;
    cin.ignore(); // Ignore leftover newline character
    cout << "\n\t\t New name of the product: ";
    getline(cin, new_name); // Use getline to allow spaces
    cout << "\n\t\t New price: $";
    cin >> new_price;
    cout << "\n\t\t New discount: %";
    cin >> new_discount;

    // SQL query to update the row in the database
    string update_query = connStr + " -Q \"UPDATE productss SET product_code='" + to_string(new_code) + "', product_name='" + new_name + "', product_price=" + to_string(new_price) + ", product_discount=" + to_string(new_discount) + " WHERE product_code='" + to_string(pkey) + "'\"";
    system(update_query.c_str()); // Execute the SQL query

    if (found)
    {
        cout << "\n\t\t Record edited.";
    }
    else
    {
        cout << "\n\n Record not found.";
    }
}
void shopping::rem()
{
    int pkey;
    cout << "\n\n\t Delete product";
    cout << "\n\n\t Product code: ";
    cin >> pkey;

    // SQL query to check if the product exists with the given product code
    string check_query = connStr + " -Q \"SELECT COUNT(*) FROM productss WHERE product_code = " + to_string(pkey) + "\" -h-1 -W";

    // Execute the SQL query to check if the product exists
    FILE *pipe = _popen(check_query.c_str(), "r");
    if (!pipe)
    {
        cout << "Failed to run query.\n";
        return;
    }
    char buffer[128];
    string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != NULL)
    {
        result += buffer;
    }
    _pclose(pipe);

    int count = stoi(result);
    if (count == 0)
    {
        cout << "\n\n\t Product not found.";
        return; // Exit the function
    }

    // Product exists, proceed with deletion
    // SQL query to delete the product from the database
    string delete_query = connStr + " -Q \"DELETE FROM productss WHERE product_code = " + to_string(pkey) + "\"";

    // Execute the SQL query to delete the product
    system(delete_query.c_str());

    cout << "\n\n\t Product deleted successfully.";
}
void shopping::display()
{
    // fstream data;
    // data.open("database.txt",ios::in);
    // cout<<"\n\n____________________________________________\n";
    // cout<<"proNo\t\tName\t\tPrice";
    // cout<<"\n\n____________________________________________\n";

    // data>>pcode>>pname>>price>>discount;
    // while(!data.eof()){
    //     cout<<pcode<<"\t\t"<<pname<<"\t\t"<<price<<"\n";
    //     data>>pcode>>pname>>price>>discount;
    // }
    // data.close();
    string sql_query = connStr + " -Q \"select * from productss\"";

    // Execute the SQL query
    system(sql_query.c_str());
}

void shopping::receipt()
{
    fstream data;
    int arrc[100]; // array of product codes
    int arrq[100]; // array of quantities
    char choice;
    int c = 0; // counter for the number of items
    float amount = 0;
    float dis = 0; // discount
    float total = 0;

    // Check if the database is empty
    string count_query = connStr + " -Q \"SELECT COUNT(*) FROM productss\" -h-1 -W";
    FILE *pipe = _popen(count_query.c_str(), "r");
    if (!pipe)
    {
        cout << "Failed to run query.\n";
        return;
    }
    char buffer[128];
    string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != NULL)
    {
        result += buffer;
    }
    _pclose(pipe);

    if (stoi(result) == 0)
    {
        cout << "\n\n Empty database";
        return;
    }

    string command = connStr + " -Q \"SELECT * FROM productss\" -o list.txt";
    int results = system(command.c_str());

    cout << "\n\n\t\t\t\t RECEIPT ";

    data.open("list.txt", ios::in);
    if (!data)
    {
        cout << "Error opening file!";
        return;
    }

    display(); // to show the list of products available

    cout << "\n___________________________________________\n";
    cout << "\n                                           \n";
    cout << "\n        Please place your order             \n";
    cout << "\n                                           \n";
    cout << "\n___________________________________________\n";

    do
    {
    m:
        cout << "\n\nEnter product code: ";
        cin >> arrc[c];
        cout << "\n\nEnter product quantity: ";
        cin >> arrq[c];

        // Check for duplicate product codes
        for (int i = 0; i < c; i++)
        {
            if (arrc[c] == arrc[i])
            {
                cout << "\n\nDuplicate product code. Please try again!";
                goto m;
            }
        }

        c++; // increment the counter for valid input
        cout << "\n\n Do you want to buy another product? If yes, press 'y', else 'no': ";
        cin >> choice;
    } while (choice == 'y');

    cout << "\n\n\t\t\t______________________RECEIPT______________________\n";
    cout << "\n Product No\t Product Name\t Product Quantity\tPrice\tAmount\tAmount with Discount\n";

    for (int i = 0; i < c; i++)
    {
        // data.clear();
        // data.seekg(0);

        while (data >> pcode >> pname >> price >> discount)
        {
            if (pcode == arrc[i])
            {
                amount = price * arrq[i];
                dis = amount - (amount * discount / 100);
                total += dis;
                cout << "\n"
                     << pcode << "\t\t" << pname << "\t\t" << arrq[i] << "\t\t" << price << "\t" << amount << "\t" << dis;
                break; // product found, no need to continue looping
            }
        }
    }

    data.close();

    cout << "\n\n___________________________________________________________";
    cout << "\n Total Amount : $" << total;
    cout << "\n\n___________________________________________________________";
}

int main()
{
    std::cout << R"(



    ______ ____  _    _ _____    ____   ______   __  __          _____ _______
    |  ____/ __ \| |  | |  __ \  |  _ \ |  ____| |  \/  |   /\   |  __ |__   __|
    | |__ | |  | | |  | | |__) | | |_) || |__    | \  / |  /  \  | |__) | | |
    |  __|| |  | | |  | |  _  /  |  _ < |  __|   | |\/| | / /\ \ |  _  /  | |
    | |   | |__| | |__| | | \ \ _| |_) _| |      | |  | |/ ____ \| | \ \  | |
    |_|    \____/ \____/|_|  \_(_|____(_|_|      |_|  |_/_/    \_|_|  \_\ |_|


    )" << '\n';

    shopping s;
    s.menu();

    return 0;
}
