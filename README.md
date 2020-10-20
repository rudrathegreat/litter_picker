# litter_picker

## Overview
In this tutorial we'll show you how to catpure GPS data from a Particle electron every 5 minutes and then publish this data to a Minnovation server.

## Setting up the Particle electron
The following code in the Particle Web IDE is used to test the GPS function of the Partcle electron using the Asset Tracker expansion board.

```

/* -----------------------------------------------------------
This example shows a lot of different features. As configured here
it'll check for a good GPS fix every 10 minutes and publish that data
if there is one. If not, it'll save you data by staying quiet. It also
registers 3 Particle.functions for changing whether it publishes,
reading the battery level, and manually requesting a GPS reading.
---------------------------------------------------------------*/

// Getting the library
#include "AssetTracker.h"



// Used to keep track of the last time we published data
long lastPublish = 0;

// How many minutes between publishes? 10+ recommended for long-time continuous publishing!
int delayMinutes = 1;
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
    

    
    // These three functions are useful for remote diagnostics. Read more below.
    Particle.variable("location", location);
    Particle.function("setPublish", setPublish);
    Particle.function("setDelay", setDelay);
}

// loop() runs continuously
void loop() {
    // You'll need to run this every loop to capture the GPS output
    t.updateGPS();

    // if the current time - the last time we published is greater than your set delay...
    if(millis()-lastPublish > delayMinutes*60*1000){
        // Remember when we published
        lastPublish = millis();
        
        //String pubAccel = String::format("%d,%d,%d",t.readX(),t.readY(),t.readZ());
        //Serial.println(pubAccel);
        //Particle.publish("A", pubAccel, 60, PRIVATE);
        
        // Dumps the full NMEA sentence to serial in case you're curious
        //Serial.println(t.preNMEA());
        
        // GPS requires a "fix" on the satellites to give good data,
        // so we should only publish data if there's a fix
        if(t.gpsFix()){
            
            location = t.readLatLon();
            location.toCharArray(latLong, 25);
            
            //pressCount++;
            String dataString = String(location);
            if (publishGPS == true){
                Particle.publish("gpsFix",dataString,60,PRIVATE);
                delay(500);
            }

        }
    }
}

int setPublish(String command){
    if (command == "on"){
        publishGPS = true;
        return 1;
    }
    if (command == "off"){
        publishGPS = false;
        return 0;
    }
}

int setDelay(String command){
    if (command == "5"){
        delayMinutes = 5;
        return 5;
    }
    if (command == "1"){
        delayMinutes = 1;
        return 1;
    }
}


```
