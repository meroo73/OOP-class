
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>

using namespace std;

float Person_Consum = 900, SquareMeter_Consum = 9, allpersonsConsum= 1650l;
float price;
const int COLUMN_WIDTH = 35;
const int year=365;
const int quart=96;

enum Use { ONCE, DAILY, MO_FR, SA_SU, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY,SUNDAY };


//struct to for custormers  and has the next pointer link costumer to the next one
struct Consumer {
    string description;
    float watt;
    float watt_standby;
    float hours;
    Use use;
    Consumer* next; // This pointer creates the linked list

    Consumer() : description(""), watt(0.0), watt_standby(0.0), hours(0.0), use(ONCE), next(nullptr) {}

};

// stucturt  of household
struct Household {
    string city;
    int square_m;
    int persons;
    bool electric_water_heating;
    float price;
    Consumer* consumers;  // This pointer points to the head of the linked list

    Household() : city(""), square_m(0), persons(0), electric_water_heating(false), price(0.0), consumers(nullptr) {}

};


//struct a year
struct Year
{
    int whi_Year;
    Use use;
    string units;
    float arr[year][quart];

};

inline Year& zeros(Year& yea)
{

    for (int i=0; i<year;i++){

        for (int j=0; j< quart; j++){
            yea.arr[i][j]=0;
        }
    }
    return yea;
}

inline Year& ones( Year& yea){

    for(int i=0; i<year; i++){

        for (int j=0; j<quart;j++){

          yea.arr[i][j]=1.0;
        }
    }
    return yea;

}


inline int Time(const int hours , int minutes)
{
    return((60* hours)+ minutes);
}



Use operator++(Use& use) {
    switch (use) {
        case MONDAY:   return TUESDAY;
        case TUESDAY:  return WEDNESDAY;
        case WEDNESDAY:return THURSDAY;
        case THURSDAY: return FRIDAY;
        case FRIDAY:   return SATURDAY;
        case SATURDAY: return SUNDAY;
        case SUNDAY:   return MONDAY;

            return use; // No change for these values
        default:return MONDAY; // Default to Monday in case of an invalid value
    }
}

// Overload the << operator for the enumeration values
ostream& operator<<(std::ostream& opera, const Use& use) {
    switch (use) {

        case MO_FR:
            cout<< "Monday to Friday" << endl;
            break;
        case SA_SU:
            cout<< "Saturday and Sunday" << endl;
            break;
        case MONDAY:
            cout<< "Monday" << endl;
            break;
        case TUESDAY:
            cout<< "Tuesday" << endl;
            break;
        case WEDNESDAY:
            cout<< "Wednesday" << endl;
            break;
        case THURSDAY:
            cout<< "Thursday" << endl;
            break;
        case FRIDAY:
            cout<< "Friday" << endl;
            break;
        case SATURDAY:
            cout<< "Saturday" << endl;
            break;
        case SUNDAY:
            cout<< "Sunday" << endl;
            break;
        case ONCE:
            cout<< "Once" << endl;
            break;
        case DAILY:
            cout<< "Daily" << endl;
            break;

        default:
            cout<< "Invalid Use"<< endl;
            break;
    }

    return opera;
}

// Overload the << operator for the enumeration values
ostream& operator<< (ostream& opera, Year& yea){
     opera<< "\n YEAR: "<< yea. whi_Year << "  (UNIT: "<< yea.units << " )" << endl;
     for (int w=0; w<10;w++){
        opera<< "\n day " << w << ":" << yea.use;
        opera<< "\n";
        ++yea.use;

        for (int i=0; i<3;i++){
            opera << i << ":00";

            for (int j=0; j<4; j++){
                opera<< setw(10) << fixed << setprecision(2) << yea.arr[i][j]<< " ";
            }
            opera<<"\n";
        }
     }
     return opera;
}


Year operator+ (Year& yea1, Year& yea2){
    Year yea = yea1;

    if ((yea1.whi_Year == yea2.whi_Year)){
        for (int i=0; i< 10; i++){

            for (int j=0; j<quart;j++){
                yea.arr[i][j] += yea2.arr[i][j];
            }
        }

    }
    return yea;
}


//overloaded binary subtraction operator -,
Year operator- (Year& yea1, Year& yea2){
    Year yea;

        if (yea1.whi_Year == yea2.whi_Year){

            for(int i=0; i<10; i++) {
                for(int j=0; j<quart;j++){

                yea.arr[i][j]= yea1.arr[i][j]- yea2.arr[i][j];

                }
            }
            yea.whi_Year = yea1.whi_Year;
            yea.use = yea1.use;

        }
        return yea;
}


