#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int LoadData(string file_name, int arr[], int &size);
int Date2Int(const string& date);
void mergeSort(int arr[], int left, int right);
void merge(int arr[], int left, int mid, int right);
int loadData2File(int* arr);



int main() {
    int arr[16807];
    int size = 0;

    if (LoadData("bitacora.txt", arr, size) != 0) {
        cout << "Hubo un problema al cargar los datos." << endl;
        return 1;
    }

    cout << "Datos cargados exitosamente. Total registros: " << size << endl;

    cout<<"Primeros 10 registros sin orden:"<<endl;
    for (int i = 0; i < 10 && i < size; i++) {
        cout << arr[i] << endl;
    }


    cout<<"Primeros 10 registros ordenados:"<<endl;
    mergeSort(arr,0,size-1);

    for (int i = 0; i < 10 && i < size; i++)
        cout << arr[i] << endl;

    

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

    int posDia= 3;
    while (posDia < date.size() && date[posDia] == ' '){ posDia++;} 
    string dia = "";
    while (posDia < date.size() && isdigit(date[posDia])) {
        dia += date[posDia];
        posDia++;
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



void merge(int arr[], int left,int mid,  int right){

    int n1=mid-left +1;
    int n2= right-mid;

    int* L= new int[n1];
    int* R= new int[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];

    delete[] L;
    delete[] R;


}
void mergeSort(int arr[], int left, int right){

    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }

}


int loadData2File(int* arr){
    


}
