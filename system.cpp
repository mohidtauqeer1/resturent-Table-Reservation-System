#include <iostream>
#include <fstream>
#include <conio.h>
#include <string>
#include <vector>
using namespace std;

// ---- File Paths ----
string loginFile = "C:\\Users\\Admin\\Desktop\\pf project\\data\\user.txt";
string slotsFile = "C:\\Users\\Admin\\Desktop\\pf project\\data\\slots_data.txt";
string menuFile = "C:\\Users\\Admin\\Desktop\\pf project\\data\\menu.txt";
string bookedreserFile = "C:\\Users\\Admin\\Desktop\\pf project\\data\\bookreservation.txt";
int lastResID = 0;

void line() {
    cout << "----------------------------------------\n";
}

void bigLine() {
    cout << "========================================\n";
}

void pause() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

void clearScreen() {
    system("cls");
}
void printTitle(const string& title) {
    system("cls");
    cout << "\n========================================\n";
    cout << "   " << title << endl;
    cout << "========================================\n\n";
}


struct Slot {
    string id;
    string time;
    string type;
    string table;
};

struct Reservation {
    string id;
    string username;
    string slotID;
    string menu;
    string phone;
    int guests;
};

struct Menu {
    string id;
    string item;
    int price;
};

// ---------------- Register User ----------------
void registerUser() {
    string username, password, role = "user";

    printTitle(" USER SIGNUP");

    // LOOP UNTIL VALID USERNAME
    while (true) {
        cout << "Enter Username: ";
        cin >> username;

        bool exists = false;
        ifstream file(loginFile);
        string u, p, r;

        while (file >> u >> p >> r) {
            if (u == username) {
                exists = true;
                break;
            }
        }
        file.close();

        if (!exists) break;

        cout << " Username already exists! Try another.\n";
    }

    // LOOP UNTIL PASSWORD IS STRONG
    while (true) {
        password = "";
        cout << "Enter Password: ";

        char ch;
        while ((ch = _getch()) != 13) {  // Hidden password
            password.push_back(ch);
            cout << "*";
        }
        cout << endl;

        bool upper=false, lower=false, digit=false, special=false;

        for (char c : password) {
            if (isupper(c)) {
                upper = true;
            }
            else if (islower(c)) {
                lower = true;
            }
            else if (isdigit(c)) {
                digit = true;
            }
            else {
                special = true;
            }
        }

        if (upper && lower && digit && special && password.length() >= 8) {
            break;
        }

        cout << "\n  Weak Password! Must include:\n";
        cout << "   - Uppercase letter\n";
        cout << "   - Lowercase letter\n";
        cout << "   - Digit\n";
        cout << "   - Special character\n";
        cout << "   - Minimum 8 characters\n";
        cout << "Try Again.\n\n";
    }


    // SAVE USER
    ofstream file(loginFile, ios::app);
    file << username << " " << password << " " << role << endl;
    file.close();

    cout << "\n✔ User registered successfully!\n";
}



// ---------------- User Login ----------------
string loginUser() {
    string inputUser, inputPass;
    string f_user, f_pass, f_role;

    cout << "\n--- USER LOGIN ---\n";

    int attempts = 0;

    while (attempts < 3) {
        cout << "Enter Username: ";
        cin >> inputUser;

        inputPass = "";
        cout << "Enter Password: ";             

        char ch;
        while ((ch = _getch()) != 13) {
            inputPass.push_back(ch);
            cout << "*";
        }
        cout << endl;

        ifstream file(loginFile);
        while (file >> f_user >> f_pass >> f_role) {
            if (f_user == inputUser && f_pass == inputPass && f_role == "user") {
                cout << "✔ Login Successful! Welcome " << inputUser << endl;
                pause();
                clearScreen();
                return inputUser;  
            }
        }
        file.close();

        attempts++;
        cout << "Wrong username or password! (" << attempts << "/3)\n";
    }

    cout << "Too many attempts.\n";
    return "";  
}



