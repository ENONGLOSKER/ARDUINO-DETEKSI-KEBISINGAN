#include <Wire.h>
#include <LiquidCrystal_I2C.h>  // Library untuk LCD I2C

// Definisi pin
#define SENSOR_PIN A0        // Data Sensor Suara
#define ISD_PIN 3            // Data ISD1820
#define SPEAKER_PIN 8        // Pin untuk speaker tambahan

// Inisialisasi LCD I2C (alamat I2C biasanya 0x27)
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Alamat I2C, 16 kolom, 2 baris

// Variabel
int sensorValue = 0;
int averageValue = 0;
const int threshold = 44.40;   // Nilai ambang batas suara
String peringatan1 = "RIBUT! "; // Pesan bagian atas
String peringatan2 = "ANDA BISA DIAM!"; // pesan bagian bawah

void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(9600);
  
  // Inisialisasi LCD I2C
  lcd.init();
  lcd.backlight();
  lcd.print("Sistem Deteksi");
  lcd.setCursor(0, 1);
  lcd.print("    Suara");
  delay(2000);
  lcd.clear();
  
  // Set pin mode
  pinMode(ISD_PIN, OUTPUT);
  digitalWrite(ISD_PIN, LOW);
  pinMode(SPEAKER_PIN, OUTPUT);
  digitalWrite(SPEAKER_PIN, LOW);
}

void loop() {
  // Reset nilai rata-rata
  averageValue = 0;
  
  // Mengambil 10 sample dan menghitung rata-rata
  for (int i = 0; i < 10; i++) {
    sensorValue = analogRead(SENSOR_PIN);
    averageValue += sensorValue;
    delay(10);  // Delay antar pembacaan
  }
  averageValue /= 10;
  
  // Tampilkan nilai di Serial Monitor
  Serial.print("Nilai sensor: ");
  Serial.print(averageValue);
  
  // Deteksi suara keras
  if (averageValue > threshold) {
    Serial.println(" - Suara keras terdeteksi!");
    
    // Tampilkan peringatan di LCD
    lcd.setCursor(0, 0);
    lcd.print(peringatan1);
    lcd.setCursor(0, 1);
    lcd.print(peringatan2);
    
    // Aktifkan modul ISD1820
    digitalWrite(ISD_PIN, HIGH);
    delay(500);  // Rekam atau putar suara
    digitalWrite(ISD_PIN, LOW);
    
    // Aktifkan buzzer internal (jika ada)
    tone(SPEAKER_PIN, 1000, 1000);  // Frekuensi 1kHz selama 1 detik
    
    delay(15000);  // Jeda sebelum deteksi berikutnya
    lcd.clear();
  } else {
    Serial.println(" - Kondisi normal");
    
    // Tampilkan status normal di LCD
    lcd.setCursor(0, 0);
    lcd.print("Status:");
    lcd.setCursor(0, 1);
    lcd.print("Normal");
    
    delay(1000);
    lcd.clear();
  }
  
  delay(100);  // Jeda antar pembacaan
}