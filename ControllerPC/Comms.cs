using System.Diagnostics;
using System.Drawing;
using System.IO.Ports;

namespace ControllerPC {

	public class Comms {

		public static SerialPort serialPort;
		public static int baudRate = 9600;
		public static string[] portNames;

		static List<int> serialMessage;

		static Stopwatch lastSendTimer;
		static int cooldownMs = 50;

		public static void Init() {
			portNames = new string[0];
			serialPort = new SerialPort();
			serialMessage = new List<int>();
			lastSendTimer = Stopwatch.StartNew();
		}
		public static void Update() {

			// Refresh portnames
			portNames = SerialPort.GetPortNames();

			if (serialPort.IsOpen == false) { return; }
			while (serialPort.BytesToRead > 0) {
				int currentByte = serialPort.ReadByte();
				
				// Start byte
				if (currentByte == 0xFF) {
					serialMessage = new List<int>();
					continue;
				}

				// End byte
				if (currentByte == 0xFE) {
					HandleCommand(serialMessage.ToArray());
					serialMessage.Clear();
					continue;
				}

				serialMessage.Add(currentByte);


			}

		}

		public static void OpenSerial(string name, int baudrate) {
			serialPort.PortName = name;
			serialPort.BaudRate = baudRate;
			serialPort.Open();
		}
		public static void CloseSerial() {
			serialPort.Close();
		}
		public static void SetCooldown(int cooldownMs) {
			Comms.cooldownMs = cooldownMs;
		}

		private static void HandleCommand(int[] command) {

			string commandString = "[";
			for (int i = 0; i < command.Length; i ++) {
				commandString += command[i]; ;
				if (i != command.Length - 1) {
					commandString += ", ";
				}
			}
			commandString += "]";

			DebugLog.Log("Recieved command: " + commandString, Color.White);

		}
		public static void SendMessage(int[] command) {

			if (serialPort.IsOpen == false) {
				DebugLog.Log("Failed to send message, not connected", Color.Red);
				return;
			}

			byte[] sendData = new byte[command.Length + 2];
			sendData[0] = 0xFF;
			sendData[sendData.Length - 1] = 0xFE;
			for (int i = 0; i < command.Length; i ++) {
				sendData[i + 1] = (byte)command[i];
			}


			serialPort.Write(sendData, 0, sendData.Length);


		}

		public static bool Ping() {
			if (lastSendTimer.ElapsedMilliseconds < cooldownMs) {
				return false;
			}
			lastSendTimer.Restart();
			SendMessage(new int[] { 0x00, 0x00 });
			return true;
		}
		public static bool SendMotor(int leftSpeed, int leftDirection, int rightSpeed, int rightDirection) {

			if (lastSendTimer.ElapsedMilliseconds < cooldownMs) {
				return false;
			}
			lastSendTimer.Restart();

			int leftSpeedAmnt = leftSpeed >> 2;
			if (leftDirection == -1) {
				leftSpeedAmnt += 0x40;
			}
			SendMessage(new int[] { 0x01, leftSpeedAmnt });


			int rightSpeedAmnt = rightSpeed >> 2;
			if (rightDirection == -1) {
				rightSpeedAmnt += 0x40;
			}
			SendMessage(new int[] { 0x02, rightSpeedAmnt });

			return true;


		}
		public static bool SendLeftMotor(int speed, int direction) {

			if (lastSendTimer.ElapsedMilliseconds < cooldownMs) {
				return false;
			}
			lastSendTimer.Restart();

			int speedAmnt = speed >> 2;
			if (direction == -1) {
				speedAmnt += 0x40;
			}
			SendMessage(new int[] { 0x01, speedAmnt });

			return true;

		}
		public static bool SendRightMotor(int speed, int direction) {

			if (lastSendTimer.ElapsedMilliseconds < cooldownMs) {
				return false;
			}
			lastSendTimer.Restart();

			int speedAmnt = speed >> 2;
			if (direction == -1) {
				speedAmnt += 0x40;
			}
			SendMessage(new int[] { 0x02, speedAmnt });

			return true;

		}


	}
}
