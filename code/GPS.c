/* -----------------------------------------------------------
This example shows a lot of different features. As configured here
it'll check for a good GPS fix every 5 minutes and publish that data
if there is one. If not, it'll save you data by staying quiet. 
---------------------------------------------------------------*/

// Getting the library
#include "AssetTracker.h"

// Used to keep track of the last time we published data
long lastPublish = 0;

// How many minutes between publishes? 
int delayMinutes = 5; // Aim to publish every 5 minutes
String location = "";

// Creating an AssetTracker named 't' for us to reference
AssetTracker t = AssetTracker();
char latLong[25];

bool publishGPS = true;


// setup() and loop() are both required. setup() runs once when the device starts
// and is used for registering functions and variables and initializing things
void setup() {
    // Sets up all the necessary AssetTracker bits
    t.begin();
    
    // Enable the GPS module. Defaults to off to save power. 
    // Takes 1.5s or so because of delays.
    t.gpsOn();
    
    // Check GPS data coordinates remotely using Particle Console
    Particle.variable("location", location);
}

// loop() runs continuously
void loop() {
    // You'll need to run this every loop to capture the GPS output
    t.updateGPS();

    // if the current time - the last time we published is greater than your set delay...
    if(millis()-lastPublish > delayMinutes*60*1000){
        // Remember when we published
        lastPublish = millis();
        
        // GPS requires a "fix" on the satellites to give good data,
        // so we should only publish data if there's a fix
        if(t.gpsFix()){
            
            location = t.readLatLon();
            location.toCharArray(latLong, 25);
            
            String dataString = String(location);
            if (publishGPS == true){
                Particle.publish("gpsFix",dataString,60,PRIVATE);
                delay(500);
            }

        }
    }
}
