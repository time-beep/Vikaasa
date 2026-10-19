# 3D Models

This folder contains STL assets intended for the Vikāsa sensing-node prototype.

## `Leaf_Mount.stl`

- **Format:** binary STL mesh.
- **Purpose:** prototype holder intended to position the optical sensor assembly near a plant leaf/stem for repeatable red/NIR observations.
- **Mesh size:** approximately 47.6 KB; the supplied mesh documentation records 950 triangles.
- **Print starting point:** the original design note suggests trying a flat orientation on the build plate and PLA or PETG; verify support needs, wall strength, fit, and orientation in the slicer for your printer/material.

## Before printing or using

1. Inspect the STL in CAD/slicer and confirm dimensions against the actual sensor/LED/photodiode module.
2. Check clearance, strain relief, cable routing, and that the holder does not shade or press the leaf in a way that changes the reading.
3. Test sensor distance/angle repeatability and mechanical durability before collecting data for comparison.
4. Record any resized/modified version separately so measurement sessions remain traceable.

**Status:** prototype only. The repository does not claim that the mount has been dimensionally or field validated. See [`../Hardware/README.md`](../Hardware/README.md) and [`../docs/hardware_setup.md`](../docs/hardware_setup.md) for context.
