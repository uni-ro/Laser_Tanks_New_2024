using ImGuiNET;

namespace ControllerPC {
	internal static class DebugLog {

		static Logger logger;
		static bool debugOutgoing = false;

		public static void Init() {

			logger = new Logger();

			Comms.SetCallback(OutgoingData);

		}

		public static void Log(string message, System.Drawing.Color color) {

			logger.Log(message, color);

		}

		public static void OutgoingData(byte[] data) {

			if (debugOutgoing == false) { return; }

			string message = "Outgoing [";
			for (int i = 0; i < data.Length; i ++) {
				message += Logger.FormatHex(data[i], 2);
				if (i != data.Length - 1) {
					message += ", ";
				}
			}
			message += "]";

			Log(message, System.Drawing.Color.Blue);

		}



		public static void Render() {

			ImGui.Begin("Log");

			ImGui.Checkbox("Debug outgoing", ref debugOutgoing);

			logger.Render();
			ImGui.End();

		}

	}
}
