#ifndef ARENA_H_
#define ARENA_H_
#include <ngl/Mat4.h>
class Arena
{
  public :
    Arena(int _width, int _depth);
    void draw() const;
  private :

    int m_width=100;
    int m_depth=100;
    float m_xExtent=50.0f;
    float m_zExtent=50.0f;
    ngl::Mat4 m_view;
    ngl::Mat4 m_project;
};

#endif