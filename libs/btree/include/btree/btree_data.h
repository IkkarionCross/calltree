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

#ifndef BTREE_DATA_H_INCLUDED
#define BTREE_DATA_H_INCLUDED

typedef unsigned int hash_t;
const hash_t INVALID_ID = 0xffffffff;

enum NodeGristType
{
  E_GRIST_UNKOWN,
  E_GRIST_SEQUENCE,
  E_GRIST_SELECTOR,
  E_GRIST_PARALLEL,
  E_GRIST_DYN_SELECTOR,
  E_GRIST_SUCCEED,
  E_GRIST_FAIL,
  E_GRIST_WORK,
  E_GRIST_TREE,
  E_GRIST_ACTION,
  E_GRIST_DECORATOR,
  E_MAX_GRIST_TYPES
};

enum SymbolTypes
{
  E_ST_UNKOWN, E_ST_TREE, E_ST_ACTION, E_ST_DECORATOR, E_ST_TYPE, E_MAX_SYMBOL_TYPES
};

enum ParameterType
{
  E_VART_UNDEFINED,
  E_VART_INTEGER,
  E_VART_FLOAT,
  E_VART_BOOL,
  E_VART_STRING,
  E_VART_HASH,
  E_VART_LIST,
  E_MAX_VARIABLE_TYPE
};

struct Identifier
{
  const char* m_Text;
  hash_t m_Hash;
};

struct Locator
{
  const char*  m_Buffer;
  unsigned int m_LineNo;
};

struct StringData
{
  const char* m_Parsed;
  const char* m_Raw;
};

struct Parameter;

union ParameterData
{
  int m_Integer;
  hash_t m_Hash;
  float m_Float;
  StringData m_String;
  Parameter* m_List;
  bool m_Bool;
};

struct Parameter
{
  Identifier m_Id;
  Locator m_Locator;
  ParameterType m_Type;
  ParameterData m_Data;
  Parameter* m_Next;
  bool m_ValueSet;
  bool m_Declared;
};

struct Action
{
  Identifier m_Id;
  Locator m_Locator;
  Parameter* m_Declarations; /* Argument declarations */
  Parameter* m_Options; /* Code-generation variables */
  bool m_Declared;
};

struct Decorator
{
  Identifier m_Id;
  Locator m_Locator;
  Parameter* m_Declarations; /* Argument declarations */
  Parameter* m_Options; /* Code-generation variables */
  bool m_Declared;
};

struct Node;
struct BehaviorTree;

struct SequenceGrist
{
  Node* m_FirstChild;
};

struct SelectorGrist
{
  Node* m_FirstChild;
};

struct ParallelGrist
{
  Node* m_FirstChild;
};

struct DynSelectorGrist
{
  Node* m_FirstChild;
};

struct DecoratorGrist
{
  Node* m_Child;
  Decorator* m_Decorator;
  Parameter* m_Parameters;
};

struct ActionGrist
{
  Action* m_Action;
  Parameter* m_Parameters;
};

struct TreeGrist
{
  BehaviorTree* m_Tree;
  Parameter* m_Parameters;
};

struct NodeGrist
{
  NodeGristType m_Type;
  union
  {
    SequenceGrist m_Sequence;
    SelectorGrist m_Selector;
    ParallelGrist m_Parallel;
    DynSelectorGrist m_DynSelector;
    DecoratorGrist m_Decorator;
    ActionGrist m_Action;
    TreeGrist m_Tree;
  };
};

enum NodeParentTypes
{
  E_NP_UNKOWN, E_NP_NODE, E_NP_TREE, E_MAX_NODE_PARENT_TYPES
};

struct NodeParent
{
  NodeParentTypes m_Type;
  union
  {
    Node* m_Node;
    BehaviorTree* m_Tree;
  };
};

struct Node
{
  NodeGrist m_Grist;
  NodeParent m_Pare;
  Locator m_Locator;
  unsigned int m_NodeId;
  Node* m_Next;
  Node* m_Prev;
  void* m_UserData;
};

struct BehaviorTree
{
  Identifier m_Id;
  Locator m_Locator;
  Parameter* m_Declarations;
  Node* m_Root;
  void* m_UserData;
  bool m_Declared;
};

typedef unsigned int mem_size_t;
typedef void* (*AllocateMemoryCallback)( const mem_size_t size );
typedef void (*FreeMemoryCallback)( void* object_pointer );

struct Allocator
{
  AllocateMemoryCallback m_Alloc; // The function that will be used for all memory allocations
  FreeMemoryCallback m_Free; // The function that will be used to free all allocated memory
};

union SymbolTypeData
{
  BehaviorTree* m_Tree;
  Action* m_Action;
  Decorator* m_Decorator;
  Parameter* m_Type;
};

struct NamedSymbol
{
  SymbolTypes m_Type;
  SymbolTypeData m_Symbol;
};

struct Include
{
  hash_t m_Hash;
  const char* m_Name;
  const char* m_Parent;
  int m_LineNo;
  void* m_UserData;
  Include* m_Next;
};

struct StringBuffer
{
  Allocator m_Allocator;
  char* m_Str;
  int m_Size;
  int m_Capacity;
};

typedef struct SParserContext* ParserContext;
typedef struct SSaverContext* SaverContext;
typedef struct SBehaviorTreeContext* BehaviorTreeContext;

typedef void (*ErrorCallback)( ParserContext, const char* msg );
typedef void (*WarningCallback)( ParserContext, const char* msg );
typedef const char* (*ParserTranslateIncludeCallback)( ParserContext,
  const char* );
typedef const char* (*SaveTranslateIncludeCallback)( SaverContext, const char* );
typedef int (*FillBufferCallback)( ParserContext, char* buffer, int maxsize );
typedef void (*FlushBufferCallback)( SaverContext, const char*, int size );

struct ParserContextFunctions
{
  ErrorCallback m_Error;
  WarningCallback m_Warning;
  FillBufferCallback m_Read;
  ParserTranslateIncludeCallback m_Translate;
};

struct SaverContextFunctions
{
  FlushBufferCallback m_Write;
  SaveTranslateIncludeCallback m_Translate;
};

#endif /* BTREE_DATA_H_INCLUDED */
