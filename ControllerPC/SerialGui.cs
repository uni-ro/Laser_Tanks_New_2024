using ClickableTransparentOverlay;
using ImGuiNET;

namespace ControllerPC {
	public static class SerialGui {

		static bool connectAutomatically = true;
		static int currentPortIndex = 0;
		static int cooldownMs = 50;
		

		public static void Init() {
			
		}

		public static void Render() {

			Comms.Update();
			currentPortIndex = Math.Min(Comms.portNames.Length - 1, currentPortIndex);

			ImGui.Begin("Serial");


			ImGui.SeparatorText("Serial Port"); // Serial Port header
			ImGui.Text("Status: " + (Comms.serialPort.IsOpen ? "Open" : "Closed"));


			currentPortIndex = Math.Max(currentPortIndex, 0);
			currentPortIndex = Math.Min(currentPortIndex, Math.Max(0, Comms.portNames.Length - 1));

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

			if (ImGui.InputInt("Cooldown MS", ref cooldownMs) == true) {
				Comms.SetCooldown(cooldownMs);
			}


			ImGui.End();

			
		}

	}
}
