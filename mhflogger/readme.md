Task:
Catch packets sent to the display unit oа the "mhf" power meter.

tips:
- try to catch on air with both TX & RX working. Do not auto-ACK. Static pl size.
- check all mandatory settings against read settings

Settings that are changed during startup (constructor + begin):
 - Constructor:
 wide_band(true), \\ speed == RF24_2MBPS \\ OK
 p_variant(false), \\ later checked in _begin_ \\ better to set true
 payload_size(32), \\ OK
 ack_payload_available(false), \\ Whether there is an ack payload waiting \\ no pl yet - OK
 dynamic_payloads_enabled(false), \\ no pl yet - OK
 pipe0_reading_address(0) \\ filled with address on openReadingPipe (MUST call), then taken on startListening. \\ 
 - begin:
 // Set 1500uS (minimum for 32B payload in ESB@250KBPS) // I don't mind
 setPALevel( RF24_PA_MAX ) ; // Restore our default PA level // only needed for transmitters - IDNM
 setDataRate( RF24_1MBPS ) ; // __!!! observe this setting!__ setting 2 MBPS is mandatory to match the datasheet default settings 
 setCRCLength( RF24_CRC_16 ) ; // 2-byte CRC // not as in datasheet, but good
 // Disable dynamic payloads // - ? need to check. first I will use 32-byte static.
 setChannel(76); // __!!! observe this setting!__

So what I need to change immediatelly after begin (based on read RX settings):

/** Setup mhf **/
begin();
setChannel(40);
openReadingPipe(0,0x0110104334);
setDataRate( RF24_2MBPS);
// setCRCLength( RF24_CRC_16 ) ; // already set
// how to close the pipe 1?
// пока не буду (tx_addr)
// setPayloadSize(32); // initially set
// enableAckPayload(); // пока не надо
// enableDynamicPayloads(); // пока не надо
startListening();
/* this seems to be enough. now go and make the .cpp file! */ 

