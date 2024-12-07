


#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <stack>

using namespace std;

// Average consumption is in kWh
int avg_cons_person = 200;
int avg_cons_sqm = 9;
int avg_water_heat = 550;

const int column_width = 35;

enum freq_of_use {
    once, daily, mo_fr, sa_su, weekly
};

string use_to_string(freq_of_use use) {
    switch (use) {
        case once:
            return "once";
        case daily:
            return "daily";
        case mo_fr:
            return "Monday to Friday";
        case sa_su:
            return "Saturday and Sunday";
        case weekly:
            return "weekly";
    }
    return "";
}

freq_of_use findFrequencyOfUse(const string &type) {
    freq_of_use use;
    if (type == "once")
        use = once;
    else if (type == "daily")
        use = daily;
    else if (type == "Monday to Friday")
        use = mo_fr;
    else if (type == "Saturday and Sunday")
        use = sa_su;
    else if (type == "weekly")
        use = weekly;
    return use;
}

freq_of_use input_use(const string &message) {
    cout << message << endl;
    cout << "daily (d)" << endl;
    cout << "mo_fr (m)" << endl;
    cout << "sa_su (s)" << endl;
    cout << "weekly (w)? ";

    char useType;
    cin >> useType;

    if (useType == 'w')
        return weekly;
    else if (useType == 'd')
        return daily;
    else if (useType == 'm')
        return mo_fr;
    else if (useType == 'o')
        return once;
    else if (useType == 's')
        return sa_su;
    else {
        cout << "invalid option enter again." << endl;
        return input_use(message);
    }
}

class Consumer {
private:
    string description;
    float watt;
    float watt_standby;
    float hours;
    Consumer *next;
    freq_of_use use;
    static int total_consumers;


public:
    // Default constructor
    Consumer() : next(nullptr) { total_consumers++; };

    ~Consumer() {
        total_consumers--;
        cout << "Consumer " << description << " at address " << this << " deleted " << endl;
    }

    string get_description() {
        return description;
    }

    float get_watt() {
        return watt;
    }

    float get_watt_standby() {
        return watt_standby;
    }

    float get_hours() {
        return hours;
    }

    freq_of_use get_use() {
        return use;
    }

    Consumer *get_next() {
        return this->next;
    }

    static int get_total_consumers() {
        return total_consumers;
    }

    void set_description(string desc) {
        this->description = std::move(desc);
    }

    void set_watt(float w) {
        this->watt = w;
    }

    void set_watt_standby(float w_s) {
        this->watt_standby = w_s;

    }

    void set_hours(float hour) {
        this->hours = hour;
    }

    void set_use(freq_of_use usage) {
        this->use = usage;
    }

    void set_next(Consumer *n) {
        this->next = n;
    }

    void input() {
        cout << "what is the description of the power consumer? ";
        getline(cin >> ws, description);
        cout << "how many watt it will have? ";
        cin >> watt;
        cout << "how many watt standby it will have? ";
        cin >> watt_standby;
        use = input_use("how often it will be used?");
        cout << "how many hours it will be operating then? ";
        cin >> hours;
    }

    void print(int index, float price_in_KWh) {
        cout << right << setw(column_width - 2) << index << ": " << fixed << setprecision(2)<< get_description() << endl;
        cout << right << setw(column_width) << "(this: " << this << ")" << endl;
        cout << right << setw(column_width) << "power consumption: " << fixed << setprecision(2)<< get_watt() << " W" << endl;
        cout << right << setw(column_width) << "power consumption standby: " << fixed << setprecision(2)<< watt_standby << " W" << endl;
        cout << right << setw(column_width) << "annual hours of use: " << fixed << setprecision(2)<< annual_hours_of_use() << " h" << endl;
        cout << right << setw(column_width) << "annual hours of standby: " << fixed << setprecision(2)<< annual_hours_of_standby() << " h" << endl;
        cout << right << setw(column_width) << "annual consumption: " << fixed << setprecision(1)<< annual_kWh() << " kWh" << endl;
        cout << right << setw(column_width) << "annual costs: " << fixed << setprecision(2)<< annual_kWh() * price_in_KWh << " EUR" << endl;
    }

    float annual_hours_of_use() {
        switch (use) {
            case once:
                return hours;
            case daily:
                return hours * 365;
            case mo_fr:
                return hours * 260;
            case sa_su:
                return hours * 104;
            case weekly:
                return hours * 52;
            default:
                return 0;
        }
    }

