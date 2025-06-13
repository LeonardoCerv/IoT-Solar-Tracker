/*
 * SOLARUS - Dual-Axis Solar Tracker System
 * 
 * This program controls a dual-axis solar tracking system using an ESP32 microcontroller.
 * The system uses 4 Light Dependent Resistors (LDRs) positioned at different angles to
 * determine the optimal direction for maximum sunlight exposure. Two servo motors adjust
 * the solar panel position: one for horizontal movement and another for tilt adjustment.
 * 
 * Data from the sensors is logged to Firebase for monitoring and analysis.
 * 
 * Hardware Requirements:
 * - ESP32 microcontroller
 * - 4 LDR sensors (positioned at 0°, 45°, 90°, 135°)
 * - 2 servo motors (horizontal and tilt control)
 * - WiFi connection for Firebase data logging
 */

// =============================================================================
// LIBRARY INCLUDES
// =============================================================================
#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseClient.h>
#include <WiFiClientSecure.h>
#include <ESP32Servo.h>
#include <time.h>

// =============================================================================
// NETWORK AND FIREBASE CONFIGURATION
// =============================================================================
// =============================================================================
// WIFI AND FIREBASE CONFIGURATION
// =============================================================================
// IMPORTANT: Replace these placeholder values with your actual credentials
// For security, consider storing sensitive values in a separate config file
// that is not committed to version control

#define WIFI_SSID "YOUR_WIFI_SSID"                                             // Replace with your WiFi network name
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"                                     // Replace with your WiFi password
#define DATABASE_URL "https://your-firebase-project-default-rtdb.firebaseio.com" // Replace with your Firebase Database URL
#define DATABASE_SECRET "YOUR_FIREBASE_API_KEY"                                // Replace with your Firebase database secret key

// =============================================================================
// HARDWARE PIN DEFINITIONS
// =============================================================================
// LDR (Light Dependent Resistor) sensor pins - positioned at different angles
const int LDR1_PIN = 34;           // Orange wire - LDR at 0° position
const int LDR2_PIN = 35;           // Purple wire - LDR at 45° position  
const int LDR3_PIN = 32;           // Blue wire - LDR at 90° position
const int LDR4_PIN = 33;           // Yellow wire - LDR at 135° position

// Servo motor control pins
const int SERVO_HORIZONTAL_PIN = 26;    // Servo for horizontal movement (azimuth)
const int SERVO_TILT_PIN = 27;          // Servo for vertical tilt (elevation)

// =============================================================================
// SERVO AND TRACKING VARIABLES
// =============================================================================
Servo horizontalServo;              // Servo object for horizontal movement
Servo tiltServo;                    // Servo object for tilt adjustment

// Current servo positions
int currentHorizontalPos = 0;       // Current horizontal position (0-180 degrees)
int currentTiltPos = 90;            // Current tilt position (90 degrees = neutral)

// Previous servo positions (used to detect changes and avoid unnecessary movements)
int lastHorizontalPos = -1;        
int lastTiltPos = -1;

// Angular positions corresponding to each LDR sensor
const int LDR_POSITIONS[] = {0, 45, 90, 135};  // Degrees for LDR1, LDR2, LDR3, LDR4

// =============================================================================
// FIREBASE AND NETWORKING OBJECTS
// =============================================================================
// =============================================================================
// FIREBASE AND NETWORKING OBJECTS
// =============================================================================
WiFiClientSecure ssl;                                      // Secure WiFi client for HTTPS connections
DefaultNetwork network;                                     // Network configuration object
AsyncClientClass client(ssl, getNetwork(network));         // Async client for Firebase operations
FirebaseApp app;                                           // Firebase application instance
RealtimeDatabase Database;                                 // Firebase Realtime Database instance
AsyncResult result;                                        // Result object for async operations
LegacyToken dbSecret(DATABASE_SECRET);                     // Authentication token for Firebase

// =============================================================================
// UTILITY FUNCTIONS
// =============================================================================

/**
 * Print error information to Serial monitor
 * @param code Error code
 * @param msg Error message
 */

/**
 * Print error information to Serial monitor
 * @param code Error code
 * @param msg Error message
 */
void printError(int code, const String &msg) {
    Serial.printf("Firebase Error - Code: %d, Message: %s\n", code, msg.c_str());
}

