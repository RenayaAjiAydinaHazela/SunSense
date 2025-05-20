# 🌤️ **Program SunSense**

## 📋 **Deskripsi Program Simulasi**

Program ini merupakan bagian dari pengembangan sistem **SunSense**, yaitu solusi jemuran cerdas yang mampu beradaptasi terhadap kondisi lingkungan secara **otomatis maupun manual** menggunakan platform mikrokontroler **Arduino Nano**.

Simulasi ini dibangun di **Wokwi**, sebuah simulator online berbasis Arduino, dengan tujuan untuk **mengujicoba logika dan alur kerja sistem** sebelum diterapkan ke perangkat keras sesungguhnya.

Beberapa komponen fisik digantikan dengan **potensiometer** dan **push button** agar fleksibel dalam pengujian berbagai skenario lingkungan seperti suhu tinggi, cahaya rendah, atau kondisi hujan.

> Fokus utama simulasi: Menguji keakuratan pengambilan keputusan, kestabilan logika servo, serta validitas data yang dikirim via komunikasi serial.
> 

---

## 🗂️ **Struktur Program**

- **🔬 Program Simulasi (Prototype Virtual)**
    
    Menggunakan komponen pengganti untuk mencerminkan perilaku sensor asli. Simulasi ini menguji logika pengendalian servo dan pengambilan keputusan berdasarkan parameter lingkungan.
    
- **🔧 Program Asli Hardware (Final Deployment)** 
    
    Dirancang untuk implementasi fisik dengan koneksi sensor dan aktuator asli, termasuk penggunaan **DHT22**, **FC-37**, **LDR**, serta **HC-05** untuk integrasi mobile app melalui Bluetooth.
    

---

## 🛠️ **Komponen Simulasi di Wokwi**

| Komponen | Fungsi | Keterangan |
| --- | --- | --- |
| **Potensiometer 1** | Simulasi suhu lingkungan | Output analog dikonversi menjadi nilai suhu (rentang 0–50°C) |
| **Potensiometer 2** | Simulasi intensitas cahaya | Mewakili nilai dari sensor LDR, digunakan untuk deteksi siang/malam |
| **Potensiometer 3** | Simulasi kelembapan udara | Input analog dikonversi menjadi % kelembapan |
| **Push Button 1** | Simulasi kondisi hujan | Nilai LOW = hujan; HIGH = tidak hujan |
| **Push Button 2** | Pergantian mode otomatis <-> manual | Ditekan sekali untuk mengubah mode kerja |
| **Push Button 3** | Kontrol manual jemuran | Aktif hanya saat berada di mode manual |
| **Servo Motor** | Simulasi aktuator penggerak jemuran | Digerakkan ke posisi 0° (masuk) atau 45° (keluar) |

> Kombinasi potensiometer dan tombol memungkinkan simulasi dinamis terhadap perubahan lingkungan dan intervensi pengguna.
> 

---

## 🔌 **Konfigurasi Pin: Simulasi vs Hardware**

### 🔧 Simulasi (Wokwi)

| Perangkat Simulasi | Pin Arduino | Tipe Pin | Fungsi dalam Simulasi |
| --- | --- | --- | --- |
| Potensiometer 1 | A0 | Analog | Suhu |
| Potensiometer 2 | A1 | Analog | Cahaya (LDR) |
| Potensiometer 3 | A2 | Analog | Kelembapan |
| Push Button 1 (Biru) | D2 | Digital | Hujan |
| Push Button 2 (Merah) | D3 | Digital | Ganti mode |
| Push Button 3 (Hijau)| D4 | Digital | Kontrol manual |
| Servo Motor | D5 | PWM | Aktuator jemuran |

### ⚙️ Hardware (Final)