    float annual_hours_of_standby() {
        return 8760 - annual_hours_of_use();
    }

    float annual_kWh() {
        return (annual_hours_of_use() * watt + annual_hours_of_standby() * watt_standby) / 1000.0f;
    }
};

int Consumer::total_consumers = 0;  // has to be static in order to be initialized outside the class

class Household {
private:
    float price_kWh;
    string power_supplier;
    int persons;
    float square_meters;
    bool electric_water_heating;
    Consumer *consumers = nullptr; // represents the consumers of the consumers list, it represents the connection between the households and consumer class in the class diagram
public:
    Household() : consumers(nullptr) {};

    ~Household() {
        Consumer *current = get_consumers();
        Consumer *to_be_deleted;

        while (current != nullptr) {
            to_be_deleted = current;

            Consumer *next = current->get_next();

            current->set_next(nullptr);

            delete to_be_deleted;

            current = next;
        }
        cout << "Household at address " << this << " deleted " << endl;
    }

    float get_price_kWh() {
        return price_kWh;
    }

    string get_power_supplier() {
        return power_supplier;
    }

    int get_persons() {
        return persons;
    }

    float get_square_meters() {
        return square_meters;
    }

    bool get_electric_water_heating() {
        return electric_water_heating;
    }

    Consumer *get_consumers() {
        return this->consumers;
    }

    void set_price_kWh(float power_price) {
        price_kWh = power_price;
    }

    void set_power_supplier(string supplier) {
        power_supplier = supplier;
    }

    void set_persons(int person) {
        persons = person;
    }

    void set_square_meters(float meters) {
        square_meters = meters;
    }

    void set_electric_water_heating(bool heating) {
        electric_water_heating = heating;
    }

    void set_consumers(Consumer *consumer) {
        this->consumers = consumer;
    }

    void add_consumer_to_household(Consumer *new_consumer) {
        if (new_consumer == nullptr) {
            cerr << "Invalid" << endl;
        }

        new_consumer->set_next(consumers);
        consumers = new_consumer;
    }

    void add_bk_consumer_to_household(Consumer *new_consumer) {
        if (new_consumer == nullptr) {
            cerr << "Invalid" << endl;
        }

        if (consumers == nullptr) {
            consumers = new_consumer;
            return;
        }

        Consumer *current = consumers;
        while (current->get_next() != nullptr)
            current = current->get_next();
        current->set_next(new_consumer);
    }

    Consumer *move_up(int k) {
        if (consumers == nullptr || k <= 1) {
            return consumers;
        }
        Consumer *current = consumers;
        Consumer *prev = nullptr;
        Consumer *prev_2 = nullptr;
        int currentPosition = 1;

        while (current != nullptr && currentPosition < k) {
            prev_2 = prev;
            prev = current;
            current = current->get_next();
            currentPosition++;
        }

        if (current == nullptr)
            return consumers;

        if (prev != nullptr) {
            prev->set_next(current->get_next());
            current->set_next(prev);
            if (prev_2 != nullptr) {
                prev_2->set_next(current);
            } else {
                return current;
            }
        }

        return consumers;
    }

    void copy_consumer(Household *from_household, Household *to_Household) {
        Consumer *current = from_household->consumers;
        if (current == nullptr)
            return;

        Consumer *copyConsumers = nullptr;
        Consumer *copyCurrent = nullptr;
        while (current != nullptr) {
            Consumer *copy = new Consumer;
            *copy = *current;
            copy->set_next(nullptr);

            if (copyConsumers == nullptr)
                copyConsumers = copy;
            else
                copyCurrent->set_next(copy);
            copyCurrent = copy;

            current = current->get_next();
        }

        copyCurrent->set_next(to_Household->consumers);
        to_Household->consumers = copyConsumers;
    }

    void input() {
        cout << "how many square metres does the households have? ";
        cin >> square_meters;

        cout << "how many persons live in this households? ";
        cin >> persons;

        char ishotwateruse;
        cout << "Is hot water produced electrically? (y(es) or n(o)): ";
        cin >> ishotwateruse;
        electric_water_heating = ishotwateruse == 'y';
        cout << "what is the price for one kWh in EUR? ";
        cin >> price_kWh;
        cout << "who is the power supplier? ";
        getline(cin >> ws, power_supplier);


        consumers = nullptr;
    }

