using ImGuiNET;

namespace ControllerPC {
	public static class ControlGui {

		static float leftMotorSpeed = 0;
		static float rightMotorSpeed = 0;

		static bool sendLeftMotorSpeed = false;
		static bool sendRightMotorSpeed = false;

		static bool wasdControl = false;
		static float wasdSpeed = 0.5f;

		public static float batteryRemaining = 0f;

		struct SendParameters {
			public int testCommand;
			public int testParameter;
		}
		static SendParameters sp;

		public static void Init() {
			sp = new SendParameters();
			sp.testCommand = 0;
			sp.testParameter = 0;
		}

		public static void Render() {

			ImGui.Begin("Controls");

			ImGui.BeginDisabled(Comms.serialPort.IsOpen == false);

			ImGui.Text("Builtin commands");
			if (ImGui.Button("Ping") == true) {
				Comms.Ping();
			}

			if (ImGui.SliderFloat("Left motor speed", ref leftMotorSpeed, -1f, 1f) == true) {
				sendLeftMotorSpeed = true;
				
			}
			if (ImGui.SliderFloat("Right motor speed", ref rightMotorSpeed, -1f, 1f) == true) {
				sendRightMotorSpeed = true;
			}
			ImGui.Checkbox("WASD Control", ref wasdControl);

			ImGui.SameLine();
			ImGui.SliderFloat("Speed", ref wasdSpeed, 0f, 1f);

			ImGui.EndDisabled();


			ImGui.NewLine();
			ImGui.Text("Telemetry");
			

			
			ImGui.ProgressBar(batteryRemaining, new System.Numerics.Vector2(ImGuiExtras.ItemWidthForTextVal("Battery life") - 5f, 0f));
			ImGui.SameLine();
			ImGui.Text("Battery life");

			ImGui.End();



			if (wasdControl == true) {

				bool changed = false;
				bool anyKey = false;

				if (ImGui.IsKeyDown(ImGuiKey.W) == true) {
					if (leftMotorSpeed != wasdSpeed) { changed = true; }
					if (rightMotorSpeed != wasdSpeed) { changed = true; }
					leftMotorSpeed = wasdSpeed;
					rightMotorSpeed = wasdSpeed;
					anyKey = true;
				}
				if (ImGui.IsKeyDown(ImGuiKey.S) == true) {
					if (leftMotorSpeed != -wasdSpeed) { changed = true; }
					if (rightMotorSpeed != -wasdSpeed) { changed = true; }
					leftMotorSpeed = -wasdSpeed;
					rightMotorSpeed = -wasdSpeed;
					anyKey = true;
				}
				if (ImGui.IsKeyDown(ImGuiKey.D) == true) {
					if (leftMotorSpeed != wasdSpeed) { changed = true; }
					if (rightMotorSpeed != -wasdSpeed) { changed = true; }
					leftMotorSpeed = wasdSpeed;
					rightMotorSpeed = -wasdSpeed;
					anyKey = true;
				}
				if (ImGui.IsKeyDown(ImGuiKey.A) == true) {
					if (leftMotorSpeed != -wasdSpeed) { changed = true; }
					if (rightMotorSpeed != wasdSpeed) { changed = true; }
					leftMotorSpeed = -wasdSpeed;
					rightMotorSpeed = wasdSpeed;
					anyKey = true;
				}

				if (anyKey == false) {
					if (leftMotorSpeed != 0f) { changed = true; }
					if (rightMotorSpeed != 0f) { changed = true; }
					leftMotorSpeed = 0f;
					rightMotorSpeed = 0f;
				}

				if (changed == true) {

					int leftSpeed = (int)Math.Abs(leftMotorSpeed * 255);
					int leftDirection = Math.Sign(leftMotorSpeed);

					int rightSpeed = (int)Math.Abs(rightMotorSpeed * 255);
					int rightDirection = Math.Sign(rightMotorSpeed);

					Comms.SendMotor(leftSpeed, leftDirection, rightSpeed, rightDirection);
				}


				

			}
			if (sendLeftMotorSpeed == true) {

				int speed = (int)Math.Abs(leftMotorSpeed * 255);
				int direction = Math.Sign(leftMotorSpeed);
				sendLeftMotorSpeed &= !Comms.SendLeftMotor(speed, direction);

			}
			if (sendRightMotorSpeed == true) {

				int speed = (int)Math.Abs(rightMotorSpeed * 255);
				int direction = Math.Sign(rightMotorSpeed);
				sendRightMotorSpeed &= !Comms.SendRightMotor(speed, direction);

			}
			

		}

	}
}
