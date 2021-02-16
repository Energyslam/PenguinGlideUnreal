# PenguinGlideUnreal
Penguin "racing" game. Made in Unreal with C++. Track infinitely generated using experimental mesh generation plugin and bezier curves for shape.

We based our project around creating an infinite track using mesh generation.

The track is build out of several parts. Whenever the player gets past the middle part, a new part is generated in place of the last part. This is done by replacing vertex data of the last part and then regenerating the mesh in order to avoid constantly creating and deleting parts.

![](https://i.imgur.com/HjjDEX0.png)

The player is a penguin inside of a snowball and has to avoid obstacles. You're slowly introduced to the obstacles one-by-one.

![](https://i.imgur.com/CyLMuKU.png)

What's a racing game without some powerups and ramps?

![](https://i.imgur.com/4ze2gyP.png)

Track eventually becomes more crowded with mechanics. The red ball moves along the mesh sideways and defeats the player while the more positively coloured ball is a power-up (points, time slows down etc)

![](https://i.imgur.com/9hvWh1W.png)
