#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <sqlite3.h>
#include <unistd.h>
#include <ctime>

using namespace std;

void userInterface();
void managerInterface();
void defaultInterface();



float revenue = 0;


string selectAll = "SELECT * FROM book;";
sqlite3* DB;


static int callback(void* data, int argc, char** argv, char** azColName)
{
    int i;
    fprintf(stderr, "%s: ", (const char*)data);
  
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
  
    printf("\n");
    return 0;
}

std::string gen_random(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}



int main(){

    int exit = 0;
    char* messaggeError;

    sqlite3* DB;

    //adding DB
    string createTbl =  
                        //publisher table
                        "CREATE TABLE publisher("
                        "publisher_name     TEXT, "
                        "address            TEXT    NOT NULL, "
                        "email              TEXT    NOT NULL, "
                        "phone_num           TEXT    NOT NULL, "
                        "bank_acc            TEXT    NOT NULL,"
                        "PRIMARY KEY (publisher_name)"
                        ");"

                        //book table
                        "CREATE TABLE book("
                        "isbn                   INTEGER, "
                        "name                   TEXT      NOT NULL, "
                        "author                 TEXT      NOT NULL, "
                        "genre                  TEXT      NOT NULL, "
                        "publisher              TEXT      NOT NULL, "
                        "pages                  INTEGER   NOT NULL," 
                        "cost                   INTEGER   NOT NULL,"
                        "publisher_fee_per      INTEGER   NOT NULL,"
                        "stock                  INTEGER   NOT NULL,"
                        "num_sold               INTEGER   NOT NULL,"
                        "PRIMARY KEY (isbn)"
                        "FOREIGN KEY(publisher) REFERENCES publisher(publisher_name)"
                        ");"

                        //orders table
                        "CREATE TABLE orders("
                        "order_num   INTEGER AUTO_INCREMENT, "
                        "orderer_email      TEXT    NOT NULL, "
                        "billing_info   TEXT    NOT NULL, "
                        "shipping_info  TEXT    NOT NULL, "
                        "tracking_num   INTEGER     NOT NULL, "
                        "total           INTEGER     NOT NULL,"
                        "PRIMARY KEY (order_num)"
                        ");"

                        //orders_request table
                        "CREATE TABLE order_request("
                        "order_num   INTEGER, "
                        "isbn           INTEGER, "
                        "quantity       INTEGER     NOT NULL,"
                        "PRIMARY KEY (order_num, isbn)"
                        "FOREIGN KEY(order_num) REFERENCES orders(order_num)"
                        "FOREIGN KEY(isbn) REFERENCES book(isbn)"
                        ");"

                        //user table
                        "CREATE TABLE user("
                        "email          TEXT, "
                        "name           TEXT, "
                        "billing_info   TEXT    NOT NULL,"
                        "shipping_info  TEXT    NOT NULL, "
                        "PRIMARY KEY (email)"
                        ");"

                        //submit table
                        "CREATE TABLE submit("
                        "order_num   INTEGER, "
                        "user_email     TEXT, "
                        "PRIMARY KEY (order_num, user_email)"
                        "FOREIGN KEY(order_num) REFERENCES orders(order_num)"
                        "FOREIGN KEY(user_email) REFERENCES user(email)"
                        ");"

                        //checkout_basket table
                        "CREATE TABLE checkout_basket("
                        "user_email     TEXT, "
                        "isbn           INTEGER     NOT NULL, "
                        "quantity       INTEGER     NOT NULL, "
                        "PRIMARY KEY (user_email, isbn)"
                        "FOREIGN KEY(user_email) REFERENCES user(email)"
                        "FOREIGN KEY(isbn) REFERENCES book(isbn)"
                        ");"
                        ;

    exit = sqlite3_open("library.db", &DB);
    exit = sqlite3_exec(DB, createTbl.c_str(), NULL, 0, &messaggeError);
    

    if (exit != SQLITE_OK) {
        std::cerr << "Error Create Table" << std::endl;
        sqlite3_free(messaggeError);
    }
    else{
        std::cout << "Tables created Successfully" << std::endl;
        cout << endl;
    }
        

    string sql(
                //inserting books
                "INSERT INTO book VALUES(88888888, 'Harry Potter', 'J.K Rowling', 'Fantasy', 'Bloomsburry', 592 , 20, 2, 100, 0);"
                "INSERT INTO book VALUES(77777777, 'The Thing', 'Steven King', 'Horror', 'Bloomsburry', 465 , 30, 3, 50, 0);"
                "INSERT INTO book VALUES(66666666, 'The Choice', 'Nora Roberts', 'Novel', 'HarperCollins', 230 , 10, 4, 70, 0);"
                "INSERT INTO book VALUES(55555555, 'The Devil Wears Prada', 'Stephen King', 'Fiction', 'Macmillan Publishers', 230 , 10, 5, 20, 0);"

                //inserting users
                "INSERT INTO user VALUES('user1@gmail.com', 'Jay', '113 Maple Street', '756 Apple Street');"
                
                // "INSERT INTO USER VALUES('user2@gmail.com', 'Michael', '234 Prune Street', '854 Kiwi Street');"
                // "INSERT INTO USER VALUES('user3@gmail.com', 'Cindy', '67 Flora Street', '453 Watermelon Street');"

               )
               ;

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    

    if (exit != SQLITE_OK) {
        std::cerr << "Error Inserting Data" << std::endl;
        sqlite3_free(messaggeError);
    }
    else{
        std::cout << "Data inserted Successfully" << std::endl;
        cout << endl;
    }

    sqlite3_close(DB);

    defaultInterface();

    
}

