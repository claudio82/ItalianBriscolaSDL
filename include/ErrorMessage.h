#pragma once
#include <iostream>
#include <SDL3/SDL.h>

class ErrorMessage
{
public:
    ErrorMessage();
    ~ErrorMessage();
  
    static void showSDlError(std::string message);
};