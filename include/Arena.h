#ifndef ARENA_H_
#define ARENA_H_
#include <ngl/Mat4.h>
#include "Directions.h"
#include "Snake.h"
#include <memory>
class Arena
{
  public :
    Arena(int _width, int _depth);
    void draw() const;
    void update(float _delta);
    void setDirection(Direction _dir);
    bool gameOver() const;
  private :

    void checkArenaCollision();

    int m_width=100;
    int m_depth=100;
    float m_xExtent=50.0f;
    float m_zExtent=50.0f;
    ngl::Mat4 m_view;
    ngl::Mat4 m_project;
    std::unique_ptr<Snake> m_snake;
};

#endif