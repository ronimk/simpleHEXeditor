# simpleHEXeditor
A small project for the fall break 2019
<br>
<br>
Tested with MCUXpresso and CodeBlocks
<br>
<br>
List of available commands:
<br>
<br>
?        - to show help [INCOMPLETE]<br>
show     - to show a block of bytes of an opened file in HEX format [COMPLETE]<br>
del      - to delete a block of bytes from the file [COMPLETE]<br>
add      - to add bytes into the open file [COMPLETE]<br>
mod      - to modify a block of bytes in the file [NOT STARTED]<br>
filesize - to print the size of the open file [COMPLETE]<br>
sav      - to save any changes made to the file [COMPLETE]<br>
ld       - to open a desired file [COMPLETE]<br>
apply    - to apply a previous patch to the file [NOT STARTED]<br>
quit     - quits the program [COMPLETE]
<br>
<br>
Deadline 20.10.
<br>
Mostly tested. A bit more testing required still
<br>
<br>
TODO:
<br>
<br>
?    - refine the help info for each command. <br>
mod<br>
apply<br>
upload the design documents<br>
refactoring and finetuning [MOSTLY DONE]<br>
<br>
Fixed problems:<br>
<br>
CRITICAL: filesize (and max_filesize) was not handled correctly causing memory leaks, wrong allocations/deallocations,
and data being pushed outside file_data buffer. [FIXED]<br>
CRITICAL: fix problems concerning signed/unsigned chars when modifying or examining the file_data buffer. [FIXED]<br>
