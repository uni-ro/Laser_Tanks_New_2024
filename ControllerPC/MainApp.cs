using ClickableTransparentOverlay;
using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ControllerPC {
	public static class MainApp {

		static void Main(string[] args) {

			new RootOverlay();

			Comms.Init();
			SerialGui.Init();
			ControlGui.Init();

			SerialMonitor.Init();
			DebugLog.Init();


		}

		public static void Render() {

			Comms.Update();

			SerialGui.Render();
			ControlGui.Render();
			SerialMonitor.Render();
			DebugLog.Render();


		}

	}

	public class RootOverlay : Overlay {

		public RootOverlay() {
			Start();
		}

		protected override void Render() {
			if (ImGui.IsKeyPressed(ImGuiKey.Escape) == true) {
				Close();
			}
			MainApp.Render();
		}

	}

}
