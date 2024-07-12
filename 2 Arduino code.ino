#include <SPI.h>
#include <SD.h>

// Definire i pin
const int MQ135_PIN = A0;   // Pin analogico per il sensore MQ-135
const int CS_PIN = 10;      // Pin CS per l'adattatore MicroSD

// Oggetto per il file sulla SD
File dataFile;

// Variabili per il timestamp approssimativo
unsigned long startTime;
unsigned long previousMillis = 0;
const long interval = 60000; // Intervallo di lettura (1 secondo)

void setup() {
  // Inizializzazione seriale per debug
  Serial.begin(9600);
  while (!Serial) {
    ; // Aspetta che la seriale sia pronta
  }

  // Inizializzazione SD
  if (!SD.begin(CS_PIN)) {
    Serial.println("Inizializzazione SD fallita!");
    while (1);
  }
  
  // Creazione file CSV
  dataFile = SD.open("dati_gas.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println("timestamp (s),valore % gas");
    dataFile.close();
  } else {
    Serial.println("Errore nella creazione del file CSV!");
    while (1);
  }

  // Inizializzazione del tempo iniziale
  startTime = millis();
}

void loop() {
  unsigned long currentMillis = millis();

  // Controlla se è passato un intervallo di un secondo
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    unsigned long elapsedSeconds = (currentMillis - startTime) / 1000;

    // Leggere il valore analogico dal sensore MQ-135
    int analogValue = analogRead(MQ135_PIN);

    // Convertire il valore analogico in una percentuale (esempio di conversione)
    float gasPercentage = analogValue / 1023.0 * 100.0;

    // Salvare i dati sulla scheda SD
    dataFile = SD.open("dati_gas.csv", FILE_WRITE);
    if (dataFile) {
      dataFile.print(elapsedSeconds);
      dataFile.print(",");
      dataFile.println(gasPercentage);
      dataFile.close();
      Serial.print("Dato salvato: ");
      Serial.print(elapsedSeconds);
      Serial.print(" s, ");
      Serial.println(gasPercentage);
    } else {
      Serial.println("Errore nell'apertura del file CSV!");
    }
  }
}
