#pragma once

#include <vector>

class Object;

class Scene {
  private:
    std::vector<Object*> objects;


  public:
    Scene(std::vector<Object*> objects);

    void Update(int deltaTimeMS);

    float Size();

    Object * GetObj(int index);

    bool IsEnd();

};
