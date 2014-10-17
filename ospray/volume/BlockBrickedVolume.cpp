//
//                 INTEL CORPORATION PROPRIETARY INFORMATION
//
//    This software is supplied under the terms of a license agreement or
//    nondisclosure agreement with Intel Corporation and may not be copied
//    or disclosed except in accordance with the terms of that agreement.
//    Copyright (C) 2014 Intel Corporation. All Rights Reserved.
//

#include <cassert>
#include "ospray/volume/BlockBrickedVolume.h"
#include "BlockBrickedVolume_ispc.h"

namespace ospray {

    void BlockBrickedVolume::createEquivalentISPC() {

        //! Get the voxel type.
        voxelType = getParamString("voxelType", "unspecified");  exitOnCondition(getVoxelType() == OSP_UNKNOWN, "unrecognized voxel type");

        //! Create an ISPC BlockBrickedVolume object and assign type-specific function pointers.
        ispcEquivalent = ispc::BlockBrickedVolume_createInstance((int) getVoxelType());

        //! Get the volume dimensions.
        volumeDimensions = getParam3i("dimensions", vec3i(0));  exitOnCondition(reduce_min(volumeDimensions) <= 0, "invalid volume dimensions");

        //! Get the transfer function.
        transferFunction = (TransferFunction *) getParamObject("transferFunction", NULL);  exitOnCondition(transferFunction == NULL, "no transfer function specified");

        //! Get the gamma correction coefficient and exponent.
        vec2f gammaCorrection = getParam2f("gammaCorrection", vec2f(1.0f));

        //! Set the volume dimensions.
        ispc::BlockBrickedVolume_setVolumeDimensions(ispcEquivalent, (const ispc::vec3i &) volumeDimensions);

        //! Set the transfer function.
        ispc::BlockBrickedVolume_setTransferFunction(ispcEquivalent, transferFunction->getEquivalentISPC());

        //! Set the sampling step size for ray casting based renderers.
        ispc::BlockBrickedVolume_setStepSize(ispcEquivalent, 1.0f / reduce_max(volumeDimensions) / getParam1f("samplingRate", 1.0f));

        //! Set the gamma correction coefficient and exponent.
        ispc::BlockBrickedVolume_setGammaCorrection(ispcEquivalent, (const ispc::vec2f &) gammaCorrection);

        //! Allocate memory for the voxel data in the ISPC object.
        ispc::BlockBrickedVolume_allocateMemory(ispcEquivalent);

    }

    void BlockBrickedVolume::setRegion(const void *source, const vec3i &index, const vec3i &count) {

        //! Range check.
        // assert(inRange(index, vec3i(0), voxelDimensions) && inRange(count, vec3i(1), voxelDimensions + vec3i(1)));

        //! Copy voxel data into the volume.
        ispc::BlockBrickedVolume_setRegion(ispcEquivalent, source, (const ispc::vec3i &) index, (const ispc::vec3i &) count);

    }

} // namespace ospray

