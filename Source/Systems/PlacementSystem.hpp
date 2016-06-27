#ifndef _SCENE_OBJECT_SYSTEM_HPP_
	#define _SCENE_OBJECT_SYSTEM_HPP_

#pragma once
#include "Leadwerks.h"
#include "../Utilities/Math/Math.hpp"
#include "../Components/Placement.hpp"
#include "../Components/World.hpp"
#include "../Repositories/PlacementRepository.hpp"

namespace Systems
{
	class PlacementSystem
	{
	public:

		static void Update(IDbRepository<Placement>* _pRepository, uint64_t entity, float dt, bool bAddVelocity)
		{
			using namespace Components;

            auto components = _pRepository->FindByEntityId(entity);

			if (!components.empty())
			{
				auto comp = components.front();

				float nFriction = 1.0f - comp.nFriction * dt;

				// < Move the object.
				comp.vVelocity *= nFriction;
				if (bAddVelocity)
				{
					auto vVelocity = comp.vVelocity * dt;
					comp.vTranslation += vVelocity;

					// < Set translation matrix.
					comp.mTranslation = Leadwerks::Mat4(comp.vTranslation.x, comp.vTranslation.y, comp.vTranslation.z);
				}

				// < Spin the object.
				comp.vSpin *= nFriction;
				auto vSpin = comp.vSpin * dt;
				comp.vRotation += vSpin;
				comp.mRotation = Leadwerks::Mat4(comp.vRotation.x, comp.vRotation.y, comp.vRotation.z);

				// < Update the world matrix.
				comp.mWorld = comp.mRotation * comp.mTranslation;

				// < Create the view matrix.
				comp.mView = comp.mWorld.Inverse();

				// < Update the forward vector.
				comp.vForward = Leadwerks::Vec3(
					Leadwerks::Math::Sin(comp.vRotation.y)
					, -Leadwerks::Math::Tan(comp.vRotation.x)
					, Leadwerks::Math::Cos(comp.vRotation.y))
					.Normalize();

				// < Update the right vector.
				comp.vRight = Leadwerks::Vec3(
					Leadwerks::Math::Cos(comp.vRotation.y)
					, Leadwerks::Math::Tan(comp.vRotation.z)
					, -Leadwerks::Math::Sin(comp.vRotation.y))
					.Normalize();

                comp = _pRepository->Save(comp);
			}
		}

		static void Drive(IDbRepository<Placement>* _pRepository, uint64_t entity, float nForce, bool bLockYAxis)
		{
			using namespace Components;

            auto components = _pRepository->FindByEntityId(entity);

			if (!components.empty())
			{
				auto comp = components.front();

				auto force = comp.vForward * nForce;
				comp.vVelocity.x += force.x;
				comp.vVelocity.z += force.z;

				if (!bLockYAxis) { comp.vVelocity.y += force.y; }

                comp = _pRepository->Save(comp);
			}
		}

		static void Strafe(IDbRepository<Placement>* _pRepository, uint64_t entity, float nForce, bool bLockYAxis)
		{
			using namespace Components;

            auto components = _pRepository->FindByEntityId(entity);

			if (!components.empty())
			{
				auto comp = components.front();

				auto force = comp.vRight * nForce;

				comp.vVelocity += force.x;
				comp.vVelocity += force.z;

				if (!bLockYAxis) { comp.vVelocity.y += force.y; }

                comp = _pRepository->Save(comp);
			}
		}

		static void Stop(IDbRepository<Placement>* _pRepository, uint64_t entity)
		{
			using namespace Components;

            auto components = _pRepository->FindByEntityId(entity);

			if (!components.empty())
			{
				auto comp = components.front();

				comp.vVelocity = Leadwerks::Vec3(0.0f, 0.0f, 0.0f);
				comp.vVelocity = Leadwerks::Vec3(0.0f, 0.0f, 0.0f);

                comp = _pRepository->Save(comp);
			}
		}

		static void SetTranslation(IDbRepository<Placement>* _pRepository, uint64_t entity, float x, float y, float z)
		{
			using namespace Components;

            auto components = _pRepository->FindByEntityId(entity);

			if (!components.empty())
			{
				auto comp = components.front();

				comp.vTranslation.x = x;
				comp.vTranslation.y = y;
				comp.vTranslation.z = z;

				comp.mTranslation = Leadwerks::Mat4(comp.vTranslation.x, comp.vTranslation.y, comp.vTranslation.z);

                comp = _pRepository->Save(comp);
			}
		}

