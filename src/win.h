#ifndef WIN_H
#define WIN_H

#include "./constants.h"
#include "./board.h"
#include "./renderer.h"
#include "./win.h"
#include "./vector.h"

int Is_GameCompleted();
RenderBuffer Get_WinRenderBuffer();
void Debug_Win();

#endif
