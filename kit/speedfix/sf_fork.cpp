// SPDX-License-Identifier: GPL-3.0-only
//
// sf_fork.cpp -- speedfix's forks of the game's functions (built against the reconstruction's headers, like
// sf_game.cpp, but apart from it: the gate of sf_game.cpp's hook links that file alone, without the forks).
#include "xikit_sdk.hpp"
#include "speedfix.h"

// skin_merge: the fork of SkinAndDrawV2 (fork/CYyModelDt__SkinAndDrawV2.fork.cpp, gated with fork.py gate --declare
// fork/skinmerge.declare.json); with xi_skinmerge_on 0 it is the game's function (the gate without declarations)
extern "C" int sf_skinmerge_installed;
int sf_skinmerge_installed;
extern "C" int sf_fork_skinmerge(const XikitHost *host) {
  int r = XI_FORK(host, CYyModelDt::SkinAndDrawV2);
  sf_skinmerge_installed = r == XIKIT_OK;
  return r;
}

