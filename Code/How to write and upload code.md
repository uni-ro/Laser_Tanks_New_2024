This is the guide for how to write code and upload the code to the ESP8266 and ESP32s for Laser Tanks.

## Environment setup
1. Download VSCode
	The programming IDE we are using is Visual Studio Code. You can download it from here: https://code.visualstudio.com/download

2. Install the required extensions.
	The required extensions are
	- PlatformIO
	- C/C++
	The recommended extensions are
	- Serial Monitor

3. Open the the Tank Source folder in VSCode
	Once you have VSCode set up, you can open the Tank Source folder. This folder contains everything for programming and uploading. You will know you have opened the correct folder if your explorer looks like this.
	 ![[Pasted image 20250313134834.png]]

## Minimal example
This is an example of how to get a code file compiled and uploaded to the microcontrollers.

### Create the code file
Under the src folder, create a new .cpp file to write the code in.
![[Pasted image 20250313164116.png]]
The code file needs a few things in it before it can be compiled and uploaded

```cpp title="Minimal example"
#include <Arduino.h>

void setup() {

}

void loop() {

}
```

### Create the platformio.ini configuration.
In order to actually upload to the microcontrollers, you need to create a platformio configuration.
An example platformio configuration looks like this
```ini title="Example platformio"
[env:Configuration_name]
platform = platform_name
board = board_name
framework = framework_name
build_src_filter =
	-<**/*.cpp>
```
- env is the name of your configuration. Put whatever you want there ```[env:Whatever]```
- platform_name refers to the microcontroller you are using. For example ```board = espressif32``` for the ESP32, and ```board = espressif8266``` for the ESP8266.
- board_name refers to what board you are using. If you are unsure, use ```board = nodemcu```
- framework refers to, i dont know just put ```framework = arduino```
- build_src_filter is where you tell platformio which files to compile. Make sure you include the line ```-<**/*.cpp>```. This tells platformio to ignore every code file in the src directory, without this, platformio will try to compile every file at the same time which will lead to errors. Include ```+<path_to_file.cpp>``` for every file you want to include in your configuration. For this example, this will be ```+<example.cpp>```

### Upload to the microcontroller
Now we are ready to upload the code to the microcontroller. First go to the platformio tab on the left.
![[Pasted image 20250313174216.png]]
Then, expand the option with the same name that you put in the ```[env:Whatever]``` and click Upload.
![[Pasted image 20250313174403.png]]

## Troubleshooting
### Board is not detected
If you're computer is not detecting the microcontroller