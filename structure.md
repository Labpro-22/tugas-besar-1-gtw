```
.
├── config
│   ├── aksi.txt
│   ├── misc.txt
│   ├── property.txt
│   ├── railroad.txt
│   ├── special.txt
│   ├── tax.txt
│   └── utility.txt
├── data
├── include
│   ├── core
│   │   ├── BangkrutDanEndGame.hpp
│   │   └── GameEngine.hpp
│   ├── models
│   │   ├── ConfigData.hpp
│   │   ├── Dadu.hpp
│   │   ├── Kartu
│   │   │   ├── DeckFactory.hpp
│   │   │   ├── DeckKartu.hpp
│   │   │   ├── KartuDanaUmum.hpp
│   │   │   ├── Kartu.hpp
│   │   │   ├── KartuKemampuanSpesial.hpp
│   │   │   └── KartuKesempatan.hpp
│   │   ├── ManagerPenjara.hpp
│   │   ├── Papan.hpp
│   │   ├── Pemain.hpp
│   │   ├── Petak
│   │   │   ├── PetakAksi.hpp
│   │   │   ├── PetakFestival.hpp
│   │   │   ├── Petak.hpp
│   │   │   ├── PetakKartu.hpp
│   │   │   ├── PetakProperti.hpp
│   │   │   └── PetakSpesial.hpp
│   │   ├── PlayerActionService.hpp
│   │   └── Properti
│   │       ├── ManagerProperti.hpp
│   │       ├── Properti.hpp
│   │       ├── PropertiRailroad.hpp
│   │       ├── PropertiStreet.hpp
│   │       └── PropertiUtility.hpp
│   ├── utils
│   │   ├── ConfigLoader.hpp
│   │   ├── FileHelper.hpp
│   │   ├── LogTransaksiGame.hpp
│   │   └── NimonspoliException.hpp
│   └── views
│       └── Formatter.hpp
├── makefile
├── README.md
├── src
│   ├── core
│   │   ├── BangkrutDanEndGame.cpp
│   │   └── GameEngine.cpp
│   ├── main.cpp
│   ├── models
│   │   ├── ConfigData.cpp
│   │   ├── Dadu.cpp
│   │   ├── Kartu
│   │   │   ├── Kartu.cpp
│   │   │   ├── KartuDanaUmum.cpp
│   │   │   ├── KartuKemampuanSpesial.cpp
│   │   │   └── KartuKesempatan.cpp
│   │   ├── ManagerPenjara.cpp
│   │   ├── Papan.cpp
│   │   ├── Pemain.cpp
│   │   ├── Petak
│   │   │   ├── PetakAksi.cpp
│   │   │   ├── Petak.cpp
│   │   │   ├── PetakFestival.cpp
│   │   │   ├── PetakKartu.cpp
│   │   │   ├── PetakProperti.cpp
│   │   │   └── PetakSpesial.cpp
│   │   ├── PlayerActionService.cpp
│   │   └── Properti
│   │       ├── ManagerProperti.cpp
│   │       ├── Properti.cpp
│   │       ├── PropertiRailroad.cpp
│   │       ├── PropertiStreet.cpp
│   │       └── PropertiUtility.cpp
│   ├── utils
│   │   ├── ConfigLoader.cpp
│   │   ├── FileHelper.cpp
│   │   └── LogTransaksiGame.cpp
│   └── views
│       └── Formatter.cpp
├── structure.md
└── testing
    ├── test_config_loader.cpp
    ├── test_kartu.cpp
    ├── test_papan.cpp
    └── test_properti.cpp

20 directories, 72 files
```
