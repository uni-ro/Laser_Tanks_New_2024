using ClickableTransparentOverlay;
using ImGuiNET;

namespace ControllerPC {
	internal class GUI : Overlay {

		bool connectAutomatically = true;
		int currentPortIndex = 0;

		bool leftMotorOn = false;
		bool rightMotorOn = false;

		struct SendParameters {
			public int testCommand;
			public int testParameter;
		}
		SendParameters sp;
		int testPingVal = 0;

		public GUI() {
			Start();
			sp = new SendParameters();
			sp.testCommand = 0;
			sp.testParameter = 0;
		}

		protected override void Render() {

			Comms.Update();
			currentPortIndex = Math.Min(Comms.portNames.Length - 1, currentPortIndex);

			ImGui.Begin("Serial");

			#region Port connecting

			ImGui.SeparatorText("Serial Port"); // Serial Port header
			ImGui.Text("Status: " + (Comms.serialPort.IsOpen ? "Open" : "Closed"));

			ImGui.BeginDisabled(Comms.serialPort.IsOpen == true); // Disable editing the port and baudrate if there is an open serial connection
			ImGui.Combo("Port", ref currentPortIndex, Comms.portNames, Comms.portNames.Length); // Allow the user to edit the port
			ImGui.InputInt("Baud Rate", ref Comms.baudRate, 0); // Allow the user to edit the baudrate
			ImGui.EndDisabled();

			if (Comms.serialPort.IsOpen == false) {
				ImGui.BeginDisabled(Comms.portNames.Length == 0); // Disable the connect button if there are no available serial ports to open
				if (ImGui.Button("Connect", new System.Numerics.Vector2(-200f, 0f)) == true || (connectAutomatically == true && Comms.portNames.Length != 0)) {
					// Check if the user has clicked the "Connect" button
					Comms.OpenSerial(Comms.portNames[currentPortIndex], Comms.baudRate);
				}
				ImGui.EndDisabled();
			}
			else {
				if (ImGui.Button("Disconnect", new System.Numerics.Vector2(-200f, 0f)) == true) {
					// Check if the user has clicked the "Disconnect" button
					Comms.CloseSerial();
					connectAutomatically = false;
				}
			}

			ImGui.SameLine();
			ImGui.Checkbox("Connect Automatically", ref connectAutomatically);



			#endregion

			ImGui.End();

			ImGui.Begin("Controls");

			ImGui.BeginDisabled(Comms.serialPort.IsOpen == false);

			ImGui.Text("Debug CMD PAR pair");
			ImGui.InputInt2("(CMD, PAR)", ref sp.testCommand);
			ImGui.SameLine();
			if (ImGui.Button("Send") == true) {
				Comms.SendMessage(new (byte, byte)[] { ((byte)sp.testCommand, (byte)sp.testParameter) });
			}

			ImGui.NewLine();
			ImGui.Text("Builtin commands");
			if (ImGui.Button("Ping") == true) {
				Comms.Ping();
			}

			if (ImGui.Checkbox("Left motor power", ref leftMotorOn) == true) {
				Comms.SetLeftMotorSpeed(leftMotorOn == true ? 255 : 0);
			}
			if (ImGui.Checkbox("Right motor power", ref rightMotorOn) == true) {
				Comms.SetRightMotorSpeed(rightMotorOn == true ? 255 : 0);
			}

			ImGui.EndDisabled();

			ImGui.End();

			DebugLog.Render();

			if (ImGui.IsKeyPressed(ImGuiKey.Escape) == true) {
				Close();
			}
		}

	}
}