Year operator*(Year& yea1, float val) {
    Year year_new = yea1;
    for (int i = 0; i < 10; i++) { // the number of rows = year
        for (int j = 0; j < quart; j++) {
            year_new.arr[i][j] = year_new.arr[i][j]*val;
        }
    }

    return year_new;
}


void SET_UNITS(Year& yea, string units) {
    yea.units = units;
}

void ADD_CONSUMPTION(Year& yea, int DAY, int START_HOUR, int END_HOUR, int START_MINUTE, int END_MINUTE, float watt )
{

    int DURATION = (60*(END_HOUR-START_HOUR))+ (END_MINUTE - START_MINUTE);
    int s;
    int LEFT_TIME;
    int hours = END_HOUR- START_HOUR;
    int quart= 4*(1+hours);
    double POWER_MIN= watt/60;
    bool Decide = false;

    cout << " DAY "; DAY;

   for (int i = 0; i < 365; i++) { // row = i = day
    if (i == DAY) {
        for (int HOUR = START_HOUR; HOUR <= END_HOUR; HOUR++) {
            double F = START_MINUTE / 15.0;
            double F_e = END_MINUTE / 15.0;

            for (int j = 1; j <= 4; j++) {
                if (!Decide) {
                    // First if-else condition is to determine the start_minute condition
                    // and start adding to the quarters by using the bool (Decide)/ switch
                    if ((0 <= START_MINUTE) && (START_MINUTE < 15) && (j == 1)) {
                        int Duration_0 = (15 * j) - START_MINUTE;
                        yea.arr[i][(j - 1) + (4 * START_HOUR)] += (POWER_MIN * Duration_0);
                        Decide = true;
                        int s = 1;
                        int LEFT_TIME = DURATION - (15 * s - START_MINUTE);
                    }
                    else if ((15 <= START_MINUTE) && (START_MINUTE < 30) && (j == 2)) {
                        int Duration_0 = 15 * j - START_MINUTE;
                        yea.arr[i][(j - 1) + (4 * START_HOUR)] += (POWER_MIN * Duration_0);
                        Decide = true;
                        int s = 2;
                        int LEFT_TIME = DURATION - (15 * s - START_MINUTE);
                    }
                    else if ((30 <= START_MINUTE) && (START_MINUTE < 45) && (j == 3)) {
                        int Duration_0 = 15 * j - START_MINUTE;
                        yea.arr[i][(j - 1) + (4 * START_HOUR)] += (POWER_MIN * Duration_0);
                        Decide = true;
                        int s = 3;
                        int LEFT_TIME = DURATION - (15 * s - START_MINUTE);
                    }
                    else if ((45 <= START_MINUTE) && (START_MINUTE < 60) && (j == 4)) {
                        int Duration_0 = 15 * j - START_MINUTE;
                        yea.arr[i][(j - 1) + (4 * START_HOUR)] += (POWER_MIN * Duration_0);
                        Decide = true;
                        int s = 4;
                        int LEFT_TIME = DURATION - (15 * s - START_MINUTE);
                    }
                }
                else {
                    if ((LEFT_TIME - 15) >= 0) {
                        yea.arr[i][(j - 1) + (4 * START_HOUR)] += POWER_MIN * 15;
                        LEFT_TIME = LEFT_TIME - 15;
                    }
                    else if ((LEFT_TIME - 15) < 0) {
                        yea.arr[i][(j - 1) + (4 * START_HOUR)] += POWER_MIN * LEFT_TIME;
                        Decide = false;
                    }
                }
            }
        Decide = false;
        }
    }
}


}

