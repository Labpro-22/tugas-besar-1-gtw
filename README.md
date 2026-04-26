[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/b842RB8g)
# Tugas Besar 1 IF2010 Pemrograman Berorientasi Objek

Program simulasi permainan board game berbasis properti dengan konsep Monopoli. Diimplementasikan menggunakan C++ dengan prinsip Object-Oriented Programming (OOP).

## Prasyarat (Prerequisites)

- **Compiler**: GCC dengan dukungan C++17 atau lebih tinggi (`g++`)
- **Build Tools**: GNU Make
- **OS**: Windows, Linux, atau macOS

Pastikan `g++` dan `make` sudah terinstall di sistem Anda:
```bash
# Linux/Mac
sudo apt-get install build-essential  # Debian/Ubuntu
brew install gcc make                 # macOS

# Windows
# Gunakan MinGW atau gunakan WSL (Windows Subsystem for Linux)
```

## Cara Menjalankan Program

### 1. Build Program
Compile semua source file dan buat executable:
```bash
make
# atau
make all
```

### 2. Jalankan Program
Setelah build berhasil, jalankan program dengan:
```bash
make run
```

### 3. Clean (Hapus Build Files)
Untuk menghapus file object dan executable yang sudah dibuat:
```bash
make clean
```

## Struktur Direktori

```
├── src/                    
│   ├── main.cpp
│   ├── core/              
│   ├── models/            
│   │   ├── Kartu/        
│   │   ├── Petak/        
│   │   └── Properti/     
│   ├── utils/            
│   └── views/            
│
├── include/               
│   ├── core/
│   ├── models/
│   ├── utils/
│   └── views/
│
├── config/               
│   ├── aksi.txt
│   ├── property.txt
│   ├── tax.txt
│   └── ...
│
├── data/                 
├── testing/              
├── makefile              
└── README.md
```

## Perintah Make yang Tersedia

| Perintah | Deskripsi |
|----------|-----------|
| `make` atau `make all` | Build project (compile + link) |
| `make run` | Build dan jalankan program |
| `make clean` | Hapus folder `build/` dan `bin/` |


## Anggota Kelompok

13524042 Suryani Mulia Utami
13524073 Keisha Rizka Syofyani
13524087 Muhammad Fakhry Zaki
13524091 Vara Azzara Ramli Pulukadang
13524109 Helena Kristela Sarhawa