    void print(int household_number) {
        cout << "H O U S E H O L D   N O   " << household_number << "   P O W E R   C O N S U M P T I O N" << endl;
        cout << "----------------------------------------------------------------------" << endl;
        cout << right << setw(column_width) << "(this: " << this << ")" << endl;
        cout << right << setw(column_width) << "price for one kWh: " << fixed << setprecision(2)
             << get_price_kWh() * 100 << " ct/kWh" << endl;
        cout << right << setw(column_width) << "power supplier: " << fixed << setprecision(2)
             << get_power_supplier() << endl;
        cout << right << setw(column_width) << "square meters: " << fixed << setprecision(2)
             << get_square_meters() << " qm" << endl;
        cout << right << setw(column_width) << "persons: " << get_persons() << endl;
        cout << right << setw(column_width) << "water heated using electricity: "
             << (get_electric_water_heating() ? "yes" : "no") << endl;
        cout << right << setw(column_width - 2) << "list of consumers" << endl;
        cout << "----------------------------------------------------------------------" << endl;

        int index = 1;
        Consumer *current = get_consumers();
        while (current) {
            current->print(index, price_kWh);
            current = current->get_next();
            index++;
        }
        if (index > 1)
            cout << "----------------------------------------------------------------------" << endl;

        float totalPowerConsumptionPerSquareMeter;
        float totalPowerConsumptionPerPerson;
        float totalPowerConsumption;
        if (get_electric_water_heating()) {
            totalPowerConsumptionPerSquareMeter = get_square_meters() * avg_cons_sqm;
            totalPowerConsumptionPerPerson = get_persons() * avg_water_heat;
            totalPowerConsumption = totalPowerConsumptionPerSquareMeter + totalPowerConsumptionPerPerson;
        } else {
            totalPowerConsumptionPerSquareMeter = get_square_meters() * avg_cons_sqm;
            totalPowerConsumptionPerPerson = get_persons() * avg_cons_person;
            totalPowerConsumption = totalPowerConsumptionPerSquareMeter + totalPowerConsumptionPerPerson;
        }

        current = get_consumers();
        while (current) {
            totalPowerConsumption += current->annual_kWh();
            current = current->get_next();
        }

        cout << right << setw(column_width) << "power consumption square meters: " << fixed << setprecision(1)
             << totalPowerConsumptionPerSquareMeter << " kWh" << endl;
        cout << right << setw(column_width) << "power consumption all persons: " << fixed << setprecision(1)
             << totalPowerConsumptionPerPerson << " kWh" << endl;
        cout << right << setw(column_width) << "total annual power consumption: " << fixed << setprecision(1)
             << totalPowerConsumption << " kWh" << endl;
        cout << right << setw(column_width) << "total annual power costs: " << fixed << setprecision(2)
             << totalPowerConsumption * get_price_kWh() << " EUR" << endl;
        cout << endl;
    }
};


class Address {
private:

    string street;
    string no;
    string zip;
    string city;

public:
    // Parametrized constructor
    Address(string street, string no, string zip, string city) : street(std::move(street)), no(std::move(no)),
                                                                 zip(std::move(zip)), city(std::move(city)) {}

    // Default constructor
    Address() : street(""), no(""), zip(""), city("") {}

    string get_street() {
        return street;
    }

    string get_no() {
        return no;
    }

    string get_zip() {
        return zip;
    }

    string get_city() {
        return city;
    }

    string to_string() {
        string myAddress;
        myAddress.append(street).append(" ").append(no).append(", ").append(zip).append(" ").append(city);
        return myAddress;
    }

    void input() {
        cout << "what is the street name? ";
        getline(cin >> ws, street);
        cout << "what is house number? ";
        cin >> no;
        cout << "what is zip code? ";
        cin >> zip;
        cout << "what is the city name? ";
        getline(cin >> ws, city);
    }

    ~Address() {
        cout << "Address " << to_string() << " at address " << this << " deleted " << endl;
    }
};


class House {
private:
    Household **households;
    int number_of_households;
    Address address;
public:
    // Parametrized constructor
    House(int n_households, const Address &addres) : number_of_households(n_households), address(addres) {
        households = new Household *[number_of_households];
        for (int i = 0; i < number_of_households; ++i) {
            households[i] = nullptr;
        }
    }


    Household *get_household(int household_number) {
        if (household_number >= 0 && household_number < number_of_households) {
            return households[household_number];
        }
        return nullptr;
    }

