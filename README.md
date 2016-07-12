Here I will make Raspberry Pi based data logger, reading data from wireless power meter MHF120200p that is "black box" using NRF24+ transmitter.

First I need to find out settings of the transmitter. To make this possible I have welded wires to RF24 transmitter terminals and actually I connect it to two devices simultaneously. 
Therefore, I need to:
1. disable any data writing to the transmitter when initializing the class
2. disable any prohibiting signal from Pi, leaving only power supply
3. code a function that will read out settings from NRF24 registers.

I will put modified class library into separate folder just not to remove all original files and be able to merge if needed.

**See http://tmrh20.github.io/RF24 for all original documentation**
