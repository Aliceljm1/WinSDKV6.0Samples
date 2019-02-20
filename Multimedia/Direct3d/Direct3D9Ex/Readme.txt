Sample to Demonstrate new D3D9Ex features.

NOTE: To compile this sample you must also download the DirectX SDK. This sample requires D3D9x*.h and D3D9x.lib which are available in the DirectX SDK.

This sample demonstrates:
- Sharing surfaces between D3D devices
- Elimination of Device lost with D3D9Ex
- GPU Priority
- Composition

The sample application renders a cursor indepedently of the scene geometry.
The scene (cubes in this case) are drawn by a D3D9Ex device that runs
in a lower priority background thread.  The image is copied to a shared
surface.  The main application thread contains a D3D9Ex device as well.
This thread runs at a higher priority and composites the shared image
with a D3D9Ex drawn cursor and text in real time.  This allows for
fluid cursor and text updates even when the scene is too complex to be
handled in real-time.

Press the UP arrow key to increase the scene complexity.
Press the DOWN arrow key to decrease the scene complexity.

Try increasing the Number of Cubes to a very high number.  The cubes will
update slowly, but the cursor will still be responsive.
