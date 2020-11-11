# Uamp
___

 ### Description:
    Ucode project: uamp - music player app in c++ 
 ### Developers:
 [Mykyta Savytskyi](https://github.com/intelek2al) | 
 [Serhiy Nikolayenko](https://github.com/N-911)
 

### Screenshots 
 ![utext main](/app/resources/main_uamp_1.png)

### Tech/framework used

#### 1.Built with

 [CMake](https://cmake.org)

#### 2.GUI library

 [Qt](https://www.qt.io)

#### 2.External Libraries

 [Taglib](https://taglib.org)
 
 [Sqlite3](https://www.sqlite.org/index.html)

 ### Installation:
    1. git clone https://github.com/intelek2al/CPP-Uamp
    2. cmake . -Bbuild -Wdev -Werror=dev && cmake --build ./build
 
 ### How to use?
    ./uamp

### Main parts:
 #### 1. Toolbar
 #### 2. Queue
 #### 3. Library

### General features:
- [x] support these audio format: mp3, pm4, wav, flac 
- [x] support such standard option:
    - [ ] play
    - [ ] pause
    - [ ] stop
    - [ ] fast forward
    - [ ] rewind
    - [ ] next
    - [ ] previous
    
- [x] control sound volume
- [x] autoplay the next song in the Queue
- [x] create/read/update and delete personal playlists
- [x] import/export playlists in format m3u
- [x] add and remove songs from the Queue
- [x] loop (repeat) a single song or Queue
- [x] shuffle songs listed in the Queue
- [x] sort songs listed in the Queue by title, genre, etc.
- [x] edit songs album art using pictures on the computer
- [x] database Library
- [x] support displaying/editing of the audio tags (Artist, Title, Album, Genre)
- [x] searching music and playlists by various criteria
- [x] support advanced sorting by the newest songs, highest raiting
- [x] searching music and playlists by various criter
- [x] support hotkeys for all standard options
- [x] error handling in logfile

### Shortcuts
* Add to Library:         Cmd + O
* Song Info:              Cmd + I
* Delete from Library:    Shift + Cmd + D
* Play:                   Space
* Stop:                   Cmd = .
* Next:                   Cmd + Right
* Previous:               Cmd + Left
* Increase Volume:        Cmd + Up
* Decrease Volume:        Cmd + Down
* Quit:         Cmd + Q

### License
[MIT](https://choosealicense.com/licenses/mit/)

---

