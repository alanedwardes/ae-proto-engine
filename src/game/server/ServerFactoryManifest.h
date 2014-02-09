#pragma once

#include "shared\EntityFactory.h"
EntityFactoryHolder g_oEntityFactoryHolder;

#include "shared\GeometryFactory.h"
GeometryFactoryHolder g_oGeometryFactoryHolder;

#include "ServerPhysicsEntity.h"
#include "ServerStaticEntity.h"
#include "ServerPlayerEntity.h"

static EntityFactory<ServerPhysicsEntity>
	server_physics_item("physics_item");

static EntityFactory<ServerStaticEntity>
	server_static_item("static_item");

static EntityFactory<ServerPlayerEntity>
	server_player("player");

#include "shared\BaseRenderedGeometry.h"
#include "ServerStaticGeometry.h"

static GeometryFactory<ServerStaticGeometry>
	server_geometry("static_brush");

static GeometryFactory<BaseRenderedGeometry>
	server_detail("detail_brush");