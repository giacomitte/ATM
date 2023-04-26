// ATM.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;

const string ClientsFileName = "clients.txt";

enum enATMMenueOptions {eQuickWithdraw = 1, eNormalWithdraw = 2, eDeposit = 3, eCheckBalance = 4, eLogout = 5};

struct stClient {
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance = 0;
};

stClient CurrentClient;

void GoBackToATMMenueScreen();
void ATMMenue();
void Login();

vector<string>Split(string Sentence, string Separator = "#//#") {
    vector<string>vString;
    short Pos = 0;
    string Word = "";

    while ((Pos = Sentence.find(Separator)) != std::string::npos) {

        Word = Sentence.substr(0, Pos);

        if (Word != "")
            vString.push_back(Word);

        Sentence.erase(0, Pos + Separator.length());
    }

    if (Sentence != "")
        vString.push_back(Sentence);

    return vString;
}

string ConvertRecordToLine(stClient Client, string Separator = "#//#") {

    string DataLine = "";

    DataLine = Client.AccountNumber + Separator;
    DataLine += Client.PinCode + Separator;
    DataLine += Client.Name + Separator;
    DataLine += Client.Phone + Separator;
    DataLine += to_string(Client.AccountBalance);

    return DataLine;
}

stClient ConvertLineToRecord(string Sentence, string Separator = "#//#") {
    stClient Client;
    vector<string>vClientData;

    vClientData = Split(Sentence);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);

    return Client;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName) {
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open()) {

        stClient Client;
        string Line;

        while (getline(MyFile, Line)) {
            Client = ConvertLineToRecord(Line, "#//#");
            if (Client.AccountNumber == AccountNumber) {

                MyFile.close();
                return true;
            }
        }
        MyFile.close();
    }

    return false;
}

