# Dragomir-Stanciu Diana-Rafaela

# Proiectarea, realizarea și testarea unei drone biomimetice cu aripi batante și control la distanță


# Fluture RC
Fluture RC este componenta software a unei dronei biomimetice inspirate de zborul fluturelui, dezvoltată ca parte a proiectului de licență. Aplicația implementează un sistem de control complet, care rulează pe un microcontroler ESP32 și este accesibil printr-o interfață web disponibilă de pe orice dispozitiv conectat la rețeaua Wi-Fi a dronei. Interfața oferă două moduri de operare — un mod experimental, dedicat testării diferitelor configurații de zbor prin modificarea  parametrilor cinematici ai aripilor în timp real, și un mod automat de tip joystick, bazat pe profile de zbor predefinite.

## Livrabile

Repository-ul conține codul sursă complet al aplicației, fără fișierele binare rezultate în urma compilării. Prin compilare se obține un singur binar firmware care include atât motorul cinematic al aripilor, cât și interfața web servită direct din memoria microcontrolerului.

## Componente hardware utilizate


- Microcontroler: ESP32 DevKitC V4
- Actuatori: 2 × servomotoare DS-S007M
- Alimentare: Baterie LiPo 2S + modul UBEC 5V 

## Structura codului

fluture_rc/
-platformio.ini      # Configurare PlatformIO (board, framework, biblioteci)
-src/
-main.cpp        # Firmware ESP32 — cinematica aripilor + comunicație WebSocket
-index_html.h    # Interfața web (HTML/CSS/JS) stocată în PROGMEM


Bibliotecile externe utilizate (ESP32Servo, WebSocketsServer) sunt declarate în platformio.ini și se descarcă automat la compilare — nu este necesară includerea lor manuală în repository.

## Compilare, instalare și lansare

Proiectul folosește framework-ul **PlatformIO**, integrat în **Visual Studio Code**.

1. Se clonează repository-ul și se deschide folderul în VS Code cu extensia PlatformIO instalată.
2. Se conectează placa ESP32 la calculator prin USB.
3. Din interfața PlatformIO se apasă butonul **Upload** (sau se rulează comanda `pio run --target upload` din terminal). Această acțiune compilează codul și încarcă firmware-ul pe microcontroler.

După încărcare, ESP32 pornește automat un access point Wi-Fi cu numele `Fluture_RC`. Pentru utilizarea aplicației:

1. Conectare de pe telefon, tabletă sau laptop la rețeaua `Fluture_RC`.
2. Deschiderea unui browser și accesarea adresei [http://192.168.4.1/](http://192.168.4.1/).
3. Selectarea modului de operare dorit — *Experimental* sau *Automat* — din meniul situat în partea superioară a paginii.

## Repository

Codul sursă este disponibil la adresa: https://github.com/rafadr15/Fluture_RC


