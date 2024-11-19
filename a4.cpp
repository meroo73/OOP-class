/***********************************************************************
Matr.-Nr:                     3140507
Nachname/Surname:             Alaghbari
Vorname/Given name:           Ameer Adin
Uni-Email:                    ameer.alaghbari@stud.uni-due.de
Studiengang/Course of studis: B.Sc ISE CE (Software)
***********************************************************************/
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;

const int Person_Consum = 200, SquareMeter_Consum = 9, Water_Heating_Consum = 550;
float price;
const int COLUMN_WIDTH = 35;

enum Use { ONCE, DAILY, MO_FR, SA_SU, WEEKLY };


//struct to for custormers  and has the next pointer link costumer to the next one
struct Consumer {
    string description;
    float watt;
    float watt_standby;
    float hours;
    Use use;
    Consumer* next; // This pointer creates the linked list


};
// stucturt  of household
struct Household {
    string city;
    int square_m;
    int persons;
    int n_devices;
    bool electric_water_heating;
    float price;
    string Power_supply;
    Consumer* consumers;  // This pointer points to the head of the linked list

};

void Input_Use(const string what, Use& use) {
    char choice;
    std::cout << "how often it will be used?" << std::endl;
    std::cout << "once   (o)" << std::endl;
    std::cout << "daily  (d)" << std::endl;
    std::cout << "mo_fr  (m)" << std::endl;
    std::cout << "sa_su  (s)" << std::endl;
    std::cout << "weekly (w)? ";
    std::cin >> choice;

    switch (choice) {
        case 'o': use = ONCE; break;
        case 'd': use = DAILY; break;
        case 'm': use = MO_FR; break;
        case 's': use = SA_SU; break;
        case 'w': use = WEEKLY; break;
        default:
            cout << "You didnt choose one of options above please choose again ";
            Input_Use(what, use);
            break;
    }
}


//creates COnsumer node and returns the pointer to it
Consumer* CREATE_CONSUMER() {
    Consumer* New_Consumer = new Consumer;

    cout << "What is the description of the power consumer? ";
    cin >>  New_Consumer->description;

    cout << "How many watts does it consume? ";
    cin >> New_Consumer->watt;

    cout << "How many watts in standby mode? ";
    cin >> New_Consumer->watt_standby;

    cout << "How often is it used?\n";
    Input_Use("Choose usage frequency: ", New_Consumer->use);

    cout << "How many hours will it operate? ";
    cin >> New_Consumer->hours;
    New_Consumer->next = nullptr;

    return New_Consumer;
}

Consumer* CREATE_NEW_CONSUM(string description, float watt, float watt_standby, Use use, float hours) {
    Consumer* New_Consumer = new Consumer;

    New_Consumer->description = description;
    New_Consumer->watt = watt;
    New_Consumer->watt_standby = watt_standby;
    New_Consumer->use = use;
    New_Consumer->hours = hours;
    New_Consumer->next = nullptr;

    return New_Consumer;
}

// addds a new consumer to the beginneing of the linked lists of conusmer sin a household
void Add_Consumer_To_Household(Household* house, Consumer* Number_Consumer) {
    Number_Consumer->next = house->consumers;
    house->consumers = Number_Consumer;
}

float Annual_Hours_Of_Use(const Consumer& consumer) {
    float annual_hours = 0;
    switch (consumer.use) {
        case ONCE:  annual_hours = consumer.hours; break;
        case DAILY: annual_hours = consumer.hours * 365; break;
        case MO_FR: annual_hours = consumer.hours * 260; break;
        case SA_SU: annual_hours = consumer.hours * 104; break;
        case WEEKLY:annual_hours = consumer.hours * 52; break;
    }
    return annual_hours;
}

// Function to calculate the annual hours of standby for a consumer
float Annual_Hours_Of_Standby(const Consumer& consumer) {
    return 8760 - Annual_Hours_Of_Use(consumer);
}


// Function to calculate the annual power consumption for a consumer in kWh
float Annual_Kwh(const Consumer& consumer) {
    // Calculate the total hours of use and standby
    float totalHours = Annual_Hours_Of_Use(consumer) + Annual_Hours_Of_Standby(consumer);

    // Calculate the energy consumption in kilowatt-hours
    float energyConsumption = (Annual_Hours_Of_Use(consumer) * consumer.watt + Annual_Hours_Of_Standby(consumer) * consumer.watt_standby) / 1000.0;

    return energyConsumption;
}



