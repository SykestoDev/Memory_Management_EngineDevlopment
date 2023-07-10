#include "dev_app.h"
#include "debug_renderer.h"
#include <iostream>
#include <stdlib.h>

//TODO include debug_renderer.h and pools.h and anything else you might need here
float Lerp(float start, float end, float t)
{
	// P1 + R(P2 - P1)
	return start + (end - start) * t;
}
float randNum(float min, float max)
{
	float r = rand() / float(RAND_MAX);
	return Lerp(min, max, r);
}

namespace end
{
	// Store time
	double delta_time = 0.0;

	// Get Time
	double dev_app_t::get_delta_time()const
	{
		return delta_time;
	}

	dev_app_t::dev_app_t()
	{
		std::cout << "Log whatever you need here.\n"; // Don’t forget to include <iostream>
	}

	// Calcuate Time
	double calc_delta_time()
	{
		static std::chrono::time_point<std::chrono::high_resolution_clock> last_time = std::chrono::high_resolution_clock::now();

		std::chrono::time_point<std::chrono::high_resolution_clock> new_time = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed_seconds = new_time - last_time;
		last_time = new_time;

		return std::min(1.0 / 15.0, elapsed_seconds.count());
	}

	// Colors
	float4 lineColor1 = float4(0, 1, 0, 1);
	float4 lineColor2 = float4(0, 1, 0, 1);
	float4 particleColor = float4(0.5f, 1, 0.7f, 1);

	// Gravity
	float3 Gravity = { 0, -9.81f, 0 };

	// Fill Pools
	void dev_app_t::FillPools(emitter& emitter)
	{
		// Allocate Memory
		int16_t freeIndex = freePool.alloc();
		// Check to see if its created
		if (freeIndex != -1)
		{
			// Allocate Memory
			int16_t sortedIndex = emitter.indices.alloc();
			if (sortedIndex != -1)
			{
				// Initiate a Particle
				particle particle;
				particle.color = emitter.spawn_color;
				particle.pos = emitter.spawn_pos;
				particle.setVelocity(randNum(-.8, .8), randNum(7, 12), randNum(-0.8, 0.8));
				// Fill in the pool;
				freePool[freeIndex] = particle;
				emitter.indices[sortedIndex] = freeIndex;
			}
			else
			{
				freePool.free(freeIndex);
			}
		}
	}
	
	// Update Pools
	void dev_app_t::UpdatePools(emitter& emitter)
	{
		// Update the particles to move 
		for (size_t j = 0; j < emitter.indices.size(); j++)
		{
			// Point to the the current Particle
			particle* currParticle = &freePool[emitter.indices[j]];

			// Check to see when to kill off the particle
			if (currParticle->pos.y < 0)
			{
				// Delete the particle
				freePool.free(emitter.indices[j]);
				emitter.indices.free(j);
				j--;
			}
			else
			{
				// Rules for the particles to follow
				currParticle->prev_pos = currParticle->pos;
				currParticle->pos += currParticle->velocity * delta_time;
				currParticle->velocity += Gravity * delta_time;
			}
		}
	}

	// Draw Particles
	void dev_app_t::DrawParticles(emitter& emitter)
	{
		// Iterate through sorted pool
		for (size_t i = 0; i < emitter.indices.size(); i++)
		{
			// Draw the particles
			particle* tempParticle = &freePool[emitter.indices[i]];
			// Add paticles to the debug_render to get drawn
			end::debug_renderer::add_line(tempParticle->prev_pos, tempParticle->pos, tempParticle->color);
		}
	}

	double elaspedTime = 0;
	int index = 0;

	// Called every Frame
	void dev_app_t::update()
	{
		// Clears lines
		debug_renderer::clear_lines();

		// Store calcuated time
		delta_time = calc_delta_time();

		//This draws the green checkmark
		/*end::debug_renderer::add_line(float3(-2, 0, 0), float3(0, -3, 0), float4(0.1f, 1, 0.1f, 1));
		end::debug_renderer::add_line(float3(0, -3, 0), float3(3,  4, 0), float4(0.1f, 1, 0.1f, 1));*/

		//TODO do your Updates here
		float3 pos1 = float3(-10.5, 0, -10.5);
		float3 pos2 = float3(-10.5, 0, 10.5);  // start position z lines
		float3 pos3 = float3(-10.5, 0, 10.5);
		float3 pos4 = float3(10.5, 0, 10.5);  // start position x lines

		// Itirate to draw the grid
		for (size_t i = 0; i < 22; i++)
		{
			// draws depth lines
			end::debug_renderer::add_line(pos1, pos2, lineColor1);
			pos1.x += 1;
			pos2.x += 1;
			// draws horizontal lines
			end::debug_renderer::add_line(pos3, pos4, lineColor2);
			pos3.z -= 1;
			pos4.z -= 1;
		}

		// Changing grid color over time
		lineColor1.x += delta_time * 0.2f;
		lineColor2.x += delta_time * 0.2f;
		if (lineColor1.x > 1 && lineColor2.x > 1)
		{
			lineColor1.y -= delta_time * 0.2f;
			lineColor2.y -= delta_time * 0.2f;
		}
		else
		{
			lineColor1.z -= delta_time * 0.5f;
			lineColor2.z -= delta_time * 0.5f;
		}

		elaspedTime += delta_time;
#pragma region Particle Emitters
		// Emitters
		e1.spawn_pos = { 0.0f,0.0f, 0.0f };
		e1.spawn_color = { 1.0f, 1.0f, 1.0f, 1.0f };
		e2.spawn_pos = { 5.5f,0.0f, 5.5f };
		e2.spawn_color = { 0.4f, 0.9f, 0.0f, 1.0f };
		e3.spawn_pos = { -5.5f,0.0f, 5.5f };
		e3.spawn_color = { 0.2f, 0.9f, 0.85f, 1.0f };
		e4.spawn_pos = { -5.5f,0.0f, -5.5f };
		e4.spawn_color = { 1.0f, 0.3f, 1.0f, 1.0f };
		e5.spawn_pos = { 5.5f,0.0f, -5.5f };
		e5.spawn_color = { 0.7f, 0.26f, 0.45f, 1.0f };

		// Fill pools over a time
		if (elaspedTime > 0.1)
		{
			FillPools(e1);
			FillPools(e2);
			FillPools(e3);
			FillPools(e4);
			FillPools(e5);
			elaspedTime -= 0.015;
		}

		// Update pools
		UpdatePools(e1);
		UpdatePools(e2);
		UpdatePools(e3);
		UpdatePools(e4);
		UpdatePools(e5);

		// Draw
		DrawParticles(e1);
		DrawParticles(e2);
		DrawParticles(e3);
		DrawParticles(e4);
		DrawParticles(e5);
#pragma endregion

	}
}


