#ifndef SNAKE_H_
#define SNAKE_H_

#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
#include <vector>
enum class Direction : char {UP,DOWN,LEFT,RIGHT,STOP};

class Snake
{
  public :
    Snake(float _x, float _y,ngl::Mat4 &_view, ngl::Mat4 &_project);
    void move();
    void draw() const;
    void setDirection(Direction _dir);
    void addSegment();
  private :

    struct Segment
    {
      ngl::Vec3 m_lastPos;
      Direction m_lastDirection;
    };
    std::vector<Segment> m_segments;
    void updateSegment(Segment &_s);
    Direction m_currentDirection=Direction::UP;
    ngl::Mat4 &m_view;
    ngl::Mat4 &m_project;


};

#endif