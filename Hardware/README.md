# 🌞 SunSense: Sistem Monitoring Cuaca dan Cahaya Berbasis Arduino Nano

Proyek ini bertujuan untuk memantau kondisi lingkungan seperti cahaya, hujan, suhu, dan kelembapan menggunakan Arduino Nano dan berbagai sensor yang digunakan untuk projek jemuran pintar. Data dapat dipantau secara langsung dan dikontrol melalui modul Bluetooth HC-05.

---

## 🧩 Daftar Isi

- [Spesifikasi Hardware](#-spesifikasi-hardware)
- [Diagram Pin Arduino](#-diagram-pin-arduino)
- [Deskripsi Pin dan Fungsi](#-deskripsi-pin-dan-fungsi)
  - [GPIO Output](#-gpio-output)
  - [UART Communication](#-uart-communication)
  - [Analog Input](#-analog-input)
  - [Sensor Digital (non-analog)](#-sensor-digital-non-analog)
- [Library Arduino](#-library-arduino)

---

## ⚙️ Spesifikasi Hardware

| Komponen             | Fungsi                                         |
|----------------------|------------------------------------------------|
| Arduino Nano         | Mikrokontroler utama                           |
| LDR                  | Deteksi intensitas cahaya                      |
| FC-37 (Sensor Hujan) | Deteksi hujan (basah/kering)                   |
| DHT22                | Sensor suhu dan kelembapan lingkungan          |
| HC-05                | Komunikasi Bluetooth serial                    |
| Servo (SG90)         | Aktuator untuk membuka/menutup mekanisme       |
| L7805                | Regulator tegangan 5V dari sumber eksternal    |

---

## 📌 Diagram Pin Arduino

| Perangkat            | Pin Arduino Nano | Jenis Pin | Keterangan             |
|----------------------|------------------|-----------|------------------------|
| LDR                  | A0               | Analog    | Sensor cahaya          |
| FC-37 (Sensor Hujan) | D2               | Digital   | Sensor hujan           |
| DHT22                | D3               | Digital   | Suhu & kelembapan      |
| Servo Motor          | D5               | PWM       | Kendali aktuator       |
| HC-05 Bluetooth      | D6 (TX), D7 (RX) | UART      | Komunikasi Serial      |

---

## 🔌 Deskripsi Pin dan Fungsi

### 🟢 GPIO Output
#### 🧭 Servo Motor
- **Pin Arduino:** D5
- **Jenis:** PWM Output
- **Fungsi:** Menggerakkan motor servo untuk membuka/menutup berdasarkan kondisi sensor.

---

### 🔁 UART Communication
#### 📶 HC-05 (Bluetooth Module)
- **TX HC-05 → RX Arduino (D6)**  
- **RX HC-05 ← TX Arduino (D7)**
- **Fungsi:** Mengirim dan menerima data dari aplikasi Bluetooth di ponsel atau perangkat lain.

---

### 📥 Analog Input
#### 🟡 LDR (Light Dependent Resistor)
- **Pin Arduino:** A0
- **Fungsi:** Mendeteksi intensitas cahaya lingkungan.
- **Cara Kerja:** Nilai resistansi berubah tergantung cahaya yang masuk, menghasilkan tegangan analog (0–5V) yang dibaca oleh Arduino.

---

### 📥 Sensor Digital (non-analog)

#### 🌧️ FC-37 (Sensor Hujan)
- **Pin Arduino:** D2
- **Fungsi:** Deteksi kehadiran air/hujan.
- **Output:** Digital HIGH/LOW sesuai kelembapan pada permukaan sensor.

#### 🌡️ DHT22 (Sensor Suhu & Kelembapan)
- **Pin Arduino:** D3
- **Fungsi:** Mengukur suhu udara (°C) dan kelembapan (%RH).
- **Catatan:** Menggunakan protokol satu kabel (One-Wire) dan memerlukan library khusus.

---

## 📦 Library Arduino

Instal melalui Library Manager di Arduino IDE:

- `DHT sensor library`
- `Adafruit Unified Sensor`
- `Servo.h` – Tersedia secara default di Arduino IDE
---

## 🏷️ Lisensi

Proyek ini dikembangkan untuk keperluan edukasi di **Electronic Engineering Polytechnic Institute of Surabaya (EEPIS)** dan bebas dimodifikasi selama mencantumkan sumber.

---

