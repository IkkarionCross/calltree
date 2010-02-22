/*******************************************************************************
 * Copyright (c) 2009-04-24 Joacim Jacobsson.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Joacim Jacobsson - first implementation
 *******************************************************************************/

#include "parser.h"

#include <string.h> // for memset....

void* ParserContextGetExtra( ParserContext pc )
{
  return pc->m_Extra;
}

void ParserContextSetExtra( ParserContext pc, void* extra )
{
  pc->m_Extra = extra;
}

void ParserContextSetCurrent( ParserContext pc, const char* curr )
{
  pc->m_Current = curr;
}

const char* ParserContextGetCurrent( ParserContext pc )
{
  return pc->m_Current;
}

int ParserContextGetLineNo( ParserContext pc )
{
  return pc->m_LineNo;
}

int Parse( ParserContext pc, ParserContextFunctions* pcf )
{
  pc->m_LineNo = 1;
  pc->m_Funcs  = *pcf;

  void* yyscanner = 0x0;
  yylex_init_extra( pc, &yyscanner );
  yyset_extra( pc, yyscanner );
  int r = yyparse( pc, yyscanner );
  yylex_destroy( yyscanner );

  memset( &pc->m_Funcs, 0, sizeof(ParserContextFunctions) );

  return r;
}

BehaviorTreeContext ParserContextGetBehaviorTreeContext( ParserContext pc )
{
  return pc->m_Tree;
}

/*
 * Parser extra functions.
 */

void yyerror( ParserContext ctx, const char* msg )
{
  if( ctx->m_Funcs.m_Error )
    ctx->m_Funcs.m_Error( ctx, msg );
}

void yywarning( ParserContext ctx, const char* msg )
{
  if( ctx->m_Funcs.m_Warning )
    ctx->m_Funcs.m_Warning( ctx, msg );
}

void yyerror( SParserContext* ctx, void*, const char* msg )
{
  yyerror( ctx, msg );
}

void yywarning( SParserContext* ctx, void*, const char* msg )
{
  yywarning( ctx, msg );
}


