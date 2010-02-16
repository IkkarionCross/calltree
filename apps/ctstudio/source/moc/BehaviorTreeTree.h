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

#ifndef BEHAVOIRTREETREE_H_INCLUDED
#define BEHAVOIRTREETREE_H_INCLUDED

#include "../GraphicsItemTypes.h"
#include "BehaviorTreeSceneItem.h"

struct BehaviorTree;

class BehaviorTreeTree: public BehaviorTreeSceneItem
{
  Q_OBJECT
public:

  enum
  {
    Type = BehaviorTreeTreeType
  };

  BehaviorTreeTree( BehaviorTree* tree );

  int type() const
  {
    return Type;
  }

  BehaviorTree* GetTree() { return m_Tree; }

protected:

  BehaviorTree* m_Tree;

};

#endif