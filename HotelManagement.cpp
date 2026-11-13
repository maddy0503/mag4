#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

/*
 * Hotel Management System
 * Rules:
 *  - 10 rooms: 101-104 Single, 201-204 Double, 301-302 Suite
 *  - Phone number: exactly 10 digits
 *  - Nights: 1 to 30
 *  - Customer name: not empty, letters and spaces only
 *  - A booked room cannot be booked again
 *  - Check-out / cancel only on a booked room
 *  - Rejected operations never change room data
 */

class Room {
private:
    int roomNumber;
    string type;
    double pricePerNight;
    bool booked;
    string customerName;
    string phone;
    int nights;

public:
    Room(int no, const string &t, double price)
        : roomNumber(no), type(t), pricePerNight(price), booked(false), nights(0) {}

    int getRoomNumber() const { return roomNumber; }
    bool isBooked() const { return booked; }
    double getBill() const { return nights * pricePerNight; }

    void book(const string &name, const string &ph, int n) {
        customerName = name;
        phone = ph;
        nights = n;
        booked = true;
    }

    void release() {
        customerName.clear();
        phone.clear();
        nights = 0;
        booked = false;
    }

    void display() const {
        cout << "Room " << roomNumber << " | " << left << setw(7) << type
             << " | Rs." << fixed << setprecision(2) << pricePerNight << "/night | "
             << (booked ? "BOOKED" : "AVAILABLE") << "\n";
    }

    void displayBooking() const {
        cout << "----- Booking Details -----\n";
        cout << "Room Number : " << roomNumber << " (" << type << ")\n";
        cout << "Customer    : " << customerName << "\n";
        cout << "Phone       : " << phone << "\n";
        cout << "Nights      : " << nights << "\n";
        cout << "Bill so far : Rs." << fixed << setprecision(2) << getBill() << "\n";
    }
};

class Hotel {
private:
    vector<Room> rooms;
    int successfulBookings;
    int successfulCheckouts;
    int cancellations;
    int rejectedOperations;
    double totalRevenue;

    Room *findRoom(int number) {
        for (size_t i = 0; i < rooms.size(); i++)
            if (rooms[i].getRoomNumber() == number) return &rooms[i];
        return NULL;
    }

    static bool validName(const string &s) {
        bool hasLetter = false;
        for (size_t i = 0; i < s.size(); i++) {
            unsigned char c = s[i];
            if (isalpha(c)) hasLetter = true;
            else if (c != ' ') return false;
        }
        return hasLetter;
    }

    static bool validPhone(const string &s) {
        if (s.size() != 10) return false;
        for (size_t i = 0; i < s.size(); i++)
            if (!isdigit(static_cast<unsigned char>(s[i]))) return false;
        return true;
    }

public:
    Hotel()
        : successfulBookings(0), successfulCheckouts(0), cancellations(0),
          rejectedOperations(0), totalRevenue(0.0) {
        for (int i = 1; i <= 4; i++) rooms.push_back(Room(100 + i, "Single", 1500));
        for (int i = 1; i <= 4; i++) rooms.push_back(Room(200 + i, "Double", 2500));
        for (int i = 1; i <= 2; i++) rooms.push_back(Room(300 + i, "Suite", 5000));
    }

    void displayAllRooms() const {
        cout << "----- All Rooms -----\n";
        for (size_t i = 0; i < rooms.size(); i++) rooms[i].display();
    }

    void displayAvailableRooms() const {
        cout << "----- Available Rooms -----\n";
        bool any = false;
        for (size_t i = 0; i < rooms.size(); i++) {
            if (!rooms[i].isBooked()) { rooms[i].display(); any = true; }
        }
        if (!any) cout << "No rooms available.\n";
    }

    void bookRoom(int number, const string &name, const string &phone, int nights) {
        Room *r = findRoom(number);
        if (!r) { rejectedOperations++; cout << "Rejected: room " << number << " does not exist.\n"; return; }
        if (r->isBooked()) { rejectedOperations++; cout << "Rejected: room " << number << " is already booked.\n"; return; }
        if (!validName(name)) { rejectedOperations++; cout << "Rejected: invalid customer name.\n"; return; }
        if (!validPhone(phone)) { rejectedOperations++; cout << "Rejected: phone number must be exactly 10 digits.\n"; return; }
        if (nights < 1 || nights > 30) { rejectedOperations++; cout << "Rejected: nights must be between 1 and 30.\n"; return; }

        r->book(name, phone, nights);
        successfulBookings++;
        cout << "Room " << number << " booked successfully for " << nights << " night(s).\n";
    }

