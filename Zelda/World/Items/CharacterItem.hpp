/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Zelda.
 *
 * Zelda is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Zelda is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Zelda. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#ifndef _CHARACTER_ITEM_HPP_
#define _CHARACTER_ITEM_HPP_

#include "ItemTypes.hpp"
#include <OgrePrerequisites.h>
#include "../../Common/World/Damage.hpp"
#include "../WorldEntity.hpp"
#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>

class CCharacter;
class btRigidBody;
class CMap;
class CCharacterItem;

class CCharacterItemContactResultCallback
  : public btCollisionWorld::ContactResultCallback {
private:
  CCharacterItem *mCharacterItem;
public:
  CCharacterItemContactResultCallback(CCharacterItem *c);

  void init();
  
  btScalar addSingleResult(btManifoldPoint &cp,
                           const btCollisionObjectWrapper *colObj0Wrap,
                           int partId0,
                           int index0,
                           const btCollisionObjectWrapper *colObj1Wrap,
                           int partId1,
                           int index1);

private:
  void collision(const btCollisionObject *obj);
};

class CCharacterItem : public CWorldEntity {
protected:
  CCharacter *mCharacter;
  const EItemVariantTypes mVariantType;
  Ogre::Entity *mAttachedMesh;
  const std::string mBoneToAttach;

  btRigidBody *mBlockPhysics;
  uint16_t mBlockPhysicsGroup;
  uint16_t mBlockPhysicsMask;
  Ogre::Vector3 mPhysicsOffset;

  btRigidBody *mDamagePhysics;
  Ogre::Vector3 mOldDamageStartPos;
  Ogre::Vector3 mOldDamageEndPos;

  CCharacterItemContactResultCallback mContactResultCallback;
public:
  CCharacterItem(CCharacter *character, const std::string &boneToAttach, EItemVariantTypes type);
  virtual ~CCharacterItem();

  virtual void exit() override;

  virtual void enterNewMap(CAbstractMap *oldMap, CAbstractMap *newMap);
  virtual void update(Ogre::Real tpf) override;
  
  void startDamage();
  void updateDamage(Ogre::Real tpf);

  const std::string &getBoneToAttach() const {return mBoneToAttach;}
  EItemVariantTypes getItemVariantType() const {return mVariantType;}
  CCharacter *getCharacter() const {return mCharacter;}
  btRigidBody *getBlockPhysics() const {return mBlockPhysics;}
  btRigidBody *getDamagePhysics() const {return mDamagePhysics;}

  void show();
  void hide();

  virtual CDamage createDamage();
  virtual Ogre::Vector3 getDamagePosition();

 private:
  void createPhysics(CAbstractMap *map);
  void destroyPhysics(CAbstractMap *map);

 protected:
  virtual EReceiveDamageResult hit(const CDamage &damage) override;
};

#endif // _CHARACTER_ITEM_HPP_
