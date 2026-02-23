#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <vector>
using namespace std;

bool again(const string& prompt) // helper function
{
    string ans;
    cout << prompt;
    
    while (true)
    {
        getline(cin, ans);
        
        if (ans.empty())
        {
            cout << "Invalid input, try again: ";
            continue;
        }
        
        char firstChar = ans[0];

        if (firstChar == 'y' || firstChar == 'Y')
            return true;
        else if (firstChar == 'n' || firstChar == 'N')
            return false;
        else
            cout << "Invalid input, try again: ";
    }
}
bool ValidName(const string& str) // helper function
{
    if (str.empty()) return false;
    return all_of(str.begin(), str.end(), [](char c) { 
        return isalpha(c) || c == ' '; 
    });
}
bool ValidNumeric(const string& str) // helper function
{
    if (str.empty()) return false;
    return all_of(str.begin(), str.end(), ::isdigit);
}
bool ValidDate(const string& date) // helper function
{
    if (date.length() != 10) return false;
    if (date[2] != '/' || date[5] != '/') return false;
    
    string day = date.substr(0, 2);
    string month = date.substr(3, 2);
    string year = date.substr(6, 4);
    
    if (!ValidNumeric(day) || !ValidNumeric(month) || !ValidNumeric(year))
        return false;
    
    int d = stoi(day);
    int m = stoi(month);
    int y = stoi(year);
    
    if (d < 1 || d > 31 || m < 1 || m > 12 || y < 2025)
        return false;
    
    return true;
}
bool ValidTime(const string& time) // helper function 
{
    if (time.length() != 5) return false;
    if (time[2] != ':') return false;
    
    string hour = time.substr(0, 2);
    string minute = time.substr(3, 2);
    
    if (!ValidNumeric(hour) || !ValidNumeric(minute))
        return false;
    
    int h = stoi(hour);
    int min = stoi(minute);
    
    if (h < 0 || h > 23 || min < 0 || min > 59)
        return false;
    
    return true;
}
string toLower(const string& str) // helper function
{
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}
bool containsSubstring(const string& str, const string& substr) // helper function 
{
    string lowerStr = toLower(str);
    string lowerSubstr = toLower(substr);
    return lowerStr.find(lowerSubstr) != string::npos;
}

struct doctor
{
    string name;
    string id;
    string specialization;
    string Available_times;
};
vector<doctor> doctors;
void save_doctor()  
{
    doctor input;
    cout << "Add the doctor's details\n";
    
    cout << "Name: ";
    getline(cin, input.name);
    while (!ValidName(input.name)) // only accepts alphabetic characters
    {   
        cout << "Invalid input, try again: ";
        getline(cin, input.name);
    }
    
    cout << "ID: ";
    getline(cin, input.id);
    while (!ValidNumeric(input.id)) // only accepts numbers
    {   
        cout << "Invalid input, try again: ";
        getline(cin, input.id);
    }
    
    cout << "Specialization: ";
    getline(cin, input.specialization);
    
    cout << "Available times: ";
    getline(cin, input.Available_times);
    
    ofstream out("doctors.txt", ios::app); // opens a file and saves those inputs into that file
    out << input.name << ',' << input.id << ',' << input.specialization << ',' << input.Available_times << '\n';
    
    out.close();
    
    cout << "Doctor added successfully!\n";
}
bool load_doctor() 
{
    ifstream read("doctors.txt");  // Open file for reading
    
    if (!read.is_open()) // checks whether it exist or no
    {
        cout << "Error: Could not open file.\n";
        return false;  // Return false if file doesn't exist
    } 
    
    doctors.clear(); // Clear existing doctors before loading
    
    string file;
    while (getline(read, file)) // reads the whole file and puts it in "file"
    {    
        stringstream ss(file);
        string name, id, specialization, Available_times ; // Declaring strings to hold the parsed values
    
        getline(ss, name, ',');
        getline(ss, id, ',');     // Every ',' is the delimiter
        getline(ss, specialization, ',');
        getline(ss, Available_times);
        
        // Store the doctor in the vector
        doctor d;
        d.name = name;
        d.id = id;
        d.specialization = specialization; 
        d.Available_times = Available_times;
        doctors.push_back(d);
    }

    read.close();
    
    return true; // Return true if file loaded successfully
}
void searchDoctor() 
{
    bool continue_search = true;
    
    while (continue_search)
    {
        cout << "Enter doctor name or ID: ";
        string D;
        getline(cin , D);

        bool found = false;
        
        if (!load_doctor()) // Check if file exists, exit if not
        {
            return ; // exit early
        }
        
        // Search through all doctors
        for (const auto& d : doctors)
        {
            if (containsSubstring(d.name, D) || d.id == D)
            {
                cout << "Name: " << d.name << endl;
                cout << "ID: " << d.id << endl;
                cout << "Specialization: " << d.specialization << endl;
                cout << "Available times: " << d.Available_times << endl;
                found = true;
            }
        }
    
        if (!found) // no matching
        {
            cout << "No doctors found.\n";
        }

        // Ask if user wants to search again
        continue_search = again("\nSearch again? ");
    }
}
void Display_by_specialization()
{
    cout << "Please enter the desired specialization: ";
    string D;
    getline(cin , D);

    bool found = false;
    int count = 0; // matchings counter
        
    if (!load_doctor()) // Check if file exists, exit if not
    {
        return ; // exit early
    }
        
    // Search through all doctos
    for (const auto& d : doctors)
    {
        if (toLower(d.specialization) == toLower(D))
        {
            count++;  
            cout << count << '-' << d.name << "\n";
            found = true;
        }
    }
    
    if (!found) // no matching
    {
        cout << "No doctors found for that specialization.\n";
    }

    // Ask if user wants to search again
    if (again("\nSearch again? "))
    {
        Display_by_specialization(); // Recursive call
    }
}

