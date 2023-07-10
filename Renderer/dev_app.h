#pragma once
#include <cstdint>
#include <chrono>
#include "pools.h"

namespace end
{
	struct emitter
	{
		float3 spawn_pos;
		float4 spawn_color;
		// indices into the shared_pool 
		sorted_pool_t<int16_t, 256> indices;
	};

	// Simple app class for development and testing purposes
	struct dev_app_t
	{
		void update();

		dev_app_t();

		double get_delta_time()const;

		void FillPools(emitter& emitter);
		void UpdatePools(emitter& emitter);
		void DrawParticles(emitter& emitter);

		// Initialize Pools
		pool_t<particle, 1042> freePool;
		emitter e1;
		emitter e2;
		emitter e3;
		emitter e4;
		emitter e5;
	};

}