# Map file specifications

From to [Creating Conquest Maps](http://www.windowsgames.co.uk/conquest_create.html)

To create your own custom maps you need to create a configuration file similar to the file World.map that comes with the Conquest game and a .bmp graphic file like World.bmp.

The Graphic File
This file must be in .bmp format and each territory must be white and completely bounded by black pixels so that the program can fill the territory with a player's colour. See the World.bmp that came with your copy of Conquest as an example. I've used the Windows Paint program to generate all my .bmp files and either done them from scratch or searched on the Internet for map files (eg www.lib.utexas.edu/maps/index.html) and used them as a starting point.

The Config File
The .map file is just a text file which can be edited with simple programs like Notepad. The first section in the .map 
file describes what the name of the graphics file is, whether it wraps around like the World does (yes or no), if you 
can scroll the map (horizontal, vertical or none) and your name as the author. You can also set whether to show up 
warnings if the game finds any problems with the .map file. It's always a good idea to leave this set to 'yes' unless 
there is a specific reason why you don't want the warning box to appear, for example if you want to have one-way 
territories.

```
[Map]
image=world.bmp
wrap=yes
scroll=horizontal
author=Your name
warn=yes
```

The second section describes the Continent names and the scores for owning them completely.

```
[Continents]
North America=5
South America=2
Africa=3
Europe=5
Asia=7
Australia=2
```

The final section describes the territories. The first parameter is the territory's name, then the "x" and "y" 
coordinates of the center of the territory in pixels from the top left corner of the bitmap, then the continent that 
the territory is in and finally list all the territories that this territory connects to. **The maximum number of 
continents that a map can have is 32**

```
[Territories]
Siam,270,139,Asia,China,India,Indonesia
Japan,322,104,Asia,Kamchatka,Mongolia
Ural,241,68,Asia,Siberia,China,Afghanistan,Ukraine
etc...
```

**The maximum number of territories that any territory can connect to is 10. The maximum number of territories that a map can have is 255.**

Using The Map
Once you put your .map and .bmp files in the folder where the Conquest.exe file lives then the map will appear in the drop down selection on the New Game dialog box.

There will almost certainly be some problems with the .map file as it's very easy to mis-spell a territory name, forget to put the continent name in after the x/y coordinates or overlook a connection. When you try and run a game with your map the program will check for any problems and give you a report on what's wrong to help you correct things. 