struct patient
{
    string name;
    string id;
    string age;
    string Phone_number;
    string medical_history;
};
vector<patient> patients;
void save_patient()
{
    patient input;
    cout << "Add the patient's details\n";
    
    cout << "Name: ";
    getline(cin, input.name);
    while (!ValidName(input.name)) // only accepts alphabetic characters
    {   
        cout << "Invalid input, try again: ";
        getline(cin, input.name);
    }
    
    cout << "ID: ";
    getline(cin, input.id);
    while (!ValidNumeric(input.id)) // only accepts numbers
    {   
        cout << "Invalid input, try again: ";
        getline(cin, input.id);
    }
    
    cout << "Age: ";
    getline(cin, input.age);
    while (!ValidNumeric(input.age) || stoi(input.age) <= 0 || stoi(input.age) > 150) // only accepts numbers in range (1-150)
    {
        cout << "Invalid input, try again: ";
        getline(cin, input.age); 
    }
    
    cout << "Phone number: ";
    getline(cin, input.Phone_number);
    while (!ValidNumeric(input.Phone_number) || input.Phone_number.length() != 11) // only accepts an 11-digit number
    {   
        cout << "Invalid input, try again: ";
        getline(cin, input.Phone_number);
    }
       
    cout << "Medical history: ";
    getline(cin, input.medical_history);
    
    ofstream out("patients.txt", ios::app);  // opens a file and saves those inputs into that file
    out << input.name << ',' << input.id << ',' << input.age << ',' << input.Phone_number << ',' << input.medical_history << '\n';
    
    out.close();
    
    cout << "patient added successfully!\n";
}
bool load_patient() 
{
    ifstream read("patients.txt"); // Open file for reading
    
    if (!read.is_open())  // checks whether it exist or no
    {
        cout << "Error: Could not open file.\n";
        return false;  // Return false if file doesn't exist
    } 
    
    patients.clear(); // Clear existing patients before loading
    
    string file;
    while (getline(read, file)) // reads the whole file and puts it in "file"
    {
        stringstream ss(file);
        string name, id, age, Phone_number, medical_history; // Declaring strings to hold the parsed values
    
        getline(ss, name, ',');
        getline(ss, id, ',');
        getline(ss, age, ',');     // Every ',' is the delimiter
        getline(ss, Phone_number, ',');
        getline(ss, medical_history);
        
        // Store the patient in the vector
        patient p;
        p.name = name;
        p.id = id;
        p.age = age;
        p.Phone_number = Phone_number; 
        p.medical_history = medical_history;
        patients.push_back(p);
    }

    read.close();
    
    return true; // Return true if file loaded successfullyreturn true; // Return true if file loaded successfully
}
void searchPatient() 
{
    bool continue_search = true;
    
    while (continue_search)
    {
        cout << "Enter patient name or ID: ";
        string P;
        getline(cin , P);

        bool found = false;
        
        if (!load_patient()) // Check if file exists, exit if not
        {
            return; // exit early
        }
        
        // Search through all patients
        for (const auto& p : patients)
        {
            if (containsSubstring(p.name, P) || p.id == P)
            {
                cout << "Name: " << p.name << endl;
                cout << "ID: " << p.id << endl;
                cout << "Age: " << p.age << endl;
                cout << "Phone: " << p.Phone_number << endl;
                cout << "Medical history: " << p.medical_history << endl;
                found = true;
            }
        }
    
        if (!found) // no matching
        {
            cout << "No patients found.\n";
        }

        // Ask if user wants to search again
        continue_search = again("\nSearch again? ");
    }
}
void Medical_report()
{
    bool continue_search = true;
    
    while (continue_search)
    {
        cout << "Please enter the patient's name: ";
        string P;
        getline(cin , P);

        bool found = false;
        
        if (!load_patient()) // Check if file exists, exit if not
        {
            return ; // exit early
        }
        
        // Search through all patients
        for (const auto& p : patients)
        {
            if (toLower(p.name) == toLower(P))
            {
                cout << p.name << " : " << p.medical_history << ".\n";
                found = true;
                break;
            }
        }
    
        if (!found) // no matching
        {
            cout << "No patients found.\n";
        }

        // Ask if user wants to search again
        continue_search = again("\nAnother patient? ");
    }
}

