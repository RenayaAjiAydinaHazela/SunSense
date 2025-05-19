# 🌞 Hardware SunSense

SunSense dikembangkan sebagai sistem pintar yang mampu merespons kondisi lingkungan secara real-time melalui sensor cahaya, hujan, suhu, dan kelembapan. Dengan memanfaatkan Arduino Nano sebagai otak utama dan modul komunikasi Bluetooth HC-05, proyek ini dirancang untuk memberikan solusi otomatisasi jemuran berbasis cuaca yang praktis dan efisien. 

Sistem ini tidak hanya dapat memantau, tetapi juga melakukan aksi langsung melalui motor servo yang dikendalikan otomatis sesuai data sensor. Pendekatan desain yang kompak, modular, dan berbasis user-experience menjadikan SunSense ideal untuk diterapkan dalam rumah tangga cerdas maupun pengembangan teknologi IoT skala kecil.

---

## ⚙️ Spesifikasi Hardware

| **Komponen** | **Tipe** | **Fungsi** |
| --- | --- | --- |
| **Mikrokontroler** | Arduino Nano | Mikrokontroler utama yang mengelola seluruh proses logika, pengambilan keputusan berdasarkan input sensor. |
| **Sensor Cahaya** | LDR | Mendeteksi intensitas pencahayaan matahari. Digunakan untuk memutuskan kapan jemuran boleh terbuka. |
| **Sensor Hujan** | FC-37 (Rain Drop) | Menentukan keberadaan air/hujan melalui hambatan listrik. Jika mendeteksi hujan (basah), sistem menutup jemuran. |
| **Sensor Suhu**  | DHT22 | Memberikan data suhu dan kelembapan lingkungan sebagai informasi tambahan untuk mengatur kondisi optimal pengeringan. |
| **Motor Servo** | SG90 | Aktuator untuk menggerakkan mekanisme buka-tutup jemuran secara otomatis. |
| **Driver Motor** | L298N | Mengatur suplai daya dan sinyal kontrol ke motor dari mikrokontroler. |
| **Regulator Tegangan** | L7805 | Mengatur dan menstabilkan tegangan 5V dari sumber daya eksternal ke komponen seperti mikrokontroler dan sensor. |
| **Bluetooth Module** | HC-05 | Komunikasi serial jarak dekat untuk kontrol manual dan monitoring via aplikasi mobile. |
| **Power Supply (Baterai)** | Ultrafire 15650A 3000mAh | Menyediakan daya untuk menggerakkan motor dan sistem kontrol secara keseluruhan. |
| **PCB** | Single Layer | Menyusun dan menyambungkan seluruh rangkaian elektronik secara rapi dan terorganisir. |
| **Bearing / Pulley** | - | Mendukung gerakan mekanik agar lebih halus, stabil, dan minim gesekan. |

---

## 📌 Diagram Pin Arduino

| Perangkat | Pin Arduino Nano | Jenis Pin | Keterangan |
| --- | --- | --- | --- |
| LDR | A0 | Analog | Sensor cahaya |
| FC-37 (Sensor Hujan) | D2 | Digital | Sensor hujan |
| DHT22 | D3 | Digital | Suhu & kelembapan |
| Servo Motor | D5 | PWM | Kendali aktuator |
| HC-05 Bluetooth | D6 (TX), D7 (RX) | UART | Komunikasi Serial |

---

## 🔌 Deskripsi Pin dan Fungsi

### 🟢 GPIO Output

### 🧭 Servo Motor

Motor servo dikendalikan melalui **pin D5** menggunakan sinyal **PWM (Pulse Width Modulation)** dari Arduino. Pergerakan servo digunakan untuk **mengoperasikan mekanisme buka-tutup jemuran** secara presisi dan otomatis. Keputusan penggerakan servo didasarkan pada hasil analisis data dari sensor lingkungan, seperti hujan, suhu, dan intensitas cahaya, guna memastikan jemuran hanya terbuka pada kondisi cuaca yang aman dan optimal.

---

### 🔁 UART Communication

### 📶 HC-05 (Bluetooth Module)

Modul HC-05 dihubungkan ke pin **D6 (TX)** dan **D7 (RX)** Arduino, memungkinkan komunikasi **serial dua arah** antara sistem dan aplikasi pada perangkat seluler. Modul ini berfungsi sebagai antarmuka **nirkabel** yang memudahkan pengguna untuk **memantau status sensor secara real-time** serta **mengontrol sistem jemuran otomatis** langsung dari jarak dekat melalui koneksi Bluetooth. Integrasi ini meningkatkan fleksibilitas dan kemudahan dalam pengoperasian sistem.

---

### 📥 Analog Input

### 🟡 LDR (Light Dependent Resistor)

Sensor LDR terhubung ke pin **A0** dan menghasilkan sinyal **analog** yang merepresentasikan intensitas pencahayaan lingkungan. Arduino membaca perubahan tegangan dalam rentang **0–5V** dan menginterpretasikannya sebagai indikator cuaca cerah atau mendung. Nilai ini digunakan sistem untuk membantu menentukan apakah jemuran otomatis perlu **dibuka atau ditutup**, berdasarkan ambang batas pencahayaan yang telah diprogram

### 📥 Digital Input

---

### 🌧️ FC-37 (Sensor Hujan)

Sensor hujan FC-37 dihubungkan ke pin **D2** dan memberikan sinyal digital berupa **HIGH** atau **LOW** berdasarkan kondisi permukaan sensornya. Ketika permukaan sensor dalam kondisi **basah**, output akan **LOW**, dan ketika **kering**, output akan **HIGH**. Informasi ini digunakan sistem sebagai indikator otomatis untuk mendeteksi keberadaan hujan dan mengaktifkan mekanisme penutupan jemuran.

### 🌡️ DHT22 (Sensor Suhu & Kelembapan)

Terhubung ke pin **D3**, sensor DHT22 menggunakan protokol komunikasi **single-wire (*One-Wire*)** untuk mengirimkan data suhu dan kelembapan secara berkala. Informasi lingkungan ini menjadi parameter pendukung yang penting dalam sistem pengambilan keputusan, guna memastikan proses pengeringan berjalan pada kondisi yang optimal dan efisien.

---

## 📦 Library Arduino

Instal melalui Library Manager di Arduino IDE:

- `DHT sensor library`
- `Adafruit Unified Sensor`
- `Servo.h` – Tersedia secara default di Arduino IDE

---

## 🏷️ Lisensi

Proyek ini merupakan inovasi berbasis mikrokontroler yang dikembangkan secara mandiri oleh mahasiswa **Electronic Engineering Polytechnic Institute of Surabaya (EEPIS)** untuk menjawab kebutuhan otomatisasi rumah tangga yang cerdas dan efisien. 
SunSense dirancang dengan pendekatan teknis yang solid, menggunakan sensor cuaca dan aktuator presisi untuk mendeteksi kondisi lingkungan secara real-time dan mengoperasikan sistem jemuran otomatis.

Proyek ini terbuka untuk dikembangkan lebih lanjut dan dapat dimodifikasi untuk kebutuhan komersial maupun edukatif, selama tetap mencantumkan sumber. Dengan pendekatan modular, efisien, dan berbasis data, SunSense menjadi solusi potensial dalam pengembangan smart home berbasis teknologi lokal yang scalable dan dapat diandalkan.
