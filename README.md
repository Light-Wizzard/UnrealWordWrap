# Unreal Word Wrap

Unreal Engine Word Wrap is written in C++ for Blueprints for use with Text Render.

This will first try to wrap on spaces, then it will look at other characters to wrap on.

Table of Content
1. [Illustrations](#Illustrations)
2. [Video](#Video)
3. [Game Release](#Game-Release)
4. [Downloads](#Downloads)

## Illustrations

From Menu pick New C++ Class
![Blueprint](https://github.com/Light-Wizzard/UnrealWordWrap/blob/main/images/FromMenu.png)

Pick Blueprint Function Library
![Blueprint](https://github.com/Light-Wizzard/UnrealWordWrap/blob/main/images/BFL.png)

Name it
![Blueprint](https://github.com/Light-Wizzard/UnrealWordWrap/blob/main/images/Name-BFL.png)

Rebuild
![Blueprint](https://github.com/Light-Wizzard/UnrealWordWrap/blob/main/images/Rebuild.png)

From Blueprint call C++ Functions
![Blueprint](https://github.com/Light-Wizzard/UnrealWordWrap/blob/main/images/BlueprintWrap.png)

UI Save
![Blueprint](https://github.com/Light-Wizzard/UnrealWordWrap/blob/main/images/SaveWrapper.png)

Main Menu where you can edit the file
![Blueprint](https://github.com/Light-Wizzard/UnrealWordWrap/blob/main/images/MainMenu.png)

Snapshot of wrap
![Blueprint](https://github.com/Light-Wizzard/UnrealWordWrap/blob/main/images/Snapshot.png)

## Video

[You Tube Channel for Light-Wizzard](https://youtu.be/fm2P9hJGLTA)

This video will explain how I implemented Word Wrap in Unreal Engine's Text Render, this project is written in C++, and it can read and write files to the Content Folder, I use the Movies folder to make sure my text files get into the game once deployed. 

I am using Unreal Engine (UE) 5.1, if you do not have a C++ project, click on Tools, then New C++ Class, scroll down to Blueprint Function Library, give it a name, and make sure to update that name in C++, for example, I named mine BFL Unreal Word Wrap (BFL_UnrealWordWrap), UE adds a U so it becomes, UBFL_UnrealWordWrap, and you must rename class to whatever name you give it, or it will not compile.

You will have to rebuild the project in VS, so make sure you have Visual Studio (VS) 2022 installed, I used the Community Edition. 

Open up the Project Solution in VS, and add the two files, you can copy them into the Source folder under the name of your project.

You can see my Blueprints nodes to call the C++ function, which implements the LoadTxtFile function to read the text from a text file, I clean the file the best I can, UE does not render all characters, if you find square boxes in your game, you know I missed some, file an issue.

The program I wrote to demo this, uses a UI to edit and save the file.

The Main Menu has this feature, it could be used to take notes as you play, it has a menu that pops up with Q or the Escape key.

I needed a way to read files and wrap them, but I found the way most wrap functions work, most only wrap on spaces, instead this function takes into account all punctuations.

I do not have a sound room, so David will speak for me.

I hope this function will help you with your application.

## Game-Release

I am a Linux guy that is forced to work in Windows, but I do test things in Linux when I am using it.

### Windows 10 or Greater

* [Game-Release Windows only](https://github.com/Light-Wizzard/UnrealWordWrap/releases/download/WindowsUnrealWordWrap/Windows.zip)

### Linux

* [Linux](https://github.com/Light-Wizzard/UnrealWordWrap/releases/download/Linux/Linux.zip)
* [Linux ARM 64](https://github.com/Light-Wizzard/UnrealWordWrap/releases/download/LinuxArm64/LinuxArm64.zip)

### Android

* [Android_ASTC](https://github.com/Light-Wizzard/UnrealWordWrap/releases/download/Android_ASTC/Android_ASTC.zip)
* [Android_DXT](https://github.com/Light-Wizzard/UnrealWordWrap/releases/download/Android_DXT/Android_DXT.zip)
* [Android_ETC2](https://github.com/Light-Wizzard/UnrealWordWrap/releases/download/Android_ETC2/Android_ETC2.zip)
* [Android_Multi](https://github.com/Light-Wizzard/UnrealWordWrap/releases/download/Android_Multi/Android_Multi.zip)


I do not have a Mac or iOS so I cannot make those releases, but I do have the source that you can compile on any device I do not have.

## Downloads

I do not have a Mac or iOS License so I cannot make those packages, but below is the Soure, you can make it yourself.

You will find a clean.bat I run to cut the size from 4 GB to 1.78 GB, you will need to right click on the project file UnrealWordWrap.uproject, and Generate Visual Studeo Project Files, and rebuild the project, then open the project in Unreal Engine, and rebuild it if required.

[Unreal Engine Project File Zipped is around 1.78 GB](https://lightwizzard.com/github/UnrealWordWrap.zip)

### End-of-File