struct Appointment
{
    string patient_id;
    string doctor_id;
    string date;
    string time;
};
vector<Appointment> Appointments;
void save_Appointment(const string& doctor_id, const string& patient_id, const string& date, const string& time)
{
    ofstream out("appointments.txt", ios::app);  // opens a file and saves those inputs into that file
    out << doctor_id << ',' << patient_id << ',' << date << ',' << time << '\n';
    
    out.close();
}
bool load_Appointment()
{
    ifstream read("appointments.txt"); // Open file for reading
    
    if (!read.is_open())  // checks whether it exist or no
    {
        cout << "Error: Could not open file.\n";
        return false;  // Return false if file doesn't exist
    } 
    
    Appointments.clear(); // Clear existing Appointments before loading
    
    string file;
    while (getline(read, file)) // reads the whole file and puts it in "file"
    {
        stringstream ss(file);
        string patient_id, doctor_id, date, time; // Declaring strings to hold the parsed values
    
        getline(ss, doctor_id, ',');
        getline(ss, patient_id, ',');
        getline(ss, date, ',');     // Every ',' is the delimiter
        getline(ss, time);
        
        // Store the Appointment in the vector
        Appointment a;
        a.patient_id = patient_id;
        a.doctor_id = doctor_id;
        a.date = date;
        a.time = time;
        Appointments.push_back(a);
    }

    read.close();
    
    return true; // Return true if file loaded successfully
}
void Book_Appointment()
{
    string d_id, p_id, date, time;
    
    cout << "Doctor ID: ";
    getline(cin, d_id);
    while (!ValidNumeric(d_id)) // only accepts numbers
    {   
        cout << "Invalid input, try again: ";
        getline(cin, d_id);
    }

    cout << "Patient ID: ";
    getline(cin, p_id);
    while (!ValidNumeric(p_id)) // only accepts numbers
    {   
        cout << "Invalid input, try again: ";
        getline(cin, p_id);
    }

    cout << "Date (DD/MM/YYYY): ";
    getline(cin, date);
    while (!ValidDate(date)) // only accepts known format
    {
        cout << "Invalid date, try again: ";
        getline(cin, date);
    }

    cout << "Time (HH:MM): ";
    getline(cin, time);
    while (!ValidTime(time)) // // only accepts valid time
    {
        cout << "Invalid time, try again: ";
        getline(cin, time);
    }

    save_Appointment(d_id, p_id, date, time); 
    
    cout << "Appointment booked successfully!\n";
}
void Cancel_Appointment()
{
    cout << "Enter patient ID to cancel appointment: ";
    string p_id;
    getline(cin, p_id);

    if (!load_Appointment()) // Check if file exists, exit if not
    {
        return; // Exit function if file doesn't exist
    } 
    
    ofstream f("temp.txt");
    bool found = false;

    for (const auto& a : Appointments) // Search through all Appointments
    {
        if (a.patient_id != p_id)
            f << a.doctor_id << "," << a.patient_id << "," << a.date << "," << a.time << "\n";
        else
            found = true;
    }

    f.close();
    remove("appointments.txt");
    rename("temp.txt", "appointments.txt");

    if (found) cout << "Appointment cancelled.\n";
    else cout << "No appointment found.\n";
}
void DisplayAppointmentsByDate() 
{
    cout << "Enter date (DD/MM/YYYY): ";
    string date;
    getline(cin, date);

    if (!load_Appointment()) // Check if file exists, exit if not
    {
        return; // Exit function if file doesn't exist
    }
    
    bool found = false;

    for (const auto& a : Appointments)
    {
        if (a.date == date)
        {
            cout << "Doctor ID: " << a.doctor_id
                 << ", Patient ID: " << a.patient_id
                 << ", Time: " << a.time << endl;
            found = true;
        }
    }

    if (!found) cout << "No appointments found for this date.\n";
}

