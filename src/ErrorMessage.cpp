#include "ErrorMessage.h"

ErrorMessage::ErrorMessage()
{}

ErrorMessage::~ErrorMessage()
{}

void ErrorMessage::showSDlError(std::string msg)
{
    std::cerr << msg << " SDL_Error " << SDL_GetError() << std::endl;
}