// ---------------- Admin Login ----------------
bool loginAdmin() {
    string username, password;
    string f_user, f_pass, f_role;

    cout << "\n--- ADMIN LOGIN ---\n";
    cout << "Enter Admin Username: ";
    cin >> username;
    cout << "Enter Admin Password: ";
    cin >> password;

    ifstream file(loginFile);
    while (file >> f_user >> f_pass >> f_role) {
        if (f_user == username && f_pass == password && f_role == "admin") {
            cout << "Admin Login Successful! Welcome " << username << endl;
            clearScreen();
            return true;
        }
    }
    file.close();

    cout << "Invalid Admin Credentials!\n";
    pause();
    return false;
}

// ---------------- Show Specific Slots ----------------
string showSlots() {
    int choice;
    string mealType;

    // -------- ASK UNTIL VALID MEAL TYPE --------
    while (true) {
        printTitle("Select Meal Type");
        cout << "1. Breakfast\n";
        cout << "2. Lunch\n";
        cout << "3. Dinner\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            mealType = "BREAKFAST";
        }
        else if (choice == 2) {
            mealType = "LUNCH";
        }
        else if (choice == 3) {
            mealType = "DINNER";
        }
        else {
            cout << "Invalid choice! Please try again.\n\n";
            continue;
        }
        break;
    }

    // -------- READ SLOTS INTO VECTOR --------
    ifstream file(slotsFile);
    if (!file.is_open()) {
        cout << "Error opening slots file!\n";
        return "";
    }

    vector<Slot> slots;
    Slot temp;

    // Read like: ID TIME TYPE TABLE
    while (file >> temp.id >> temp.time >> temp.type >> temp.table) {
        slots.push_back(temp);
    }
    file.close();

    // -------- DISPLAY MATCHING SLOTS --------
    cout << "\n---- Available " << mealType << " Slots ----\n";
    cout << "ID\tTIME\tTYPE\tTABLE\n";
    
    //     bool found = false;
    
    // for (int i = 0; i < slots.size(); i++) {
    //     if (slots[i].type == mealType) {
    
    //         cout << slots[i].id << "\t"
    //              << slots[i].time << "\t"
    //              << slots[i].type << "\t"
    //              << slots[i].table << endl;
    
    //         found = true;
    //     }
    // }


    bool found = false;
    for (auto &s : slots) {
        if (s.type == mealType) {
            cout << s.id << "\t"
                 << s.time << "\t"
                 << s.type << "\t"
                 << s.table << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No slots found for " << mealType << ".\n";
    }

    return mealType;
}


// ------------ Reservation ID Generatior -------------
string generateAlphaNumericID() {
    char prefix = 'A';
    int number = 1;

    ifstream fin("slot_id.txt");
    if (fin.is_open()) {
        fin >> prefix >> number;
        fin.close();
    }

    number++;

    if (number > 999) {
        number = 1;
        prefix++;
    }

    ofstream fout("slot_id.txt");
    fout << prefix << " " << number;
    fout.close();

    string numStr = to_string(number);

    if (numStr.length() == 1) numStr = "00" + numStr;
    else if (numStr.length() == 2) numStr = "0" + numStr;

    return string(1, prefix) + numStr;
}

