#include "Arena.h"
#include <ngl/Util.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/NGLStream.h>
#include <iostream>

Arena::Arena(int _width,int _depth) : m_width{_width}, m_depth{_depth}
{
  setCamera(CameraMode::_2D);
  resetGame();
}

void Arena::resetGame() 
{
  m_snake=std::make_unique<Snake>(0,0,m_view,m_project);
  m_food = std::make_unique<Food>(m_width,m_depth,m_view,m_project);
  m_score=0;
}

void Arena::draw() 
{

  if(m_cameraMode==CameraMode::_3D)
  {
    ngl::Vec3 look;
    switch(m_currentDirection)
    {
      case Direction::UP : look.set(0,0,1); break;
      case Direction::DOWN : look.set(0,0,-1); break;
      case Direction::LEFT : look.set(1,0,0); break;
      case Direction::RIGHT: look.set(-1,0,0); break;
      case Direction::STOP : break;
    }
    auto pos=m_snake->getPos();
    m_view=ngl::lookAt({0,20,60},{0,0,0},{0,1,0});
  }

  ngl::Transformation tx;
  ngl::ShaderLib::use(ngl::nglColourShader);
  ngl::ShaderLib::setUniform("Colour",1.0f,0.0f,1.0f,0.0f);
  for(float x=-m_xExtent; x<=m_xExtent; ++x)
  {
    tx.setPosition(x,0,-m_zExtent);
    auto MVP=m_project*m_view*tx.getMatrix();
    ngl::ShaderLib::setUniform("MVP",MVP);
    ngl::VAOPrimitives::draw("cube");
    tx.setPosition(x,0,m_zExtent);
    tx.setScale(0.8f,0.8f,0.8f);
    MVP=m_project*m_view*tx.getMatrix();
    ngl::ShaderLib::setUniform("MVP",MVP);
    ngl::VAOPrimitives::draw("cube");
  }
  for(float z=-m_zExtent; z<=m_zExtent; ++z)
  {
    tx.setPosition(-m_xExtent,0,z);
    auto MVP=m_project*m_view*tx.getMatrix();
    ngl::ShaderLib::setUniform("MVP",MVP);
    ngl::VAOPrimitives::draw("cube");
    tx.setPosition(m_xExtent,0,z);
    tx.setScale(0.8f,0.8f,0.8f);
    MVP=m_project*m_view*tx.getMatrix();
    ngl::ShaderLib::setUniform("MVP",MVP);
    ngl::VAOPrimitives::draw("cube");
  }
  m_snake->draw();
  m_food->draw();
}

void Arena::setDirection(Direction _dir)
{
  m_currentDirection=_dir;
  m_snake->setDirection(_dir);
}

bool Arena::gameOver() const
{
  return !m_snake->isAlive();
}

void Arena::checkArenaCollision()
{
  auto pos=m_snake->getPos();
  if(pos.m_x <=-m_xExtent || pos.m_x>=m_xExtent ||
     pos.m_z <=-m_zExtent || pos.m_z >=m_zExtent)
    {
      m_snake->setDead();
      m_snake->setDirection(Direction::STOP);
    }
}

int Arena::getScore() const
{
  return m_score;
}

void Arena::update()
{
  static int count=0;
  m_snake->move();
  m_food->update();
  checkArenaCollision();
  if(m_snake->getPos() == m_food->getPos())
  {
    m_snake->addSegment();
    m_food->resetFood();
    m_score++;
  }


}

void Arena::setCamera(CameraMode _mode)
{
  switch(_mode)
  {
    case CameraMode::_2D :
        m_xExtent=m_width/2.0f;
        m_zExtent=m_depth/2.0f;  
        m_view=ngl::lookAt({0,-m_zExtent,0},{0,1,0},{0,0,1});
        m_project=ngl::ortho(-m_xExtent-1,m_xExtent+1,m_zExtent+1,-m_zExtent-1,0.1f,300.0f); 
        m_cameraMode=CameraMode::_2D;
    break;

    case CameraMode::_3D :
        m_cameraMode=CameraMode::_3D;
        m_project=ngl::perspective(45.0f,1.0f,0.1f,300.0f); 
    break;
  }
}