    Household *operator[](int household_number) {
        if (household_number >= 0 && household_number < number_of_households) {
            return households[household_number];
        }
        return nullptr;
    }

    void set_household(Household *household, int number_of_household) {
        Household *this_household = get_household(number_of_household);

        if (this_household != nullptr) {
            cout << "Household already exists! " << endl;

        } else if (number_of_household > number_of_households) {
            cout << "Invalid number of Household, the available number of households in this House: "
                 << number_of_households << endl;
        } else {
            households[number_of_household] = household;
        }
    }

    void print_all() {
        cout << "======================================================================" << endl;
        cout << right << setw(column_width) << " House " << endl;
        cout << "======================================================================" << endl;
        cout << right << setw(column_width) << "(this: " << &households << ") " << endl;
        cout << right << setw(column_width) << "address: " << address.to_string() << " " << endl;
        cout << right << setw(column_width) << "number of households: " << number_of_households << " " << endl;
        cout << right << setw(column_width) << "total numbers of all consumers: " << Consumer::get_total_consumers()
             << endl;

        cout << "======================================================================" << endl;

        for (int i = 0; i < number_of_households; i++) {
            if (households[i] != nullptr) {
                households[i]->print(i);
            }
        }
        cout << "======================================================================" << endl;
    }

    void write_to_file(const string &file_name, char delim) {
        ofstream file;
        file.open(file_name);
        file << "A5" << delim << number_of_households << delim << address.get_street() << delim
             << address.get_no() << delim << address.get_zip() << delim << address.get_city() << endl;
        for (int i = 0; i < number_of_households; i++) {
            if (households[i] != nullptr) {
                file << "household" << delim << i << delim
                     << (households[i]->get_electric_water_heating() ? "true" : "false") << delim
                     << households[i]->get_persons() << delim << households[i]->get_square_meters() << delim
                     << households[i]->get_price_kWh() << delim << households[i]->get_power_supplier() << endl;
                if (households[i]->get_consumers()) {
                    Consumer *current = households[i]->get_consumers();
                    while (current) {
                        file << "consumer" << delim << current->get_description() << delim << current->get_hours()
                             << delim << use_to_string(current->get_use()) << delim << current->get_watt() << delim
                             << current->get_watt_standby() << endl;
                        current = current->get_next();
                    }
                }
            }
        }
        file.close();
    }

    void read_from_file(House *&house, const string &file_name, char delim) {
        ifstream file(file_name); // open file in read

        string line;
        int current_household_number;

        while (getline(file, line)) {
            if (line.find("A5") != string::npos) {

                istringstream stream(line);
                string element;

                vector<string> elements;
                while (getline(stream, element, delim)) {
                    elements.push_back(element);
                }

                int num_households = stoi(elements[1]);
                house = new House(num_households, (Address) {elements[2], elements[3], elements[4], elements[5]});
            } else if (line.find("household") != string::npos) {

                istringstream stream(line);
                string element;

                vector<string> elements;
                while (getline(stream, element, delim)) {

                    elements.push_back(element);
                }

                int num_household = stoi(elements[1]);
                current_household_number = num_household;

                if (num_household >= 0) {
                    if (house->get_household(num_household) == nullptr) {
                        house->set_household(new Household(), num_household);
                    }
                }
                Household *newHousehold = house->get_household(num_household);
                newHousehold->set_electric_water_heating(elements[2] == "true");

                newHousehold->set_persons(stoi(elements[3]));
                newHousehold->set_square_meters(stof(elements[4]));
                newHousehold->set_price_kWh(stof(elements[5]));
                newHousehold->set_power_supplier(elements[6]);
            } else if (line.find("consumer") != string::npos) {
                istringstream stream(line);
                string element;

                vector<string> elements;
                while (getline(stream, element, delim)) {
                    elements.push_back(element);
                }

                Consumer *newConsumer = new Consumer();

                newConsumer->set_description(elements[1]);
                newConsumer->set_hours(stof(elements[2]));

                freq_of_use usetype = findFrequencyOfUse(elements[3]);
                newConsumer->set_use(usetype);
                newConsumer->set_watt(stof(elements[4]));
                newConsumer->set_watt_standby(stof(elements[5]));
                newConsumer->set_next(nullptr);

                house->get_household(current_household_number)->add_bk_consumer_to_household(newConsumer);
            }
        }

        file.close();
    }

