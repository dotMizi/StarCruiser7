# StarCruiser7
Star Cruiser 7 is an homage to the late 70s game Star Raiders by Doug Neubauer, published by Atari. It is a Star Raiders-like remake for the Nintendo 3DS game console. My goal was to bring this milestone in the development of computer and console games a bit into the modern era, while maintaining the character and simplicity of this iconic 8-bit game and still using specific features of the 3DS console such as 3D screen, two displays, a faster CPU or the touch function. Even though diving through a star field is by no means realistic, it achieved a wonderful 3d effect on the Atari and this still works on the 3DS. Therefore, this unrealistic effect has been preserved. It should feel like you are playing the game on an Atari 400 home computer, but it should be as much fun as a real 3DS game.

Even though by now the 3DS game console is out of date and no longer commercially available, it still provides the perfect platform for Star Cruiser 7: It is easy to program with devkitPro and the 3D screen fits the game format perfectly. Modern VR systems would require major changes to the overall game mechanics and make the charm of the original disappear.

So let's play!

## Things that still need to be done
- the game lacks sound (I haven't had a chance to deal with 3DS audio yet)
- the attack strategy of the enemies is not very sophisticated and happens rather randomly (in combat and strategy mode)
- the soft renderer could be replaced by the 3DS hardware shader, but I fear that if the 3D rendering is too perfect, the 8-bit character will be lost. This is where my rudimentary and incomplete software renderer gives the feeling of a highly optimised home computer program from the 80s :-)
- Fixing hundreds of bugs that, in my experience, lurk in such code

## Description of the game
The original idea is certainly inspired by the movies of the time, Battlestar Galactica and Star Wars. The player defends his own starbases against the enemy Zylons. The best way to get a general overview is to read the original Star Raiders manual. It can be found as a scan on many internet sites. Any internet search engine will help you.  It is a manual that, as was very common at the time, gives detailed information also about technical realisation. It also helped me a lot to program Star Cruiser 7. 

## Build
For the build you need the devkitPro tool chain.

## Future possible developments
- Zylon Basestars differ in the original only by the used bitmap and the colouring of other opponents. Surely it would be right to make them appear much bigger and more difficult to defeat.
- The weak docking sequence of the original is certainly due to the low hardware resources. A better docking sequence, especially with more interaction by the player, would be desirable.
- A multiplayer mode 
- I read somewhere that Doug Neubauer would have liked an ending sequence, such as flying through the Death Star equatorial trench in Star Wars and destroying a boss, if he had more memory available  

## Controls
Analog Pad - The pad is used to control the StarCruiser. Left and right make our spaceship fly a curve accordingly, if you push the pad up, the nose dives down and if you push it down, it bumps up. In Aft View it behaves in exactly the same way, but of course it feels the other way round. In menu mode, up and down can also be used to select a menu item.

Digital Pad - Left and Right switches the views in the touchscreen. There are the views Cockpit, Sector Scan and Galactic Map. Up and down also allow you to select a menu item in menu mode and during the game the up button accelerates the ship while the down button slows it down. The Zylon ships are not very fast, so for combat it is good to reduce the speed significantly from the starting speed.

A button - A is the fire button, it fires a left or right photon torpedo alternately if they are not down.

B button - A activates or deactivates the shield.

X button - X switches between front and aft view.

Y button - Y activates or cancels the hyperwarp sequence.

Start button - The Start button activates a selected menu item or activates the Cancel menu in the game.

Touch screen - Tapping on the cockpit view activates or deactivates the attack computer. Tapping the Galactic Map selects a sector for the hyperwarp. 

The shoulder buttons are currently not used.

## Trivia
Star Cruiser 7 is the name of the spaceship that the player operates in Star Raiders.

The content of this text used brand names and trademarks are the property of their respective owners and are used only descriptive.
