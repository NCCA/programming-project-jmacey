#include "Food.h"
#include <ngl/Random.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>
Food::Food(int _w, int _h,ngl::Mat4 &_view, ngl::Mat4 &_project)  : m_width{_w},m_height{_h},m_view{_view}, m_project{_project}
{
  resetFood();
}

void Food::resetFood()
{
    m_timeOut=ngl::Random::getIntFromGeneratorName("foodTimer");
    m_pos.set(ngl::Random::getIntFromGeneratorName("foodRangeWidth"),0.0f,
    ngl::Random::getIntFromGeneratorName("foodRangeHeight"));
}

void Food::draw() const
{
  auto tx=ngl::Transformation();
  tx.setPosition(m_pos);
  auto MVP=m_project*m_view*tx.getMatrix();
  ngl::ShaderLib::setUniform("MVP",MVP);
  ngl::VAOPrimitives::draw("sphere");
}

void Food::update()
{
  if(--m_timeOut <=0)
    resetFood();
}

ngl::Vec3 Food::getPos() const
{
  return m_pos;
}
