This is the communication between the tank and controller.
The tank communicates using both Serial and ESPNow.

The communication is very simple in an effort to make it more expandable.
The first byte, ignoring start byte for serial, is the message header and signifies what kind of message you are sending.
The rest of the message can be used however in accordance to the kind of message header.
The only restriction is bytes ```0xFF``` and ```0xFE```. These are reserved for the serial start and end bytes. Even when using ESPNow these bytes may not be used.
The maximum length of a message is 255 bytes, we should not even be getting close to this.

There is a difference between Serial and ESPNow. The difference is that ESPNow is sent and recieved as the entire message, no start of message or end of message bytes are needed. Serial however, is sent only 1 byte at a time. This means that a start byte and end byte is required.

## Serial communication differences
Serial communication requires an extra byte marking the start of the message, and an extra one for the end of message.
The start of the message is signified by a ```0xFF``` or ```255```
The end of the message is signified by a ```0xFE``` or ```254```

## ESPNow communication differences
ESPNow handles the protocol for you, as such a start byte and end byte is not included.

## Messages
Here is a list of all the message types and how to use them

|First byte|Message type|
|---|---|
|0x00|Ping|
|0x01|Left motor speed|
|0x02|Right motor speed|

### Ping - 0x00
The ping message is used to check if the controller is communicating with the tank, or if the tank is communicating with the controller. 

|Byte number|Options|Description|
|---|---|---|
|0|0x00|Signifies the message type is a ping|
|1|0x00 or 0x01|Signifies whether the message is a ping - 0x00, or a response to a ping 0x01|

### Left motor speed - 0x01
This message is used to tell the tank to set the left motor to a specific speed

|Byte number|Options|Description|
|---|---|---|
|0|0x01|Signifies the message type is setting the left motor speed|
|1|0x00 - 0x3F, 0x40 - 0x7F|Speed and direction of the motor. Forward between 0x00 - 0x3F where 0x00 is slowest and 0x3F is fastest. Backward between 0x40 - 0x7F where 0x40 is slowest and 0x7F is fastest|

### Right motor speed - 0x02
This message is used to tell the tank to set the right motor to a specific speed

|Byte number|Options|Description|
|---|---|---|
|0|0x02|Signifies the message type is setting the right motor speed|
|1|0x00 - 0x3F, 0x40 - 0x7F|Speed and direction of the motor. Forward between 0x00 - 0x3F where 0x00 is slowest and 0x3F is fastest. Backward between 0x40 - 0x7F where 0x40 is slowest and 0x7F is fastest|