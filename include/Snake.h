#ifndef SNAKE_H_
#define SNAKE_H_

#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
#include <vector>
#include "Directions.h"
class Snake
{
  public :
    Snake(float _x, float _y,ngl::Mat4 &_view, ngl::Mat4 &_project);
    void move();
    void draw() const;
    void setDirection(Direction _dir);
    void addSegment();
    bool isAlive() const ;
  private :
    ngl::Vec3 m_pos;
    Direction m_currentDirection=Direction::UP;
    std::vector<ngl::Vec3> m_segments;
    void updateSegment(ngl::Vec3 &_s);
    void checkSelfCollision();
    ngl::Mat4 &m_view;
    ngl::Mat4 &m_project;
    bool m_alive=true;


};

#endif