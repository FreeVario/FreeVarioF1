When you have multiple boards to work with (Linux only):

Keep config_hal.h seperate for each board as this will act as the "glue"
Setup a new configuration via CubeMx.
Configure main.c and copy and configure config_hal.h
Create a new "Source Folder" (rightclick on project and slect new -> Source Folder)
Name it FreeVarioLib
Add it as to include path (rightclick Add/Remove include path)
Quit TrueStudio
Open a terminal and go to your new project folder.
Delete FreeVarioLib
Create a symbolic link FreeVarioLib to the same folder in your Git repository source.
(ln -s <path to freevario repo>/FreeVarioLib FreeVarioLib)
Start TrueStudio
Have fun, changes made in your new board project will actually be done in the Git repo.
Oh and don't clean. You will have to repeat the steps.

FreeVarioLib must therefor allways be platform independent, but HAL dependant. 


