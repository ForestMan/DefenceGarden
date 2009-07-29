//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------
#undef STARTENUM
#undef E
#undef ENDENUM

#if ENUM
#define STARTENUM(X,Y) X = Y,
#define E(X) X,
#define ENDENUM()
#elif LUA
#define STARTENUM(X,Y) "luaEnumCounter=" #Y "; " #X "= luaEnumCounter; "
#define E(X) " luaEnumCounter = luaEnumCounter+1; " #X " = luaEnumCounter ; "
#define ENDENUM() 
#endif
