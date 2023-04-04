#include "Arena.h"
#include <ngl/Util.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
Arena::Arena(int _width,int _depth) : m_width{_width}, m_depth{_depth}
{
  m_xExtent=m_width/2.0f;
  m_zExtent=m_depth/2.0f;

  m_view=ngl::lookAt({0,-m_zExtent,0},{0,1,0},{0,0,1});
  m_project=ngl::ortho(-m_xExtent,m_xExtent,-m_zExtent,m_zExtent,0.1f,300.0f); 
  m_snake=std::make_unique<Snake>(0,0,m_view,m_project);
}

void Arena::draw() const
{
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
    MVP=m_project*m_view*tx.getMatrix();
    ngl::ShaderLib::setUniform("MVP",MVP);
    ngl::VAOPrimitives::draw("cube");
  }
  m_snake->draw();
}

void Arena::setDirection(Direction _dir)
{
  m_snake->setDirection(_dir);
}

bool Arena::gameOver() const
{
  return !m_snake->isAlive();
}

void Arena::checkArenaCollision()
{
  auto pos=m_snake->getPos();
  if(pos.m_x <=-m_xExtent || pos.m_y>=m_xExtent ||
    pos.m_z <=-m_zExtent || pos.m_z>=m_zExtent)
    {
      m_snake->setDead();
      m_snake->setDirection(Direction::STOP);
    }
}

void Arena::update()
{
  static int count=0;
  if(++count %100)
    m_snake->addSegment();
  m_snake->move();
  checkArenaCollision();
  


}