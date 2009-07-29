//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------


/**
 * @condfile
 *
 * This file contains a set of most public includes available to
 * the external programmer. Use of this file is strictly for
 * convenience: As a rule, Playground files can be sparsely
 * included, and will include their own dependencies. If you
 * come across a Playground file with a missing dependency,
 * please let us know on the forums and we'll fix it. Also, if
 * there's a useful file that pflib.h is missing, let us know.
 *
 * Playground uses the \#ifndef syntax in headers to minimize
 * compile time in the cases where a file was already included.
 * We duplicate the \#ifndef bracket within the file itself, so
 * it's not strictly required, but we like things to build more
 * quickly so that our developers can be more efficient.
 *
 */

#ifndef PFLIB_H_INCLUDED
#define PFLIB_H_INCLUDED

#ifndef PFLIBCORE_H_INCLUDED
#include "pflibcore.h"
#endif

#ifndef DEBUG_H_INCLUDED
#include "debug.h"
#endif

#ifndef STR_H_INCLUDED
#include "str.h"
#endif

#ifndef PLATFORM_H_INCLUDED
#include "platform.h"
#endif

#ifndef STRINGTABLE_H_INCLUDED
#include "stringtable.h"
#endif

#ifndef FORWARD_H_INCLUDED
#include "forward.h"
#endif

#ifndef EVENT_H_INCLUDED
#include "event.h"
#endif

#ifndef FILE_H_INCLUDED
#include "file.h"
#endif

#ifndef RENDERER_H_INCLUDED
#include "renderer.h"
#endif

#ifndef SOUND_H_INCLUDED
#include "sound.h"
#endif

#ifndef SOUNDMANAGER_H_INCLUDED
#include "soundmanager.h"
#endif

#ifndef ANIMATEDTEXTURE_H_INCLUDED
#include "animatedtexture.h"
#endif

#ifndef MODEL_H_INCLUDED
#include "model.h"
#endif

#ifndef MSNZONE_H_INCLUDED
#include "msnzone.h"
#endif

#ifndef ANIMATEDSPRITE_H_INCLUDED
#include "animatedsprite.h"
#endif

#ifndef TEXTSPRITE_H_INCLUDED
#include "textsprite.h"
#endif

#ifndef ANIMTASK_H_INCLUDED
#include "animtask.h"
#endif

#ifndef TASKLIST_H_INCLUDED
#include "tasklist.h"
#endif

#ifndef ASSETMAP_H_INCLUDED
#include "assetmap.h"
#endif

#ifndef TEXTEDIT_H_INCLUDED
#include "textedit.h"
#endif

#ifndef WINDOWMANAGER_H_INCLUDED
#include "windowmanager.h"
#endif

#ifndef WINDOWSTYLE_H_INCLUDED
#include "windowstyle.h"
#endif

#ifndef IMAGE_H_INCLUDED
#include "image.h"
#endif

#ifndef BUTTON_H_INCLUDED
#include "button.h"
#endif

#ifndef SLIDER_H_INCLUDED
#include "slider.h"
#endif

#ifndef SCREEN_H_INCLUDED
#include "screen.h"
#endif

#ifndef MODALWINDOW_H_INCLUDED
#include "modalwindow.h"
#endif

#ifndef CLOCK_H_INCLUDED
#include "clock.h"
#endif

#ifndef DIALOG_H_INCLUDED
#include "dialog.h"
#endif

#ifndef DRAWSPEC_H_INCLUDED
#include "drawspec.h"
#endif

#ifndef ENCRYPT_H_INCLUDED
#include "encrypt.h"
#endif

#ifndef FIXBYTEORDER_H_INCLUDED
#include "fixbyteorder.h"
#endif

#ifndef FLASHHOST_H_INCLUDED
#include "flashhost.h"
#endif

#ifndef PFLUA_H_INCLUDED
#include "pflua.h"
#endif

#ifndef LUAAUTOBLOCK_H_INCLUDED
#include "luaautoblock.h"
#endif

#ifndef LUATABLE_H_INCLUDED
#include "luatable.h"
#endif

#ifndef LUAPARTICLESYSTEM_H_INCLUDED
#include "luaparticlesystem.h"
#endif

#ifndef MAT_H_INCLUDED
#include "mat.h"
#endif

#ifndef MESSAGE_H_INCLUDED
#include "message.h"
#endif

#ifndef MESSAGELISTENER_H_INCLUDED
#include "messagelistener.h"
#endif

#ifndef SCRIPT_H_INCLUDED
#include "script.h"
#endif

#ifndef SIMPLEHTTP_H_INCLUDED
#include "simplehttp.h"
#endif

#ifndef SIMPLEXML_H_INCLUDED
#include "simplexml.h"
#endif

#ifndef RANDOM_H_INCLUDED
#include "random.h"
#endif

#ifndef VERTEXSET_H_INCLUDED
#include "vertexset.h"
#endif

#ifndef PFHISCORES_H_INCLUDED
#include "pfhiscores.h"
#endif

#ifndef LIGHT_H_INCLUDED
#include "light.h"
#endif

#ifndef GAMESTATE_H_INCLUDED
#include "gamestate.h"
#endif

#ifndef PREFS_H_INCLUDED
#include "prefs.h"
#endif

#endif // PFLIB_H_INCLUDED
