# Sistemi Klient-Server me UDP dhe Monitorim përmes HTTP

## Përshkrimi

Ky projekt është një sistem klient-server i zhvilluar në C++, i cili përdor UDP sockets për komunikim dhe një HTTP server të integruar për monitorim.

Disa klientë mund të lidhen me serverin dhe të kryejnë operacione me file, varësisht nga roli që kanë (Admin ose Normal).

Serveri nuk mban lidhje të përhershme me klientët; çdo mesazh trajtohet në mënyrë të pavarur duke përdorur UDP.

Serveri menaxhon klientët aktivë, përpunon komandat, kontrollon të drejtat e qasjes dhe ofron statistika në kohë reale përmes një ndërfaqeje web.

## Karakteristikat Kryesore

* Komunikim me UDP (pa lidhje të përhershme)
* HTTP server i integruar për monitorim
* Kontroll i qasjes bazuar në role (Admin / Normal)
* Maksimumi 4 klientë aktivë njëkohësisht
* Sistem pritjeje (queue) kur serveri mbushet
* Komanda për menaxhim të file-ave
* Mekanizëm ping për të mbajtur klientët aktivë
* Heqje automatike e klientëve joaktivë (timeout)
* Statistika në kohë reale në format Text dhe JSON
* Delay i vogël për përdoruesit normal (~2 sekonda)

## Teknologjitë e Përdorura

* C++
* Windows Sockets (Winsock)
* Protokolli UDP
* Protokolli HTTP
* Format JSON
* File I/O

## Kërkesat

* Windows 7 / 8 / 10 / 11
* Kompajler g++ ose MSVC
* Biblioteka Winsock (`ws2_32.lib`)


## Kompilimi dhe Ekzekutimi

### Kompilimi i Serverit
```bash
g++ server.cpp admin.cpp httpServer.cpp -o server.exe -lws2_32
```

### Nisja e Serverit
```bash
server.exe
```

### Kompilimi i Klientit
```bash
g++ klient.cpp -o klient.exe -lws2_32
```

### Nisja e Klientit
```bash
klient.exe
```