void defaultInterface(){
    int choice = -1;
    cout << "Which Interface would you like to run the program as?"<< endl;
    cout << "  (1) User Interface" << endl;
    cout << "  (2) Manager Interface" << endl;
    cin >> choice; 
    switch (choice){
        case 1: userInterface(); break;  
        case 2: managerInterface(); break;  
    }
}

void userInterface(){
    int exit = 0;
    char* messaggeError;
    exit = sqlite3_open("library.db", &DB);
    
    
    int choice = -1;
    
    cout << "What would you like to do?" << endl;
    cout << "  (1) Browse All Books" << endl;
    cout << "  (2) Search for a Book" << endl;
    cout << "  (3) Add a book to cart" << endl;
    cout << "  (4) Display current Cart" << endl; 
    cout << "  (5) Checkout" << endl;
    cout << "  (6) Return to main menu" << endl;
    cin >> choice; 
    switch (choice){

        //showing all data ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        case 1:
            cout << "===============All Books===============" <<endl;
            cout << "\n"; 
            sqlite3_exec(DB, selectAll.c_str(), callback, NULL, NULL);
            cout << "=======================================" <<endl;
            cout << "\n"; 

        userInterface();  
        break;

        //search by book name++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        case 2: {
            char bookName[100];
            string t;
            cout << "Please input your book name: ";
            cin.getline(bookName,sizeof(bookName));
            cin.getline(bookName,sizeof(bookName));
            t = string(bookName);

            exit = sqlite3_open("library.db", &DB);
            std::string searchBookQuery = "SELECT * FROM book WHERE name = '" + t + "';";
            cout << "\n"; 
            sqlite3_exec(DB, searchBookQuery.c_str(), callback, NULL, NULL);

            userInterface();  
            break;
        }

        //add a book in cart ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        case 3: {
            char isbn[100];
            string t;
            cout << "Please input ISBN of BOOK you want to add in your cart: ";
            cin.getline(isbn,sizeof(isbn));
            cin.getline(isbn,sizeof(isbn));
            t = string(isbn);

            //exit = sqlite3_open("library.db", &DB);
            
            //SELECT EXISTS(SELECT 1 FROM myTbl WHERE u_tag="tag");
            //IF [NOT] EXISTS ( SELECT 1 FROM MyTable WHERE ... )

            cout << "value of t: " << t << endl;

            std::string checkCartQuery = "INSERT INTO checkout_basket VALUES('user1@gmail.com', \'" + t + "\', 1)";
            
            std::string addCartQuery = "INSERT INTO checkout_basket VALUES('user1@gmail.com', \'" + t + "\', 1);";
            
            //ON CONFLICT ("+t+") DO UPDATE SET QUANTITY = QUANTITY + 1;

            exit = sqlite3_exec(DB, addCartQuery.c_str(), NULL, 0, &messaggeError);
            // if(exit == 1) {
            //     cout << "yes" << endl;
            // }else{
            //     cout << "no" << endl;
            // }
            

            // cout << "HERE:" << sqlite3_exec(DB, checkCartSql.c_str(), NULL, 0, &messaggeError) << endl;
            // std::string checkCartSql = "SELECT EXISTS( SELECT 1 FROM CHECKOUT_BASKET WHERE ISBN=\'"+t+"\');";
            // int result = sqlite3_exec(DB, checkCartSql.c_str(), NULL, 0, &messaggeError);
            // cout << "RESULT: " << result << endl;

            // if(sqlite3_exec(DB, checkCartSql.c_str(), NULL, 0, &messaggeError) == 1){//matching book is already in cart
            //     std::string incQuantitySql = "UPDATE CHECKOUT_BASKET SET QUANTITY = QUANTITY + 1 WHERE ISBN = " + t + ";";
            //     sqlite3_exec(DB, incQuantitySql.c_str(), NULL, 0, &messaggeError);
            //     cout << "Quantity increased"<< endl;

            // }else{//matching book doesn't exist in cart
            //     std::string addCartQuery = "INSERT INTO CHECKOUT_BASKET VALUES ('user1@gmail.com', \'" + t + "\', 0);";
            //     sqlite3_exec(DB, addCartQuery.c_str(), NULL, 0, &messaggeError);
            //     cout << "Book is added in cart"<< endl;
            // }
            userInterface();  
            break;
        }

        //showing cart data ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        case 4: {
            std::string showCartQuery = "SELECT isbn, quantity FROM checkout_basket;";
            sqlite3_exec(DB, showCartQuery.c_str(), callback, NULL, NULL);
            userInterface();  
            break;
        }

        //check out ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        case 5: {
           /*//orders table
                        "CREATE TABLE orders("
                        "order_num   INTEGER, "
                        "orderer_email      TEXT    NOT NULL, "
                        "billing_info   TEXT    NOT NULL, "
                        "shipping_info  TEXT    NOT NULL, "
                        "tracking_num   INTEGER     NOT NULL, "
                        "total          INTEGER     NOT NULL,"
                        "PRIMARY KEY (order_num)"
                        ");"
            */
            char billingInfo[100];
            char shippingInfo[100];

            string b, s;
            cout << "Enter Billing Info: ";
            cin.getline(billingInfo,sizeof(billingInfo));
            cin.getline(billingInfo,sizeof(billingInfo));
            b = string(billingInfo);

            cout << "Enter Shipping Info: ";
            cin.getline(shippingInfo,sizeof(shippingInfo));
            cin.getline(shippingInfo,sizeof(shippingInfo));
            s = string(shippingInfo);

            // // Providing a seed value
	        // srand((unsigned) time(NULL));

	        // // Get a random number
	        // int random = rand();

            // srand((unsigned)time(NULL) * getpid());     
            // std::cout << gen_random(12) << "\n";   
            

            std::string makeOrderQuery = "INSERT INTO orders (orderer_email, billing_info, shipping_info, tracking_num, total) VALUES('user1@gmail.com', " + b + ", " + s + ", " + gen_random(12) + ", 0);";


            sqlite3_exec(DB, makeOrderQuery.c_str(), callback, NULL, NULL);

            //clear cart   
            std::string deleteAllRowsQuery = "DELETE FROM ceckout_basket; INSERT ";
            exit = sqlite3_exec(DB, deleteAllRowsQuery.c_str(), callback, NULL, NULL);
            cout << "Exit: "<< exit << endl;
            userInterface();  
            break;
        }


        
        case 6: defaultInterface(); break;

    }
}



