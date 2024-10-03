using ImGuiNET;

namespace ControllerPC {
	internal static class SerialMonitor {

		static Logger logger;

		public static void Init() {

			logger = new Logger();

		}

		public static void DataRecieved(int data) {

			if (data == '\n') {
				logger.Log("", System.Drawing.Color.White);
			}
			else {
				logger.AddToLastMsg((char)data + "");
			}

		}

		public static void Render() {

			ImGui.Begin("Serial Monitor");

			logger.Render();

			ImGui.End();

		}

	}
}
