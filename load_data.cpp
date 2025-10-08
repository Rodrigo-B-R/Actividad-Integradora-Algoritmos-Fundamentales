#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int LoadData(string file_name, int arr[], int &size);
int Date2Int(const string& date);

int main() {
    int arr[16807];
    int size = 0;

    if (LoadData("bitacora.txt", arr, size) != 0) {
        cout << "Hubo un problema al cargar los datos." << endl;
        return 1;
    }

    cout << "Datos cargados exitosamente. Total registros: " << size << endl;

    
    for (int i = 0; i < 10 && i < size; i++) {
        cout << arr[i] << endl;
    }

    return 0;
}

int LoadData(string file_name, int arr[], int &size) {
    string line; 
    ifstream inputFile(file_name);

    if (!inputFile.is_open()) {
        cerr << "Error: Could not open " << file_name << "!" << endl;
        return 1;
    }

    int i = 0;
    while (getline(inputFile, line)) {
        const string date = line.substr(0, 15);
        int dateInt = Date2Int(date);
        arr[i] = dateInt;
        i++;
    }

    size = i; 
    return 0; 
}

int Date2Int(const string& date) {
    string meses[12] = {"Jan","Feb","Mar","Apr","May","Jun",
                        "Jul","Aug","Sep","Oct","Nov","Dec"};
    
    string mesStr = date.substr(0,3);  

    int posMes = 3;
    while (posMes < date.size() && date[posMes] == ' '){ posMes++;} 
    string dia = "";
    while (posMes < date.size() && isdigit(date[posMes])) {
        dia += date[posMes];
        posMes++;
    }

    // Rellenar día con cero si tiene un dígito
    if (dia.size() == 1) dia = "0" + dia;

    // Extraer la hora (últimos 8 caracteres)
    string hora = date.substr(date.size() - 8, 8);  // "HH:MM:SS"

    // Convertir mes a número
    string mesNum;
    for (int i = 0; i < 12; i++) {
        if (meses[i] == mesStr) {
            mesNum = (i < 9 ? "0" : "") + to_string(i + 1);
            break;
        }
    }

    
    string horaNum = "";
    for (char c : hora)
        if (c != ':') horaNum += c;

    
    string fechaNumStr = mesNum + dia + horaNum;

    int fechaNum = stoi(fechaNumStr);

    cout << date << " : " << fechaNumStr << " : " << fechaNum << endl;

    return fechaNum;
}
