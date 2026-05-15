# 42 — Embedded Pool

> Piscine embarquée réalisée à 42 — programmation bare-metal d'un microcontrôleur AVR en C, sans OS, sans abstraction.  

---

## Modules

| # | Thème | Ce que j'y ai appris |
|---|-------|----------------------|
| 00 | Makefiles & Toolchain | Chaîne de compilation AVR, flashage avec `avrdude` |
| 01 | Timers | Comptage hardware, prescalers, génération d'intervalles précis |
| 02 | UART | Communication série asynchrone, envoi/réception de données vers le PC |
| 03 | PWM | Génération de signaux modulés, contrôle de luminosité et de moteurs |
| 04 | Interruptions | ISR, gestion d'événements hardware sans polling actif |
| 05 | ADC | Lecture de capteurs analogiques, conversion 10 bits |
| 06 | I2C | Bus deux fils, communication maître/esclave avec périphériques externes |
| 07 | EEPROM | Persistance de données sans alimentation, lecture/écriture registres internes |
| 08 | SPI | Protocole synchrone haute vitesse, communication avec capteurs et mémoires |

---

## Schéma électronique

Le fichier [`elec42_pool.pdf`](./elec42_pool.pdf) contient le schéma de câblage de référence utilisé tout au long de la piscine : brochage de l'ATmega328P, connexions des périphériques (LEDs, boutons, capteurs), et alimentation.

---

## Stack technique

- **MCU :** ATmega328P (Arduino Uno)
- **Langage :** C bas niveau, manipulation directe des registres (`DDRx`, `PORTx`, `PINx`)
- **Toolchain :** `avr-gcc`, `avr-objcopy`, `avrdude`
- **Moniteur série :** `screen` sur `/dev/ttyUSB0`
- **Environnement :** Linux

---

## Mise en route

```bash
ls /dev/ttyUSB*                  # Repérer le port du microcontrôleur
make flash                       # Compiler et flasher
screen /dev/ttyUSB0 115200       # Ouvrir le moniteur série (Ctrl+A puis D pour quitter)
```

---

## Protocoles & mémoire

### UART — Universal Asynchronous Receiver-Transmitter

Communication **asynchrone** point-à-point sur deux fils (TX / RX). Il n'y a pas d'horloge partagée : émetteur et récepteur se synchronisent uniquement via un débit (baud rate) fixé à l'avance, encadré par des bits de start et de stop.  
C'est le protocole utilisé pour dialoguer avec le PC via `screen` — débogage en temps réel, affichage de valeurs capteur, traçage d'exécution.

| Paramètre | Valeur typique | Rôle |
|-----------|---------------|------|
| Baud rate | 9600 – 115200 | Vitesse de transmission en bits/s |
| Data bits | 8 | Taille d'un mot transmis |
| Stop bit | 1 | Marqueur de fin de trame |
| Parité | None / Even / Odd | Détection d'erreur simple |

---

### I2C — Inter-Integrated Circuit

Bus **synchrone** deux fils (SDA / SCL) conçu pour connecter plusieurs périphériques sur la même paire de câbles. Chaque esclave possède une adresse 7 bits unique ; le maître initie toutes les transactions et génère l'horloge.  
Chaque octet transmis doit être acquitté (ACK) par le récepteur — le protocole intègre donc une détection d'erreur native.

| Condition | Signification |
|-----------|---------------|
| START | Le maître prend le contrôle du bus |
| Adresse + R/W | Sélectionne l'esclave et le sens du transfert |
| ACK / NACK | L'esclave confirme la réception ou signale une erreur |
| STOP | Le maître libère le bus |

Fréquence standard : **100 kHz** (mode fast : 400 kHz). Adapté aux capteurs (température, IMU, OLED) qui ne nécessitent pas un débit élevé.

---

### SPI — Serial Peripheral Interface

Protocole **synchrone full-duplex** sur quatre fils (MOSI, MISO, SCK, SS). La transmission est bidirectionnelle simultanée : pendant que le maître envoie un octet, il en reçoit un en retour sur le même coup d'horloge.  
Chaque esclave dispose de sa propre ligne de sélection (SS), ce qui élimine toute ambiguïté d'adressage et permet des fréquences bien supérieures à l'I2C.

| Fil | Direction | Rôle |
|-----|-----------|------|
| MOSI | Maître → Esclave | Données sortantes |
| MISO | Esclave → Maître | Données entrantes |
| SCK | Maître → Esclave | Horloge de synchronisation |
| SS | Maître → Esclave | Activation de l'esclave ciblé |

Typiquement utilisé pour les mémoires flash externes, les afficheurs et les convertisseurs rapides.

---

### EEPROM — Mémoire non volatile interne

L'ATmega328P intègre **1 Ko d'EEPROM** directement accessible par registres, totalement indépendante de la mémoire programme (Flash) et de la RAM. Son contenu survit aux coupures d'alimentation et aux resets.

Cas d'usage : stockage de configurations persistantes, compteurs, valeurs d'étalonnage. À distinguer de la Flash (écriture au flashage uniquement) et de la SRAM (volatile, perdue à l'extinction).

> ⚠️ Durée de vie limitée à ~100 000 cycles d'écriture par cellule — un paramètre critique dans les architectures embarquées longue durée.

---

*Projet réalisé dans le cadre du cursus 42 — tous les exercices sont personnels et non copiés.*