//linked list of the move up function that uses moves consumer to the top of the node k-1
Consumer* Move_Up(Consumer* head, int k) {
    if (k < 2) return head;// k is less than 2, there's no need to move any node.

    Consumer* prev = nullptr;
    Consumer* current = head;
    Consumer* next = head->next;
    int i = 1;

    while (i < k && next != nullptr) {
        prev = current;
        current = next;
        next = next->next;
        i++;
    }
//Move through the linked list until either k positions are reached
    if (i == k) {
        current->next = prev;
        if (prev != nullptr) {
            prev->next = next;
        } else {
            head = current;
        }
    }

    return head;
}




//
void Print_Consumer(Consumer* consumer, int number, Household* household) {
    // Check for null pointer
    if (consumer == nullptr) {
        std::cout << " error please try again " << endl;
        return;
    }

    // Print consumer information
    cout << std::right << setw(COLUMN_WIDTH) << number << ": " << consumer->description
              << " (at address: " << consumer << ")" << endl;
    cout << right << (COLUMN_WIDTH) << "power consumption: " << left << fixed << setprecision(2) << consumer->watt << " W" << endl;
    cout << right << (COLUMN_WIDTH) << "power consumption standby: " << std::left << std::fixed << std::setprecision(2) << consumer->watt_standby << " W" << std::endl;
    cout << right << (COLUMN_WIDTH) << "annual hours of use: " << std::left << std::fixed << std::setprecision(2) << Annual_Hours_Of_Use(*consumer) << " h" << std::endl;
    cout << right << (COLUMN_WIDTH) << "annual hours of standby: " << std::left << std::fixed << std::setprecision(2) << Annual_Hours_Of_Standby(*consumer) << " h" << std::endl;
    cout << right << (COLUMN_WIDTH) << "annual consumption: " << std::left << std::fixed << std::setprecision(2) << Annual_Kwh(*consumer) << " kWh" << std::endl;
    cout << right << (COLUMN_WIDTH) << "annual costs: " << std::left << std::fixed << std::setprecision(2) << (Annual_Kwh(*consumer) * price) << " EUR" << std::endl;
}


// using the Print_Household with the new added feature "at address";
void Print_Household(Household* household, float price_KWH, int household_number)
{

    cout << "H O U S H E H O L D  N O.  " << household_number << "  P O W E R   C O N S U M P T I O N\n";
    string hot_water_choice;
    float PowerConsum = SquareMeter_Consum*household->square_m;
    float allpersonsConsum;
    cout << left << setw(COLUMN_WIDTH) << "----------------------------------------------------------------------" << "\n";

    if (household->electric_water_heating == true)
        {
        hot_water_choice = "yes";
        allpersonsConsum = Water_Heating_Consum * household->persons;
            }
            else {
                    hot_water_choice = "no";
                allpersonsConsum= Person_Consum * household->persons;
                        }

    cout << left << setw(COLUMN_WIDTH) << "city: " << right << household->city << "  (at address: " << household << ") \n";
    cout << left << setw(COLUMN_WIDTH) << "price for one kWh: " << right << price_KWH << " ct/kWh\n";
    cout << left << setw(COLUMN_WIDTH) << "square meters: " << right << household->square_m << " qm\n";
    cout << left << setw(COLUMN_WIDTH) << "Supplier : " << right << household->Power_supply << endl;
    cout << left << setw(COLUMN_WIDTH) << "persons: " << right << household->persons << "\n";
    cout << left << setw(COLUMN_WIDTH) << "water heated using electricity: " << right << hot_water_choice << "\n";
    cout << left << setw(COLUMN_WIDTH) << "list of consumers\n" << endl;
    cout << setw(2*COLUMN_WIDTH) << "----------------------------------------------------------------------" << "\n";



    Consumer* Consumer_NOW = household->consumers;
    float Total_Consum_SquareMeters = PowerConsum;
    float Total_Consum_Persons = allpersonsConsum;
    float Total_Consum = Total_Consum_SquareMeters + Total_Consum_Persons;
    float Total_Costs = Total_Consum * price_KWH / 100;
    int number = 1;

    while (Consumer_NOW != nullptr)
        {

            Print_Consumer(Consumer_NOW, number, household);
            float Annual_Kwh_Consumer = Annual_Kwh(*Consumer_NOW);
            Total_Consum += Annual_Kwh_Consumer;
            Total_Costs += Annual_Kwh_Consumer * household->price;
            Consumer_NOW = Consumer_NOW->next;
            number++;
                }

    cout << left << setw(COLUMN_WIDTH) << "----------------------------------------------------------------------" << "\n";
    cout << left << setw(COLUMN_WIDTH) << "power consumption square meters: " << right << fixed << setprecision(1) << PowerConsum << " kWh\n";
    cout << left << setw(COLUMN_WIDTH) << "power consumption all persons: " << right << fixed << setprecision(1) << allpersonsConsum << " kWh\n";
    cout << left << setw(COLUMN_WIDTH) << "total annual power consumption: " << right << fixed << setprecision(1) <<  Total_Consum << " kWh\n";
    cout << left << setw(COLUMN_WIDTH) << "total annual power costs: " << right << fixed << setprecision(1) << Total_Costs << " EUR\n\n";
    }



