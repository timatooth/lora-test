// rf95_server.pde
// -*- mode: C++ -*-

#include <SPI.h>
#include <RH_RF95.h>

// Singleton instance of the radio driver
RH_RF95 rf95;
//rxLed
int led = 8;

void setup() 
{
  pinMode(led, OUTPUT);     
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
  if (!rf95.init()){
    Serial.println("init failed");
  }
  rf95.setFrequency(916.000);
  //rf95.setModemConfig(RH_RF95::Bw125Cr48Sf4096); // longest and slowest but might be shit. 1s transmit time
  // 23 seems to be shit, clipping or not TCXO perhaps.
  rf95.setTxPower(20, true);
}

void loop()
{
  if (rf95.available()) {
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len)) {
      digitalWrite(led, HIGH);
      Serial.print("got request: ");
      Serial.println((char*)buf);

      // Send a reply
      uint8_t data[] = "Sup";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      Serial.println("Sent a reply");
      digitalWrite(led, LOW);
    }
    else {
      Serial.println("recv failed");
    }
  }
}


