#include "StdAfx.h"
#include "PhysicsManager.h"

#define PHYSICS_DEBUG 1

CPhysicsManager::CPhysicsManager(Ogre::SceneManager *pSceneManager)
: m_pSceneManager(pSceneManager), m_pGhostPairCallback(NULL) {
    m_bDisplayDebugInfo = true;

    mBroadphase = new btDbvtBroadphase();
    mCollisionConfig = new btDefaultCollisionConfiguration();
    mDispatcher = new btCollisionDispatcher(mCollisionConfig);
    mSolver = new btSequentialImpulseConstraintSolver();

    m_pPhyWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfig);
    m_pPhyWorld->setGravity(btVector3(0,-GRAVITY_FACTOR,0));
    m_pPhyWorld->getDispatchInfo().m_allowedCcdPenetration = 0.000f;

#ifdef PHYSICS_DEBUG
    m_pDbgDraw = new BtOgre::DebugDrawer(m_pSceneManager->getRootSceneNode(), m_pPhyWorld);
    m_pPhyWorld->setDebugDrawer(m_pDbgDraw);
#endif

	m_pGhostPairCallback = new btGhostPairCallback();
    mBroadphase->getOverlappingPairCache()->setInternalGhostPairCallback(m_pGhostPairCallback);

    toggleDisplayDebugInfo();
}
CPhysicsManager::~CPhysicsManager() {
#ifdef PHYSICS_DEBUG
    delete m_pDbgDraw;
#endif

    for (int i = m_pPhyWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
        btCollisionObject * obj = m_pPhyWorld->getCollisionObjectArray()[i];

        btRigidBody * body = btRigidBody::upcast(obj);

        if (body && body->getMotionState())
            delete body->getMotionState();

        m_pPhyWorld->removeCollisionObject(obj);

        delete obj;
    }

	//destroy collision shapes
	for (auto &sh : m_CollisionObjects) {
		auto triShape = dynamic_cast<btBvhTriangleMeshShape*>(sh.second.getShape());
		if (triShape) {
			delete triShape->getMeshInterface();
		}
		delete sh.second.getShape();
	}
	m_CollisionObjects.clear();

	if (m_pGhostPairCallback)
		delete m_pGhostPairCallback;

    delete m_pPhyWorld;

    delete mSolver;
    delete mBroadphase;
    delete mDispatcher;
    delete mCollisionConfig;

    //NxOgre::World::destroyWorld();
    //NxOgre::World::destroySingletons();
}
btCollisionWorld *CPhysicsManager::getCollisionWorld()
{
    return m_pPhyWorld->getCollisionWorld();
}

btBroadphaseInterface *CPhysicsManager::getBroadphase()
{
    return mBroadphase;
}
void CPhysicsManager::update(Ogre::Real tpf) {
	// handle Messages
	while (m_Messages.size() > 0) {
		switch (m_Messages.front()->getType()) {
		case CPhysicsMessage::PMT_CREATE:
			m_pPhyWorld->addCollisionObject(const_cast<btCollisionObject*>(m_Messages.front()->getCollisionObject()));
			m_Messages.front()->getCollisionObject()->activate();
			break;
		case CPhysicsMessage::PMT_DELETE:
			if (btRigidBody::upcast(m_Messages.front()->getCollisionObject())) {
				if (btRigidBody::upcast(m_Messages.front()->getCollisionObject())->getMotionState()) {
					delete  btRigidBody::upcast(m_Messages.front()->getCollisionObject())->getMotionState();
					btRigidBody::upcast(const_cast<btCollisionObject*>(m_Messages.front()->getCollisionObject()))->setMotionState(NULL);
				}
			}
			m_pPhyWorld->removeCollisionObject(const_cast<btCollisionObject*>(m_Messages.front()->getCollisionObject()));
			delete m_Messages.front()->getCollisionObject();
			break;
		}
		delete m_Messages.front();
		m_Messages.pop_front();
	}


	 //Update Bullet world. Don't forget the debugDrawWorld() part!
    m_pPhyWorld->stepSimulation(tpf, 4);
#ifdef PHYSICS_DEBUG
    m_pPhyWorld->debugDrawWorld();
    m_pDbgDraw->step();
#endif
}
void CPhysicsManager::toggleDisplayDebugInfo() {
#ifdef PHYSICS_DEBUG
    m_bDisplayDebugInfo = !m_bDisplayDebugInfo;

    m_pDbgDraw->setDebugMode(m_bDisplayDebugInfo);
    // Ogre::LogManager::getSingleton().logMessage(Ogre::String("PhsicsDebug: ") + (m_bDisplayDebugInfo ? "yes" : "no"));
#endif
}
void CPhysicsManager::deleteLater(const btCollisionObject *pCO) {
	for (auto pMsg : m_Messages) {
		if (pMsg->getType() == CPhysicsMessage::PMT_DELETE && pCO == pMsg->getCollisionObject()) {
			// already queued
			return;
		}
	}
	m_Messages.push_back(new CPhysicsMessage(CPhysicsMessage::PMT_DELETE, pCO));
}
