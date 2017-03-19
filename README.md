# LolStatsQueryV2-QtUi_Public
This is a simple Qt UI built over a modified version of my League of Legends Stats Query project. It is written in both C++ and Python.

Functionality:
  1. Type in the Summoner Name of any player and select a ranked season from the seasons drop down menu.
  2. Champions that were played by that Summoner in that season of ranked play are listed in the Champions list.
  3. Clicking a champion's name displays some statistics and adjusts the background image.
  4. You can search for specific champions using the Champion Search bar.
  
  
  The Champion Search bar is implemented using a Trie built ontop of a QtMap. By using this datastructure combined with a simple Depth First Search Algorithm, the search bar will automatically predict what champion you are searching for and narrow down the Champion List accordingly.
  
 **UPDATE** 3/18/2017:
  - Added Local Cache to speed up repeat search queries.
  - Added Search History drop down menu, which pulls past queries from the local cache
  - Created a predicate function for parsing the string data
  - added functions for clearing and updating the main display
  Some Screenshots and Demos:
  
  ![Alt text](/Screenshots/screen1.jpg?raw=true "screen1")
  ![Alt text](/Screenshots/demo1.gif?raw=true "demo1")
  ![Alt text](/Screenshots/demo2.gif?raw=true "demo2")
  
  
  Please note this is still a work in progress and there will be plenty more features to come!
  
  
