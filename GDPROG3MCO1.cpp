// GDPROG3MCO1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>
#include "Vector2.h"
#include "Scene.h"
#include "Human.h"
#include "Player.h"
#include "IOinterface.h"
#include "ConsoleIO.h"

using namespace std;

const int display_deltaTimeMS = 30;
const int deltaTimeMS = 70;

void DisplayLoop(Scene scene, IOinterface* io, Player* player) {
	while (player->GetPos().SqrMagnitude() < 0.1f * 0.1f)
	{
		io->DisplayLargeLevel(scene, player->GetPos());
	}

	while (scene.IsEnd() == false)
	{
		io->DisplayScene(scene, player->GetPos());

		this_thread::sleep_for(chrono::milliseconds(display_deltaTimeMS));
	}
}

void UpdateLoop(Scene scene, IOinterface *io, Player* player) {
	while (scene.IsEnd() == false)
	{
		scene.Update(deltaTimeMS);

		this_thread::sleep_for(chrono::milliseconds(deltaTimeMS));
	}

	io->DisplayLevelEnd();
}

void MovePlayer(Scene scene, IOinterface* io, Player* player) {
	while (scene.IsEnd() == false)
	{
		static Vector2 movementVec;
		movementVec *= 0;
		Vector2* adjustmentVec = io->GetMovementInput();

		movementVec += *adjustmentVec;
		delete adjustmentVec;
		player->SetMoveDir(movementVec);
	}
}

int main()
{
	IOinterface* io = new ConsoleIO();

	int level = 1;

	io->DisplayStart();

	while (true)
	{
		auto player = new Player(*new Vector2(0, 0));

		vector<Object*> sceneobjs = {player};

		for (int i_h = 0; i_h < level; i_h++)
		{
			int spawnRange = 7;
			int i_h_x = (rand() % (spawnRange - (-spawnRange) + 1)) + (-spawnRange);
			int i_h_y = (rand() % (spawnRange - (-spawnRange) + 1)) + (-spawnRange);
			sceneobjs.push_back(new Human(*new Vector2(i_h_x, i_h_y)));
		}

		Scene newScene = *new Scene(sceneobjs);

		bool playing = true;
		thread DisplayThread(DisplayLoop, newScene, io, player);
		thread UpdateThread(UpdateLoop, newScene, io, player);
		thread MoveListenThread(MovePlayer, newScene, io, player);

		DisplayThread.join();
		UpdateThread.join();
		MoveListenThread.join();

		level++;
	}
}