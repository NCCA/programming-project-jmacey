#include "Snake.h"
#include <ngl/VAOPrimitives.h>
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>
#include <algorithm>
#include <iostream>
#include <ngl/NGLStream.h>
constexpr float g_step=1.0f;

Snake::Snake(float _x, float _z,ngl::Mat4 &_view, ngl::Mat4 &_project) : m_pos{_x,0.0f,_z}, m_view{_view}, m_project{_project}
{

}

void Snake::setDirection(Direction _dir)
{
  m_currentDirection=_dir;
}

void Snake::addSegment()
{
  m_segments.push_back(m_pos);
  updateSegment(m_pos);
}

void Snake::updateSegment(ngl::Vec3 &_s)
{
  switch(m_currentDirection)
  {
    case Direction::UP :
      _s.m_z += g_step;
    break;
    case Direction::DOWN :
       _s.m_z -= g_step;
    break;
    case Direction::LEFT :
       _s.m_x -= g_step;
    break;
    case Direction::RIGHT :
       _s.m_x += g_step;
    break;
    default : break;
  }
  //std::cout<<_s.m_lastPos<<"\n";
}


void Snake::move()
{

  // update lead segment
  if(m_segments.size()>=1 && m_currentDirection !=Direction::STOP)
  {
    std::rotate(std::begin(m_segments),std::begin(m_segments)+1,std::end(m_segments));
    m_segments[0]=m_pos;
  }
  updateSegment(m_pos);
  checkSelfCollision();
 
}
void Snake::draw() const
{
  ngl::ShaderLib::use(ngl::nglColourShader);
  ngl::ShaderLib::setUniform("Colour",1.0f,0.0f,0.0f,1.0f);
  auto draw=[this](ngl::Vec3 _p){
    auto tx=ngl::Transformation();
    tx.setPosition(_p);
    tx.setScale(.9f,.9f,.9f);
    auto MVP=m_project*m_view*tx.getMatrix();
    ngl::ShaderLib::setUniform("MVP",MVP);
    ngl::VAOPrimitives::draw(ngl::cube);
  };
  draw(m_pos);
  for(auto s : m_segments)
  {
    draw(s);
  }
}

bool Snake::isAlive() const
{
  return m_alive;
}

void Snake::checkSelfCollision()
{
  if(std::any_of(std::begin(m_segments),std::end(m_segments),[this](auto p){
    return m_pos==p;
  }))
  {
    m_currentDirection=Direction::STOP;
    m_alive=false;
  }
}

ngl::Vec3 Snake::getPos() const
{
  return m_pos;
}

void Snake::setDead()
{
  m_alive=false;
}