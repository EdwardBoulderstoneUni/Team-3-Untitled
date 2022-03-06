#pragma once
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Copyright (c) 2008-2021 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  


#ifndef PX_PHYSICS_NXPHYSICS_API
#define PX_PHYSICS_NXPHYSICS_API
/** \addtogroup physics
@{
*/

/**
This is the main include header for the Physics SDK, for users who
want to use a single #include file.

Alternatively, one can instead directly #include a subset of the below files.
*/

// Foundation SDK 
#include "foundation/Px.h"
#include "../../include/PhysX/foundation/PxAllocatorCallback.h"
#include "../../include/PhysX/foundation/PxAssert.h"
#include "../../include/PhysX/foundation/PxBitAndData.h"
#include "../../include/PhysX/foundation/PxBounds3.h"
#include "../../include/PhysX/foundation/PxErrorCallback.h"
#include "../../include/PhysX/foundation/PxErrors.h"
#include "../../include/PhysX/foundation/PxFlags.h"
#include "../../include/PhysX/foundation/PxIntrinsics.h"
#include "../../include/PhysX/foundation/PxIO.h"
#include "../../include/PhysX/foundation/PxMat33.h"
#include "../../include/PhysX/foundation/PxMat44.h"
#include "../../include/PhysX/foundation/PxMath.h"
#include "../../include/PhysX/foundation/PxMathUtils.h"
#include "../../include/PhysX/foundation/PxPlane.h"
#include "../../include/PhysX/foundation/PxQuat.h"
#include "../../include/PhysX/foundation/PxSimpleTypes.h"
#include "../../include/PhysX/foundation/PxStrideIterator.h"
#include "../../include/PhysX/foundation/PxTransform.h"
#include "../../include/PhysX/foundation/PxUnionCast.h"
#include "../../include/PhysX/foundation/PxVec2.h"
#include "../../include/PhysX/foundation/PxVec3.h"
#include "../../include/PhysX/foundation/PxVec4.h"

//Not physics specific utilities and common code
#include "../../include/PhysX/common/PxCoreUtilityTypes.h"
#include "../../include/PhysX/common/PxPhysXCommonConfig.h"
#include "../../include/PhysX/common/PxRenderBuffer.h"
#include "../../include/PhysX/common/PxBase.h"
#include "../../include/PhysX/common/PxTolerancesScale.h"
#include "../../include/PhysX/common/PxTypeInfo.h"
#include "../../include/PhysX/common/PxStringTable.h"
#include "../../include/PhysX/common/PxSerializer.h"
#include "../../include/PhysX/common/PxMetaData.h"
#include "../../include/PhysX/common/PxMetaDataFlags.h"
#include "../../include/PhysX/common/PxSerialFramework.h"
#include "../../include/PhysX/common/PxPhysicsInsertionCallback.h"

//Task Manager
#include "../../include/PhysX/task/PxTask.h"

// Cuda Mananger
#if PX_SUPPORT_GPU_PHYSX
#include "gpu/PxGpu.h"
#endif

//Geometry Library
#include "../../include/PhysX/geometry/PxBoxGeometry.h"
#include "../../include/PhysX/geometry/PxBVHStructure.h"
#include "../../include/PhysX/geometry/PxCapsuleGeometry.h"
#include "../../include/PhysX/geometry/PxConvexMesh.h"
#include "../../include/PhysX/geometry/PxConvexMeshGeometry.h"
#include "../../include/PhysX/geometry/PxGeometry.h"
#include "../../include/PhysX/geometry/PxGeometryHelpers.h"
#include "../../include/PhysX/geometry/PxGeometryQuery.h"
#include "../../include/PhysX/geometry/PxHeightField.h"
#include "../../include/PhysX/geometry/PxHeightFieldDesc.h"
#include "../../include/PhysX/geometry/PxHeightFieldFlag.h"
#include "../../include/PhysX/geometry/PxHeightFieldGeometry.h"
#include "../../include/PhysX/geometry/PxHeightFieldSample.h"
#include "../../include/PhysX/geometry/PxMeshQuery.h"
#include "../../include/PhysX/geometry/PxMeshScale.h"
#include "../../include/PhysX/geometry/PxPlaneGeometry.h"
#include "../../include/PhysX/geometry/PxSimpleTriangleMesh.h"
#include "../../include/PhysX/geometry/PxSphereGeometry.h"
#include "../../include/PhysX/geometry/PxTriangle.h"
#include "../../include/PhysX/geometry/PxTriangleMesh.h"
#include "../../include/PhysX/geometry/PxTriangleMeshGeometry.h"