void managerInterface(){
    int exit = 0;
    char* messaggeError;
    exit = sqlite3_open("library.db", &DB);
    string selectAll = "SELECT * FROM BOOK;";
    int choice = -1;
    int isbn;
    char bookName[100];
    char author[100];
    char genre[100];
    char publisher[100];
    int pages;
    int price;
    int abc = 12345;
    cout << "What would you like to do?"<< endl;
    cout << "  (1) Browse All Books" << endl;
    cout << "  (2) Add book to the library" << endl;
    cout << "  (3) Remove book from the library" << endl;
    cout << "  (4) Display a report" << endl;
    cout << "  (5) Return to main menu" << endl;
    cin >> choice; 
    switch (choice){
        case 1: sqlite3_exec(DB, selectAll.c_str(), callback, NULL, NULL);
        cout << "\n"; 
        managerInterface();  
        case 2: {
        cout << "Please input your book ISBN: ";
        cin >> isbn;

        cout << "Please input your book name: ";
        cin.getline(bookName,sizeof(bookName));
        cin.getline(bookName,sizeof(bookName));

        cout << "Please input your book author: ";
        cin.getline(author,sizeof(author));

        cout << "Please input your book genre: ";
        cin.getline(genre,sizeof(genre));

        cout << "Please input your book publisher: ";
        cin.getline(publisher,sizeof(publisher));
        
        cout << "Please input your book pages: ";
        cin >> pages;

        cout << "Please input your book price: ";
        cin >> price;
        exit = sqlite3_open("library.db", &DB);
        string selectAll = "SELECT * FROM BOOK;";
        string sql = ("INSERT INTO BOOK VALUES(" + to_string(isbn)  + ", \'" + bookName + "',  '"+author + "\' , \'" + genre + "\' , \'" + publisher + "\', "  + to_string(pages) +", " + to_string(price) +");");
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
        
        managerInterface();
        break;
        }

        case 3: {

            cout << "Please enter the ISBN of the book you would like to remove: ";
            cin >> isbn;

            exit = sqlite3_open("library.db", &DB);
            string sql = "DELETE FROM BOOK WHERE ISBN = " + to_string(isbn) + ";";
            cout << sql;
            exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
            managerInterface();
            break;

        }
        case 4: defaultInterface(); break;  
    //     }
    }
}