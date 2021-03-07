
#pragma once
/*----------------------------------------------]
[ AboutUs                                       ]
[ Michael Sanders                               ]
[ ---                                           ]
[ AboutUs.h                                     ]
[ This class paints developer info to a Render  ]
[ Window.                                       ]
[----------------------------------------------*/
#pragma once
#include "BGraphic.h"
using namespace sf;

class AboutUs {
private:
	RenderWindow* aboutWindow;
	BText devNames;
	BText devHeader;
	Font* MyFont;
public:
	AboutUs() {
		MyFont = new Font();
		if (!MyFont->loadFromFile("fonts/font1.otf")) { //load font
			std::cerr << "Could not load font " << "fonts/font2.ttf" << std::endl;
			std::exit(1);
		}
	}
	~AboutUs() = default;
	void show();

};
