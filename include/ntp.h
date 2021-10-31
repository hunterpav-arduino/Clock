#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 2 * 60 *60);

void ntp_setup(){
  Serial.println("NTP setup...");
  timeClient.begin();
}

void ntp_update(){
  timeClient.update();
  // Serial.println(timeClient.getFormattedTime());
}