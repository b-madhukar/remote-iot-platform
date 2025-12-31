# Remote IoT Platform - v1.0

## Project Overview 
This is a **remote-ready IoT data ingestion platform** built with ESP32, MQTT, and Node-RED.  
Current milestone (v1.0) shows basic MQTT-to-Node-RED data flow.

## Architecture
ESP32 → MQTT → Node-RED → Debug


## Folder Structure
```
iot-platform/
├── esp32/
│ └── mqtt_publisher.ino
├── node-red/
│ └── flows-v1.json
├── docs/
└── README.md
```

## How to Run
1. Open Node-RED  
2. Import `flows-v1.json`  
3. Run ESP32 code to publish MQTT data  
4. Observe data in Node-RED debug panel

## Next Steps
- Add JSON parsing (v1.1)  
- Add InfluxDB persistence (v2.0)  
- Add validation rules (v3.0)  
- Build Grafana dashboards (v4.0)
