#include <WifiClient.h>

// Create an instance of the WifiClient class
WifiClient client;

void setup() {
    // Start the Serial communication
    Serial.begin(115200);

}

void loop() {
    String message = "Hello there, my friend";
    unsigned char table[25];

    for (size_t i = 0; i < 22; i++)
    {
        table[i] = message[i];
    }

    // Send the message
    client.sendMessage(table);

    // Wait for 2 second before sending the next message
    delay(2000);
}