| Perangkat Asli | Pin Arduino Nano | Tipe Pin | Keterangan |
| --- | --- | --- | --- |
| LDR (Sensor Cahaya) | A1 | Analog | Mengukur intensitas pencahayaan |
| FC-37 (Sensor Hujan) | D8 | Digital | Deteksi permukaan basah/kering |
| DHT22 | D6 | Digital | Mengukur suhu & kelembapan |
| Servo Motor | D7 | PWM | Menggerakkan mekanisme jemuran |
| HC-05 Bluetooth | D0 (TX), D1 (RX) | UART | Komunikasi dengan mobile app |

---

## 🧠 **Alur Logika Program**

Alur program pada sistem SunSense terbagi menjadi **5 tahap utama** yang berlangsung secara berulang (loop) selama sistem berjalan. Masing-masing tahap dijelaskan lebih detail berikut ini:

---

### 🟢 **1. Pembacaan Sensor Simulasi**

Sistem membaca data dari tiga potensiometer sebagai pengganti sensor fisik:

- **Potensiometer 1 (A0):**
    - Disimulasikan sebagai **suhu lingkungan**.
    - Nilai analog (0–1023) dikonversi ke skala suhu (sekitar 0–50°C) menggunakan rumus linear.
    - Tujuannya untuk meniru pembacaan dari sensor **DHT22** atau **LM35**.
- **Potensiometer 2 (A1):**
    - Disimulasikan sebagai **intensitas cahaya** (LDR).
    - Nilai analog tinggi → cahaya terang, nilai rendah → cahaya gelap.
    - Threshold digunakan untuk menentukan siang atau malam.
- **Potensiometer 3 (A2):**
    - Disimulasikan sebagai **kelembapan udara**.
    - Dikonversi ke skala kelembapan (0–100%).
    - Mewakili kemampuan udara untuk menahan uap air, berkaitan dengan potensi hujan atau penguapan.

🔄 *Pembacaan dilakukan secara terus-menerus setiap siklus loop untuk menangkap perubahan kondisi lingkungan secara real time.*

---

### 🟨 **2. Pembacaan Input Push Button**

Sistem menggunakan **tiga push button digital** untuk mengontrol dan menyimulasikan peristiwa:

- **Push Button 1 (D2) – Deteksi Hujan:**
    - Jika ditekan (LOW), maka sistem mendeteksi bahwa **hujan sedang turun**.
    - Jika tidak ditekan (HIGH), maka cuaca dianggap **kering**.
    - Ini meniru pembacaan digital dari sensor **FC-37** yang mendeteksi tetesan air.
- **Push Button 2 (D3) – Toggle Mode Otomatis/Manual:**
    - Menekan tombol ini akan **mengubah mode kerja** sistem secara *toggle*:
        - Jika sebelumnya **otomatis**, maka berubah ke **manual**.
        - Jika sebelumnya **manual**, kembali ke **otomatis**.
    - Digunakan `flag` (variabel penanda) untuk menyimpan status mode saat ini.
- **Push Button 3 (D4) – Kontrol Manual:**
    - Hanya aktif saat berada dalam **mode manual**.
    - Jika ditekan → jemuran **keluar** (servo ke 45°).
    - Jika dilepas → jemuran **masuk** (servo ke 0°).
    - Simulasi ini memberi pengguna kontrol langsung atas jemuran, seperti **override** otomatis.

---

### 🟧 **3. Logika Keputusan Sistem (Decision Making)**

### 🔁 **Jika Mode Otomatis Aktif:**

Sistem akan **menganalisis parameter lingkungan** untuk menentukan apakah jemuran sebaiknya dikeluarkan atau dimasukkan:

| Parameter | Nilai Ideal | Keterangan |
| --- | --- | --- |
| Suhu | > 30°C | Semakin tinggi suhu, semakin baik untuk pengeringan pakaian |
| Cahaya | > 300 (dari 1023) | Menandakan siang hari (terang), cocok untuk menjemur |
| Kelembapan | < 80% | Kelembapan rendah → udara kering → mempercepat pengeringan |
| Deteksi Hujan | Tidak hujan (HIGH) | Jika ada hujan → jemuran wajib masuk tanpa mempedulikan parameter lain |

