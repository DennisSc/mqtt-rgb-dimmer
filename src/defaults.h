//------------------------------------------------------------------------------
// SET BY PLATFORMIO
//------------------------------------------------------------------------------

//#define DEBUG_PORT              Serial


// -----------------------------------------------------------------------------
// HARDWARE
// -----------------------------------------------------------------------------

#define SERIAL_BAUDRATE         115200
#define BUTTON_PIN              0
#define RELAY_PIN               12
#define HOSTNAME                DEVICE
#define BUFFER_SIZE             1024
#define HEARTBEAT_INTERVAL      300000
#define FS_VERSION_FILE         "/fsversion"


// -----------------------------------------------------------------------------
// WIFI
// -----------------------------------------------------------------------------

#define WIFI_RECONNECT_INTERVAL 300000
#define WIFI_MAX_NETWORKS       3
#define AP_PASS                 "fibonacci"
#define OTA_PASS                "fibonacci"
#define OTA_PORT                8266
#define NOFUSS_SERVER           "http://192.168.1.100"
#define NOFUSS_INTERVAL         3600000

// -----------------------------------------------------------------------------
// MQTT
// -----------------------------------------------------------------------------

#define MQTT_SERVER             ""
#define MQTT_PORT               1883
#define MQTT_TOPIC              "/test/switch/{identifier}"
#define MQTT_RETAIN             true
#define MQTT_RECONNECT_DELAY    10000
#define MQTT_STATUS_TOPIC       ""
#define MQTT_IP_TOPIC           "/ip"
#define MQTT_VERSION_TOPIC      "/version"
#define MQTT_FSVERSION_TOPIC    "/fsversion"
#define MQTT_HEARTBEAT_TOPIC    "/heartbeat"

//--------------------------------------------------------------------------------
// DRIVERS
//--------------------------------------------------------------------------------




// ws2812 RGB dimmer consts and vars
#define LED_PIN 2

#define FADESTEPS 256
#define FADECLOCK 20

#define NUM_LEDS 60
#define CHIPSET     WS2812
#define COLOR_ORDER GRB

bool showRainbowCycle;
bool doSlowCycle;

int FadeStepCounter;

int R_old;
int G_old;
int B_old;

int R_now;
int G_now;
int B_now;

int R_target;
int G_target;
int B_target;

float R_Add;
float G_Add;
float B_Add;