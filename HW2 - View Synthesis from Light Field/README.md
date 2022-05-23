**Background**
<pre>
A light field records both the radiance and direction info of light rays from scene.
Generally, a 4D light field is parameterized by two plane, view and focal plane.
There are two typical devices, camera array and light-field camera.
Due to the rich info recorded, light fields enable many exciting applications.
</pre>

**Introduction**
<pre>
This program synthesizes novel views from a recorded light filed,
fullfilled the basic requirements of the assignment.

For the enhanced features stated in the assignment, 
which is to support a variable focal length, 
that is not complete.
But it could react to the Vz and focal length, and shows the correct bilinear view point (set of the 4 neighbour camera),
however, it is in full size image, without shifting to correct the spot and the magnification of image.
</pre>

**Environment**
<pre>
visual studio 2019 C++ compiler
</pre>

**Command**
<pre>
C:\....\ veiwSynthesis LF_dir viewpoint_coord focal_length
i.e. veiwSynthesis light_filed_views 200 200 0 100
viewpoint position(200,200,0) with focal length 100
</pre>