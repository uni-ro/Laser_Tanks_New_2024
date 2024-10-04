using System.Diagnostics;
using System.Drawing;
using System.IO.Ports;

namespace ControllerPC {

	public class Comms {

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



		public static int refreshCounter = 0;
		static string serialString = "";

		public static SerialPort serialPort;
		public static int baudRate = 9600;

		public static string[] portNames;

		static int startByteProg = 0;
		static int waitForMessage = -1;

		static int cooldownMs = 50;
		static Stopwatch cooldownTime;

		static Action<byte[]> outgoingCallback;

		public static void Init() {
			portNames = new string[0];
			serialPort = new SerialPort();
			cooldownTime = new Stopwatch();
			cooldownTime.Start();

			outgoingCallback = (byte[] data) => { };

		}

		public static void SetCallback(Action<byte[]> outgoingCallback) {
			Comms.outgoingCallback = outgoingCallback;
		}

		public static void SetCooldown(int cooldownMs) {
			Comms.cooldownMs = cooldownMs;
		}

		public static void Update() {

			// Refresh portnames
			portNames = SerialPort.GetPortNames();

			if (ReadReadySerial() == true) {
				CommandRecieved(waitForMessage);
			}



		}

		static void CommandRecieved(int numBytes) {

			int numCommands = (numBytes - 1) / 2;
			int checksum = 0xFF + 0x7F + numCommands;

			List<(int, int)> dataList = new List<(int, int)>();
			for (int i = 0; i < numCommands; i++) {

				(int, int) newEntry = (ReadSerial(), ReadSerial());
				dataList.Add(newEntry);
				checksum += newEntry.Item1 + newEntry.Item2;

			}
			checksum %= 0x100;


			int serialChecksum = ReadSerial();

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

		public static void HandleCommand(int command, int parameter) {

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
				SendMessage(0x01, 0x00);

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
			if (command == 0x02) {
				// Battery voltage
				float max = 230f;
				float min = 175f;
				ControlGui.batteryRemaining = (parameter - min) / (max - min);

			}

		}


		public static void OpenSerial(string portName, int baudRate) {

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
		public static void CloseSerial() {

			try {
				serialPort.Close();
				DebugLog.Log("Closed serial port", Color.Green);
			}
			catch (Exception e) {
				DebugLog.Log("Failed to close serial port: " + e.Message, Color.Red);
			}

		}
		public static string[] GetPortNames() {
			return portNames;
		}


		public static bool SendMessage(byte command, byte parameter) {
			return SendMessage(new byte[] { command }, new byte[] { parameter });
		}
		public static bool SendMessage(byte[] commands, byte[] parameters) {

			byte[] data = PackData(commands, parameters);
			return SendMessage(data);

		}
		public static bool SendMessage(byte[] data) {

			if (cooldownTime.ElapsedMilliseconds <= cooldownMs) {
				return false;
			}

			try {
				serialPort.Write(data, 0, data.Length);

				outgoingCallback(data);

				cooldownTime.Restart();
				return true;
			}
			catch (Exception e) {
				DebugLog.Log("Failed to send data: " + e.Message, Color.Red);
			}

			return false;

		}

		public static bool ReadReadySerial() {

			if (serialPort == null || serialPort.IsOpen == false) { return false; }

			while (waitForMessage == -1 && serialPort.BytesToRead != 0) {

				int currentByte = ReadSerial();


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
				return true;
			}
			return false;

		}

		private static int ReadSerial() {
			int serialByte = serialPort.ReadByte();
			SerialMonitor.DataRecieved(serialByte);
			return serialByte;
		}

		public static bool Ping() {
			DebugLog.Log("Ping", Color.White);
			return SendMessage(0x00, 0x00);
		}

		public static bool SendMotor(byte leftSpeed, byte leftDirection, byte rightSpeed, byte rightDirection) {

			byte[] command = new byte[] { 0x02, 0x03, 0x04, 0x05 };
			byte[] parameter = new byte[] { leftSpeed, leftDirection, rightSpeed, rightDirection };
			return SendMessage(command, parameter);

		}

		public static bool SendLeftMotor(byte speed, byte direction) {
			return SendMessage(new byte[] { 0x02, 0x03 }, new byte[] { speed, direction });
		}
		public static bool SendLeftMotorSpeed(byte speed) {
			return SendMessage(0x02, speed);
		}
		public static bool SendLeftMotorDirection(byte direction) {
			return SendMessage(0x03, direction);
		}

		public static bool SendRightMotor(byte speed, byte direction) {
			return SendMessage(new byte[] { 0x04, 0x05 }, new byte[] { speed, direction });
		}
		public static bool SendRightMotorSpeed(byte speed) {
			return SendMessage(0x04, speed);
		}
		public static bool SendRightMotorDirection(byte direction) {
			return SendMessage(0x05, direction);
		}

		public static void MessageRecieved() {

			// Skip to the 1st start byte
			while (ReadSerial() != 0xFF && serialPort.BytesToRead != 0) ;
			if (serialPort.BytesToRead == 0) { return; }

			// Skip to the 2nd start byte
			while (ReadSerial() != 0x7F && serialPort.BytesToRead != 0) ;
			if (serialPort.BytesToRead == 0) { return; }

		}

		public static byte[] PackData(byte[] commands, byte[] parameters) {

			byte[] totalMessage = new byte[commands.Length * 2 + 4];

			totalMessage[0] = 0xFF;
			totalMessage[1] = 0x7F;

			totalMessage[2] = (byte)commands.Length;

			for (int i = 0; i < commands.Length; i++) {
				totalMessage[3 + 2 * i] = commands[i];
				totalMessage[4 + 2 * i] = parameters[i];
			}

			totalMessage[totalMessage.Length - 1] = 0;
			for (int i = 0; i < totalMessage.Length - 1; i++) {
				totalMessage[totalMessage.Length - 1] += totalMessage[i];
			}

			return totalMessage;

		}

	}
}