		static void SetTranslation(IDbRepository<Placement>* _pRepository, uint64_t entity, Leadwerks::Vec3 vTranslation)
		{
			using namespace Components;

            auto components = _pRepository->FindByEntityId(entity);

			if (!components.empty())
			{
				auto comp = components.front();

				comp.vTranslation = vTranslation;

				comp.mTranslation = Leadwerks::Mat4(comp.vTranslation.x, comp.vTranslation.y, comp.vTranslation.z);

                comp = _pRepository->Save(comp);
			}
		}

		static void AddTranslation(IDbRepository<Placement>* _pRepository, uint64_t entity, float x, float y, float z)
		{
			using namespace Components;

            auto components = _pRepository->FindByEntityId(entity);

			if (!components.empty())
			{
				auto comp = components.front();

				comp.vTranslation.x += x;
				comp.vTranslation.y += y;
				comp.vTranslation.z += z;

				comp.mTranslation = Leadwerks::Mat4(comp.vTranslation.x, comp.vTranslation.y, comp.vTranslation.z);

                comp = _pRepository->Save(comp);
			}
		}

		static void AddTranslation(IDbRepository<Placement>* _pRepository, uint64_t entity, Leadwerks::Vec3 vTranslation)
		{
			using namespace Components;

            auto components = _pRepository->FindByEntityId(entity);

			if (!components.empty())
			{
				auto comp = components.front();

				comp.vTranslation.x += vTranslation.x;
				comp.vTranslation.y += vTranslation.y;
				comp.vTranslation.z += vTranslation.z;

				comp.mTranslation = Leadwerks::Mat4(comp.vTranslation.x, comp.vTranslation.y, comp.vTranslation.z);

                comp = _pRepository->Save(comp);
			}
		}

		static void SetRotation(IDbRepository<Placement>* _pRepository, uint64_t entity, float x, float y, float z)
		{
			using namespace Components;

            auto components = _pRepository->FindByEntityId(entity);

			if (!components.empty())
			{
				auto comp = components.front();

				comp.vRotation.x = x;
				comp.vRotation.y = y;
				comp.vRotation.z = z;

				Leadwerks::Mat4 rotX, rotY;
				Math::MatrixRotationX(&rotX, comp.vRotation.x);
				Math::MatrixRotationY(&rotY, comp.vRotation.y);
				Math::MatrixRotationZ(&comp.mRotation, comp.vRotation.z);
				comp.mRotation *= rotX;
				comp.mRotation *= rotY;

                comp = _pRepository->Save(comp);
			}
		}

		static void SetRotation(IDbRepository<Placement>* _pRepository, uint64_t entity, Leadwerks::Vec3 vRotation)
		{
			using namespace Components;

            auto components = _pRepository->FindByEntityId(entity);

			if (!components.empty())
			{
				auto comp = components.front();

				comp.vRotation.x = vRotation.x;
				comp.vRotation.y = vRotation.y;
				comp.vRotation.z = vRotation.z;

				Leadwerks::Mat4 rotX, rotY;
				Math::MatrixRotationX(&rotX, comp.vRotation.x);
				Math::MatrixRotationY(&rotY, comp.vRotation.y);
				Math::MatrixRotationZ(&comp.mRotation, comp.vRotation.z);
				comp.mRotation *= rotX;
				comp.mRotation *= rotY;

                comp = _pRepository->Save(comp);
			}
		}

		static void AddRotation(IDbRepository<Placement>* _pRepository, uint64_t entity, float x, float y, float z)
		{
			using namespace Components;

            auto components = _pRepository->FindByEntityId(entity);

			if (!components.empty())
			{
				auto comp = components.front();

				comp.vRotation.x += x;
				comp.vRotation.y += y;
				comp.vRotation.z += z;

				Leadwerks::Mat4 rotX, rotY;
				Math::MatrixRotationX(&rotX, comp.vRotation.x);
				Math::MatrixRotationY(&rotY, comp.vRotation.y);
				Math::MatrixRotationZ(&comp.mRotation, comp.vRotation.z);
				comp.mRotation *= rotX;
				comp.mRotation *= rotY;

                comp = _pRepository->Save(comp);
			}
		}

