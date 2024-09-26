
using System.IO.Ports;
using System.Drawing;
using System.Reflection;

namespace ControllerPC {

	internal class Comms {

		// Comms
		// Start byte A - 0xFF
		// Start byte B - 0x7F
		// Num data

		// Info header
		// Data

		// Info header
		// Data

		// ...

		// Info header
		// Data

		// Checksum (sum of all values, looped within 255)



		// Info headers
		// 0 - ping
		// 1 - ping return


		static GUI gui;

		internal static int refreshCounter = 0;
		static string serialString = "";

		internal static SerialPort serialPort;
		internal static int baudRate = 9600;

		internal static string[] portNames;

		static int startByteProg = 0;
		static int waitForMessage = -1;

		static void Main(string[] args) {

			portNames = new string[0];
			serialPort = new SerialPort();

			gui = new GUI();
			
		}

		internal static void Update() {
			portNames = SerialPort.GetPortNames();

			if (DebugLog.monitorSerial == true) {

				while (serialPort.IsOpen == true && serialPort.BytesToRead != 0) {
					char serialChar = (char)serialPort.ReadByte();

					if (DebugLog.messages[DebugLog.messages.Count() - 1].Item2 != Color.Blue) {
						DebugLog.Log("", Color.Blue);
					}

					if (serialChar != '\n') {
						(string, Color) lastLogMessage = DebugLog.messages[DebugLog.messages.Count() - 1];
						if (DebugLog.hexView == false) {
							lastLogMessage.Item1 += serialChar;
						}
						else {
							lastLogMessage.Item1 += ((int)serialChar).ToString("X") + " ";
						}
						
						DebugLog.messages[DebugLog.messages.Count() - 1] = lastLogMessage;
					}
					else {
						DebugLog.Log("", Color.Blue);
					}

					//MessageRecieved();
				}

			}
			
			if (serialPort.IsOpen == true) {
				while (waitForMessage == -1 && serialPort.BytesToRead != 0) {

					int currentByte = serialPort.ReadByte();

					if (startByteProg == 2) {
						waitForMessage = currentByte * 2 + 1;
						startByteProg = 0;
					}
					if (startByteProg == 1) {
						if (currentByte == 0x7F) {
							startByteProg++;
						}
						else {
							startByteProg = 0;
						}
					}
					if (startByteProg == 0) {
						if (currentByte == 0xFF) {
							startByteProg++;
						}
						else {
							startByteProg = 0;
						}
					}

				}
				if (waitForMessage != -1 && serialPort.BytesToRead >= waitForMessage) {
					CommandRecieved(waitForMessage);
				}

			}
			
			
		}


		static void CommandRecieved(int numBytes) {

			int numCommands = (numBytes - 1) / 2;
			int checksum = 0xFF + 0x7F + numCommands;

			List<(int, int)> dataList = new List<(int, int)>();
			for (int i = 0; i < numCommands; i ++) {

				(int, int) newEntry = (serialPort.ReadByte(), serialPort.ReadByte());
				dataList.Add(newEntry);
				checksum += newEntry.Item1 + newEntry.Item2;
				
			}
			checksum %= 0x100;


			int serialChecksum = serialPort.ReadByte();


			if (checksum == serialChecksum) {

				for (int i = 0; i < numCommands; i++) {
					HandleCommand(dataList[i].Item1, dataList[i].Item2);
				}

			}
			else {
				DebugLog.Log("Checksum failed", Color.Red);
				DebugLog.Log("Expected: " + checksum, Color.Red);
				DebugLog.Log("Got: " + serialChecksum, Color.Red);
			}

			waitForMessage = -1;


		}

		internal static	void HandleCommand(int command, int parameter) {

			if (command == 0x00) {
				// Echo

				string senderName = "";
				switch (parameter) {
					case 0:
					senderName = "PC";
					break;

					case 1:
					senderName = "Master ESP";
					break;

					case 2:
					senderName = "Slave ESP";
					break;

				}

				DebugLog.Log("Ping from" + senderName, Color.White);
				SendMessage(new (byte, byte)[] { (0x01, 0x00) });

			}
			if (command == 0x01) {
				// Echo return

				string senderName = "";
				switch (parameter) {
					case 0:
					senderName = "PC";
					break;

					case 1:
					senderName = "Master ESP";
					break;

					case 2:
					senderName = "Slave ESP";
					break;

				}

				DebugLog.Log("Pong from " + senderName, Color.White);
			}

		}


		internal static void OpenSerial(string portName, int baudRate) {
			
			try {
				serialPort = new SerialPort(portName, baudRate);
				serialPort.Open(); 
				DebugLog.Log("Opened serial port " + portName + " @" + baudRate, Color.Green);
			}
			catch (Exception e) {
				DebugLog.Log("Failed to open Serial Port", Color.Red);
				DebugLog.Log(e.Message, Color.Red);
			}

		}
		internal static void CloseSerial() {

			try {
				serialPort.Close();
				DebugLog.Log("Closed serial port", Color.Green);
			}
			catch (Exception e) {
				DebugLog.Log("Failed to close serial port: " + e.Message, Color.Red);
			}

		}
		internal static string[] GetPortNames() {
			return portNames;
		}



		internal static void SendMessage((byte, byte)[] data) {

			byte[] totalMessage = new byte[data.Length * 2 + 4];

			totalMessage[0] = 0xFF;
			totalMessage[1] = 0x7F;

			totalMessage[2] = (byte)data.Length;

			for (int i = 0; i < data.Length; i ++) {
				totalMessage[3 + 2 * i] = data[i].Item1;
				totalMessage[4 + 2 * i] = data[i].Item2;
			}

			totalMessage[totalMessage.Length - 1] = 0;
			for (int i = 0; i < totalMessage.Length - 1; i ++) {
				totalMessage[totalMessage.Length - 1] += totalMessage[i];
			}

			try {

				serialPort.Write(totalMessage, 0, totalMessage.Length);
				
				/*
				string debugMessage = "Sending data [";
				for (int i = 0; i < totalMessage.Length; i++) {
					debugMessage += DebugLog.FormatHex(totalMessage[i], 2);
					if (i != totalMessage.Length - 1) {
						debugMessage += ", ";
					}
				}
				debugMessage += "]";
				DebugLog.Log(debugMessage, Color.White);
				*/
			}
			catch (Exception e) {
				DebugLog.Log("Failed to send data: " + e.Message, Color.Red);
			}
			


			

		}
		internal static void Ping() {
			DebugLog.Log("Ping", Color.White);
			SendMessage(new (byte, byte)[] { (0x00, 0x00) });
		}
		internal static void SetLeftMotorSpeed(int speed) {
			SendMessage(new (byte, byte)[] { (0x02, (byte)speed) });
		}
		internal static void SetRightMotorSpeed(int speed) {
			SendMessage(new (byte, byte)[] { (0x03, (byte)speed) });
		}

		internal static void MessageRecieved() {

			// Skip to the 1st start byte
			while (serialPort.ReadByte() != 0xFF && serialPort.BytesToRead != 0);
			if (serialPort.BytesToRead == 0) { return; }

			// Skip to the 2nd start byte
			while (serialPort.ReadByte() != 0x7F && serialPort.BytesToRead != 0) ;
			if (serialPort.BytesToRead == 0) { return; }

		}

	}
}
