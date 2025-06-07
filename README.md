# GardenSandboxRepo

This repository contains the code for a small sandbox project in Unreal Engine. The building system supports placing actors using a data asset that describes the building and an optional ghost preview.

## Building hierarchy

- `AGardenBuildingBase` – base class for all placeable buildings.
- `AGardenGhostBuildingBase` – lightweight version used for placement previews.
- `AGardenStructure` – example of a concrete building derived from `AGardenBuildingBase`.
- `AGardenStructureGhost` – ghost counterpart of `AGardenStructure`.

`UBuildingDataAsset` now references both the placed building class and a ghost class derived from `AGardenGhostBuildingBase`.