void calculateTotalBill()
{
    string consultation_fees, medicine_cost;
                
    cout << "Enter consultation fees: ";
    getline(cin, consultation_fees);
    while (!ValidNumeric(consultation_fees)) // only accepts numbers
    {   
        cout << "Invalid input, try again: ";
        getline(cin, consultation_fees);
    }
                
     cout << "Enter medicines cost: ";
    getline(cin, medicine_cost);
    while (!ValidNumeric(medicine_cost)) // only accepts numbers
    {   
        cout << "Invalid input, try again: ";
        getline(cin, medicine_cost);
    }

    cout << "Total bill = " << stoi(medicine_cost) + stoi(consultation_fees) << " EGP\n";
}

int main() 
{
    cout << "--- Hospital Management System ---\n";
    cout << "0-Exit\n1-Add New Doctor\n2-Add New Patient\n3-Book Appointment\n4-Cancel Appointment\n5-Display all appointments\n6-Search for a doctor by credentials\n7-Search for a patient by credentials\n8-Total bill\n9-Search for a doctor by specialization\n10-Patient medical report\n";

    do
    {
        string input;
        cout << "\nEnter your choice: ";
        getline(cin, input);
        
        // Validate: non-empty and all digits
        if (input.empty() || !all_of(input.begin(), input.end(), ::isdigit))
        {
            cout << "Invalid input, try again: ";
            continue;  
         }   
    
        int choice = stoi(input);
    
        // Validate: in range 1-4
        if (choice < 0 || choice > 10)
        {
            cout << "Invalid input, try again: ";
            continue;
        }
    
        // Execute valid choice
        switch (choice)
        {
            case 0: cout << "Exiting...\n"; return 0;
            case 1:
            {
                int n;
                cout << "How many doctors to add? ";
                cin >> n;
                cin.ignore();
                for (int i =0; i < n ; i++)
                {
                    cout << "Doctor number " << i+1 << endl;
                    save_doctor();
                }
                break;
            }    
            case 2:
           {
               int n;
               cout << "How many patients to add? ";
               cin >> n;
               cin.ignore();
               for (int i=0; i < n ; i++)
               {
                   cout << "Patient number " << i+1 << endl ;
                   save_patient();
               }
               break;
            }
            case 3: Book_Appointment(); break;
            case 4: Cancel_Appointment(); break;
            case 5: DisplayAppointmentsByDate(); break;
            case 6: searchDoctor(); break;
            case 7: searchPatient(); break;
            case 8: calculateTotalBill(); break;
            case 9: Display_by_specialization(); break;
            case 10: Medical_report(); break;
        }
        
    }while(again("\nAgain? "));
    
    return 0;
}