**Logika gabungan (AND):**

```c
if (suhu > 30 && cahaya > 300 && kelembapan < 80 && !hujan) {
    // jemuran keluar
} else {
    // jemuran masuk
}
```

📌 **Catatan penting:**

Jika **salah satu saja** dari syarat di atas tidak terpenuhi, sistem akan memutuskan untuk **memasukkan jemuran** demi keamanan.

---

### ⛔ **Jika Mode Manual Aktif:**

- Sistem **mengabaikan semua data sensor**, termasuk deteksi hujan.
- **Keputusan hanya diambil dari tombol push button 3:**
    - Tekan → jemuran keluar.
    - Lepas → jemuran masuk.
- Cocok untuk kondisi tertentu di mana pengguna ingin mengambil alih kontrol sepenuhnya (misalnya, menjemur sepatu atau pakaian khusus dalam cuaca ekstrem tapi mendesak).

---

### 🟦 **4. Pengendalian Aktuator (Servo Motor)**

Servo motor digunakan untuk mensimulasikan pergerakan jemuran dengan dua posisi utama:

| Posisi Servo | Sudut | Aksi Jemuran |
| --- | --- | --- |
| 0° | MINIMAL | Jemuran masuk |
| 45° | TERBUKA | Jemuran keluar |

### 🔄 Alur Pengendalian:

- Program hanya akan **menggerakkan servo** jika posisi saat ini berbeda dari posisi sebelumnya untuk menghindari getaran servo yang tidak perlu (hemat energi & memperpanjang umur servo).

```c
if (statusSekarang != statusSebelumnya) {
   servo.write(sudutBaru);
}
```

---

### 📤 **5. Pengiriman Data ke Serial Monitor**

Semua data penting akan ditampilkan di **Serial Monitor** untuk tujuan debugging, pemantauan, dan dokumentasi logika sistem.

**Data yang dikirim meliputi:**

- Nilai suhu dalam °C
- Nilai kelembapan dalam %
- Nilai cahaya (0–1023)
- Status hujan (Ya/Tidak)
- Mode aktif (Otomatis/Manual)
- Aksi jemuran (Keluar/Masuk)

📦 Format contoh di Serial Monitor:

```
Mode: Otomatis
Suhu: 32.5 °C
Kelembapan: 65.3 %
Cahaya: 540
Status Hujan: Tidak
Jemuran: Keluar
-----------------------------
```
Data ini juga dapat dimanfaatkan pada tahap selanjutnya untuk **dikirim ke aplikasi mobile** via Bluetooth HC-05 dalam implementasi hardware.

---

## 🔍 **Threshold Logika Otomatis (Contoh)**

| Parameter | Ambang Batas | Keputusan Sistem |
| --- | --- | --- |
| Suhu | > 30°C | Jemuran dapat keluar |
| Intensitas Cahaya | > 300 (analog) | Siang, jemuran dapat keluar |
| Kelembapan | < 80% | Aman untuk menjemur |
| Status Hujan | Tidak hujan | Jemuran dapat keluar |

> Jika salah satu parameter tidak memenuhi, jemuran akan masuk otomatis untuk perlindungan.
> 

---

## 📡 **Integrasi ke Perangkat Mobile (Tahap Hardware)**

- Komunikasi dua arah menggunakan **HC-05 Bluetooth**.
- Data suhu, kelembapan, kondisi hujan, dan status jemuran ditampilkan di aplikasi mobile.
- Pengguna juga bisa mengganti mode dan mengontrol jemuran dari aplikasi.

---

## 💻 **Simulasi Online**
   
   👉 [Klik di sini untuk membuka simulasi di Wokwi](https://wokwi.com/projects/429473945254121473)


  <img src="https://github.com/RenayaAjiAydinaHazela/SunSense/blob/main/Program/Asset/Screenshot%202025-05-11%20200524.png">
  
---