    ~House() {
        for (int i = 0; i < number_of_households; i++) {
            if (households[i] != nullptr) {
                delete households[i];
            }
        }
        delete[] households;
        cout << "House at address " << this << "deleted" << endl;
    }

};

int main() {
    House *myHouse = nullptr;

    cout << " CALCULATION OF AVERAGE POWER COSTS FOR A HOUSEHOLD! - Class Version \n" << endl;

    char opt = ' ';
    while (opt != 'q') {
        cout << "q quit" << endl;
        cout << "d delete house" << endl;
        cout << "h house initialisation" << endl;
        cout << "i input power consumer" << endl;
        cout << "u move up power consumer" << endl;
        cout << "p print household" << endl;
        cout << "a print all households" << endl;
        cout << "n new household" << endl;
        cout << "c copy all consumers (added to already existing ones)" << endl;
        cout << "r read data from file" << endl;
        cout << "w write data into file" << endl;
        cout << ">> ";
        cin >> opt;
        cin.ignore();

        if (opt == 'p') { // print households
            int number_of_household;
            cout << "number of household? ";
            cin >> number_of_household;

            Household *household = myHouse->get_household(number_of_household);
            if (household) {
                household->print(number_of_household);
            }
        } else if (opt == 'h') { // initialize the House
            int number_of_households;
            cout << "how many households does the house have? ";
            cin >> number_of_households;

            Address addr;
            addr.input();

            myHouse = new House(number_of_households, addr);
        } else if (opt == 'r') { // read from file
            string file_name;
            char delim;
            cout << "input file name: ";
            cin >> file_name;
            cout << "input separator character: ";
            cin >> delim;
            cout << "input file " << file_name << " opened..." << endl;
            myHouse->read_from_file(myHouse, file_name, delim);
            cout << "input file " << file_name << " closed" << endl;
        } else if (opt == 'w') { // write to file
            string file_name;
            cout << "input file name: ";
            cin >> file_name;
            cout << "input separator character: ";
            char delim;
            cin >> delim;
            cout << "output file " << file_name << " opened..." << endl;
            myHouse->write_to_file(file_name, delim);
            cout << "output file " << file_name << " closed" << endl;
        } else if (opt == 'i') {  // input power consumer
            int number_of_household;
            cout << "number of household? ";
            cin >> number_of_household;

            if (myHouse) {
                Household *household = myHouse->get_household(number_of_household);
                if (household) {
                    Consumer *new_consumer = new Consumer();
                    new_consumer->input();
                    household->add_consumer_to_household(new_consumer);
                }
            } else {
                cout << "house is a nullptr, please first choose h to initialise a new house" << endl;
            }
        } else if (opt == 'u') { // move up a consumer
            int number_of_household;
            int number_of_consumer;
            cout << "number of household? ";
            cin >> number_of_household;
            cout << "which one? ";
            cin >> number_of_consumer;

            Household *household = myHouse->get_household(number_of_household);
            if (household) {
                household->move_up(number_of_consumer);
            }
        } else if (opt == 'n') { // add new households
            int number_of_household;
            cout << "Number of households: ";
            cin >> number_of_household;
            if (myHouse) {
                Household *new_household = new Household();
                new_household->input();
                myHouse->set_household(new_household, number_of_household);
            } else {
                cout << "house is a nullptr, please first choose h to initialise a new house" << endl;
            }
        } else if (opt == 'c') { // copy consumers
            int number_of_houeshold_1;
            int number_of_houeshold_2;

            cout << "number of household from which to copy consumers? ";
            cin >> number_of_houeshold_1;

            Household *household_1 = myHouse->get_household(number_of_houeshold_1);

            cout << "number of household to copy to? ";
            cin >> number_of_houeshold_2;

            Household *TempHousehold = myHouse->get_household(number_of_houeshold_2);
            household_1->copy_consumer(household_1, TempHousehold);
        } else if (opt == 'a') { // print all
            if (myHouse) {
                myHouse->print_all();
            } else {
                cout << "house is a nullptr, please first choose h to initialise a new house" << endl;
            }
        } else if (opt == 'd') {
            if (myHouse)
                delete myHouse;
            else {
                cout << "house is nullptr, please first choose h to initialize a new house" << endl;
            }
        } else if (opt == 'q') { // quit
            break;
        } else {
            cout << "Invalid Option, try again!\n";
        }
    }

    return 0;
}