// PhysX Core SDK
#include "../../include/PhysX/PxActor.h"
#include "../../include/PhysX/PxAggregate.h"
#include "../../include/PhysX/PxArticulation.h"
#include "../../include/PhysX/PxArticulationReducedCoordinate.h"
#include "../../include/PhysX/PxArticulationJoint.h"
#include "../../include/PhysX/PxArticulationJointReducedCoordinate.h"
#include "../../include/PhysX/PxArticulationLink.h"
#include "../../include/PhysX/PxBatchQuery.h"
#include "../../include/PhysX/PxBatchQueryDesc.h"
#include "../../include/PhysX/PxClient.h"
#include "../../include/PhysX/PxConstraint.h"
#include "../../include/PhysX/PxConstraintDesc.h"
#include "../../include/PhysX/PxContact.h"
#include "../../include/PhysX/PxContactModifyCallback.h"
#include "../../include/PhysX/PxDeletionListener.h"
#include "../../include/PhysX/PxFiltering.h"
#include "../../include/PhysX/PxForceMode.h"
#include "../../include/PhysX/PxFoundation.h"
#include "../../include/PhysX/PxLockedData.h"
#include "../../include/PhysX/PxMaterial.h"
#include "../../include/PhysX/PxPhysics.h"
#include "../../include/PhysX/PxPhysicsVersion.h"
#include "../../include/PhysX/PxPhysXConfig.h"
#include "../../include/PhysX/PxQueryFiltering.h"
#include "../../include/PhysX/PxQueryReport.h"
#include "../../include/PhysX/PxRigidActor.h"
#include "../../include/PhysX/PxRigidBody.h"
#include "../../include/PhysX/PxRigidDynamic.h"
#include "../../include/PhysX/PxRigidStatic.h"
#include "../../include/PhysX/PxScene.h"
#include "../../include/PhysX/PxSceneDesc.h"
#include "../../include/PhysX/PxSceneLock.h"
#include "../../include/PhysX/PxShape.h"
#include "../../include/PhysX/PxSimulationEventCallback.h"
#include "../../include/PhysX/PxSimulationStatistics.h"
#include "../../include/PhysX/PxVisualizationParameter.h"
#include "../../include/PhysX/PxPruningStructure.h"

//Character Controller
#include "../../include/PhysX/characterkinematic/PxBoxController.h"
#include "../../include/PhysX/characterkinematic/PxCapsuleController.h"
#include "../../include/PhysX/characterkinematic/PxController.h"
#include "../../include/PhysX/characterkinematic/PxControllerBehavior.h"
#include "../../include/PhysX/characterkinematic/PxControllerManager.h"
#include "../../include/PhysX/characterkinematic/PxControllerObstacles.h"
#include "../../include/PhysX/characterkinematic/PxExtended.h"

//Cooking (data preprocessing)
#include "../../include/PhysX/cooking/Pxc.h"
#include "../../include/PhysX/cooking/PxConvexMeshDesc.h"
#include "../../include/PhysX/cooking/PxCooking.h"
#include "../../include/PhysX/cooking/PxTriangleMeshDesc.h"
#include "../../include/PhysX/cooking/PxBVH33MidphaseDesc.h"
#include "../../include/PhysX/cooking/PxBVH34MidphaseDesc.h"
#include "../../include/PhysX/cooking/PxMidphaseDesc.h"

//Extensions to the SDK
#include "../../include/PhysX/extensions/PxDefaultStreams.h"
#include "../../include/PhysX/extensions/PxDistanceJoint.h"
#include "../../include/PhysX/extensions/PxExtensionsAPI.h"
#include "../../include/PhysX/extensions/PxFixedJoint.h"
#include "../../include/PhysX/extensions/PxJoint.h"
#include "../../include/PhysX/extensions/PxJointLimit.h"
#include "../../include/PhysX//extensions/PxPrismaticJoint.h"
#include "../../include/PhysX/extensions/PxRevoluteJoint.h"
#include "../../include/PhysX/extensions/PxRigidBodyExt.h"
#include "../../include/PhysX/extensions/PxShapeExt.h"
#include "../../include/PhysX/extensions/PxSimpleFactory.h"
#include "../../include/PhysX/extensions/PxSmoothNormals.h"
#include "../../include/PhysX/extensions/PxSphericalJoint.h"
#include "../../include/PhysX/extensions/PxStringTableExt.h"
#include "../../include/PhysX/extensions/PxTriangleMeshExt.h"
#include "../../include/PhysX/extensions/PxConvexMeshExt.h"

//Serialization
#include "../../include/PhysX/extensions/PxSerialization.h"
#include "../../include/PhysX/extensions/PxBinaryConverter.h"
#include "../../include/PhysX/extensions/PxRepXSerializer.h"

//Vehicle Simulation
#include "../../include/PhysX/vehicle/PxVehicleComponents.h"
#include "../../include/PhysX/vehicle/PxVehicleDrive.h"
#include "../../include/PhysX/vehicle/PxVehicleDrive4W.h"
#include "../../include/PhysX/vehicle/PxVehicleDriveTank.h"
#include "../../include/PhysX/vehicle/PxVehicleSDK.h"
#include "../../include/PhysX/vehicle/PxVehicleShaders.h"
#include "../../include/PhysX/vehicle/PxVehicleTireFriction.h"
#include "../../include/PhysX/vehicle/PxVehicleUpdate.h"
#include "../../include/PhysX/vehicle/PxVehicleUtil.h"
#include "../../include/PhysX/vehicle/PxVehicleUtilControl.h"
#include "../../include/PhysX/vehicle/PxVehicleUtilSetup.h"
#include "../../include/PhysX/vehicle/PxVehicleUtilTelemetry.h"
#include "../../include/PhysX/vehicle/PxVehicleWheels.h"
#include "../../include/PhysX/vehicle/PxVehicleNoDrive.h"
#include "../../include/PhysX/vehicle/PxVehicleDriveNW.h"

//Connecting the SDK to Visual Debugger
#include "../../include/PhysX/pvd/PxPvdSceneClient.h"
#include "../../include/PhysX/pvd/PxPvd.h"
#include "../../include/PhysX/pvd/PxPvdTransport.h"
/** @} */
#endif