    void checkout(int number) {
        Room *r = findRoom(number);
        if (!r) { rejectedOperations++; cout << "Rejected: room " << number << " does not exist.\n"; return; }
        if (!r->isBooked()) { rejectedOperations++; cout << "Rejected: room " << number << " is not booked.\n"; return; }

        double bill = r->getBill();
        r->displayBooking();
        cout << "Total amount paid: Rs." << fixed << setprecision(2) << bill << "\n";
        totalRevenue += bill;
        r->release();
        successfulCheckouts++;
        cout << "Check-out completed.\n";
    }

    void cancelBooking(int number) {
        Room *r = findRoom(number);
        if (!r) { rejectedOperations++; cout << "Rejected: room " << number << " does not exist.\n"; return; }
        if (!r->isBooked()) { rejectedOperations++; cout << "Rejected: room " << number << " is not booked.\n"; return; }

        r->release();
        cancellations++;
        cout << "Booking for room " << number << " cancelled.\n";
    }

    void viewBooking(int number) {
        Room *r = findRoom(number);
        if (!r) { rejectedOperations++; cout << "Rejected: room " << number << " does not exist.\n"; return; }
        if (!r->isBooked()) { cout << "Room " << number << " is currently available (no booking).\n"; return; }
        r->displayBooking();
    }

    void displaySummary() const {
        cout << "----- Hotel Summary -----\n";
        cout << "Successful bookings    : " << successfulBookings << "\n";
        cout << "Successful check-outs  : " << successfulCheckouts << "\n";
        cout << "Cancellations          : " << cancellations << "\n";
        cout << "Rejected operations    : " << rejectedOperations << "\n";
        cout << "Total revenue          : Rs." << fixed << setprecision(2) << totalRevenue << "\n";
    }
};

bool readInt(const string &prompt, int &value) {
    cout << prompt;
    if (!(cin >> value)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
}

void showMenu() {
    cout << "\n===== Hotel Management System =====\n";
    cout << "1. Display All Rooms\n";
    cout << "2. Display Available Rooms\n";
    cout << "3. Book Room\n";
    cout << "4. Check-out\n";
    cout << "5. Cancel Booking\n";
    cout << "6. View Booking Details\n";
    cout << "7. Display Summary\n";
    cout << "8. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    Hotel hotel;
    int choice = 0;

    do {
        showMenu();
        if (!readInt("", choice)) {
            cout << "Invalid choice. Enter a number between 1 and 8.\n";
            choice = 0;
            continue;
        }

        switch (choice) {
        case 1:
            hotel.displayAllRooms();
            break;
        case 2:
            hotel.displayAvailableRooms();
            break;
        case 3: {
            int room, nights;
            string name, phone;
            if (!readInt("Enter room number: ", room)) { cout << "Rejected: invalid room number.\n"; break; }
            cout << "Enter customer name: ";
            getline(cin, name);
            cout << "Enter phone number (10 digits): ";
            getline(cin, phone);
            if (!readInt("Enter number of nights: ", nights)) { cout << "Rejected: invalid number of nights.\n"; break; }
            hotel.bookRoom(room, name, phone, nights);
            break;
        }
        case 4: {
            int room;
            if (!readInt("Enter room number: ", room)) { cout << "Rejected: invalid room number.\n"; break; }
            hotel.checkout(room);
            break;
        }
        case 5: {
            int room;
            if (!readInt("Enter room number: ", room)) { cout << "Rejected: invalid room number.\n"; break; }
            hotel.cancelBooking(room);
            break;
        }
        case 6: {
            int room;
            if (!readInt("Enter room number: ", room)) { cout << "Rejected: invalid room number.\n"; break; }
            hotel.viewBooking(room);
            break;
        }
        case 7:
            hotel.displaySummary();
            break;
        case 8:
            cout << "Thank you. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Enter a number between 1 and 8.\n";
        }
    } while (choice != 8);

    return 0;
}