void Input_Household(Household* house) {
    std::cout << "how many square metres does the household have? ";
    std::cin >> house->square_m;
    std::cout << "how many persons live in this household? ";
    std::cin >> house->persons;

    string heat;
    cout << "is hot water heated using electricity? (y(es) or n(o)) ";
    cin >> heat;
    house->electric_water_heating = (heat == "y" || heat == "yes");
    house->consumers = nullptr;

     cout << "what is the price for one kWh in EUR?";
    cin >> house -> price;

    cout << "who is the power supplier?";
    cin >> house -> Power_supply;
}



void Input_City(string* city) {
     cin >> *city;

 }

  void input_integer (string str, int &x){

     cout << str << endl;
     cin >> x;
 }


Household* Copy_Consumers(const Household* fromHouse, Household* toHouse) {
    // Create a reversed copy of consumers from fromHouse
    Consumer* reversedCopy = nullptr;
    Consumer* currentConsumer = fromHouse->consumers;
    while (currentConsumer != nullptr) {
        Consumer* newConsumer = new Consumer(*currentConsumer); // Deep copy of consumer
        newConsumer->next = reversedCopy;
        reversedCopy = newConsumer;
        currentConsumer = currentConsumer->next;
    }

    toHouse->consumers = nullptr;
    currentConsumer = reversedCopy;
    while (currentConsumer != nullptr) {
        Consumer* temp = currentConsumer->next;
        currentConsumer->next = toHouse->consumers;
        toHouse->consumers = currentConsumer;
        currentConsumer = temp;
    }

    return toHouse;
}


void writeCSV(const string& destination_csv_file, char separator, Household* households[], int numberOfHouseholds, const string& city) {
    ofstream file(destination_csv_file);
    if (!file.is_open()) {
        cerr << "Error opening file: " << destination_csv_file << endl;
        return;
    }

    file << "A4" << separator << numberOfHouseholds << separator << city << endl;
    for (int i = 0; i < numberOfHouseholds; i++) {
        Household* house = households[i];
        if (house != nullptr) {
            file << "household" << separator << i + 1 << separator << house->city << separator;
            file << (house->electric_water_heating ? "true" : "false") << separator;
            file << house->persons << separator;
            file << house->square_m << separator;
            file << house->price << separator;
            file << house->Power_supply << endl;

            Consumer* current = house->consumers;
            while (current != nullptr) {
                file << "consumer" << separator << current->description << separator << current->hours << separator << current->use << separator << current->watt << separator << current->watt_standby << endl;
                current = current->next;
            }
        }
    }
    file.close();
}
vector<string> readCSV(const string& destination_csv_file, char separator) {
    vector<string> components;
    ifstream file(destination_csv_file);

    if (!file.is_open()) {
        cerr << "Error opening file: " << destination_csv_file << endl;
        return components;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string component;
        while (getline(iss, component, separator)) {
            components.push_back(component);
        }
    }

    file.close();
    return components;
}

void processComponents(const vector<string>& components, string& city, int& numberOfHouseholds, Household* households[]) {
    if (components.empty()) {
        cout << "Empty component list" << endl;
        return;
    }

    auto iter = components.begin();
    string identifier = *iter++;
    if (identifier != "A4") {
        cout << "Incorrect file format." << endl;
        return;
    }

    numberOfHouseholds = stoi(*iter++);
    city = *iter++;

    while (iter != components.end()) {
        string recordType = *iter++;
        if (recordType == "household") {
            int index = stoi(*iter++) - 1;
            households[index] = new Household();
            households[index]->city = city;
            households[index]->electric_water_heating = (*iter++ == "true");
            households[index]->persons = stoi(*iter++);
            households[index]->square_m = stoi(*iter++);
            households[index]->price = stof(*iter++);
            households[index]->Power_supply = *iter++;
        } else if (recordType == "consumer") {
            int householdIndex = stoi(*iter++) - 1;
            Consumer* c = new Consumer();
            c->description = *iter++;
            c->hours = stof(*iter++);
            string useString = *iter++;
            c->watt = stof(*iter++);
            c->watt_standby = stof(*iter++);

            if (useString == "once") c->use = ONCE;
            else if (useString == "daily") c->use = DAILY;
            else if (useString == "mo_fr") c->use = MO_FR;
            else if (useString == "sa_su") c->use = SA_SU;
            else if (useString == "weekly") c->use = WEEKLY;

            Add_Consumer_To_Household(households[householdIndex], c);
        }
    }
}


