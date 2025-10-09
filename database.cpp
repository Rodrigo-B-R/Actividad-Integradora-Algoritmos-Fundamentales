#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int LoadData(string file_name, int arr[], int &size);
int Date2Int(const string& date);
void mergeSort(int arr[], int left, int right);
void merge(int arr[], int left, int mid, int right);
int loadData2File(int arr[], int size,string filename);



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
    
    if(loadData2File(arr,size,"bitacora_ordenada.txt") ==1){
        cout<<"Succesful"<<endl;
    }
    else{ cout<<"Failed loading the data"<<endl;}

    

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
    
    // --- Mes ---
    string mesStr = date.substr(0,3);  

    // --- Día ---
    int posDia = 3;
    while (posDia < date.size() && date[posDia] == ' ') posDia++; 
    string dia = "";
    while (posDia < date.size() && isdigit(date[posDia])) {
        dia += date[posDia];
        posDia++;
    }

    if (dia.size() == 1) dia = "0" + dia;

    // --- Saltar espacios antes de la hora ---
    while (posDia < date.size() && date[posDia] == ' ') posDia++;

    // --- Hora ---
    string hora = "";
    while (posDia < date.size() && (isdigit(date[posDia]) || date[posDia] == ':')) {
        hora += date[posDia];
        posDia++;
    }

    // --- Convertir mes ---
    string mesNum;
    for (int i = 0; i < 12; i++) {
        if (meses[i] == mesStr) {
            mesNum = (i < 9 ? "0" : "") + to_string(i + 1);
            break;
        }
    }

    // --- Limpiar hora ---
    string horaNum = "";
    for (char c : hora)
        if (isdigit(c)) horaNum += c;

    // --- Combinar ---
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


int loadData2File(int arr[], int size,string file_name){
   
    ofstream outFile(file_name);

    if (!outFile.is_open()) {
        cerr << "Error al abrir el archivo " << file_name << " para escritura." << endl;
        return 0;
    }

    for (int line=0; line< size; line++){
        
        outFile << arr[line];
        if (line < size - 1)  // evita salto extra al final
            outFile << "\n";
       


    }

    outFile.close();

    return 1;


    


}
