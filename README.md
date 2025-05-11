# 🔨 Smart Knocker - Firebase Remote Control for ESP32

A humorous-yet-useful ESP32-based project designed to remind upstairs neighbors that "you're not alone" – with class.  
This device listens to commands via **Firebase Realtime Database** and activates a servo motor for either single knock or rapid knocking mode.

---

## 🚀 Features
- Realtime remote control via Firebase
- Three modes:
  - `0` - Off
  - `1` - Single Knock
  - `2` - Rapid Knock
- Servo movement controlled via ESP32
- Easily extendable with Telegram/N8N/Firebase UI in the future

---

## 📦 Folder Structure
├── include/
│ └── secrets.h # Your private WiFi and Firebase credentials
├── src/
│ └── main.cpp # Main firmware logic
├── .gitignore
├── platformio.ini
└── README.md
---

## 🔐 Security
Private data is stored in `secrets.h`, which is excluded from Git using `.gitignore`.  
Never upload credentials to public repositories.

---

## 📡 Realtime Command Structure
Firebase path: `/knocker/command`

- `0` → Stop servo
- `1` → Single knock
- `2` → Rapid knock (repeating)

---

## 🔧 Getting Started
1. Clone the repo
2. Create your own `secrets.h`
3. Flash to ESP32 using PlatformIO
4. Send values to Firebase to control remotely!

---

## 🧠 Future Plans
- Telegram integration
- Dashboard on Firebase Hosting
- Sensor-based automatic activation
