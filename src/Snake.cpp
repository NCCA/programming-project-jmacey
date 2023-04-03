#include "Snake.h"
#include <ngl/VAOPrimitives.h>
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>
#include <algorithm>
#include <iostream>
#include <ngl/NGLStream.h>
constexpr float g_step=1.0f;

Snake::Snake(float _x, float _z,ngl::Mat4 &_view, ngl::Mat4 &_project) : m_view{_view}, m_project{_project}
{
  m_segments.push_back({{_x,0.0f,_z},m_currentDirection});
}

void Snake::setDirection(Direction _dir)
{
  //m_currentDirection=_dir;
  m_segments[0].m_lastDirection=_dir;
}

void Snake::addSegment()
{
  auto lastSegment=m_segments[m_segments.size()];
  updateSegment(lastSegment);
  m_segments.push_back(lastSegment);
}

void Snake::updateSegment(Segment &_s)
{
  switch(_s.m_lastDirection)
  {
    case Direction::UP :
      _s.m_lastPos.m_z += g_step;
    break;
    case Direction::DOWN :
       _s.m_lastPos.m_z -= g_step;
    break;
    case Direction::LEFT :
       _s.m_lastPos.m_x -= g_step;
    break;
    case Direction::RIGHT :
       _s.m_lastPos.m_x += g_step;
    break;
    default : break;
  }
  std::cout<<_s.m_lastPos<<"\n";
}


void Snake::move()
{

  // update lead segment
  updateSegment(m_segments[0]);
  if(m_segments.size()>1)
  {
    for(size_t i=1; i<m_segments.size(); ++i)
    {
      m_segments[i]=m_segments[i+1];
    }
    
    for(size_t i=1; i<m_segments.size(); ++i)
    {
      updateSegment(m_segments[i]);
    }
  }
  // for(auto &s : m_segments)
  // {
  // switch(s.m_lastDirection)
  // {
  //   case Direction::UP :
  //     s.m_lastPos.m_z += g_step;
  //   break;
  //   case Direction::DOWN :
  //      s.m_lastPos.m_z -= g_step;
  //   break;
  //   case Direction::LEFT :
  //      s.m_lastPos.m_x -= g_step;
  //   break;
  //   case Direction::RIGHT :
  //      s.m_lastPos.m_x += g_step;
  //   break;
  //   default : break;
  // }
  // std::cout<<s.m_lastPos<<"\n";
  // }

  // if(m_segments.size() > 1)
  // {
  //   for(size_t i=0; i<m_segments.size();  ++i)
  //   {
  //     m_segments[i]= m_segments[i+1];
  //   }
  // }

  std::cout<<"size is "<<m_segments.size()<<"\n";
}

void Snake::draw() const
{
  ngl::ShaderLib::use(ngl::nglColourShader);
  ngl::ShaderLib::setUniform("Colour",1.0f,0.0f,0.0f,1.0f);

  for(auto s : m_segments)
  {
    auto tx=ngl::Transformation();
    tx.setPosition(s.m_lastPos);
    tx.setScale(10,10,10);
    auto MVP=m_project*m_view*tx.getMatrix();
    ngl::ShaderLib::setUniform("MVP",MVP);
    ngl::VAOPrimitives::draw(ngl::cube);
  }
}
