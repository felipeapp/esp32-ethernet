#include <ETH.h>
#include <HTTPClient.h>

HTTPClient http;
bool eth_connected = false;

void WiFiEvent(WiFiEvent_t event) {
    switch (event) {
    case ARDUINO_EVENT_ETH_START:
        Serial.println("ETH Started");
        ETH.setHostname("esp32-leozim");
        break;

    case ARDUINO_EVENT_ETH_CONNECTED:
        Serial.println("ETH Connected");
        break;

    case ARDUINO_EVENT_ETH_GOT_IP:
        Serial.print("ETH MAC: ");
        Serial.print(ETH.macAddress());

        Serial.print(", IPv4: ");
        Serial.print(ETH.localIP());

        if (ETH.fullDuplex())
            Serial.print(", FULL_DUPLEX");

        Serial.print(", ");
        Serial.print(ETH.linkSpeed());
        Serial.print("Mbps");

        Serial.print(", Hostname: ");
        Serial.println(ETH.getHostname());

        eth_connected = true;
        break;

    case ARDUINO_EVENT_ETH_DISCONNECTED:
        Serial.println("ETH Disconnected");
        eth_connected = false;
        break;

    case ARDUINO_EVENT_ETH_STOP:
        Serial.println("ETH Stopped");
        eth_connected = false;
        break;

    default:
        Serial.println("Unknown event");
        break;
    }
}

void testClient() {
    http.begin("https://10.230.0.46/api/v2/ping");
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
        Serial.println();
        Serial.println(http.getString());
    } else {
        Serial.println();
        Serial.println("[HTTP] GET... failed, error: ");
        Serial.println(http.errorToString(httpCode));
    }

    http.end();
}

void setup() {
    Serial.begin(115200);
    WiFi.onEvent(WiFiEvent);

    delay(10000);

    Serial.println("Calling ETH.begin!");
    ETH.begin(1, 16, 23, 18, ETH_PHY_LAN8720, ETH_CLOCK_GPIO0_IN);
}

void loop() {
    delay(5000);

    if (eth_connected)
        testClient();
    else
        Serial.println("Ethernet not connected!");
}