// ---------------- Book Reservation ----------------
void bookReservation(const string& username) {

     string mealType = showSlots();

    string slotID;
    cout << "\nEnter Slot ID to Book: ";
    cin >> slotID;

    // ---------------- LOAD SLOTS INTO VECTOR ----------------
    vector<Slot> slots;
    ifstream slotRead(slotsFile);

    Slot temp;
    bool exists = false;
    
    while (slotRead >> temp.id >> temp.time >> temp.type >> temp.table) {
        if (temp.id == slotID) exists = true;
        slots.push_back(temp);
    }
    slotRead.close();

    if (!exists) {
        cout << "Invalid Slot ID!\n";
        return;
    }

    // ---------------- REMOVE SELECTED SLOT ----------------
    vector<Slot> updatedSlots;
    for (auto& s : slots) {
        if (s.id != slotID)
            updatedSlots.push_back(s);
    }

    // ---------------- Rewrite File ----------------
    ofstream slotWrite(slotsFile);
    for (auto& s : updatedSlots) {
        slotWrite << s.id << " " << s.time << " " << s.type << " " << s.table << endl;
    }
    slotWrite.close();

    // ---------------------------------------------
    cout << "\nSlot Booked Successfully!\n";
    cout << "Your table is confirmed for " << mealType
     << " at " << temp.time << endl;

    // ----------- Menu selection -----------
    string wantMenu;
    cout << "\nWould you like to order from the menu? (yes/no): ";
    cin >> wantMenu;

    string selectedMenu = "None";

    if (wantMenu == "yes") {
        ifstream menu(menuFile);
        string item;

        printTitle("Available Menu");
        while (getline(menu, item))
            cout << item << endl;

        menu.close();

        cout << "\nEnter Menu Name: ";
        cin.ignore();
        getline(cin, selectedMenu);
    }

    // ----------- Extra Details -----------
    string phone;
    int guests;

    cout << "Enter Phone Number: ";
    cin >> phone;

    cout << "Enter Number of Guests: ";
    cin >> guests;

    string ID = generateAlphaNumericID();

    // ---------------- LOAD + ADD RESERVATION ----------------
    vector<Reservation> list;
    Reservation r;

    ifstream in(bookedreserFile);
    while (in >> r.id >> r.username >> r.slotID >> r.menu >> r.phone >> r.guests)
        list.push_back(r);
    in.close();

     // ---------------- New Reservation ----------------
    Reservation newRes;
    newRes.id = ID;
    newRes.username = username;
    newRes.slotID = slotID;
    newRes.menu = selectedMenu;
    newRes.phone = phone;
    newRes.guests = guests;

    list.push_back(newRes);

    // ---------------- Rewrite File ----------------
    ofstream out(bookedreserFile);
    for (auto& x : list) {
        out << x.id << " " << x.username << " " << x.slotID << " "
            << x.menu << " " << x.phone << " " << x.guests << endl;
    }
    out.close();

    cout << "\nReservation Successful!\nYour Reservation ID: " << ID << endl;
}

// ------------ Only Menu Show -------------
void showMenu() {
    ifstream file(menuFile);
    if (!file.is_open()) {
        cout << "Error opening menu file!\n";
        return;
    }

    cout << "\n------------- MENU -------------\n";
    cout << "ID\tITEM\t\tPRICE\n";
    cout << "--------------------------------\n";

    string id, item;
    int price;

    while (file >> id >> item >> price) {
        cout << id << "\t" << item << "\t\t" << price << endl;
    }
    pause();
}

// ------------ View Reservation -------------
void viewReservation(const string& username) {
    vector<Reservation> list;
    Reservation r;

    ifstream file(bookedreserFile);
    while (file >> r.id >> r.username >> r.slotID >> r.menu >> r.phone >> r.guests)
        list.push_back(r);
    file.close();

    bool found = false;

    cout << "\n----- Your Reservations -----\n";

    for (auto& x : list) {
        if (x.username == username) {
            found = true;
            cout << "Reservation ID: " << x.id << endl;
            cout << "Slot ID: " << x.slotID << endl;
            cout << "Menu: " << x.menu << endl;
            cout << "Phone: " << x.phone << endl;
            cout << "Guests: " << x.guests << endl;
            cout << "-----------------------------\n";
        }
    }

    if (!found)
        cout << "No reservations found for " << username << endl;
        pause();
}

