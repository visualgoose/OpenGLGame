#pragma once

#include <SDL3/SDL_assert.h>

#ifdef OGLGAME_SHOWDBGINFO
#define vgassert(condition) SDL_assert(condition)
#else
#define vgassert(condition)
#endif