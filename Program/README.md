# Program 


## 📋 Deskripsi Program Simulasi
Program ini mensimulasikan sistem jemuran otomatis atau manual menggunakan Arduino.  
Simulasi dilakukan menggunakan potensiometer dan push button untuk menggantikan sensor fisik.

Terdapat dua mode:
- **Otomatis**: Jemuran keluar/masuk berdasarkan data simulasi dari potensiometer dan push button.
- **Manual**: Jemuran dikontrol secara manual menggunakan push button.

Data kondisi juga dikirimkan melalui Bluetooth ke perangkat lain.

---

## 🗂️ Struktur Program
- **Program Simulasi** → Program ini dibuat khusus untuk simulasi menggunakan komponen sederhana.
- **Program Asli Hardware** → *(Progress)*

---

## 🛠️ Komponen Simulasi
- **2 Potensiometer**:
  - Potensio 1 → Simulasi sensor suhu.
  - Potensio 2 → Simulasi sensor cahaya.

- **3 Push Button**:
  - Push Button 1 (Warna Biru) → Simulasi kondisi hujan.
  - Push Button 2 (Warna Merah)→ Ganti mode Otomatis/Manual.
  - Push Button 3 (Warna Hijau)→ Kontrol jemuran masuk/keluar (hanya aktif di mode Manual).

- **1 Servo Motor**:
  - Untuk menggerakkan jemuran keluar (sudut 45°) atau masuk (sudut 0°).

---

## 🧠 Alur Program
1. **Baca nilai potensiometer**:
   - Suhu: 0–50°C.
   - Cahaya: nilai terang–gelap.

2. **Baca push button**:
   - Push button hujan → Simulasi ada/tidak ada hujan.
   - Push button mode → Ganti dari otomatis ke manual dan sebaliknya.
   - Push button kontrol → (Jika mode manual) mengatur jemuran keluar/masuk.

3. **Buat keputusan**:
   - **Mode otomatis**: Jemuran otomatis keluar/masuk berdasarkan kondisi suhu, cahaya, dan hujan.
   - **Mode manual**: Jemuran dikontrol langsung via push button kontrol.

4. **Gerakkan servo** sesuai keputusan.

5. **Kirim data** suhu, cahaya, kondisi hujan, mode, dan posisi jemuran melalui Bluetooth.

---
6. **Contoh Gambar Simulasi**
  <img src="https://github.com/RenayaAjiAydinaHazela/SunSense/blob/main/Program/Screenshot%202025-04-29%20114008.png">
  ---

## 📄 Cara Mensimulasikan
Gunakan link ini untuk simulasi ke wokwi https://wokwi.com/projects/429473945254121473