// ------------ Update Reservation -------------
void updateReservation(const string& username) {
    vector<Reservation> list;
    Reservation r;

    ifstream file(bookedreserFile);
    while (file >> r.id >> r.username >> r.slotID >> r.menu >> r.phone >> r.guests)
        list.push_back(r);
    file.close();

    string id;
    cout << "\nEnter Reservation ID to Update: ";
    cin >> id;

    bool found = false;
    for (auto &x : list) {
        if (x.id == id && x.username == username) {
            found = true;

            int choice;
            cout << "\nWhat do you want to update?\n";
            cout << "1. Change Slot\n";
            cout << "2. Change Menu\n";
            cout << "3. Change Phone\n";
            cout << "4. Change Guests\n";
            cout << "Enter choice: ";
            cin >> choice;

            if (choice == 1) {
                string newSlot = showSlots();
                cout << "Enter New Slot ID: ";
                cin >> x.slotID;
                cout << "Slot updated!\n";
            }
            else if (choice == 2) {
                cout << "Enter New Menu: ";
                cin.ignore();
                getline(cin, x.menu);
                cout << "Menu updated!\n";
            }
            else if (choice == 3) {
                cout << "Enter New Phone: ";
                cin >> x.phone;
                cout << "Phone updated!\n";
            }
            else if (choice == 4) {
                cout << "Enter New Guests Count: ";
                cin >> x.guests;
                cout << "Guests updated!\n";
            }
            else {
                cout << "Invalid choice!\n";
            }
            break;
        }
    }

    if (!found) {
        cout << "Reservation not found!\n";
        return;
    }

    // Rewrite file
    ofstream out(bookedreserFile);
    for (auto &x : list) {
        out << x.id << " " << x.username << " " << x.slotID << " "
            << x.menu << " " << x.phone << " " << x.guests << endl;
    }
    out.close();

    cout << "Reservation Updated Successfully!\n";
    pause();
}


// ------------ Cancel Reservation -------------
void cancelReservation(const string& username) {
   string id;
    cout << "\nEnter Reservation ID to Cancel: ";
    cin >> id;

    vector<Reservation> list, updated;
    Reservation r;

    ifstream in(bookedreserFile);

    while (in >> r.id >> r.username >> r.slotID >> r.menu >> r.phone >> r.guests)
        list.push_back(r);
    in.close();

    bool deleted = false;
    for (auto& x : list) {
        if (x.id == id && x.username == username)
            deleted = true;
        else
            updated.push_back(x);
    }

    ofstream out(bookedreserFile);
    for (auto& x : updated) {
        out << x.id << " " << x.username << " " << x.slotID << " "
            << x.menu << " " << x.phone << " " << x.guests << endl;
    }
    out.close();

    if (deleted)
       {
         cout << "Reservation cancelled successfully!\n";
    }
    else
        {cout << "Reservation ID not found!\n";
        }
        pause();

}

// ------------ Add Slots -------------
void addSlots() {
    string slotId, time, slotType, tableNo;
    string f_id, f_time, s_type, t_no;

    cout << "Enter Slot TYPE (BREAKFAST, LUNCH, DINNER)\n";
    cin >> slotType;
    if (slotType == "BREAKFAST") {
        time = "9:00-12:00";
    }
    else if (slotType == "LUNCH") {
        time = "1:00-4:00";
    }
    else if (slotType == "DINNER") {
        time = "6:00-12:00";
    }
    bool tableTaken;
    do {
        tableTaken = false;
        cout << "Enter Slot TABLE NO: ";
        cin >> tableNo;

        ifstream readFile(slotsFile);
        while (readFile >> f_id >> f_time >> s_type >> t_no) {
            if (t_no == "table:" + tableNo) { 
                tableTaken = true;
                cout << "⚠ Table already reserved! Please enter another table.\n";
                break;
            }
        }
        readFile.close();

    } while (tableTaken);  // repeat if table is taken

    ofstream file(slotsFile, ios::app);
    if (!file.is_open()) {
        cout << "Error opening file!\n";
        return;
    }

    file << generateAlphaNumericID() << " "  << time << " "  << slotType << " "  << "table:" << tableNo << endl;

    file.close();
    cout << "Slot added successfully!\n";
    pause();
}