void displayConsumers(Consumer* currentConsumer) {
    int counter = 1;

    while (currentConsumer != nullptr) {
        cout << counter << ": " << currentConsumer->description << endl;
        cout << "Watt: " << currentConsumer->watt << endl;
        cout << "Watt Standby: " << currentConsumer->watt_standby << endl;
        cout << "Hours: " << currentConsumer->hours << endl;
        cout << "Usage: " << currentConsumer->use << endl;  // If you want to display the usage type
        cout << "======================" << endl;

        // Move to the next consumer in the list
        currentConsumer = currentConsumer->next;
        counter++;
    }
}

int main() {
    int n_households;

    float PRICE_KILO_WATT = price * 100;
    cout << "CALCULATION OF AVERAGE POWER COSTS FOR A HOUSEHOLD!\n";
    cout << "How many households does the house have? ";
    cin >> n_households;

    Household** households_arr = new Household*[n_households]{nullptr};

    Household* templateHousehold = new Household;
    cout << "In which city is the house located? ";
    cin.ignore();
    getline(cin, templateHousehold->city);
    cout << "What is the price for one kWh in EUR? ";
    cin >> templateHousehold->price;

    string destination_csv_file;
    char separator;
    vector<string> result;

    char opt = ' ';
    while (opt != 'q') {
        cout << "\n q quit\n i input power consumer\n u move up power consumer\n p print household\n a print all households\n n new household\n c copy all consumers (added to already existing ones)\n r read data from file\n w write data into file\n >> ";
        cin >> opt;

        switch (opt) {
        case 'p':
        {   int n;
            cout << "\n Number of household? >> ";
            cin >> n;
            Print_Household(households_arr[n - 1], PRICE_KILO_WATT, n);
            break;
        }
        case 'i':
        {   int n;
            cout << "\n Number of household? >> ";
            cin >> n;
            if (households_arr[n - 1] != nullptr)
            {
                Consumer* new_consumer = CREATE_CONSUMER();
                Add_Consumer_To_Household(households_arr[n - 1], new_consumer);
            }
            else
            {
                cout << "Household does not exist, please try again!";
            }
            break;
        }
        case 'u':
        {   int n;
            cout << "\n Number of household? >> ";
            cin >> n;
            cout << "which one? ";
            int y;
            cin >> y;
            households_arr[n - 1]->consumers = Move_Up(households_arr[n - 1]->consumers, y);
            break;
        }
        case 'n': {
            int n;
            cout << "\n Number of household? >> ";
            cin >> n;
            if (n >= 1 && n <= n_households && households_arr[n - 1] == nullptr) {
            households_arr[n - 1] = new Household;
            households_arr[n - 1]->city = templateHousehold->city;
            households_arr[n - 1]->price = templateHousehold->price;
            Input_Household(households_arr[n - 1]);
        }
            else {
                cout << "Already exists";
            }
            break;
            }
        case 'c':
        {
            int n_of_h1;
            int n_of_h2;
            cout << "number of household from which to copy consumers? >> ";
            cin >> n_of_h1;
            cout << "number of household to copy to? >> ";
            cin >> n_of_h2;
            Household* New_Household = Copy_Consumers(households_arr[n_of_h1 - 1], households_arr[n_of_h2 - 1]);
            households_arr[n_of_h2 - 1] = New_Household;
            break;
        }
        case 'a':
        {
            for (int n = 0; n < n_households; n++)
            {
                if (households_arr[n] != nullptr)
                {
                    Print_Household(households_arr[n], PRICE_KILO_WATT, n + 1);
                }
            }
            break;
        }

        case 'r':
            {
                cout << "Input file name: ";
                cin >> destination_csv_file;
                cout << "Input separator character: ";
                cin >> separator;
                result = readCSV(destination_csv_file, separator);
                processComponents(result, templateHousehold->city, n_households, households_arr);
                break;
            }
            case 'w': {
                cout << "Output file name: ";
                cin >> destination_csv_file;
                cout << "Output separator character: ";
                cin >> separator;
                writeCSV(destination_csv_file, separator, households_arr, n_households, templateHousehold->city);
                break;
            }
            case 'q':
                break;
            default:
                cout << "Invalid Character, retry again!\n";
        }
    }

    for (int i = 0; i < n_households; i++) {
        delete households_arr[i];
    }
    delete[] households_arr;
    delete templateHousehold;

    return 0;
}
