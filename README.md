# Raytra

- A raytracer written in C++, featuring:
  - Blinn-phong shading
  - Reflection
  - Soft shadowing
  - Smooth normals
  - Acceleration structure: BVH Tree
- Coming up: refraction

## Dependencies

- OpenEXR:
    - `raytra` outputs EXR images by default
    - For Mac OS X, OpenEXR can be installed via Homwbrew by running
    ```
    brew install openexr
    ```
    - For other systems, please refer to http://www.openexr.com/ for more information

## Getting started

```
make
./raytra scene-files/bunny.scn bunny.exr 3 1
open bunny.exr # `bunny.exr` is the output of the renderer
```

Here's the output:
![](samples/bunny.exr)