// ------------ Add Menu -------------
void addMenu(){
     vector<Menu> menuList;
    ifstream file(menuFile);

    // Load previous menu items
    Menu temp;
    while (file >> temp.id >> temp.item >> temp.price) {
        menuList.push_back(temp);
    }
    file.close();

    // Input new menu item
    Menu newItem;
    cout << "\nEnter Item ID: ";
    cin >> newItem.id;
    cout << "Enter Item Name: ";
    cin >> newItem.item;
    cout << "Enter Price: ";
    cin >> newItem.price;

    // Add to vector
    menuList.push_back(newItem);

    // Save back to file
    ofstream out(menuFile);
    for (auto &m : menuList) {
        out << m.id << " " << m.item << " " << m.price << "\n";
    }
    out.close();

    cout << "\nMenu item added successfully!\n";
    
}

// ------------ Admin Delete Slots -------------

void deleteSlot() {
    vector<Slot> slots;
    Slot s;

    ifstream file(slotsFile);
    while (file >> s.id >> s.time >> s.type >> s.table)
        slots.push_back(s);
    file.close();

    string id;
    cout << "\nEnter Slot ID to Delete: ";
    cin >> id;

    bool found = false;
    vector<Slot> updated;

    for (auto &x : slots) {
        if (x.id == id) {
            found = true;
        } else {
            updated.push_back(x);
        }
    }

    if (!found) {
        cout << "Slot not found!\n";
        return;
    }

    ofstream out(slotsFile);
    for (auto &x : updated) {
        out << x.id << " " << x.time << " " << x.type << " " << x.table << endl;
    }
    out.close();

    cout << "Slot deleted successfully!\n";
}

// ------------ Admin Edit Slots -------------
void editSlot() {
    vector<Slot> slots;
    Slot s;

    ifstream file(slotsFile);
    while (file >> s.id >> s.time >> s.type >> s.table)
        slots.push_back(s);
    file.close();

    string id;
    cout << "\nEnter Slot ID to Edit: ";
    cin >> id;

    bool found = false;

    for (auto &x : slots) {
        if (x.id == id) {
            found = true;

            int choice;
            cout << "\n1. Change Time\n2. Change Type\n3. Change Table\nEnter Choice: ";
            cin >> choice;

            if (choice == 1) {
                cout << "Enter New Time: ";
                cin >> x.time;
            }
            else if (choice == 2) {
                cout << "Enter Meal Type (BREAKFAST/LUNCH/DINNER): ";
                cin >> x.type;
            }
            else if (choice == 3) {
                cout << "Enter New Table: ";
                cin >> x.table;
            }
            else {
                cout << "Invalid choice!\n";
            }
            break;
        }
    }

    if (!found) {
        cout << "Slot not found!\n";
        return;
    }

    ofstream out(slotsFile);
    for (auto &x : slots)
        out << x.id << " " << x.time << " " << x.type << " " << x.table << endl;

    out.close();

    cout << "Slot updated successfully!\n";
}

// ------------ Delete Menu Item -------------

void deleteMenuItem() {
    vector<Menu> menuList;
    Menu m;

    ifstream file(menuFile);
    while (file >> m.id >> m.item >> m.price)
        menuList.push_back(m);
    file.close();

    string id;
    cout << "\nEnter Menu ID to Delete: ";
    cin >> id;

    bool found = false;
    vector<Menu> updated;

    for (auto &x : menuList) {
        if (x.id == id) {
            found = true;
        } else {
            updated.push_back(x);
        }
    }

    if (!found) {
        cout << "Menu item not found!\n";
        return;
    }

    ofstream out(menuFile);
    for (auto &x : updated)
        out << x.id << " " << x.item << " " << x.price << endl;

    out.close();

    cout << "Menu item deleted successfully!\n";
}