		static void AddRotation(IDbRepository<Placement>* _pRepository, uint64_t entity, Leadwerks::Vec3 vRotation)
		{
			using namespace Components;

            auto components = _pRepository->FindByEntityId(entity);

			if (!components.empty())
			{
				auto comp = components.front();

				comp.vRotation.x += vRotation.x;
				comp.vRotation.y += vRotation.y;
				comp.vRotation.z += vRotation.z;

				Leadwerks::Mat4 rotX, rotY;
				Math::MatrixRotationX(&rotX, comp.vRotation.x);
				Math::MatrixRotationY(&rotY, comp.vRotation.y);
				Math::MatrixRotationZ(&comp.mRotation, comp.vRotation.z);
				comp.mRotation *= rotX;
				comp.mRotation *= rotY;

                comp = _pRepository->Save(comp);
			}
		}

		static void SetVelocity(IDbRepository<Placement>* _pRepository, uint64_t entity, float x, float y, float z)
		{
			using namespace Components;

            auto components = _pRepository->FindByEntityId(entity);

			if (!components.empty())
			{
				auto comp = components.front();

				comp.vVelocity.x = x;
				comp.vVelocity.y = y;
				comp.vVelocity.z = z;

                comp = _pRepository->Save(comp);
			}
		}

		static void SetVelocity(IDbRepository<Placement>* _pRepository, uint64_t entity, Leadwerks::Vec3 vVelocity)
		{
			using namespace Components;

            auto components = _pRepository->FindByEntityId(entity);

			if (!components.empty())
			{
				auto comp = components.front();

				comp.vVelocity = vVelocity;

                comp = _pRepository->Save(comp);
			}
		}


		static void AddVelocity(IDbRepository<Placement>* _pRepository, uint64_t entity, float x, float y, float z)
		{
			using namespace Components;

            auto components = _pRepository->FindByEntityId(entity);

			if (!components.empty())
			{
				auto comp = components.front();

				comp.vVelocity.x += x;
				comp.vVelocity.y += y;
				comp.vVelocity.z += z;

                comp = _pRepository->Save(comp);
			}
		}

		static void AddVelocity(IDbRepository<Placement>* _pRepository, uint64_t entity, Leadwerks::Vec3 vVelocity)
		{
			using namespace Components;

            auto components = _pRepository->FindByEntityId(entity);

			if (!components.empty())
			{
				auto comp = components.front();

				comp.vVelocity.x += vVelocity.x;
				comp.vVelocity.y += vVelocity.y;
				comp.vVelocity.z += vVelocity.z;

                comp = _pRepository->Save(comp);
			}
		}

		static void SetSpin(IDbRepository<Placement>* _pRepository, uint64_t entity, float x, float y, float z)
		{
			using namespace Components;

            auto components = _pRepository->FindByEntityId(entity);

			if (!components.empty())
			{
				auto comp = components.front();

				comp.vSpin.x = x;
				comp.vSpin.y = y;
				comp.vSpin.z = z;

                comp = _pRepository->Save(comp);
			}
		}

		static void SetSpin(IDbRepository<Placement>* _pRepository, uint64_t entity, Leadwerks::Vec3 vSpin)
		{
			using namespace Components;

            auto components = _pRepository->FindByEntityId(entity);

			if (!components.empty())
			{
				auto comp = components.front();

				comp.vSpin = vSpin;

                comp = _pRepository->Save(comp);
			}
		}


		static void AddSpin(IDbRepository<Placement>* _pRepository, uint64_t entity, float x, float y, float z)
		{
			using namespace Components;

            auto components = _pRepository->FindByEntityId(entity);

			if (!components.empty())
			{
				auto comp = components.front();

				comp.vSpin.x += x;
				comp.vSpin.y += y;
				comp.vSpin.z += z;

                comp = _pRepository->Save(comp);
			}
		}

		static void AddSpin(IDbRepository<Placement>* _pRepository, uint64_t entity, Leadwerks::Vec3 vSpin)
		{
			using namespace Components;

            auto components = _pRepository->FindByEntityId(entity);

			if (!components.empty())
			{
				auto comp = components.front();

				comp.vSpin.x += vSpin.x;
				comp.vSpin.y += vSpin.y;
				comp.vSpin.z += vSpin.z;

                comp = _pRepository->Save(comp);
			}
		}

	}; // < end class.
} // < end system.

#endif _SCENE_OBJECT_SYSTEM_HPP_