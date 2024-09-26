using ImGuiNET;
using System.Drawing;

namespace ControllerPC {
	internal static class DebugLog {

		internal static List<(string, Color)> messages;
		private static bool scrollToBottom;
		private static bool pauseOutput;
		internal static bool monitorSerial;
		internal static bool hexView;

		static DebugLog() {
			messages = new List<(string, Color)>();
			monitorSerial = true;
		}

		internal static void Log(string msg, Color color) {
			messages.Add((msg, color));
			scrollToBottom = true;
		}


		internal static void Render() {

			ImGui.Begin("Log");

			if (ImGui.Button("Clear") == true) {
				messages.Clear();
			}
			ImGui.SameLine();
			ImGui.Checkbox("Pause output", ref pauseOutput);
			ImGui.SameLine();
			ImGui.Checkbox("Monitor serial", ref monitorSerial);
			ImGui.SameLine();
			ImGui.Checkbox("Hex view",ref hexView);

			ImGui.PushStyleColor(ImGuiCol.FrameBg, new System.Numerics.Vector4(0.16f, 0.16f, 0.16f, 0.54f));
			ImGui.PushStyleVar(ImGuiStyleVar.FrameBorderSize, 1f);
			ImGui.BeginChildFrame((uint)"DebugLog messages child".GetHashCode(), new System.Numerics.Vector2(-1f, 200f));
			ImGui.PopStyleColor();
			ImGui.PopStyleVar();


			if (scrollToBottom == true) {
				scrollToBottom = false;
				ImGui.SetScrollHereY(-ImGui.GetScrollMaxY());
			}

			for (int i = 0; i < messages.Count; i++) {

				ImGui.PushStyleColor(ImGuiCol.Text, new System.Numerics.Vector4(messages[i].Item2.R, messages[i].Item2.G, messages[i].Item2.B, 1f));
				ImGui.TextWrapped(messages[i].Item1);
				ImGui.PopStyleColor();



			}

			ImGui.EndChild();


			ImGui.End();

		}

		public static string FormatHex(int hexValue, int numDigits = 6) {

			string hexString = hexValue.ToString("X");
			while (hexString.Length < numDigits) {
				hexString = "0" + hexString;
			}
			hexString = "0x" + hexString;
			return hexString;

		}

		public static Color? ParseColor(string colorString) {

			switch (colorString.ToLower()) {

				case "red":
				return Color.FromArgb(255, 0, 0);

				case "orange":
				return Color.FromArgb(255, 127, 0);

				case "yellow":
				return Color.FromArgb(255, 255, 0);

				case "lime":
				return Color.FromArgb(127, 255, 0);

				case "green":
				return Color.FromArgb(0, 255, 0);

				case "teal":
				return Color.FromArgb(0, 255, 127);

				case "cyan":
				return Color.FromArgb(0, 255, 255);

				case "aqua":
				return Color.FromArgb(0, 127, 255);

				case "blue":
				return Color.FromArgb(0, 0, 255);

				case "purple":
				return Color.FromArgb(127, 0, 255);

				case "magenta":
				return Color.FromArgb(255, 0, 255);

				case "pink":
				return Color.FromArgb(255, 0, 127);

				case "white":
				return Color.FromArgb(255, 255, 255);

				case "black":
				return Color.FromArgb(0, 0, 0);

			}

			if (colorString.Length != 6) {
				return null;
			}

			int red = 0;
			int green = 0;
			int blue = 0;

			red += HexToDec(colorString[0]) * 16;
			red += HexToDec(colorString[1]);
			green += HexToDec(colorString[2]) * 16;
			green += HexToDec(colorString[3]);
			blue += HexToDec(colorString[4]) * 16;
			blue += HexToDec(colorString[5]);

			if (red >= 256 || green >= 256 || blue >= 256) {
				return null;
			}

			return Color.FromArgb(255, red, green, blue);

		}

		private static int HexToDec(char hex) {

			switch (hex) {
				case '0': return 0;
				case '1': return 1;
				case '2': return 2;
				case '3': return 3;
				case '4': return 4;
				case '5': return 5;
				case '6': return 6;
				case '7': return 7;
				case '8': return 8;
				case '9': return 9;
				case 'A': return 10;
				case 'B': return 11;
				case 'C': return 12;
				case 'D': return 13;
				case 'E': return 14;
				case 'F': return 15;
				case 'a': return 10;
				case 'b': return 11;
				case 'c': return 12;
				case 'd': return 13;
				case 'e': return 14;
				case 'f': return 15;
				default: return 256;
			}

		}



	}
}

