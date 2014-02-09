#pragma once

#include "shared\GameObjectFactory.h"
EntityFactoryHolder g_oEntityFactoryHolder;

#include "shared\BaseSimulatedEntity.h"
#include "ClientPhysicsEntity.h"
#include "ClientPlayerEntity.h"

static GameObjectFactory<ClientPhysicsEntity>
	client_physics_item("physics_item");

static GameObjectFactory<BaseSimulatedEntity>
	client_static_item("static_item");

static GameObjectFactory<ClientPlayerEntity>
	client_player("player");

#include "shared\BaseSimulatedGeometry.h"
#include "shared\BaseRenderedGeometry.h"

static GameObjectFactory<BaseSimulatedGeometry>
	client_geometry("static_brush");

static GameObjectFactory<BaseRenderedGeometry>
	client_detail("detail_brush");