vector<stClient>LoadDataFromFileToVector(string FileName) {
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    vector<stClient>vClients;
    if (MyFile.is_open()) {
        string Line;
        stClient Client;
        while (getline(MyFile, Line)) {
            Client = ConvertLineToRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

vector<stClient>SaveClientsDataToFile(string FileName, vector<stClient>& vClients) {

    fstream MyFile;
    MyFile.open(FileName, ios::out);

    if (MyFile.is_open()) {

        string Line;

        for (stClient& c : vClients) {
            
            Line = ConvertRecordToLine(c);
            MyFile << Line << endl;
        }
        MyFile.close();
    }
    return vClients;
}

bool FindClientByAccountNumber(string AccountNumber, vector<stClient>vClients, stClient& Client) {

    for (stClient& c : vClients) {
        if (c.AccountNumber == AccountNumber) {
            Client = c;
            return true;
        }
    }
    return false;
}

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, stClient& Client) {
    vector<stClient>vClients = LoadDataFromFileToVector(ClientsFileName);
    for (stClient& c : vClients) {
        if (c.AccountNumber == AccountNumber && c.PinCode == PinCode) {
            Client = c;
            return true;
        }
    }
    return false;
}

string ReadAccountNumber() {
    string AccountNumber;
    getline(cin >> ws, AccountNumber);
    return AccountNumber;
}

string ReadPinCode() {
    string PinCode;
    getline(cin >> ws, PinCode);
    return PinCode;
}

bool Deposit(string AccountNumber, double Amount, vector<stClient>& vClients) {

    char Answer = 'N';
    cout << "\nAre You Sure You Want Preform This Transaction: ";
    cin >> Answer;

    if (toupper(Answer) == 'Y') {

        for (stClient& c : vClients) {
            if (c.AccountNumber == AccountNumber) {
                c.AccountBalance += Amount;
                SaveClientsDataToFile(ClientsFileName, vClients);

                cout << "\nDone Successfully, New Balance is: " << c.AccountBalance << endl;
                return true;
            }
        }
        return false;
    }
}

void ShowDepositScreen() {
    cout << "\n_______________________________________\n";
    cout << "\t   Deposit Screen";
    cout << "\n_______________________________________\n";

    stClient Client;
    vector<stClient>vClients = LoadDataFromFileToVector(ClientsFileName);

    double Amount = 0;
    cout << "\nEnter Deposit Amount: ";
    cin >> Amount;

    Deposit(CurrentClient.AccountNumber, Amount, vClients);
    CurrentClient.AccountBalance += Amount;
}

short ReadQuickWithdrawOption() {

    short Choice;
    cout << "\nChoose An Option From [1-9]: ";
    cin >> Choice;

    while (Choice < 1 || Choice > 9) {
        cout << "\nChoose An Option From [1-9]: ";
        cin >> Choice;
    }

    return Choice;
}

short GetQuickWithdrawAmount(short QuickWithdrawOption) {
    switch (QuickWithdrawOption) {
    case 1:
        return 20;
    case 2:
        return 50;
    case 3:
        return 100;
    case 4:
        return 200;
    case 5:
        return 400;
    case 6:
        return 600;
    case 7:
        return 800;
    case 8:
        return 100;
    default:
        return 0;
    }
}

void PreformQuickWithdrawOption(short QuickWithdrawOption) {
    if (QuickWithdrawOption == 9)
        return;

    short WithdrawAmount = GetQuickWithdrawAmount(QuickWithdrawOption);

    if (WithdrawAmount > CurrentClient.AccountBalance) {
        cout << "\nWithdraw Amount Exceeds The Balance\n";

        short WithdrawAmount = GetQuickWithdrawAmount(QuickWithdrawOption);

    }

    vector<stClient>vClients = LoadDataFromFileToVector(ClientsFileName);
    Deposit(CurrentClient.AccountNumber, WithdrawAmount * -1, vClients);
    CurrentClient.AccountBalance -= WithdrawAmount;
}

void ShowQuickWithdrawScreen() {
    cout << "\n_______________________________________\n";
    cout << "\t   Withdraw Screen";
    cout << "\n_______________________________________\n";
    cout << "   [1] 20 \t [2] 50\n";
    cout << "   [3] 100 \t [4] 200\n";
    cout << "   [5] 400 \t [6] 600\n";
    cout << "   [7] 800 \t [8] 1000\n";
    cout << "   [9] Exit\n";
    cout << "_______________________________________\n";

    cout << "Your Balance: " << CurrentClient.AccountBalance << endl;


    PreformQuickWithdrawOption(ReadQuickWithdrawOption());
    
}

void ShowNormalWithdrawScreen() {
    cout << "\n_______________________________________\n";
    cout << "\t   Withdraw Screen";
    cout << "\n_______________________________________\n";

    vector<stClient>vClients = LoadDataFromFileToVector(ClientsFileName);


    int WithdrawAmount;
    cout << "\nEnter Withdraw Amount Multiple Of 5': ";
    cin >> WithdrawAmount;

    while (WithdrawAmount % 5 != 0) {
        cout << "\nEnter Withdraw Amount Multiple Of 5': ";
        cin >> WithdrawAmount;
    }
    while (WithdrawAmount > CurrentClient.AccountBalance) {
        cout << "\nWithdraw Amount Exceeds The Balance, You Can Withdraw Up to: " << CurrentClient.AccountBalance << ", Enter Withdraw Amount : ";
        cin >> WithdrawAmount;
    }

    Deposit(CurrentClient.AccountNumber, WithdrawAmount * -1, vClients);
    CurrentClient.AccountBalance -= WithdrawAmount;
}

void ShowCheckBalanceScreen() {

    cout << "\n_______________________________________\n";
    cout << "\t   Deposit Screen";
    cout << "\n_______________________________________\n";

    cout << "\nYour Balance: " << CurrentClient.AccountBalance << endl;

}

void ShowLogoutScreen() {
    Login();
}

short ReadATMMenueOption() {
    short Choice;
    cout << "\nChooce An Option From [1-5]: ";
    cin >> Choice;

    while (Choice < 1 || Choice > 5) {
        cout << "\nChooce An Option From [1-5]: ";
        cin >> Choice;
    }
    return Choice;
}

void GoBackToATMMenueScreen() {
    cout << "\nPress Any Key To Go Back To ATM Menue Screen...";
    system("pause>0");
    ATMMenue();
}

void PreformATMMenueOption(enATMMenueOptions ATMMenueOption) {
    switch (ATMMenueOption) {
    case enATMMenueOptions::eQuickWithdraw:
        system("cls");
        ShowQuickWithdrawScreen();
        GoBackToATMMenueScreen();
        break;
    case enATMMenueOptions::eNormalWithdraw:
        system("cls");
        ShowNormalWithdrawScreen();
        GoBackToATMMenueScreen();
        break;
    case enATMMenueOptions::eDeposit:
        system("cls");
        ShowDepositScreen();
        GoBackToATMMenueScreen();
        break;
    case enATMMenueOptions::eCheckBalance:
        system("cls");
        ShowCheckBalanceScreen();
        GoBackToATMMenueScreen();
        break;
    case enATMMenueOptions::eLogout:
        system("cls");
        ShowLogoutScreen();
        GoBackToATMMenueScreen();
        break;
    }
}

void ATMMenue() {

    system("cls");
    cout << "\n_________________________________\n";
    cout << "\t ATM Menue Screen";
    cout << "\n_________________________________\n";
    cout << "   [1] Quick Withdraw\n";
    cout << "   [2] Normal Withdraw\n";
    cout << "   [3] Deposit\n";
    cout << "   [4] Check Balance\n";
    cout << "   [5] Logout\n";
    cout << "\n_________________________________\n";

    PreformATMMenueOption((enATMMenueOptions)ReadATMMenueOption());
}

bool LoadClientInfo(string AccountNumber, string PinCode) {

    if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient)) 
        return true;
    else
        return false;
}

void Login() {

    system("cls");
    cout << "\n_________________________________\n";
    cout << "\t  Login Screen";
    cout << "\n_________________________________\n";


    bool LoginFailed = false;
    string AccountNumber, PinCode;

    do {

        if (LoginFailed) {
            cout << "\nInvalid Account Number/PinCode\n";
        }

        cout << "\nEnter Account Number: ";
        AccountNumber = ReadAccountNumber();

        cout << "Enter PinCode: ";
        PinCode = ReadPinCode();

        LoginFailed = !LoadClientInfo(AccountNumber, PinCode);
        
    } while (LoginFailed);

    CurrentClient.AccountNumber = AccountNumber;

    ATMMenue();
}

int main()
{
    Login();
}


