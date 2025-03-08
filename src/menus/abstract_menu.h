#pragma once

namespace SDL {
class Surface;
}

// FIXME: That ontology is pretty bad. Find something better.
enum MenuType : int { Main, Options, Exit, Keys_Keyboard, Keys_Mouse, Keys_Pad, Play };

class AbstractMenu {
   public:
	virtual void Draw(SDL::Surface* surf) const = 0;
	virtual int ProcessEvent() = 0;
};
