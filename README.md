# AIEYear2Engine
Place for AIE Year 2 tutorial and engine work.

## Controls:
Note: works for Numpad too
Ctrl + ...

| Character | Action 	|
| --- 		| --- 		|
|Esc or Alt+F4 		|Quit|
|` 		|Cycle to the next state|
|0 		|Character Controller |
|1 		|Static Camera|
|2 		|FlyCamera|
|3 		|Orbit Camera|
|4 		|Travel Camera (moves between points)|
|5 		|Location Camera (sets camera to distinct points)|
|F1 	|toggle overlay on/ off|

|Draw State||
| --- 		| --- 		|
|7 		|Dot mode|
|8 		|Polygon mode|
|9 		|Fill mode|
|- 		|Previous Draw State|
|= 		|Next Draw State|

|Movement||
| --- 		| --- 		|
|LMB 	||
|MMB 	|Look around|
|RMB 	|Mouse picking|
|W/S 	|Forward/ Back|
|A/D 	|Strafe Left/ Right|
|R/F 	|Rise/ Fall|
| NUM+/NUM-	| Movement speed faster/ slower|
|Ctrl + W/S |Zoom in/ out|
|Ctrl + A/D |Orbit around origin|
|Space 	|Reset Camera/ toggle on/ off|
|P 		|Pause movement (Orbit)|


## Coding Convention:
### Variable Prefixes: 
|Variable type|Variable name|
| --- 		| --- 		|
|Function names|void GetPos()|
|float|fSpeed |
|vector 2, 3, 4's|v3Position|
|matrices 2, 3, 4's|m4WorldPos |

## Known Issues:
- For Menu items in ImGui - if a collapsing header will not open - try opening the one above first.
- Some combinations of buttons may distort the camera: press 'space' to reset

## Tutorials:
https://github.com/johnsietsma/PhysicsTestBed/tree/master/deps/physx/lib/vc14win32
http://learnopengl.com/
http://www.alanzucconi.com/category/shader/


## Optional Tools: 
Make your shader code prettier with [Nshader](https://github.com/samizzo/nshader/)
