#include "ConsoleIO.h"

#include <fstream>
#include <iostream>
#include <string>
#include "Vector2.h"
#include "Scene.h"
#include "Human.h"
#include "Player.h"
#include "OutputFormatting.h"

using namespace std;
using namespace OutputFormatting;

static Vector2 ScreenSize = *new Vector2(50, 25);
const int linePadding = 4;
const float ObjectRadius = 0.7f;

void ResetCursor(bool clear) {
	printf("\r");

	for (int i = 0; i < ScreenSize.y() + linePadding; i++) {
		printf("\033[A");
	}

	if (clear) {
		for (int i = 0; i < ScreenSize.y() + linePadding; i++) {
			for (int j = 0; j < ScreenSize.x() + linePadding; j++)
				printf(" ");

			printf("\n");
		}
		for (int i = 0; i < ScreenSize.y() + linePadding; i++) {
			printf("\033[A");
		}
	}
}
void DisplaySceneWSize(Scene scene, Vector2 center, float screenScale) {

	cout << "\033[?25l";
	ResetCursor(false);

	Vector2 halfScreen = ScreenSize;
	halfScreen *= 0.5f * screenScale;

	Vector2 displayBotLeft = center;
	displayBotLeft -= halfScreen;

	for (int i_y = ScreenSize.y() - 1; 0 <= i_y; i_y--)
	{
		for (int i_x = 0; i_x < ScreenSize.x(); i_x++)
		{
			Vector2* xy_pos = new Vector2(i_x, i_y);
			*xy_pos *= screenScale;
			*xy_pos += displayBotLeft;

			string xy_char = " ";

			for (int i_o = 0; i_o < scene.Size(); i_o++)
			{
				auto i_obj = scene.GetObj(i_o);

				auto nodeSqrDist = Vector2::SqrDistance(i_obj->GetPos(), *xy_pos);

				if (nodeSqrDist <= ObjectRadius * ObjectRadius) {

					if (nodeSqrDist <= powf(ObjectRadius * 0.3f, 2)) {
						xy_char = "X";
					}
					else if (nodeSqrDist <= powf(ObjectRadius * 0.8f, 2)) {
						xy_char = "*";
					}
					else {
						xy_char = ".";
					}


					if (dynamic_cast<Player*>(i_obj) != NULL) {
						xy_char = ToBold(ToBrightGreen(xy_char));
					}
					else {
						auto human = dynamic_cast<Human*>(i_obj);

						if (human != NULL) {
							if (human->getLeader() == NULL) {

							}
							else {
								xy_char = ToGreen(xy_char);
							}
						}
					}
					break;
				}
			}

			delete xy_pos;

			cout << xy_char;
		}

		cout << endl;
	}
}

void ConsoleIO::DisplayStart()
{
	ResetCursor(true);

	cout << "WASD + [enter] to change directions." << endl;
	cout << "Chase down and infect all humans!" << endl;
	cout << "Enter any character to start..." << endl;
	char buffer;
	cin >> buffer;
}

void ConsoleIO::DisplayLargeLevel(Scene scene, Vector2 center)
{
	DisplaySceneWSize(scene, center, 1.0f);
}

void ConsoleIO::DisplayLevelEnd()
{
	ResetCursor(true);

	cout << "Enter any character to continue to the next level..." << endl;
}

void ConsoleIO::DisplayScene(Scene scene, Vector2 center) {
	DisplaySceneWSize(scene, center, 0.4f);
}

Vector2* ConsoleIO::GetMovementInput()
{
	Vector2* adjustmentVec = new Vector2(0, 0);
	char movementKey = ' ';
	cin >> movementKey;

	switch (movementKey)
	{
	case 'w':
		adjustmentVec = new Vector2(0, 1);
		break;
	case 'a':
		adjustmentVec = new Vector2(-1, 0);
		break;
	case 's':
		adjustmentVec = new Vector2(0, -1);
		break;
	case 'd':
		adjustmentVec = new Vector2(1, 0);
		break;
	default:
		break;
	}

    return adjustmentVec;
}
