#pragma once

#include "shared\GameObjectFactory.h"
EntityFactoryHolder g_oEntityFactoryHolder;

#include "ServerPhysicsEntity.h"
#include "ServerStaticEntity.h"
#include "ServerPlayerEntity.h"

static GameObjectFactory<ServerPhysicsEntity>
	server_physics_item("physics_item");

static GameObjectFactory<ServerStaticEntity>
	server_static_item("static_item");

static GameObjectFactory<ServerPlayerEntity>
	server_player("player");

#include "shared\BaseRenderedGeometry.h"
#include "ServerStaticGeometry.h"

static GameObjectFactory<ServerStaticGeometry>
	server_geometry("static_brush");

static GameObjectFactory<BaseRenderedGeometry>
	server_detail("detail_brush");