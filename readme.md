# Arduino LoRa mussings

Add `RadioHead` library zip to arduino IDE

Arduino board quirks:

One board `#C1038` is actually running the Uno 328p bootloader not the Duemilanove. Newer one is the older Duemilanove with 328p bootloader.

Max TX power (23dBm) seems to perform like shit. Better luck with 13-20dBm.

Different spread factors defined in RF_RF95H.h for setting different `ModemConfigs`
Bw125Cr48Sf4096 is very slow and long 1s duty cycle time on the channel.

NZ-AU use 915-927ish ISM band.