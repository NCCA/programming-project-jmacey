#ifdef WIN32
    #define SDL_MAIN_HANDLED
#endif
#include <SDL2/SDL.h>
#include <cstdlib>
#include <iostream>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/Random.h>
#include <ngl/Util.h>
#include <ngl/Text.h>
#include <chrono>
#include "Arena.h"

/// @brief function to quit SDL with error message
/// @param[in] _msg the error message to send
void SDLErrorExit(const std::string &_msg);

/// @brief initialize SDL OpenGL context
SDL_GLContext createOpenGLContext( SDL_Window *window);


int main(int argc, char * argv[])
{
  // under windows we must use main with argc / v so jus flag unused for params
  NGL_UNUSED(argc);
  NGL_UNUSED(argv);
  // Initialize SDL's Video subsystem
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0 )
  {
    // Or die on error
    SDLErrorExit("Unable to initialize SDL");
  }

  // now create our window
  int screenWidth=600;
  int screenHeight=600;
  SDL_Window *window=SDL_CreateWindow("SNAKE ",
                                      SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,
                                      screenWidth,
                                      screenHeight,
                                      SDL_WINDOW_OPENGL  | SDL_WINDOW_ALLOW_HIGHDPI
                                     );
  // check to see if that worked or exit
  if (!window)
  {
    SDLErrorExit("Unable to create window");
  }

  // Create our opengl context and attach it to our window

   SDL_GLContext glContext=createOpenGLContext(window);
   if(!glContext)
   {
     SDLErrorExit("Problem creating OpenGL context ");
   }
   // make this our current GL context (we can have more than one window but in this case not)
   SDL_GL_MakeCurrent(window, glContext);
   SDL_RaiseWindow(window);
  /* This makes our buffer swap syncronized with the monitor's vertical refresh */
  SDL_GL_SetSwapInterval(1);
  // we need to initialise the NGL lib which will load all of the OpenGL functions, this must
  // be done once we have a valid GL context but before we call any GL commands. If we dont do
  // this everything will crash
  // Now build Arena
  constexpr int ArenaWidth=40;
  constexpr int ArenaHeight=40;
  ngl::NGLInit::initialize();
  // setup checker shader
  ngl::ShaderLib::use(ngl::nglCheckerShader);
  ngl::ShaderLib::setUniform("lightDiffuse", 1.0f, 1.0f, 1.0f, 1.0f);
  ngl::ShaderLib::setUniform("checkOn", true);
  ngl::ShaderLib::setUniform("lightPos", 0.0f,2.0f,0.0f);
  ngl::ShaderLib::setUniform("colour1", 0.9f, 0.9f, 0.9f, 1.0f);
  ngl::ShaderLib::setUniform("colour2", 0.6f, 0.6f, 0.6f, 1.0f);
  ngl::ShaderLib::setUniform("checkSize", 10.0f);
  ngl::ShaderLib::setUniform("normalMatrix",ngl::Mat3());

  // create our sphere for the rendering
  ngl::VAOPrimitives::createSphere("sphere",0.5f,20.0f);
  // and floor
  ngl::VAOPrimitives::createTrianglePlane("floor",ArenaWidth,ArenaHeight,10,10,ngl::Vec3(0,1,0));
  // We can add distributions to ngl::Random like this by default there are
  // no integer ones.
  std::uniform_int_distribution<> foodTimer(10,200);
  ngl::Random::addIntGenerator("foodTimer",foodTimer);
  std::uniform_int_distribution<> foodRangeWidth(-(ArenaWidth-1)/2,(ArenaWidth-1)/2);
  ngl::Random::addIntGenerator("foodRangeWidth",foodRangeWidth);
  
  std::uniform_int_distribution<> foodRangeHeight(-(ArenaHeight-1)/2,(ArenaHeight-1)/2);
  ngl::Random::addIntGenerator("foodRangeHeight",foodRangeHeight);


  auto arena=Arena(ArenaWidth,ArenaHeight);


  auto textRender = std::make_unique<ngl::Text>("fonts/Arial.ttf", 18);
  textRender->setScreenSize(screenWidth, screenHeight);


  // now clear the screen and swap whilst NGL inits (which may take time)
  SDL_GL_SwapWindow(window);
  // flag to indicate if we need to exit
  bool quit=false;
  // sdl event processing data structure
  SDL_Event event;
  auto previousTime=std::chrono::high_resolution_clock::now();
  auto currentTime = std::chrono::high_resolution_clock::now();
  float delta=0.01f;
  while(!quit)
  {
  glClearColor(0.7,0.7,0.7,1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    while ( SDL_PollEvent(&event) )
    {
      switch (event.type)
      {
        // this is the window x being clicked.
        case SDL_QUIT : quit = true; break;
        // if the window is re-sized pass it to the ngl class to change gl viewport
        // note this is slow as the context is re-create by SDL each time
        case SDL_WINDOWEVENT :
        break;

        // now we look for a keydown event
        case SDL_KEYDOWN:
        {
          switch( event.key.keysym.sym )
          {
            // if it's the escape key quit
            case SDLK_ESCAPE :  quit = true; break;
            case SDLK_UP : arena.setDirection(Direction::UP); break;
            case SDLK_DOWN : arena.setDirection(Direction::DOWN); break;
            case SDLK_LEFT : arena.setDirection(Direction::LEFT); break;
            case SDLK_RIGHT : arena.setDirection(Direction::RIGHT); break;
            case SDLK_SPACE : arena.setDirection(Direction::STOP); break;
            case SDLK_r : arena.resetGame(); break;
            case SDLK_1 : arena.setCamera(Arena::CameraMode::_2D); break;
            case SDLK_2 : arena.setCamera(Arena::CameraMode::_3D); break;
            
            //case SDLK_a : arena.addSegment(); break;
            default : break;
          } // end of key process
        } // end of keydown

        break;
      default : break;
      } // end of event switch
    } // end of poll events

    auto currentTime = std::chrono::high_resolution_clock::now();
    textRender->setColour(0.0f,0.0f,0.0f);
    textRender->renderText(50,screenHeight-60, fmt::format("Score {}",arena.getScore()));

    arena.draw();
    arena.update();
    if(arena.gameOver())
    {
      textRender->setColour(1.0f, 0.0f, 0.0f);
      textRender->renderText(100, screenWidth/2, "Game Over Press R to reset");
    }
    delta = std::chrono::duration<float,std::chrono::seconds::period>(currentTime-previousTime).count();
    previousTime=currentTime;
    SDL_GL_SwapWindow(window);

    SDL_Delay(delta*850);

  }
  // now tidy up and exit SDL
 SDL_Quit();
 // whilst this code will never execute under windows we need to have a return from
 // SDL_Main!
 return EXIT_SUCCESS;
}


SDL_GLContext createOpenGLContext(SDL_Window *window)
{
    // Note you may have to change this depending upon the driver (Windows is fussy)
    // stick to 4.5 as the core base level for NGL works ok
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  // set multi sampling else we get really bad graphics that alias
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,4);
  // Turn on double buffering with a 24bit Z buffer.
  // You may need to change this to 16 or 32 for your system
  // on mac up to 32 will work but under linux centos build only 16
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  // enable double buffering (should be on by default)
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  //
  return SDL_GL_CreateContext(window);

}

void SDLErrorExit(const std::string &_msg)
{
  std::cerr<<_msg<<"\n";
  std::cerr<<SDL_GetError()<<"\n";
  SDL_Quit();
  exit(EXIT_FAILURE);
}