void ADD_CONSUMPTION(Year& yea, Use use, int START_HOUR, int END_HOUR, int START_MINUTE, int END_MINUTE, float watt) {
    switch (use) {
        case DAILY: {
            for (int f = 0; f < year; f++) {
                ADD_CONSUMPTION(yea, f, START_HOUR, END_HOUR, START_MINUTE, END_MINUTE, watt);
            }
            break;
        }
        case MO_FR: {
            yea.use = MONDAY;
            for (int f = 0; f < year; f++) {
                if (yea.use != SATURDAY && yea.use != SUNDAY) {
                    ADD_CONSUMPTION(yea, f, START_HOUR, END_HOUR, START_MINUTE, END_MINUTE, watt);
                }
                ++yea.use;
            }
            break;
        }
        case SA_SU: {
            int count = 6;
            for (int f = 6; f < year; f++) {
                if ((f == 6) || (count == 13)) {
                    ADD_CONSUMPTION(yea, f, START_HOUR, END_HOUR, START_MINUTE, END_MINUTE, watt);
                    count = 6;
                }
                count++;
            }

            int count0 = 7;
            for (int f = 7; f < year; f++) {
                if ((f == 7) || (count== 13)) {
                    ADD_CONSUMPTION(yea, f, START_HOUR, END_HOUR, START_MINUTE, END_MINUTE, watt);
                    count = 7;
                }
                count++;
            }
            break;
        }
        case ONCE: {
            // handled in main
            break;
        }
        case MONDAY: {
            int count = 0;
            for (int f = 0; f < year; f++) {
                if ((f == 0) || (count == 7)) {
                    ADD_CONSUMPTION(yea, f, START_HOUR, END_HOUR, START_MINUTE, END_MINUTE, watt);
                    count = 0;
                }
                count++;
            }
            break;
        }
        case TUESDAY: {
            int count = 1;
            for (int f = 1; f < year; f++) {
                if ((f == 1) || (count == 8)) {
                    ADD_CONSUMPTION(yea, f, START_HOUR, END_HOUR, START_MINUTE, END_MINUTE, watt);
                    count = 1;
                }
                count++;
            }
            break;
        }
        case WEDNESDAY: {
            int count = 2;
            for (int f = 2; f < year; f++) {
                if ((f == 2) || (count == 9)) {
                    ADD_CONSUMPTION(yea, f, START_HOUR, END_HOUR, START_MINUTE, END_MINUTE, watt);
                    count = 2;
                }
                count++;
            }
            break;
        }
        case THURSDAY: {
            int count = 3;
            for (int f = 3; f < year; f++) {
                if ((f == 3) || (count == 10)) {
                    ADD_CONSUMPTION(yea, f, START_HOUR, END_HOUR, START_MINUTE, END_MINUTE, watt);
                    count = 3;
                }
                count++;
            }
            break;
        }
        case FRIDAY: {
            int count = 4;
            for (int f = 4; f < year; f++) {
                if ((f == 4) || (count == 11)) {
                    ADD_CONSUMPTION(yea, f, START_HOUR, END_HOUR, START_MINUTE, END_MINUTE, watt);
                    count = 4;
                }
                count++;
            }
            break;
        }
        case SATURDAY: {
            int count = 5;
            for (int f = 5; f < year; f++) {
                if ((f == 5) || (count == 12)) {
                    ADD_CONSUMPTION(yea, f, START_HOUR, END_HOUR, START_MINUTE, END_MINUTE, watt);
                    count = 5;
                }
                count++;
            }
            break;
        }
        case SUNDAY: {
            int count = 6;
            for (int f = 6; f < year; f++) {
                if ((f == 6) || (count == 13)) {
                    ADD_CONSUMPTION(yea, f, START_HOUR, END_HOUR, START_MINUTE, END_MINUTE, watt);
                    count = 6;
                }
                count++;
            }
            break;
        }
    }
}



inline float sum(Year& yea){
    float TOTAL= 0.00;
    for (int i=0; i< 10; i++){
        for (int j=0; j<12; j++){
            TOTAL+= yea.arr[i][j];

        }
    }
    return TOTAL;
}


