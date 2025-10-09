
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int MAX_RECORDS = 16807;

// Estructura para almacenar cada registro del log
struct Registro {
    long long fecha;   // Fecha numérica tipo MMDDHHMMSS
    string fecha_str;  // Fecha original (texto)
    string ip;
    string mensaje;
};

// Prototipos de funciones
long long Date2Int(const string& date);
int LoadData(string file_name, Registro data[]);
void mergeSort(Registro arr[], int left, int right);
void merge(Registro arr[], int left, int mid, int right);
int loadData2File(Registro data[], int size, string filename);

int main() {
    Registro registros[MAX_RECORDS];
    int numRegistros = LoadData("bitacora.txt", registros);

    if (numRegistros == -1) {
        cout << "Hubo un problema al cargar los datos." << endl;
        return 1;
    }

    cout << "Datos cargados exitosamente. Total registros: " << numRegistros << endl;

    cout << "\n--- Primeros 10 registros sin orden ---" << endl;
    for (int i = 0; i < 10 && i < numRegistros; i++) {
        cout << registros[i].fecha << " (" << registros[i].fecha_str << ") "
             << registros[i].ip << " " << registros[i].mensaje << endl;
    }

    // Ordenar los registros cronológicamente
    mergeSort(registros, 0, numRegistros - 1);

    cout << "\n--- Primeros 10 registros ordenados ---" << endl;
    for (int i = 0; i < 10 && i < numRegistros; i++) {
        cout << registros[i].fecha << " (" << registros[i].fecha_str << ") "
             << registros[i].ip << " " << registros[i].mensaje << endl;
    }

    if (loadData2File(registros, numRegistros, "bitacora_ordenada_v2.txt") == 1) {
        cout << "\nDatos ordenados guardados exitosamente en bitacora_ordenada_v2.txt" << endl;
    } else {
        cout << "\nFallo al guardar los datos ordenados." << endl;
    }

    return 0;
}

/**
 * Convierte una fecha tipo "Jun  1 00:01:09" a número MMDDHHMMSS.
 * Ejemplo: "Jun  1 00:01:09" → 0601000109
 */
long long Date2Int(const string& date) {
    string meses[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    string mesStr = date.substr(0, 3);
    string diaStr = date.substr(4, 2);
    if (diaStr[0] == ' ') diaStr[0] = '0';

    string horaStr = date.substr(7, 8); // "HH:MM:SS"

    int mes = 0;
    for (int i = 0; i < 12; i++) {
        if (meses[i] == mesStr) {
            mes = i + 1;
            break;
        }
    }

    int dia = stoi(diaStr);
    int hora = stoi(horaStr.substr(0, 2));
    int minuto = stoi(horaStr.substr(3, 2));
    int segundo = stoi(horaStr.substr(6, 2));

    // Cálculo uniforme tipo MMDDHHMMSS
    long long fechaInt = mes * 100000000LL +
                         dia * 1000000LL +
                         hora * 10000LL +
                         minuto * 100LL +
                         segundo;

    return fechaInt;
}

/**
 * Carga los datos desde el archivo de texto.
 */
int LoadData(string file_name, Registro data[]) {
    ifstream inputFile(file_name);
    if (!inputFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << file_name << "!" << endl;
        return -1;
    }

    string line;
    int count = 0, skipped = 0;

    while (getline(inputFile, line) && count < MAX_RECORDS) {
        if (line.length() < 16) { skipped++; continue; }

        size_t date_end = line.find(' ', 15);
        if (date_end == string::npos) { skipped++; continue; }

        size_t ip_end = line.find(' ', date_end + 1);
        if (ip_end == string::npos) { skipped++; continue; }

        string dateStr = line.substr(0, date_end);
        string ip = line.substr(date_end + 1, ip_end - date_end - 1);
        string mensaje = line.substr(ip_end + 1);

        long long fechaInt = Date2Int(dateStr);
        data[count] = {fechaInt, dateStr, ip, mensaje};
        count++;
    }

    inputFile.close();
    cout << "Líneas ignoradas: " << skipped << endl;
    return count;
}

/**
 * Mezcla dos subarreglos ordenados.
 */
void merge(Registro arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    Registro* L = new Registro[n1];
    Registro* R = new Registro[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].fecha <= R[j].fecha) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

/**
 * Ordena un arreglo de registros con MergeSort.
 */
void mergeSort(Registro arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

/**
 * Guarda los datos ordenados en un archivo de salida.
 */
int loadData2File(Registro data[], int size, string file_name) {
    ofstream outFile(file_name);
    if (!outFile.is_open()) {
        cerr << "Error al abrir el archivo " << file_name << " para escritura." << endl;
        return 0;
    }

    for (int i = 0; i < size; ++i) {
        outFile << data[i].fecha << " (" << data[i].fecha_str << ") "
                << data[i].ip << " " << data[i].mensaje;
        if (i < size - 1) outFile << "\n";
    }

    outFile.close();
    return 1;
}

