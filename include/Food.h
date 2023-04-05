#ifndef FOOD_H_
#define FOOD_H_
#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
class Food
{
  public :
    Food(int _w, int _d,ngl::Mat4 &_view, ngl::Mat4 &_project);
    ~Food()=default;
    void draw() const;
    void update() ;
    ngl::Vec3 getPos() const;
    bool needsUpdate();
    void resetFood();

  private :

    ngl::Vec3 m_pos;
    int m_width;
    int m_height;
    int m_timeOut=100;
    ngl::Mat4 &m_view;
    ngl::Mat4 &m_project;

};

#endif