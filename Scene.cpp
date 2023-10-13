
#include "Scene.h"
#include "Object.h"

Scene::Scene(std::vector<Object*> objects){

	this->objects = objects;
}

void Scene::Update(int deltaTimeMS) {

	for (int i_o = 0; i_o < objects.size(); i_o++)
	{
		std::vector<Object*> nearby;

		for (int ii_o = 0; ii_o < objects.size(); ii_o++)
		{
			Vector2 i_ii_offset = objects[i_o]->GetPos();
			i_ii_offset -= objects[ii_o]->GetPos();

			if (i_ii_offset.SqrMagnitude() < 2.0f * 2.0f) {
				nearby.push_back(objects[ii_o]);
			}
		}

		objects[i_o]->Update(nearby, (float)deltaTimeMS / 1000.0f);
	}
}

float Scene::Size() {

	return objects.size();
}

Object * Scene::GetObj(int index) {

	return objects[index];
}

bool Scene::IsEnd() {

	for (int i_o = 0; i_o < objects.size(); i_o++)
	{
		auto i_obj = objects[i_o];

		if (i_obj->IsDone() == false) {
			return false;
		}
	}

	return true;
}

