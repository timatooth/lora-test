// rf95_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing client
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf95_server
#include <SPI.h>
#include <RH_RF95.h>

// Singleton instance of the radio driver
RH_RF95 rf95;

int rxLed = 8;

void setup()
{
  // Rocket Scream Mini Ultra Pro with the RFM95W only:
  // Ensure serial flash is not interfering with radio communication on SPI bus
  //  pinMode(4, OUTPUT);
  //  digitalWrite(4, HIGH);
  pinMode(rxLed, OUTPUT);

  Serial.begin(9600);
  while (!Serial) ; // BusyWait for serial port to be available
  if (!rf95.init()) {
    Serial.println("init failed");
  }
  //13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  //NZ, AU, US prefer 915MHz
  rf95.setFrequency(916.000);

  //use slower longer transmission config

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  // narrow long range: Bw31_25Cr48Sf512
  // wider long range: Bw125Cr48Sf4096
  // fast and wide: Bw500Cr45Sf128
  // 23 seems to be shit, clipping or not TCXO perhaps.
  rf95.setTxPower(20, false);
  //rf95.setModemConfig(RH_RF95::Bw125Cr48Sf4096); //long range and shittly slow 1s transmit time
}

void loop()
{
  Serial.println("Sending to rf95_server");
  // Send a message to rf95_server
  uint8_t data[] = "hi";
  rf95.send(data, sizeof(data));

  rf95.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  digitalWrite(rxLed, LOW);
  if (rf95.waitAvailableTimeout(1100 + random(100, 500)))
  {
    // Should be a reply message for us now

    if (rf95.recv(buf, &len))
    {
      digitalWrite(rxLed, HIGH);
      Serial.print("got reply: ");
      Serial.println((char*)buf);
      //      Serial.print("RSSI: ");
      //      Serial.println(rf95.lastRssi(), DEC);
      delay(500);

    }
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply, is rf95_server running?");
  }
  delay(500);
}


