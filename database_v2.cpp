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
int binarySearchInicio(Registro data[], int size, long long fechaInicio);
int binarySearchFin(Registro data[], int size, long long fechaFin);

int main() {
Registro registros[MAX_RECORDS];
int numRegistros = LoadData("bitacora.txt", registros);


if (numRegistros == -1) {
    cout << "Hubo un problema al cargar los datos." << endl;
    return 1;
}

cout << "Datos cargados exitosamente. Total registros: " << numRegistros << endl;

mergeSort(registros, 0, numRegistros - 1);
cout << "\nRegistros ordenados correctamente." << endl;

string fechaInicioStr, fechaFinStr;
cout << "\nIngrese la fecha de inicio (formato Ej: Jun1 00:00:00): ";
getline(cin, fechaInicioStr);
cout << "Ingrese la fecha de fin (formato Ej: Jun 30 23:59:59): ";
getline(cin, fechaFinStr);

long long fechaInicio = Date2Int(fechaInicioStr);
long long fechaFin = Date2Int(fechaFinStr);

int idxInicio = binarySearchInicio(registros, numRegistros, fechaInicio);
int idxFin = binarySearchFin(registros, numRegistros, fechaFin);

if (idxInicio == -1 || idxFin == -1 || idxInicio > idxFin) {
    cout << "No se encontraron registros en ese rango." << endl;
    return 0;
}

cout << "\n--- Registros en el rango ---\n";
for (int i = idxInicio; i <= idxFin; ++i) {
    cout << registros[i].fecha << " (" << registros[i].fecha_str << ") "
         << registros[i].ip << " " << registros[i].mensaje << endl;
}

return 0;


}



#include <sstream>

long long Date2Int(const string& date) {
    string mesStr, diaStr, horaStr;
    stringstream ss(date);
    ss >> mesStr >> diaStr >> horaStr;  // divide por espacios

    // Corrige día si tiene un solo dígito
    if (diaStr.size() == 1) diaStr = "0" + diaStr;

    string meses[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    int mes = 0;
    for (int i = 0; i < 12; i++) {
        if (meses[i] == mesStr) {
            mes = i + 1;
            break;
        }
    }

    int hora = stoi(horaStr.substr(0, 2));
    int minuto = stoi(horaStr.substr(3, 2));
    int segundo = stoi(horaStr.substr(6, 2));

    long long fechaInt = mes * 100000000LL +
                         stoi(diaStr) * 1000000LL +
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

* Búsqueda binaria para encontrar el índice del primer registro >= fechaInicio.
  */
  int binarySearchInicio(Registro data[], int size, long long fechaInicio) {
  int left = 0, right = size - 1, result = -1;
  while (left <= right) {
  int mid = left + (right - left) / 2;
  if (data[mid].fecha >= fechaInicio) {
  result = mid;
  right = mid - 1;
  } else {
  left = mid + 1;
  }
  }
  return result;
  }

/**

* Búsqueda binaria para encontrar el índice del último registro <= fechaFin.
  */
  int binarySearchFin(Registro data[], int size, long long fechaFin) {
  int left = 0, right = size - 1, result = -1;
  while (left <= right) {
  int mid = left + (right - left) / 2;
  if (data[mid].fecha <= fechaFin) {
  result = mid;
  left = mid + 1;
  } else {
  right = mid - 1;
  }
  }
  return result;
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
