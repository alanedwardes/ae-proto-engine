#pragma once

#include "shared\EntityFactory.h"
EntityFactoryHolder g_oEntityFactoryHolder;

#include "shared\GeometryFactory.h"
GeometryFactoryHolder g_oGeometryFactoryHolder;

#include "shared\BaseSimulatedEntity.h"
#include "ClientPhysicsEntity.h"
#include "ClientPlayerEntity.h"

static EntityFactory<ClientPhysicsEntity>
	client_physics_item("physics_item");

static EntityFactory<BaseSimulatedEntity>
	client_static_item("static_item");

static EntityFactory<ClientPlayerEntity>
	client_player("player");

#include "shared\BaseSimulatedGeometry.h"
#include "shared\BaseRenderedGeometry.h"

static GeometryFactory<BaseSimulatedGeometry>
	client_geometry("static_brush");

static GeometryFactory<BaseRenderedGeometry>
	client_detail("detail_brush");