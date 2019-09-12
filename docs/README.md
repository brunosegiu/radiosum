# Radiosum

Radiosum is an implementation of the methods presented in [A general two-pass method integrating specular and diffuse reflection](http://www0.cs.ucl.ac.uk/research/vr/Projects/VLF/vlfpapers/multi-pass_hybrid/Sillion_F__A_general_two-pass_method_integrating_specular_and_diffuse_reflections.pdf).

The approach proposed involves computing extended form factors, a geometric relationship between surfaces (or patches) that are the basis for the [radiosity method](https://en.wikipedia.org/wiki/Radiosity_(computer_graphics)). In this particular project three algoritms were developed.

Additionally, the tool includes a GUI which enables quick and simple editing of the objects/surfaces materials. Linear interpolation is optional, but already built-in.

Additional documentation can be found [here](https://github.com/brunosegiu/radiosum/blob/master/docs/latex/report.pdf). Spanish only.

![UI](https://raw.githubusercontent.com/brunosegiu/radiosum/master/docs/latex/assets/ui-ex.png)

### Hemicube + portal rendering for planar reflections
Form factors are calculated using by rendering a hemicube, located at the surface's barycenter and directed by its normal.
The specular extensions are derived in subsequent draw passes, by drawing the whole geometery from the portal's perspective.

### Hemicube + raytraced reflections
Form factors are calculated using by rendering a hemicube, located at the surface's barycenter and directed by its normal.
The specular extensions are derived in subsequent draw passes, rays are emitted stocastically covering the whole mirror; for each of these rays its reflection path is followed.

### Hemisphere + raytraced reflections
Form factors are derived from a stochastic process, which involves casting rays from pseudo-random directions included in the hemisphere centered at the surface's barycenter.
The rays' path is followed, and additional rays are casted when a hit with a specular surface is registered.

![street](https://raw.githubusercontent.com/brunosegiu/radiosum/master/docs/latex/assets/sdif.png)

## Dependencies and platforms
Radiosum was built, tested and thought as a Windows only project, since graphics drivers are heavily optimized in the platform.
Third party libraries include:

1. SDL2
2. Embree
3. GLEW
4. Dear ImGUI
5. GLM
6. SDL2Image
7. Eigen