void Input_Use(const std::string what, Use& use) {
    char choice;
    std::cout << what << "\n " << std::endl;
    std::cout << "Monday    (0)" << std::endl;
    std::cout << "Tuesday   (1)" << std::endl;
    std::cout << "Wednesday (2)" << std::endl;
    std::cout << "Thursday  (3)" << std::endl;
    std::cout << "Friday    (4)" << std::endl;
    std::cout << "Saturday  (5)" << std::endl;
    std::cout << "Sunday    (6)" << std::endl;
    std::cout << "once      (o)" << std::endl;
    std::cout << "daily     (d)" << std::endl;
    std::cout << "mo_fr     (m)" << std::endl;
    std::cout << "sa_su     (s)" << std::endl;
    std::cin >> choice;

    switch (choice) {
        case 'o': use = ONCE; break;
        case 'd': use = DAILY; break;
        case 'm': use = MO_FR; break;
        case 's': use = SA_SU; break;
        case '0': use = MONDAY; break;
        case '1': use = TUESDAY; break;
        case '2': use = WEDNESDAY; break;
        case '3': use = THURSDAY; break;
        case '4': use = FRIDAY; break;
        case '5': use = SATURDAY; break;
        case '6': use = SUNDAY; break;

        default:
            cout << "You didnt choose one of options above please choose again ";
            Input_Use(what, use);
            break;
    }
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




int main()
{
    Household* household = new Household;
    household->consumers = nullptr;
    string city;
    int n_households;
    int n = 0;
    string start, ende;
    float val;
    char ACTUAL_TOTAL;
    string units;

    Year ACTUAL;
    Year TOTAL;
    ACTUAL.whi_Year = TOTAL.whi_Year = 2024;
    ACTUAL.use = TOTAL.use = MONDAY;
    ACTUAL.units = TOTAL.units = 'watt';
    zeros(ACTUAL);
    zeros(TOTAL);
    float PRICEkwh = 30.00;
    n_households = 10;
    Household** household_arr = new Household *[n_households]{nullptr};

    cout << "YEARLY CONSUMPTION QUARTER HOUR! \n" << endl;



    for (int i = 0; i < n_households; i++)
    {
        household_arr[i] = nullptr;
    }



    char opt = 's';
    while (opt != 'q')
    {
        cout << "q quit" << endl ;
        cout << "a add actual to total" << endl;
        cout << "m subtract actual from total (using operator -)" << endl;
        cout << "s scalar multiplication" << endl;
        cout << "c change unit" << endl;
        cout << "v sum up values" << endl;
        cout << "o output actual (using operator <<)" << endl;
        cout << "t output total (using operator<<)" << endl;
        cout << "u add consumption according to frequency of use (call functions ADD_CONSUMPTION)" << endl;
        cout << "y set actual to ones (call function ones)" << endl;
        cout << "z set actual to zeros (call function zeros)" << endl;
        cin >> opt;

        switch (opt) {
            case 'a': {
                TOTAL = TOTAL + ACTUAL;
                break;
            }
            case 'm': {
                TOTAL = TOTAL - ACTUAL;
                break;
            }
            case 's': {
                cout << "\n a for actual: \n t for total: ";
                cin >> ACTUAL_TOTAL;
                cout << "\n value of scalar? ";
                cin >> val;
                if (ACTUAL_TOTAL == 'a') {
                    ACTUAL = ACTUAL * val;
                } else if (ACTUAL_TOTAL == 't') {
                    TOTAL = TOTAL * val;
                }
                break;
            }
            case 'c': {
                cout << "\n a for actual: \n t for total  : ";
                cin >> ACTUAL_TOTAL;
                cout << "\n what is the new unit? ";
                cin >> units;
                if (ACTUAL_TOTAL == 'a') {
                    SET_UNITS(ACTUAL, units);
                }
                if (ACTUAL_TOTAL == 't') {
                    SET_UNITS(TOTAL, units);
                }
                break;
            }
            case 'y': {
                cout << "\n   ";
                ones(ACTUAL);
                break;
            }
            case 'v': {
                float sum_act = sum(ACTUAL);
                cout << "Sum actual: " << sum_act << " " << ACTUAL.units << "\n";
                float sum_tot = sum(TOTAL);
                cout << "Sum Total: " << sum_tot << " " << TOTAL.units << "\n";
                break;
            }
            case 'u': {
                int day_;

                Input_Use("How often will it be used?\n ", ACTUAL.use);
                if (ACTUAL.use == ONCE) {
                    cout << "\n on which day? ";
                    cin >> day_;
                }
                cout << "from hour:minute? ";
                cin >> start;
                cout << "to hour:minute? ";
                cin >> ende;

                int START_H, START_M, END_H, END_M;
                istringstream ss_start(start);

                getline(ss_start, start, ':');
                START_H = stoi(start);

                getline(ss_start, start);
                START_M = stoi(start);

                istringstream ss(ende);

                getline(ss, ende, ':');
                END_H = stoi(ende);

                getline(ss, ende);
                END_M = stoi(ende);

                cout << "how many watt it will have? ";
                float watt;
                cin >> watt;

                if (ACTUAL.use == ONCE) {
                    ADD_CONSUMPTION(ACTUAL, day_, START_H, END_H, START_M, END_M, watt);
                } else {
                    ADD_CONSUMPTION(ACTUAL, ACTUAL.use, START_H, END_H, START_M, END_M, watt);
                }
                break;
            }
            case 'z': {
                cout << "\n   ";
                zeros(ACTUAL);
                break;
            }
            case 't': {
                cout << "\n";
                cout << TOTAL << endl;
                break;
            }
            case 'o': {
                cout << "\n ";
                cout << ACTUAL << endl;
                break;
            }
            case 'q': {
                break;
            }
            default: {
                cout << "Invalid Character, retry again!\n";
            }
        }
    }
    return 0;
}