// =============================================================================
// SETUP FUNCTION - SYSTEM INITIALIZATION
// =============================================================================

// =============================================================================
// SETUP FUNCTION - SYSTEM INITIALIZATION
// =============================================================================
void setup() {
    // Initialize serial communication for debugging
    Serial.begin(115200);
    Serial.println("\n=== SOLARUS Solar Tracker System Initializing ===");

    // -----------------------------------------------------------------------------
    // SERVO MOTOR INITIALIZATION
    // -----------------------------------------------------------------------------
    Serial.println("Initializing servo motors...");
    horizontalServo.attach(SERVO_HORIZONTAL_PIN);
    tiltServo.attach(SERVO_TILT_PIN);
    
    // Set servos to initial positions
    horizontalServo.write(currentHorizontalPos);    // Start at 0 degrees (facing direction of LDR1)
    tiltServo.write(currentTiltPos);                // Start at 90 degrees (neutral tilt)
    delay(1000);                                    // Allow servos time to reach initial position
    Serial.println("Servos initialized and positioned.");

    // -----------------------------------------------------------------------------
    // LDR SENSOR PIN CONFIGURATION
    // -----------------------------------------------------------------------------
    Serial.println("Configuring LDR sensor pins...");
    pinMode(LDR1_PIN, INPUT);
    pinMode(LDR2_PIN, INPUT);
    pinMode(LDR3_PIN, INPUT);
    pinMode(LDR4_PIN, INPUT);
    Serial.println("LDR sensors configured.");

    // -----------------------------------------------------------------------------
    // WIFI NETWORK CONNECTION
    // -----------------------------------------------------------------------------
    Serial.println("Connecting to WiFi network...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting");
    
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(300);
    }
    
    Serial.println("\nWiFi connected successfully!");
    Serial.printf("IP Address: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("Signal Strength: %d dBm\n", WiFi.RSSI());

    // -----------------------------------------------------------------------------
    // FIREBASE DATABASE INITIALIZATION
    // -----------------------------------------------------------------------------
    Serial.printf("Initializing Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);
    
    ssl.setInsecure();  // Use insecure connection for simplicity (consider using certificates in production)

    // Initialize Firebase app and database connection
    initializeApp(client, app, getAuth(dbSecret));
    app.getApp<RealtimeDatabase>(Database);
    Database.url(DATABASE_URL);
    client.setAsyncResult(result);
    
    Serial.println("Firebase initialized successfully!");
    Serial.println("=== System Initialization Complete ===\n");
}

// =============================================================================
// MAIN LOOP - SOLAR TRACKING AND DATA LOGGING
// =============================================================================
void loop() {
    // -----------------------------------------------------------------------------
    // SENSOR DATA ACQUISITION
    // -----------------------------------------------------------------------------
    // Read analog values from all 4 LDR sensors (higher values = more light)
    int ldrReadings[4] = {
        analogRead(LDR1_PIN),    // 0° position
        analogRead(LDR2_PIN),    // 45° position
        analogRead(LDR3_PIN),    // 90° position
        analogRead(LDR4_PIN)     // 135° position
    };

    // Display current sensor readings for debugging
    Serial.printf("LDR Readings - [0°]:%d [45°]:%d [90°]:%d [135°]:%d\n", 
                  ldrReadings[0], ldrReadings[1], ldrReadings[2], ldrReadings[3]);

    // -----------------------------------------------------------------------------
    // LIGHT SOURCE DETECTION ALGORITHM
    // -----------------------------------------------------------------------------
    // Find the two LDR sensors with the highest light readings
    // This helps determine the optimal direction for maximum sunlight
    int brightestLDR = 0;           // Index of LDR with highest reading
    int secondBrightestLDR = -1;    // Index of LDR with second highest reading
    
    // Iterate through all LDR sensors to find the two brightest
    for (int i = 1; i < 4; i++) {
        if (ldrReadings[i] > ldrReadings[brightestLDR]) {
            secondBrightestLDR = brightestLDR;  // Previous brightest becomes second
            brightestLDR = i;                   // New brightest found
        } else if (secondBrightestLDR == -1 || ldrReadings[i] > ldrReadings[secondBrightestLDR]) {
            secondBrightestLDR = i;             // New second brightest found
        }
    }

    // -----------------------------------------------------------------------------
    // HORIZONTAL SERVO POSITION CALCULATION
    // -----------------------------------------------------------------------------
    const int LIGHT_DIFFERENCE_THRESHOLD = 50;  // Threshold for considering two readings similar
    
    if (secondBrightestLDR != -1 && 
        abs(ldrReadings[brightestLDR] - ldrReadings[secondBrightestLDR]) < LIGHT_DIFFERENCE_THRESHOLD) {
        // If two LDRs have similar readings, position servo between them
        currentHorizontalPos = (LDR_POSITIONS[brightestLDR] + LDR_POSITIONS[secondBrightestLDR]) / 2;
        Serial.printf("Light source between sensors %d and %d, positioning at %d°\n", 
                      brightestLDR, secondBrightestLDR, currentHorizontalPos);
    } else {
        // Otherwise, point directly at the brightest LDR
        currentHorizontalPos = LDR_POSITIONS[brightestLDR];
        Serial.printf("Light source at sensor %d, positioning at %d°\n", 
                      brightestLDR, currentHorizontalPos);
    }

    // -----------------------------------------------------------------------------
    // TILT SERVO POSITION CALCULATION
    // -----------------------------------------------------------------------------
    // Calculate tilt based on the difference between the two brightest sensors
    // Larger differences suggest the light source is at an angle
    int lightIntensityDifference = abs(ldrReadings[brightestLDR] - 
                                      (secondBrightestLDR != -1 ? ldrReadings[secondBrightestLDR] : 0));
    
    // Determine tilt angle based on light intensity difference
    int tiltAdjustment = 0;
    if (lightIntensityDifference > 100) {
        tiltAdjustment = 30;        // Large difference = significant tilt needed
    } else if (lightIntensityDifference > 50) {
        tiltAdjustment = 15;        // Medium difference = moderate tilt needed
    } else {
        tiltAdjustment = 0;         // Small difference = minimal tilt needed
    }
    
    currentTiltPos = 90 + tiltAdjustment;  // 90° is neutral position
    
    // If second brightest sensor has higher reading than brightest, invert tilt direction
    if (secondBrightestLDR != -1 && ldrReadings[secondBrightestLDR] > ldrReadings[brightestLDR]) {
        currentTiltPos = 90 - tiltAdjustment;
    }

    // -----------------------------------------------------------------------------
    // SERVO MOTOR CONTROL
    // -----------------------------------------------------------------------------
    // Only move servos if position has changed (reduces wear and power consumption)
    if (currentHorizontalPos != lastHorizontalPos) {
        horizontalServo.write(currentHorizontalPos);
        lastHorizontalPos = currentHorizontalPos;
        Serial.printf("→ Horizontal servo moved to %d°\n", currentHorizontalPos);
    }

    if (currentTiltPos != lastTiltPos) {
        tiltServo.write(currentTiltPos);
        lastTiltPos = currentTiltPos;
        Serial.printf("↕ Tilt servo moved to %d°\n", currentTiltPos);
    }

    // -----------------------------------------------------------------------------
    // FIREBASE DATA LOGGING
    // -----------------------------------------------------------------------------
    // Create JSON string with timestamp and sensor data for cloud logging
    time_t currentTime = time(nullptr);
    String sensorDataJSON = "{\"timestamp\":" + String(currentTime) +
                           ",\"ldr1\":" + String(ldrReadings[0]) +
                           ",\"ldr2\":" + String(ldrReadings[1]) +
                           ",\"ldr3\":" + String(ldrReadings[2]) +
                           ",\"ldr4\":" + String(ldrReadings[3]) +
                           ",\"horizontal_pos\":" + String(currentHorizontalPos) +
                           ",\"tilt_pos\":" + String(currentTiltPos) + "}";

    // Push data to Firebase Realtime Database
    Serial.print("📤 Uploading sensor data to Firebase... ");
    String pushKey = Database.push<object_t>(client, "/sensor_readings", object_t(sensorDataJSON.c_str()));
    
    // Check if data upload was successful
    if (client.lastError().code() == 0) {
        Serial.printf("✅ Success! Data key: %s\n", pushKey.c_str());
        Serial.printf("📊 Data: %s\n", sensorDataJSON.c_str());
    } else {
        Serial.println("❌ Failed!");
        printError(client.lastError().code(), client.lastError().message());
    }

    Serial.println("---");  // Separator line for readability
    delay(500);  // Wait 500ms before next reading cycle
}