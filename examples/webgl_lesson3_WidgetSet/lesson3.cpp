/*
	This file is part of ReWeb3D.

    ReWeb3D is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License.

    ReWeb3D is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ReWeb3D.  If not, see <http://www.gnu.org/licenses/>.

    Copyright (C) 2011 Vicomtech Foundation, 
	Donostia - San Sebastian, Spain.
    All Rights Reserved.

    Written by Mauricio Aristizabal <maristi7@eafit.edu.co>
*/

/*
 * Lesson 2 of webgl-lessons http://learningwebgl.com/blog/?p=28
 */

#include <Wt/WApplication>
#include <Wt/WServer>
#include <Wt/WEnvironment>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WImage>
#include <Wt/WText>
#include <Wt/WTimer>
#include <Wt/WPushButton>
#include "DrawGLWidget.h"


using namespace Wt;

const std::string fragShad("#ifdef GL_ES\n"
                           "precision highp float;\n"
                           "#endif\n"
                           "\n"
                           "varying vec4 vColor;\n"
                           "\n"
                           "void main(void) {\n"
                           "    gl_FragColor = vColor;\n"
                           "}\n");


const std::string vertShad("attribute vec3 aVertexPosition;\n"
                           "attribute vec4 aVertexColor;\n"
                           "\n"
                           "uniform mat4 uMVMatrix;\n"
                           "uniform mat4 uPMatrix;\n"
                           "uniform mat4 uRMatrix;\n"
                           "\n"
                           "varying vec4 vColor;\n"
                           "\n"
                           "void main(void) {\n"
                           "    gl_Position = uPMatrix * uMVMatrix * uRMatrix * vec4(aVertexPosition, 1.0);\n"
                           "    vColor = aVertexColor;\n"
                           "}\n");



/*
 * A pretty basic WebGL demo application
 */
class WebGLDemo : public WApplication
{
public:
  WebGLDemo(const WEnvironment& env);
  ~WebGLDemo();
  void animate();

private:
  void updateShaders();
  void resetShaders();
  WContainerWidget *top;
  WContainerWidget *glContainer_;
  WContainerWidget *glContainer2_;
  DrawGLWidget *drawGLWidget_;
  DrawGLWidget *drawGLWidget2_;
  WTextArea *fragmentShaderText_;
  WTextArea *vertexShaderText_;
  WTimer *timer;
  WPushButton *Button_;
};





WebGLDemo::WebGLDemo(const WEnvironment& env)
  : WApplication(env)
{

   /*
    * In WidgetSet mode, we create and bind containers to existing
    * divs in the web page. In this example, we create a single div
    * whose DOM id was passed as a request argument.
    */
   top = new Wt::WContainerWidget();
   const std::string *div = env.getParameter("div");
   if (div) {
     setJavaScriptClass(*div);
     bindWidget(top, *div);
   } else {
     std::cerr << "Missing: parameter: 'div'" << std::endl;
     return;
   }
   std::string googAnalytics(" var _gaq = _gaq || [];"
                             "_gaq.push(['_setAccount', 'UA-22447986-1']);"
                             "_gaq.push(['_trackPageview']);"
                             ""
                             "(function() {"
                             "  var ga = document.createElement('script'); ga.type = 'text/javascript'; ga.async = true;"
                             "  ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';"
                             "  var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(ga, s);"
                             "})();");

  doJavaScript(googAnalytics);
  setTitle("Demo");

  top->addWidget(new WText("Wt/WebGL Lesson 3: A Bit of Movement"));
  top->addWidget(new WBreak());

  drawGLWidget_ = 0;
  drawGLWidget2_ = 0;

  glContainer_ = new WContainerWidget(top);
  glContainer_->resize(500, 500);
  glContainer_->setInline(false);


  glContainer2_ = new WContainerWidget(top);
  glContainer2_->resize(500, 500);
  glContainer2_->setInline(false);

  updateShaders();

  WPushButton *buttonAlert= new WPushButton("start Anim 1", top);
  buttonAlert->clicked().connect(drawGLWidget_->clientSideFcn_);

  WPushButton *buttonAlert2= new WPushButton("start Anim 2", top);
  buttonAlert2->clicked().connect(drawGLWidget2_->clientSideFcn_);

  //Set up the Timer in order to animate the scene.
  //timer = new WTimer(drawGLWidget_);
  //timer->setInterval(25);
  //timer->start();
  //timer->timeout().connect(this,&WebGLDemo::animate);

  /////////////////////////////////////////////
  root()->addWidget(new WBreak());
  root()->addWidget(new WBreak());
  root()->addWidget(new WBreak());
  root()->addWidget(new WText("This demo has been developed by Vicomtech-IK4 Research Center"));

  root()->addWidget(new WBreak());
  root()->addWidget(new WText("Based on the learningwebgl.com lessons."));
  root()->addWidget(new WBreak());
  root()->addWidget(new WText("www.vicomtech.es"));

  //root()->addWidget(new WBreak());
  //root()->addWidget(new WImage("vicomLogoSm.png"));
  /////////////////////////////////////////////
}

WebGLDemo::~WebGLDemo(){
    delete glContainer_;
    delete drawGLWidget_;
    delete drawGLWidget2_;
    delete fragmentShaderText_;
    delete vertexShaderText_;
    delete timer;

}

void WebGLDemo::updateShaders()
{
    //delete drawGLWidget_;
    drawGLWidget_ = new DrawGLWidget(glContainer_);
    drawGLWidget_->resize(500, 500);
    drawGLWidget_->setShaders(vertShad,fragShad);
    drawGLWidget_->setAlternativeContent(new WImage("nowebgl.png"));

    drawGLWidget2_ = new DrawGLWidget(glContainer2_);
    drawGLWidget2_->resize(500, 500);
    drawGLWidget2_->setShaders(vertShad,fragShad);
    drawGLWidget2_->setAlternativeContent(new WImage("nowebgl.png"));

}
void WebGLDemo::animate()
{
    // Call the repaintGL() to update the paint event
    drawGLWidget_->repaintGL(DrawGLWidget::PAINT_GL);
}

WApplication *createApplication(const WEnvironment& env)
{
  return new WebGLDemo(env);
}

int main(int argc, char **argv)
{
    Wt::WServer server(argv[0]);

  // Use default server configuration: command line arguments and the
  // wthttpd configuration file.
  server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);

  // Application entry points. Each entry point binds an URL with an
  // application (with a callback function used to bootstrap a new
  // application).

  // The following is the default entry point. It configures a
  // standalone Wt application at the deploy path configured in the
  // server configuration.
  //server.addEntryPoint(Wt::Application, createApplication);

  // The following adds an entry point for a widget set. A widget set
  // must be loaded as a JavaScript from an HTML page.
  server.addEntryPoint(Wt::WidgetSet, createApplication, "/WebGL.js");

  // Start the server (in the background if there is threading support)
  // and wait for a shutdown signal (e.g. Ctrl C, SIGKILL)
  if (server.start()) {
    Wt::WServer::waitForShutdown();

    // Cleanly terminate all sessions
    server.stop();
  }
  //return WRun(argc, argv, &createApplication);
}

