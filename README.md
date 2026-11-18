# Sistem Palang Parkir Otomatis & Penghitung Kendaraan (IoT)

Sistem otomatisasi palang pintu parkir berbasis mikrokontroler (Arduino) yang dilengkapi dengan fitur penghitung jumlah kendaraan secara *real-time*. Proyek ini memiliki dua jalur terpisah (pintu masuk dan pintu keluar) serta penyimpanan data non-volatil menggunakan **EEPROM** internal agar data jumlah kendaraan tidak hilang meskipun daya listrik terputus.

> **Informasi Kompetisi:** Proyek ini dikembangkan dan diikutsertakan untuk mewakili **SMKN 9 Surakarta** dalam ajang kompetisi **KRENOVA 2026 UNISRI** (Universitas Slamet Riyadi).

## 📋 Daftar Isi

* [Tim Pengembang](#-tim-pengembang)

* [Fitur Utama](#-fitur-utama)

* [Cara Kerja Sistem](#-cara-kerja-sistem)

* [Komponen Perangkat Keras (Hardware)](#-komponen-perangkat-keras-hardware)

* [Struktur Repositori](#-struktur-repositori)

* [Skema Rangkaian (Wiring)](#-skema-rangkaian-wiring)

* [Instalasi & Penggunaan](#-instalasi--penggunaan)

* [Kontribusi](#-kontribusi)

## 👥 Tim Pengembang

Proyek ini dirancang dan dibangun oleh **Tim "Satu Frekuensi"** dari **SMKN 9 Surakarta**:

* **Dimmanuel Adhitya Sulistyo (TJKT 1)**

* **Ceva Aska Salsabil (TJKT 1)**

## ✨ Fitur Utama

* **Dua Jalur Terpisah:** Menggunakan 1 gerbang khusus untuk kendaraan masuk dan 1 gerbang khusus untuk kendaraan keluar.

* **Deteksi Jarak Presisi:** Menggunakan sensor ultrasonik untuk mendeteksi keberadaan kendaraan pada jarak $\le 10\text{ cm}$.

* **Jeda Pengaman Otomatis:** Palang pintu memiliki waktu tunda (*delay*) selama **3 detik** setelah kendaraan melewati sensor sebelum menutup kembali untuk mencegah palang mengenai bodi kendaraan.

* **Penyimpanan Permanen (EEPROM):** Data jumlah kendaraan yang terparkir disimpan secara otomatis ke dalam memori **EEPROM** Arduino, sehingga data tetap aman saat perangkat mati atau di-*restart*.

* **Monitoring Real-Time:** Menampilkan informasi status palang dan total kendaraan yang berada di area parkir melalui layar **LCD I2C 16x2**.

## ⚙️ Cara Kerja Sistem

1. **Alur Pintu Masuk:**

   * Sensor Ultrasonik Masuk mendeteksi objek/kendaraan pada jarak $\le 10\text{ cm}$.

   * Motor Servo Masuk bergerak membuka palang sebesar $90^\circ$.

   * Setelah kendaraan melaju dan keluar dari jangkauan sensor ($> 10\text{ cm}$), sistem menghitung mundur selama **3 detik**.

   * Palang pintu menutup kembali ke posisi semula ($0^\circ$).

   * Jumlah kendaraan di dalam parkiran bertambah (`+1`), disimpan ke **EEPROM**, dan diperbarui pada layar **LCD I2C**.

2. **Alur Pintu Keluar:**

   * Sensor Ultrasonik Keluar mendeteksi kendaraan yang akan keluar pada jarak $\le 10\text{ cm}$.

   * Motor Servo Keluar bergerak membuka palang sebesar $90^\circ$.

   * Setelah kendaraan meninggalkan area sensor ($> 10\text{ cm}$), sistem menunggu selama **3 detik** sebelum menutup palang ($0^\circ$).

   * Jumlah kendaraan di dalam parkiran berkurang (`-1`), diperbarui ke **EEPROM**, dan ditampilkan pada layar **LCD I2C**.

## 🛠️ Komponen Perangkat Keras (Hardware)

Untuk membangun proyek ini, komponen yang dibutuhkan adalah:

* **1x** Arduino Uno / Nano (atau mikrokontroler kompatibel)

* **2x** Sensor Ultrasonik HC-SR04 (1 untuk Masuk, 1 untuk Keluar)

* **2x** Motor Servo SG90 / MG996R (1 untuk Palang Masuk, 1 untuk Palang Keluar)

* **1x** LCD 16x2 dengan Modul I2C

* **1x** Breadboard & Kabel Jumper secukupnya

* **1x** Catu Daya / Adaptor 5V (Disarankan menggunakan catu daya eksternal untuk menyuplai daya motor servo agar Arduino tidak *reset*)

## 📂 Struktur Repositori

```
├── Code/
│   └── SourceCode.ino   # Source code utama program Arduino
├── Images/
│   ├── skema_rangkaian.jpg          # Gambar skema jalur kabel
│   └── skema_rangkaian_simple.jpg   # Gambar skema jalur kabel secara simpel
└── README.md

```

## 🔌 Skema Rangkaian (Wiring)

Anda dapat melihat gambar diagram rangkaian lengkap pada folder `/skema`. Berikut adalah tabel pemetaan pin standar yang digunakan pada proyek ini *(sesuaikan dengan kode pada folder `/sketch`)*:

| Komponen | Pin Komponen | Pin Arduino | 
| ----- | ----- | ----- | 
| **LCD I2C 16x2** | VCC / GND | 5V / GND | 
|  | SDA / SCL | A4 / A5 | 
| **Ultrasonik Masuk** | TRIG / ECHO | Pin 2 / Pin 3 | 
| **Ultrasonik Keluar** | TRIG / ECHO | Pin 5 / Pin 6 | 
| **Servo Masuk** | Signal (Kuning/Oranye) | Pin 4 | 
| **Servo Keluar** | Signal (Kuning/Oranye) | Pin 7 | 

*(Catatan: Pastikan semua pin `GND` dari sensor, servo, catu daya eksternal, dan Arduino terhubung menjadi satu / Common Ground).*

## 🚀 Instalasi & Penggunaan

1. **Clone Repositori**

   ```
   git clone https://github.com/dimmanuelas/IoT-Palang-Otomatis-dan-Penghitung-Kendaraan.git
   
   ```

2. **Buka Source Code**
   Buka file `.ino` yang ada di dalam folder `Code/` menggunakan **Arduino IDE**.

3. **Instal Library yang Dibutuhkan**
   Pastikan Anda sudah menginstal library berikut melalui *Library Manager* di Arduino IDE:

   * `LiquidCrystal_I2C` (oleh Frank de Brabander)

   * `Servo` (Bawaan Arduino IDE)

   * `EEPROM` (Bawaan Arduino IDE)

4. **Rakit Rangkaian & Upload**

   * Hubungkan seluruh komponen sesuai dengan tabel wiring atau gambar pada folder `Images/`.

   * Pilih **Board** dan **Port** yang sesuai di Arduino IDE, lalu klik **Upload**.

## 🤝 Kontribusi

Kontribusi, laporan *bug*, atau saran pengembangan fitur sangat terbuka. Silakan buat *Issue* baru atau lakukan *Pull Request* pada repositori ini.

*Dibuat dengan semangat inovasi oleh **Tim Satu Frekuensi** — **SMKN 9 Surakarta** untuk **KRENOVA 2026 UNISRI**.*
