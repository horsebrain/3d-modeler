//This file simply defines all the global variables used by the application
//They are extern'd in various headers for other files to use.

#include "wxModeler.h"
#include "App_Log.h"
#include "camera.h"
#include "model.h"

CApp_Log Log("Modeler.log");
Camera view;
Model model;
Modeler * modeler;
ModelerGLCanvas * canvas;
int SCREEN_WIDTH, SCREEN_HEIGHT;