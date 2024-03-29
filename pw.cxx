// generated by Fast Light User Interface Designer (fluid) version 1.0106

#include "pw.h"

Fl_Window *PWWind=(Fl_Window *)0;

Fl_Button *keio=(Fl_Button *)0;

Fl_Input *arc=(Fl_Input *)0;

Fl_Button *cancel=(Fl_Button *)0;

unsigned char PassWord[800];

/*Make closer callback here*/
void close(Fl_Widget *w, void *data)
{
   Fl::quit();  
}
/*Make a get callback here*/
void getstring(Fl_Widget *w, void *data) {
   unsigned char *px;
   px = arc->value();
   sprintf(PassWord, "%s", px);
}

int password_window() {
  Fl_Window* w;
  { Fl_Window* o = PWWind = new Fl_Window(215, 174, "Enter Password:");
    w = o;
    o->align(FL_ALIGN_CENTER);
    { Fl_Button* o = keio = new Fl_Button(25, 140, 75, 25, "Keio");
      o->shortcut(0xff0d);
      o->align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
      o->callback(getstring, NULL);
    }
    { Fl_Button* o = cancel = new Fl_Button(120, 140, 85, 25, "Cancel");
      o->shortcut(0xff1b);
      o->callback(close, NULL);
    }
    { Fl_Input* o = arc = new Fl_Input(25, 75, 175, 30, "Password");
      o->labeltype(FL_SHADOW_LABEL);
      o->align(FL_ALIGN_TOP_LEFT);
    }
    /*o->end();*/    Fl::run();
  }
}
