# Wayback3D
A simple game engine meant to make 3d games that look like they were made in the 90s.


## Current features:
- Object hierarchy with types: Object, Mesh, Camera, Capsule, StaticMesh, AnimatedMesh, and AnimatedObj
- UI hierarchy with types: UIElement, UIImage, and UIText
- Ability to have multiple cameras and draw from whichever you want
- Basic collisions for Capsules
- Light maps for static objects with point and line lights
- Light grid for dynamic objects containing light value and dominant light direction
- Mesh support from obj and fbx files
- Armature and animation support from fbx files
- Bone parenting support for children of rigged objects
- Scripting with Lua
- 2d sound with volume and loop control
- 3d sound with volume, falloff, and loop control

'''text
my-project/
├── docs/
│   ├── index.md
│   └── guide.md
├── src/
│   └── app.py
├── README.md
└── package.json
'''


## TO DO:
- Add decals
- Add some sort of particle system
- Add editor with ImGui


## How to run test scene yourself
### Windows
Prerequisites: MinGW/GCC, git, and CMake 3.16 or newer
Make sure gcc, g++, and mingw32-make are on your path.
1. git clone https://github.com/liamdpearson/Wayback3D
2. cd Wayback3D
3. cmake -S . -B build -G "MinGW Makefiles"
4. cmake --build build
5. ./release