// ------------ Edit Menu Item Slots -------------
void editMenuItem() {
    vector<Menu> menuList;
    Menu m;

    ifstream file(menuFile);
    while (file >> m.id >> m.item >> m.price)
        menuList.push_back(m);
    file.close();

    string id;
    cout << "\nEnter Menu ID to Edit: ";
    cin >> id;

    bool found = false;

    for (auto &x : menuList) {
        if (x.id == id) {
            found = true;

            int choice;
            cout << "\n1. Change Item Name\n2. Change Price\nEnter Choice: ";
            cin >> choice;

            if (choice == 1) {
                cout << "Enter New Item Name: ";
                cin >> x.item;
            }
            else if (choice == 2) {
                cout << "Enter New Price: ";
                cin >> x.price;
            }
            else {
                cout << "Invalid choice!\n";
            }
            break;
        }
    }

    if (!found) {
        cout << "Menu item not found!\n";
        return;
    }

    ofstream out(menuFile);
    for (auto &x : menuList)
        out << x.id << " " << x.item << " " << x.price << endl;

    out.close();

    cout << "Menu updated successfully!\n";
}

// ------------ Search Reservations -------------
void searchReservation() {
    vector<Reservation> list;
    Reservation r;

    ifstream file(bookedreserFile);
    while (file >> r.id >> r.username >> r.slotID >> r.menu >> r.phone >> r.guests)
        list.push_back(r);
    file.close();

    int choice;
    cout << "\nSearch Reservations:\n";
    cout << "1. By Username\n";
    cout << "2. By Meal Type (slot type)\n";
    cout << "3. By Slot ID\n";
    cout << "Enter Choice: ";
    cin >> choice;

    string input;
    bool found = false;

    if (choice == 1) {
        cout << "Enter Username: ";
        cin >> input;

        for (auto &x : list) {
            if (x.username == input) {
                found = true;
                cout << x.id << " " << x.username << " " << x.slotID
                     << " " << x.menu << " " << x.phone << " " << x.guests << endl;
            }
        }
    }
    else if (choice == 2) {
        cout << "Enter Type (BREAKFAST/LUNCH/DINNER): ";
        cin >> input;

        for (auto &x : list) {
            // slot type is inside slots file so load and check
            ifstream slot(slotsFile);
            Slot s;
            while (slot >> s.id >> s.time >> s.type >> s.table) {
                if (s.id == x.slotID && s.type == input) {
                    found = true;
                    cout << x.id << " " << x.username << " " << x.slotID
                         << " " << x.menu << " " << x.phone << " " << x.guests << endl;
                }
            }
        }
    }
    else if (choice == 3) {
        cout << "Enter Slot ID: ";
        cin >> input;

        for (auto &x : list) {
            if (x.slotID == input) {
                found = true;
                cout << x.id << " " << x.username << " " << x.slotID
                     << " " << x.menu << " " << x.phone << " " << x.guests << endl;
            }
        }
    }
    else {
        cout << "Invalid choice!\n";
        return;
    }

    if (!found) {
        cout << "No matching reservation found.\n";
    }
}


// ------------ Show Booked Slots -------------
void BookedReservation(){
    vector<Reservation> list;
    Reservation r;

    ifstream file(bookedreserFile);
    while (file >> r.id >> r.username >> r.slotID >> r.menu >> r.phone >> r.guests)
        list.push_back(r);
    file.close();

    printTitle("All Booked Reservations");

    for (auto& x : list) {
        cout << x.id << " "
             << x.username << " "
             << x.slotID << " "
             << x.menu << " "
             << x.phone << " "
             << x.guests << endl;
    }
}

