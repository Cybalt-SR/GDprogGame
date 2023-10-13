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