int main() {

    string mainChoice;
    string loggedUser = "";
    bool isAdmin = false;

    while (true) {
       printTitle("RESTAURANT RESERVATION SYSTEM");
        cout << "1. Register User\n";
        cout << "2. Login User\n";
        cout << "3. Login Admin\n";
        cout << "4. Exit\n";
        cout << "Enter Choice: ";
        cin >> mainChoice;
        // ----------- Register User -----------
        if (mainChoice == "1") {
            registerUser();
            pause();
            clearScreen();
        }

        // ----------- User Login -----------
        else if (mainChoice == "2") {

    loggedUser = loginUser(); 
    if (loggedUser != "") {

        string userChoice;
        while (true) {
            clearScreen();
            printTitle("USER MENU ");
            cout << "1. Book Reservation\n";
            line();
            cout << "2. View Reservation\n";
            line();
            cout << "3. Cancel Reservation\n";
            line();
            cout << "4. Show Slots\n";
            line();
            cout << "5. Update Reservation\n";
            line();
            cout << "6. View Menu\n";
            line();
            cout << "7. Logout\n";
            line();
            cout << "Enter Choice: ";
            cin >> userChoice;

            if (userChoice == "1")
               { 
                bookReservation(loggedUser);
                pause();
                clearScreen();
            }
            else if (userChoice == "2")
               {
                 viewReservation(loggedUser);
                clearScreen();

            }
            else if (userChoice == "3")
               { 
                clearScreen();
                cancelReservation(loggedUser);
            }
            else if (userChoice == "4")
               { 

                showSlots();
                pause();
                clearScreen();
            }
            else if (userChoice == "5")
               { 

                updateReservation(loggedUser);
                clearScreen();

            }
            else if (userChoice == "6")
              {  

                showMenu();
                clearScreen();

            }
            else if (userChoice == "7") {
                cout << "Logged out successfully!\n";
                pause();
                clearScreen();
                break;
            }
            else
               { cout << "Invalid choice!\n";
            }
        }
    }
}


        // ----------- Admin Login -----------
       else if (mainChoice == "3") {

    if (loginAdmin()) {

        string adminChoice;
        while (true) {
            clearScreen();
            printTitle("ADMIN MENU");
            cout << "1. Show All Slots\n";
            line();
            cout << "2. Add Slots\n";
            line();
            cout << "3. Edit Slot\n";
            line();
            cout << "4. Delete Slot\n";
            line();
            cout << "5. Add Menu\n";
            line();
            cout << "6. Edit Menu\n";
            line();
            cout << "7. Delete Menu\n";
            line();
            cout << "8. Show Menu\n";
            line();
            cout << "9. Search Reservation\n";
            line();
            cout << "10. Show Booked Reservation\n";
            line();
            cout << "11. Logout\n";
            line();
            cout << "Enter Choice: ";
            cin >> adminChoice;

            if (adminChoice == "1") {
                showSlots();
                  pause();
                  clearScreen();
            }
            else if (adminChoice == "2") {
                addSlots();
                 pause();
                  clearScreen();
            }
            else if (adminChoice == "3") {
                editSlot();
                 pause();
                  clearScreen();
            }
            else if (adminChoice == "4") {
                deleteSlot();
                 pause();
                  clearScreen();
            }
            else if (adminChoice == "5"){
                 addMenu();
                  pause();
                  clearScreen();
            }
            else if (adminChoice == "6"){
                 editMenuItem();
                  pause();
                  clearScreen();
            }
            else if (adminChoice == "7"){
                 deleteMenuItem();
                  pause();
                  clearScreen();
            }
            else if (adminChoice == "8") {
                showMenu();
                 pause();
                  clearScreen();
            }
            else if (adminChoice == "9") {
                searchReservation();
                 pause();
                  clearScreen();
            }
            else if (adminChoice == "10") {
                BookedReservation();
                 pause();
                  clearScreen();
            }
            else if (adminChoice == "11") {
                cout << "Admin logged out!\n";
                 pause();
                  clearScreen();
                break;
            }
            else {
                cout << "Invalid choice!\n";
            }
        }
    }
}


        // ----------- EXIT -----------
        else if (mainChoice == "4") {
            cout << "\nExiting Program...\n";
            break;
        }

        else {
            cout << "Invalid Choice! Try again.\n";
        }
    }

    return 0;
}